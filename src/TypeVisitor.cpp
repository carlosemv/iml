#include "TypeVisitor.h"
#include "CompilerExceptions.h"
#include "ProgramLexer.h"

TypeVisitor::TypeVisitor() : sym_table(1)
{}

void TypeVisitor::visit(CropNode& node)
{
    if (node.command) {
        check_id(node.image->token,
            node.image->ftype);
    }

    node.section->visit(*this);
    check_section(node.section->token,
        node.section->ftype);

    node.image->visit(*this);
    check_image(node.image->token,
        node.image->ftype);

    node.ftype = FullType(ExprType::Image);
}

void TypeVisitor::visit(DimensionsNode& node)
{
    node.width->visit(*this);
    check_num(node.width->token,
        node.width->ftype);

    node.height->visit(*this);
    check_num(node.height->token,
        node.height->ftype);

    node.ftype = FullType(ExprType::Dimensions);
    node.ftype.list_types = {node.width->ftype,
        node.height->ftype};
}

void TypeVisitor::visit(ExportNode& node)
{
    node.image->visit(*this);
    check_image(node.image->token,
        node.image->ftype);

    node.path->visit(*this);
    check_path(node.path->token,
        node.path->ftype);
}

void TypeVisitor::visit(FlipNode& node)
{
    if (node.command) {
        check_id(node.image->token,
            node.image->ftype);
    }

    node.image->visit(*this);
    check_image(node.image->token,
        node.image->ftype);

    node.ftype = FullType(ExprType::Image);
}

void TypeVisitor::visit(IfNode& node)
{
    sym_table.emplace_front();

    node.condition->visit(*this);
    check_bool(node.condition->token,
        node.condition->ftype);

    for (auto& cmd : node.cmds)
        cmd->visit(*this);

    for (auto& cmd : node.else_body)
        cmd->visit(*this);        

    sym_table.pop_front();
}

void TypeVisitor::visit(FunctionNode& node)
{
    // node.name->ftype set during parsing
    auto name = node.name->token.value().text;
    for (auto& param : node.params) {
        // param->ftype set during parsing
        node.name->ftype.params.push_back(param->ftype);
    }
    sym_table.front()[name] = node.name->ftype;

    sym_table.emplace_front();

    for (auto& param : node.params) {
        auto id = param->token.value().text;
        sym_table.front()[id] = param->ftype;
    }

    for (auto& cmd : node.cmds) {
        cmd->visit(*this);
    }

    sym_table.pop_front();
}

void TypeVisitor::visit(ForNode& node)
{
    sym_table.emplace_front();

    node.path->visit(*this);
    check_path(node.path->token,
        node.path->ftype);

    auto iter = node.iterator->token.value().text;
    sym_table.front()[iter] = FullType(ExprType::Path);
    for (auto& cmd : node.cmds) {
        cmd->visit(*this);
    }

    sym_table.pop_front();
}

void TypeVisitor::visit(ImportNode& node)
{
    node.path->visit(*this);
    check_path(node.path->token,
        node.path->ftype);

    node.ftype = FullType(ExprType::Image);
}

void TypeVisitor::visit(ModifyNode& node)
{
    if (node.command) {
        check_id(node.image->token,
            node.image->ftype);
    }

    node.image->visit(*this);
    check_image(node.image->token,
        node.image->ftype);

    node.factor->visit(*this);
    check_num(node.factor->token,
        node.factor->ftype);

    node.ftype = FullType(ExprType::Image);
}

void TypeVisitor::visit(ResizeNode& node)
{
    if (node.command) {
        check_id(node.image->token,
            node.image->ftype);
    }

    node.image->visit(*this);
    check_image(node.image->token,
        node.image->ftype);

    node.resize->visit(*this);
    if (node.resize_type == ResizeType::Absolute) {
        check_dimensions(node.resize->token,
            node.resize->ftype);
    } else {
        check_num(node.resize->token,
            node.resize->ftype);
    }

    node.ftype = FullType(ExprType::Image);
}

void TypeVisitor::visit(RotateNode& node)
{
    if (node.command) {
        check_id(node.image->token,
            node.image->ftype);
    }

    node.image->visit(*this);
    check_image(node.image->token,
        node.image->ftype);

    node.rotation->visit(*this);
    check_num(node.rotation->token,
        node.rotation->ftype);

    node.ftype = FullType(ExprType::Image);
}

void TypeVisitor::visit(SectionNode& node)
{
    node.left->visit(*this);
    check_num(node.left->token,
        node.left->ftype);

    node.upper->visit(*this);
    check_num(node.upper->token,
        node.upper->ftype);

    node.right->visit(*this);
    check_num(node.right->token,
        node.right->ftype);

    node.lower->visit(*this);
    check_num(node.lower->token,
        node.lower->ftype);

    node.ftype = FullType(ExprType::Section);
    node.ftype.list_types = {
        node.left->ftype,
        node.upper->ftype,
        node.right->ftype,
        node.lower->ftype
    };
}

void TypeVisitor::visit(UnOpNode& node)
{
    node.expr->visit(*this);
    auto expr_type = node.expr->ftype;

    if (node.token) {
        auto op = node.token.value();
        if (op.type == ProgramLexer::UNMINUS_T) {
            if (expr_type.is_num()) {
                node.ftype.type = expr_type.type;
                return;
            }
        } else if (op.type == ProgramLexer::NOT_T) {
            if (expr_type.type == ExprType::Bool) {
                node.ftype.type = expr_type.type;
                return;
            }
        } else if (op.type == ProgramLexer::DIMENSIONS_T) {
            if (expr_type.type == ExprType::Image) {
                node.ftype.type = ExprType::Dimensions;
                node.ftype.list_types = {
                    FullType(ExprType::Integer),
                    FullType(ExprType::Integer)
                };
                return;
            }
        } else if (ProgramLexer::channel_token(op.type)) {
            if (expr_type.type == ExprType::Image) {
                node.ftype.type = expr_type.type;
                return;
            }
        } else {
            throw CompilerException("Unary operation at "
                + op.pos_string() + " has invalid token type");
        }

        throw SemanticException("Invalid operation \'"
            + op.text + "\' at " + op.pos_string()
            + ", on a " + expr_type.to_string());
    } else {
        throw CompilerException("Unary operation"\
            " has no defining token\n");
    }
}

void TypeVisitor::visit(AssignNode& node)
{
    node.expr->visit(*this);
    node.id->ftype = node.expr->ftype;

    if (not node.id->token)
        throw CompilerException("Variable has no defining token");

    sym_table.front()[node.id->token.value().text] =
        node.id->ftype;
}

void TypeVisitor::visit(PrintNode& node)
{
    node.expr->visit(*this);
}

void TypeVisitor::visit(ReturnNode& node)
{
    node.expr->visit(*this);
    if (node.expr->ftype.type != node.function_type.type) {
        auto expr = node.expr->token.value();
        throw SemanticException("Returned expression \'"
            + expr.text + "\' at " + expr.pos_string()
            + ", has invalid type "
            + node.expr->ftype.to_string()
            + "; expected " + node.function_type.to_string());
    }
}

FullType TypeVisitor::binop_type(Token op,
    FullType lhs, FullType rhs)
{
    switch (op.type) {
        case ProgramLexer::AND_T:
        case ProgramLexer::OR_T:
            if (lhs.type == rhs.type and
                    lhs.type == ExprType::Bool)
                return lhs;
            break;
        case ProgramLexer::EQUALS_T:
        case ProgramLexer::NEQUALS_T:
            if ((lhs.is_num() and rhs.is_num())
                    or (lhs.type == rhs.type
                    and lhs.type != ExprType::Image))
                return FullType(ExprType::Bool);
            break;
        case ProgramLexer::LEQ_T:
        case ProgramLexer::GEQ_T:
        case ProgramLexer::LESS_T:
        case ProgramLexer::GREATER_T:
            if ((lhs.is_num() and rhs.is_num()))
                return FullType(ExprType::Bool);
            break;
        case ProgramLexer::PLUS_T:
            if (lhs.type == rhs.type)
                return lhs;
            if (lhs.type == ExprType::Image
                    or rhs.type == ExprType::Image) {
                if (rhs.is_num() or lhs.is_num())
                    return FullType(ExprType::Image);
            }
            if (lhs.is_num() and rhs.is_num())
                return FullType(ExprType::Float);
            if (lhs.is_list() and rhs.is_num())
                return lhs;
            if (rhs.is_list() and lhs.is_num())
                return rhs;
            break;
        case ProgramLexer::MINUS_T:
            if (lhs.type == rhs.type) {
                if (lhs.type != ExprType::Path)
                    return lhs;
                break;
            }

            if (lhs.is_num() and rhs.is_num())
                return FullType(ExprType::Float);

            if (lhs.type == ExprType::Image
                and rhs.type == ExprType::Section)
                return lhs;
            if (rhs.type == ExprType::Image
                and lhs.type == ExprType::Section)
                return rhs;
            break;
        case ProgramLexer::MULT_T:
            if (lhs.type == rhs.type) {
                if (lhs.type != ExprType::Path)
                    return lhs;
                break;
            }

            if (lhs.type == ExprType::Image
                or rhs.type == ExprType::Image) {
                if (rhs.is_num() or lhs.is_num())
                    return FullType(ExprType::Image);
            }
            if (lhs.is_num() and rhs.is_num())
                return FullType(ExprType::Float);
            if (lhs.is_list() and rhs.is_num())
                return lhs;
            if (rhs.is_list() and lhs.is_num())
                return rhs;

            if (lhs.is_list() and rhs.is_list()) {
                if (lhs.type == ExprType::Section)
                    return lhs;
                return rhs;
            }
            break;
        case ProgramLexer::DIV_T:
            if (lhs.type == rhs.type) {
                return lhs;
            }

            if (lhs.type == ExprType::Image
                and rhs.is_num())
                return lhs;

            if (lhs.is_num() and rhs.is_num())
                return FullType(ExprType::Float);

            if (lhs.is_list() and rhs.is_num())
                return lhs;
            break;
        default:
            throw CompilerException("Binary operation at "
                + op.pos_string()
                + " has invalid token type");
    }

    throw SemanticException("Invalid operation \'"
        + op.text + "\' at " + op.pos_string()
        + ", between " + lhs.to_string()
        + " and " + rhs.to_string());
}

void TypeVisitor::visit(BinOpNode& node)
{
    node.lhs->visit(*this);
    node.rhs->visit(*this);

    auto ltype = node.lhs->ftype;
    auto rtype = node.rhs->ftype;

    if (node.token) {
        node.ftype = binop_type(node.token.value(), ltype, rtype);
    } else {
        throw CompilerException("Binary operation"\
            " has no defining token\n");
    }
}

void TypeVisitor::visit(IdNode& node)
{
    if (not node.token)
        throw CompilerException("Variable has no defining token");

    bool found = false;
    for (auto context : sym_table) {
        auto it = context.find(node.token.value().text);
        if (it != context.end()) {
            node.ftype = it->second;
            found = true;
            break;
        }
    }

    if (not found)
        throw SemanticException("Reference to undefined variable \'"
            + node.token.value().text + "\' in "
            + node.token.value().pos_string());
}

void TypeVisitor::visit(CallNode& node)
{
    auto func = node.func->token.value().text;

    bool found = false;
    for (auto context : sym_table) {
        auto it = context.find(func);
        if (it != context.end()) {
            node.ftype = it->second;
            found = true;
            break;
        }
    }
    if (not found)
        throw SemanticException("Call to undefined function \'"
            + func + "\' in " + node.token.value().pos_string());

    for (auto& arg : node.args)
        arg->visit(*this);

    bool invalid = false;
    if (node.ftype.params.size() != node.args.size())
        invalid = true;
    if (not invalid) {
        for (auto i = 0u; i < node.args.size(); ++i) {
            if (node.args[i]->ftype.type 
                    != node.ftype.params[i].type) {
                invalid = true;
                break;
            }
        }
    }

    if (invalid) {
        std::string err = "Call to function \'" + func 
            + "\' in " + node.token.value().pos_string()
            + " has invalid arguments (";
        for (auto i = 0u; i < node.args.size(); ++i) {
            err += node.args[i]->ftype.to_string();
            if (i != node.args.size()-1)
                err += ", ";
        }
        err += "), expected (";
        for (auto i = 0u; i < node.ftype.params.size(); ++i) {
            err += node.ftype.params[i].to_string();
            if (i != node.ftype.params.size()-1)
                err += ", ";
        }
        err += ")";

        throw SemanticException(err);
    }
}

void TypeVisitor::visit(ProgramNode& node)
{
    for (auto& cmd : node.cmds)
        cmd->visit(*this);
}

void TypeVisitor::visit([[maybe_unused]] ScalarNode& node)
{}


void TypeVisitor::check_bool(
    std::optional<Token> tok, const FullType& type)
{
    if (tok) {
        auto op = tok.value();
        if (type.type != ExprType::Bool)
            throw SemanticException("Expression \'"
                + op.text + "\' at " + op.pos_string()
                + ", has invalid type " + type.to_string()
                + "; expected a boolean");
    } else {
        throw CompilerException("Expression"\
            " has no defining token\n");
    }
}

void TypeVisitor::check_num(
    std::optional<Token> tok, const FullType& type)
{
    if (tok) {
        auto op = tok.value();
        if (not type.is_num())
            throw SemanticException("Expression \'"
                + op.text + "\' at " + op.pos_string()
                + ", has invalid type " + type.to_string()
                + "; expected a numerical type");
    } else {
        throw CompilerException("Expression"\
            " has no defining token\n");
    }
}

void TypeVisitor::check_image(
    std::optional<Token> tok, const FullType& type)
{
    if (tok) {
        auto op = tok.value();
        if (type.type != ExprType::Image)
            throw SemanticException("Expression \'"
                + op.text + "\' at " + op.pos_string()
                + ", has invalid type " + type.to_string()
                + "; expected an image");
    } else {
        throw CompilerException("Expression"\
            " has no defining token\n");
    }
}

void TypeVisitor::check_path(
    std::optional<Token> tok, const FullType& type)
{
    if (tok) {
        auto op = tok.value();
        if (type.type != ExprType::Path)
            throw SemanticException("Expression \'"
                + op.text + "\' at " + op.pos_string()
                + ", has invalid type " + type.to_string()
                + "; expected a path");
    } else {
        throw CompilerException("Expression"\
            " has no defining token\n");
    }
}

void TypeVisitor::check_id(
    std::optional<Token> tok, const FullType& type)
{
    if (tok) {
        auto op = tok.value();
        if (type.type != ExprType::Id)
            throw SemanticException("Expression \'"
                + op.text + "\' at " + op.pos_string()
                + ", has invalid type " + type.to_string()
                + "; expected an identifier");
    } else {
        throw CompilerException("Expression"\
            " has no defining token\n");
    }
}

void TypeVisitor::check_dimensions(
    std::optional<Token> tok, const FullType& type)
{
    if (tok) {
        auto op = tok.value();
        if (type.type != ExprType::Dimensions)
            throw SemanticException("Expression \'"
                + op.text + "\' at " + op.pos_string()
                + ", has invalid type " + type.to_string()
                + "; expected dimensions");
    } else {
        throw CompilerException("Expression"\
            " has no defining token\n");
    }
}

void TypeVisitor::check_section(
    std::optional<Token> tok, const FullType& type)
{
    if (tok) {
        auto op = tok.value();
        if (type.type != ExprType::Section)
            throw SemanticException("Expression \'"
                + op.text + "\' at " + op.pos_string()
                + ", has invalid type " + type.to_string()
                + "; expected a section");
    } else {
        throw CompilerException("Expression"\
            " has no defining token\n");
    }
}
