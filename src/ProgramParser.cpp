#include "ProgramParser.h"

#include "CompilerExceptions.h"
#include <utility>

#include "AST/BinOpNode.h"
#include "AST/CropNode.h"
#include "AST/DimensionsNode.h"
#include "AST/FlipNode.h"
#include "AST/IdNode.h"
#include "AST/ImportNode.h"
#include "AST/ResizeNode.h"
#include "AST/RotateNode.h"
#include "AST/ScalarNode.h"
#include "AST/SectionNode.h"
#include "AST/UnOpNode.h"

ProgramParser::ProgramParser(std::unique_ptr<Lexer> _lex) :
    Parser(std::move(_lex))
{}


ProgramParser::ProgramParser(const std::string& _in) :
    Parser(std::make_unique<ProgramLexer>(_in))
{}

std::optional<std::function<std::unique_ptr<CommandNode>()>>
    ProgramParser::get_command()
{
    while (match(ProgramLexer::COMMENT_T)) {}

    switch (curr_token.type)
    {
        case ProgramLexer::PRINT_T:
            return std::bind(&ProgramParser::print_stmt, this);
        case ProgramLexer::SAVE_T:
            return std::bind(&ProgramParser::export_stmt, this);
        case ProgramLexer::FOR_T:
            return std::bind(&ProgramParser::for_stmt, this);
        case ProgramLexer::IF_T:
            return std::bind(&ProgramParser::if_stmt, this);
        case ProgramLexer::FUNCTION_T:
            return std::bind(&ProgramParser::function_stmt, this);
        case ProgramLexer::ID_T:
            if (peek(1).type == ProgramLexer::ASSIGN_T)
                return std::bind(&ProgramParser::assignment, this);
            else
                return std::bind(&ProgramParser::expression, this);
        case ProgramLexer::ROTATE_T:
        case ProgramLexer::RESIZE_T:
        case ProgramLexer::CROP_T:
        case ProgramLexer::FLIP_T:
        case ProgramLexer::MODIFY_T:
            return std::bind(&ProgramParser::img_expr, this);
        default:
            return std::nullopt;
    }
}

ProgramNode ProgramParser::parse()
{
    std::vector<std::unique_ptr<CommandNode>> cmds;
    // while current token indicates a command
    while (auto command = get_command()) {
        // call command function
        cmds.push_back(command.value()());
    }

    if (not match(Lexer::EOF_T))
        throw_unexpected("end of input");

    return ProgramNode(ProgramLexer::PROG_T, std::move(cmds));
}

std::unique_ptr<PrintNode> ProgramParser::print_stmt()
{
    Token print_tok = curr_token;
    if (not match(ProgramLexer::PRINT_T))
        throw_unexpected(ProgramLexer::PRINT_T);

    return std::make_unique<PrintNode>(print_tok, expression());
}

std::unique_ptr<ExportNode> ProgramParser::export_stmt()
{
    Token export_tok = curr_token;
    if (not match(ProgramLexer::SAVE_T))
        throw_unexpected(ProgramLexer::SAVE_T);

    std::unique_ptr<ExprNode> img = expression();

    if (not match(ProgramLexer::AS_T))
        throw_unexpected(ProgramLexer::AS_T);

    return std::make_unique<ExportNode>(export_tok,
        std::move(img), expression());
}

std::unique_ptr<ExprNode> ProgramParser::import_expr()
{
    Token import_tok = curr_token;
    if (not match(ProgramLexer::IMAGE_OP_T))
        throw_unexpected(ProgramLexer::IMAGE_OP_T);
    if (not match(ProgramLexer::IN_T))
        throw_unexpected(ProgramLexer::IN_T);

    return std::make_unique<ImportNode>(import_tok,
        expression());
}

std::unique_ptr<AssignNode> ProgramParser::assignment()
{
    Token id_tok = curr_token;
    if (not match(ProgramLexer::ID_T))
        throw_unexpected(ProgramLexer::ID_T);

    Token assign_tok = curr_token;
    if (not match(ProgramLexer::ASSIGN_T))
        throw_unexpected(ProgramLexer::ASSIGN_T);

    return std::make_unique<AssignNode>(assign_tok,
        IdNode(id_tok), expression());
}

std::unique_ptr<ForNode> ProgramParser::for_stmt()
{
    Token for_tok = curr_token;
    if (not match(ProgramLexer::FOR_T))
        throw_unexpected(ProgramLexer::FOR_T);

    bool recursive = match(ProgramLexer::ALL_T);

    Token id_tok = curr_token;
    if (not match(ProgramLexer::ID_T))
        throw_unexpected(ProgramLexer::ID_T);
    IdNode iterator(id_tok);

    if (not match(ProgramLexer::IN_T))
        throw_unexpected(ProgramLexer::IN_T);

    std::unique_ptr<ExprNode> path = expression();

    if (not match(ProgramLexer::LBRACE_T))
        throw_unexpected(ProgramLexer::LBRACE_T);

    std::vector<std::unique_ptr<CommandNode>> cmds;
    while (auto command = get_command()) {
        cmds.push_back(command.value()());
    }

    if (not match(ProgramLexer::RBRACE_T))
        throw_unexpected(ProgramLexer::RBRACE_T);

    return std::make_unique<ForNode>(for_tok, recursive,
        iterator, std::move(path), std::move(cmds));
}

FullType ProgramParser::get_type(const Token& tok)
{
    switch (tok.type)
    {
        case ProgramLexer::DIMS_T:
            return FullType(ExprType::Dimensions);
        case ProgramLexer::SECTION_T:
            return FullType(ExprType::Section);
        case ProgramLexer::INT_T:
            return FullType(ExprType::Integer);
        case ProgramLexer::FLOAT_T:
            return FullType(ExprType::Float);
        case ProgramLexer::IMAGE_T:
            return FullType(ExprType::Image);
        case ProgramLexer::PATH_T:
            return FullType(ExprType::Path);
        case ProgramLexer::NONE_T:
            return FullType(ExprType::None);
    }

    return FullType(ExprType::Invalid);
}

std::unique_ptr<FunctionNode> ProgramParser::function_stmt()
{
    Token func_tok = curr_token;
    if (not match(ProgramLexer::FUNCTION_T))
        throw_unexpected(ProgramLexer::FUNCTION_T);

    Token id_tok = curr_token;
    if (not match(ProgramLexer::ID_T))
        throw_unexpected(ProgramLexer::ID_T);
    IdNode name(id_tok);

    if (not match(ProgramLexer::LPAREN_T))
        throw_unexpected(ProgramLexer::LPAREN_T);

    std::vector<std::unique_ptr<IdNode>> params;
    Token type_tok;
    bool expect = false;
    do {
        id_tok = curr_token;
        if (match(ProgramLexer::ID_T)) {
            auto param = std::make_unique<IdNode>(id_tok);
            if (not match(ProgramLexer::COLON_T))
                throw_unexpected(ProgramLexer::COLON_T);
            type_tok = curr_token;
            if (not match({ProgramLexer::DIMS_T, ProgramLexer::SECTION_T,
                    ProgramLexer::INT_T, ProgramLexer::FLOAT_T,
                    ProgramLexer::IMAGE_T, ProgramLexer::PATH_T})) {
                throw_unexpected(ProgramLexer::TYPE_T);
            }
            param->ftype = get_type(type_tok);
            params.push_back(std::move(param));
        } else if (expect) {
            throw_unexpected(ProgramLexer::ID_T);
        }

        expect = true;
    } while (match(ProgramLexer::SEP_T));

    if (not match(ProgramLexer::RPAREN_T))
        throw_unexpected(ProgramLexer::RPAREN_T);

    name.ftype = FullType(ExprType::None);
    if (match(ProgramLexer::COLON_T)) {
        type_tok = curr_token;
        if (not match({ProgramLexer::DIMS_T, ProgramLexer::SECTION_T,
                ProgramLexer::INT_T, ProgramLexer::FLOAT_T,
                ProgramLexer::IMAGE_T, ProgramLexer::PATH_T,
                ProgramLexer::NONE_T})) {
            throw_unexpected(ProgramLexer::TYPE_T);
        }
        name.ftype = get_type(type_tok);
    }

    if (not match(ProgramLexer::LBRACE_T))
        throw_unexpected(ProgramLexer::LBRACE_T);

    std::vector<std::unique_ptr<CommandNode>> cmds;
    while (auto command = get_command()) {
        cmds.push_back(command.value()());
    }

    if (not match(ProgramLexer::RBRACE_T))
        throw_unexpected(ProgramLexer::RBRACE_T);

    return std::make_unique<FunctionNode>(func_tok,
        name, std::move(params), std::move(cmds));
}

std::unique_ptr<IfNode> ProgramParser::if_stmt()
{
    Token if_tok = curr_token;
    if (not match(ProgramLexer::IF_T))
        throw_unexpected(ProgramLexer::IF_T);

    std::unique_ptr<ExprNode> cond = expression();

    if (not match(ProgramLexer::LBRACE_T))
        throw_unexpected(ProgramLexer::LBRACE_T);

    std::vector<std::unique_ptr<CommandNode>> body;
    while (auto command = get_command()) {
        body.push_back(command.value()());
    }

    if (not match(ProgramLexer::RBRACE_T))
        throw_unexpected(ProgramLexer::RBRACE_T);


    std::vector<std::unique_ptr<CommandNode>> else_body;
    if (match(ProgramLexer::ELSE_T)) {
        if (curr_token.type == ProgramLexer::IF_T) {
            else_body.push_back(if_stmt());
        } else {
            if (not match(ProgramLexer::LBRACE_T))
                throw_unexpected(ProgramLexer::LBRACE_T);

            while (auto command = get_command()) {
                else_body.push_back(command.value()());
            }

            if (not match(ProgramLexer::RBRACE_T))
                throw_unexpected(ProgramLexer::RBRACE_T);
        }
    }

    return std::make_unique<IfNode>(if_tok, std::move(cond),
        std::move(body), std::move(else_body));
}

std::unique_ptr<ExprNode> ProgramParser::img_expr()
{
    Token expr_tok = curr_token;
    switch (curr_token.type)
    {
        case ProgramLexer::ROTATE_T:
        {
            skip();
            std::unique_ptr<ExprNode> img = expression();
            if (not match(ProgramLexer::BY_T))
                throw_unexpected(ProgramLexer::BY_T);
            return std::make_unique<RotateNode>(
                expr_tok, std::move(img), expression());
        }
        case ProgramLexer::RESIZE_T:
        {
            skip();
            std::unique_ptr<ExprNode> img = expression();
            ResizeType re_type;
            if (curr_token.type == ProgramLexer::TO_T)
                re_type = ResizeType::Absolute;
            else if (curr_token.type == ProgramLexer::BY_T)
                re_type = ResizeType::Relative;
            else
                throw_unexpected("resize specifier");
            skip();
            return std::make_unique<ResizeNode>(expr_tok,
                re_type, std::move(img), expression());
        }
        case ProgramLexer::CROP_T:
        {
            skip();
            std::unique_ptr<ExprNode> section = expression();
            if (not match(ProgramLexer::FROM_T))
                throw_unexpected(ProgramLexer::FROM_T);
            return std::make_unique<CropNode>(expr_tok,
                std::move(section), expression());
        }
        case ProgramLexer::FLIP_T:
        {
            skip();
            std::unique_ptr<ExprNode> img = expression();
            FlipDirection dir;
            if (curr_token.type == ProgramLexer::VERTICALLY_T)
                dir = FlipDirection::Vertical;
            else if (curr_token.type == ProgramLexer::HORIZONTALLY_T)
                dir = FlipDirection::Horizontal;
            else
                throw_unexpected("flip direction");
            skip();
            return std::make_unique<FlipNode>(
                expr_tok, dir, std::move(img));
        }
        case ProgramLexer::MODIFY_T:
        {
            skip();
            std::unique_ptr<ExprNode> img = expression();
            Modification mod;
            if (auto mod_o = get_modification(curr_token); mod_o) {
                mod = mod_o.value();
                skip();
            } else {
                throw_unexpected("modification specifier");
            }

            if (not match(ProgramLexer::BY_T))
                throw_unexpected(ProgramLexer::BY_T);

            return std::make_unique<ModifyNode>(expr_tok,
                mod, std::move(img), expression());
        }
        default:
            throw_unexpected("image expression");
    }

    return nullptr;
}

std::optional<Modification> ProgramParser::get_modification(const Token& tok)
{
    switch (tok.type)
    {
        case ProgramLexer::SHARPNESS_T:
            return Modification::Sharpness;
        case ProgramLexer::BRIGHTNESS_T:
            return Modification::Brightness;
        case ProgramLexer::CONTRAST_T:
            return Modification::Contrast;
        case ProgramLexer::COLOR_T:
            return Modification::Color;
        default:
            return std::nullopt;
    }
}

std::unique_ptr<ExprNode> ProgramParser::shape_expr()
{
    if (not match(ProgramLexer::LPAREN_T))
        throw_unexpected(ProgramLexer::LPAREN_T);

    std::array<std::unique_ptr<ExprNode>, 4> components;

    components[0] = expression();
    if (not match(ProgramLexer::SEP_T))
        throw_unexpected(ProgramLexer::SEP_T);
    components[1] = expression();
    if (not match(ProgramLexer::SEP_T)) {
        // two components, make dimensions
        if (not match(ProgramLexer::RPAREN_T))
            throw_unexpected(ProgramLexer::RPAREN_T);

        return std::make_unique<DimensionsNode>(ProgramLexer::DIMS_LIT_T,
            std::move(components[0]), std::move(components[1]));
    }

    // four components, make section
    components[2] = expression();
    if (not match(ProgramLexer::SEP_T))
        throw_unexpected(ProgramLexer::SEP_T);
    components[3] = expression();
    if (not match(ProgramLexer::RPAREN_T))
        throw_unexpected(ProgramLexer::RPAREN_T);

    return std::make_unique<SectionNode>(ProgramLexer::SECTION_LIT_T,
        std::move(components[0]), std::move(components[1]),
        std::move(components[2]), std::move(components[3]));
}

std::unique_ptr<ExprNode> ProgramParser::expression()
{
    std::unique_ptr<ExprNode> root = bool_expr();

    if (match(ProgramLexer::LPAREN_T)) {
        switch (curr_token.type) {
            case ProgramLexer::R_T:
            case ProgramLexer::G_T:
            case ProgramLexer::B_T:
            {
                root = std::make_unique<UnOpNode>(
                    curr_token, std::move(root));
                skip();
                if (not match(ProgramLexer::RPAREN_T))
                    throw_unexpected(ProgramLexer::RPAREN_T);
                return root;
            }
            default:
                throw_unexpected("channel");
        }
    }

    Token op_tok = curr_token;
    if (match(ProgramLexer::DIMENSIONS_T))
        return std::make_unique<UnOpNode>(op_tok, std::move(root));

    return root;
}

std::unique_ptr<ExprNode> ProgramParser::bool_expr()
{
    std::unique_ptr<ExprNode> root = arith_expr();

    Token op_tok = curr_token;
    while (match({ProgramLexer::AND_T, ProgramLexer::OR_T,
            ProgramLexer::EQUALS_T, ProgramLexer::NEQUALS_T,
            ProgramLexer::LEQ_T, ProgramLexer::GEQ_T,
            ProgramLexer::LESS_T, ProgramLexer::GREATER_T})) {
        root = std::make_unique<BinOpNode>(
            std::move(root), op_tok, arith_expr());
        op_tok = curr_token;
    }

    return root;
}

std::unique_ptr<ExprNode> ProgramParser::arith_expr()
{
    std::unique_ptr<ExprNode> root = term();

    Token op_tok = curr_token;
    while (match({ProgramLexer::PLUS_T, ProgramLexer::MINUS_T})) {
        root = std::make_unique<BinOpNode>(
            std::move(root), op_tok, term());
        op_tok = curr_token;
    }

    return root;
}

std::unique_ptr<ExprNode> ProgramParser::term()
{
    std::unique_ptr<ExprNode> root = factor();

    Token op_tok = curr_token;
    while (match({ProgramLexer::MULT_T, ProgramLexer::DIV_T})) {
        root = std::make_unique<BinOpNode>(
            std::move(root), op_tok, factor());
    }

    return root;
}

std::unique_ptr<ExprNode> ProgramParser::factor()
{
    Token op_tok = curr_token;
    if (match(ProgramLexer::MINUS_T)) {
        op_tok.type = ProgramLexer::UNMINUS_T;
        return std::make_unique<UnOpNode>(op_tok, factor());
    } else if (match(ProgramLexer::NOT_T)) {
        return std::make_unique<UnOpNode>(op_tok, factor());
    }

    return atom();
}

std::unique_ptr<ExprNode> ProgramParser::atom()
{
    std::unique_ptr<ExprNode> root = primary();

    return root;
}

std::unique_ptr<ExprNode> ProgramParser::primary()
{
    switch (curr_token.type) {
        case ProgramLexer::INTEGER_LIT_T:
        case ProgramLexer::FLOAT_LIT_T:
        case ProgramLexer::PATH_LIT_T:
        case ProgramLexer::TRUE_T:
        case ProgramLexer::FALSE_T:
        {
            ExprType type = ExprType::Invalid;
            if (curr_token.type == ProgramLexer::INTEGER_LIT_T)
                type = ExprType::Integer;
            else if  (curr_token.type == ProgramLexer::FLOAT_LIT_T)
                type = ExprType::Float;
            else if (curr_token.type == ProgramLexer::PATH_LIT_T)
                type = ExprType::Path;
            else
                type = ExprType::Bool;

            ScalarNode node(curr_token, type);
            skip();
            return std::make_unique<ScalarNode>(node);
        }
        case ProgramLexer::ID_T:
        {
            IdNode node(curr_token);
            skip();
            if (curr_token.type == ProgramLexer::LPAREN_T) {
                auto next = peek(1).type;
                if (next != ProgramLexer::R_T
                        and next != ProgramLexer::G_T
                        and next != ProgramLexer::B_T) {
                    return function_call(node);
                }
            }
            return std::make_unique<IdNode>(node);
        }
        case ProgramLexer::IMAGE_OP_T:
            return import_expr();
        case ProgramLexer::ROTATE_T:
        case ProgramLexer::RESIZE_T:
        case ProgramLexer::CROP_T:
        case ProgramLexer::FLIP_T:
        case ProgramLexer::MODIFY_T:
            return img_expr();
        case ProgramLexer::LPAREN_T:
            if (peek(2).type == ProgramLexer::SEP_T) {
                return shape_expr();
            } else {
                skip();
                std::unique_ptr<ExprNode> root = expression();
                if (not match(ProgramLexer::RPAREN_T))
                    throw_unexpected(ProgramLexer::RPAREN_T);
                return root;
            }
        default:
            throw_unexpected("primary expression");
    }

    return nullptr;
}

std::unique_ptr<ExprNode> ProgramParser::function_call(IdNode func)
{
    Token call_tok = func.token.value();
    call_tok.type = ProgramLexer::CALL_T;

    if (not match(ProgramLexer::LPAREN_T))
        throw_unexpected(ProgramLexer::LPAREN_T);

    std::vector<std::unique_ptr<ExprNode>> args;
    bool expect = false;
    do {
        if (expect or curr_token.type != ProgramLexer::RPAREN_T)
            args.push_back(expression());
        expect = true;
    } while (match(ProgramLexer::SEP_T));

    if (not match(ProgramLexer::RPAREN_T))
        throw_unexpected(ProgramLexer::RPAREN_T);

    return std::make_unique<CallNode>(call_tok,
        func, std::move(args));
}

void ProgramParser::throw_unexpected(const std::string& expected)
{
    auto err = error_msg(expected, curr_token);
    if (curr_token.type == Token::INVALID_T)
        throw LexicalException(err);
    throw SyntacticException(err);
}

void ProgramParser::throw_unexpected(Token::t_type expected)
{
    auto expect_name = lex->token_name(expected);
    auto err = error_msg(expect_name, curr_token);
    if (curr_token.type == Token::INVALID_T)
        throw LexicalException(err);
    throw SyntacticException(err);
}
