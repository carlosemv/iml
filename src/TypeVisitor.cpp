#include "TypeVisitor.h"
#include "ProgramLexer.h"
#include "CompilerExceptions.h"

TypeVisitor::TypeVisitor() : sym_table(1)
{}

void TypeVisitor::visit(CropNode& node)
{
    if (node.command) {
        check_id(node.image.get()->token,
            node.image.get()->ftype);
    }

    node.section.get()->visit(*this);
    check_section(node.section.get()->token,
        node.section.get()->ftype);

    node.image.get()->visit(*this);
    check_image(node.image.get()->token,
        node.image.get()->ftype);

    node.ftype = FullType(ExprType::Image);
}

void TypeVisitor::visit(DimensionsNode& node)
{
    node.width.get()->visit(*this);
    check_num(node.width.get()->token,
        node.width.get()->ftype);

    node.height.get()->visit(*this);
    check_num(node.height.get()->token,
        node.height.get()->ftype);

    node.ftype = FullType(ExprType::Dimensions);
    node.ftype.list_types = {node.width.get()->ftype,
        node.height.get()->ftype};
}

void TypeVisitor::visit(ExportNode& node)
{
    node.image.get()->visit(*this);
    check_image(node.image.get()->token,
        node.image.get()->ftype);

    node.path.get()->visit(*this);
    check_path(node.path.get()->token,
        node.path.get()->ftype);
}

void TypeVisitor::visit(FlipNode& node)
{
    if (node.command) {
        check_id(node.image.get()->token,
            node.image.get()->ftype);
    }

    node.image.get()->visit(*this);
    check_image(node.image.get()->token,
        node.image.get()->ftype);

    node.ftype = FullType(ExprType::Image);
}

void TypeVisitor::visit(ForNode& node)
{
    sym_table.emplace_front();

    node.path.get()->visit(*this);
    check_path(node.path.get()->token,
        node.path.get()->ftype);

    auto iter = node.iterator.get()->token.value().text;
    sym_table.front()[iter] = FullType(ExprType::Path);
    for (auto& cmd : node.cmds) {
        cmd.get()->visit(*this);
    }

    sym_table.pop_front();
}

void TypeVisitor::visit(ImportNode& node)
{
    node.path.get()->visit(*this);
    check_path(node.path.get()->token,
        node.path.get()->ftype);

    node.ftype = FullType(ExprType::Image);
}

void TypeVisitor::visit(ModifyNode& node)
{
    if (node.command) {
        check_id(node.image.get()->token,
            node.image.get()->ftype);
    }

    node.image.get()->visit(*this);
    check_image(node.image.get()->token,
        node.image.get()->ftype);

    node.factor.get()->visit(*this);
    check_num(node.factor.get()->token,
        node.factor.get()->ftype);

    node.ftype = FullType(ExprType::Image);
}

void TypeVisitor::visit(ResizeNode& node)
{
    if (node.command) {
        check_id(node.image.get()->token,
            node.image.get()->ftype);
    }
    
    node.image.get()->visit(*this);
    check_image(node.image.get()->token,
        node.image.get()->ftype);

    node.resize.get()->visit(*this);
    if (node.resize_type == ResizeType::Absolute) {
        check_dimensions(node.resize.get()->token,
            node.resize.get()->ftype);
    } else {
        check_num(node.resize.get()->token,
            node.resize.get()->ftype);
    }

    node.ftype = FullType(ExprType::Image);
}

void TypeVisitor::visit(RotateNode& node)
{
    if (node.command) {
        check_id(node.image.get()->token,
            node.image.get()->ftype);
    }

    node.image.get()->visit(*this);
    check_image(node.image.get()->token,
        node.image.get()->ftype);

    node.rotation.get()->visit(*this);
    check_num(node.rotation.get()->token,
        node.rotation.get()->ftype);

    node.ftype = FullType(ExprType::Image);
}

void TypeVisitor::visit(SectionNode& node)
{
    node.left.get()->visit(*this);
    check_num(node.left.get()->token,
        node.left.get()->ftype);

    node.upper.get()->visit(*this);
    check_num(node.upper.get()->token,
        node.upper.get()->ftype);

    node.right.get()->visit(*this);
    check_num(node.right.get()->token,
        node.right.get()->ftype);

    node.lower.get()->visit(*this);
    check_num(node.lower.get()->token,
        node.lower.get()->ftype);

    node.ftype = FullType(ExprType::Section);
    node.ftype.list_types = {
        node.left.get()->ftype,
        node.upper.get()->ftype,
        node.right.get()->ftype,
        node.lower.get()->ftype
    };
}

void TypeVisitor::visit(UnOpNode& node)
{
    node.expr.get()->visit(*this);
    auto expr_type = node.expr.get()->ftype;

    if (node.token) {
        auto op = node.token.value();
        if (op.type == ProgramLexer::UNMINUS_T) {
            if (expr_type.is_num()) {
                node.ftype.type = expr_type.type;
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
    node.expr.get()->visit(*this);
    node.id.get()->ftype = node.expr.get()->ftype;

    if (not node.id.get()->token)
        throw CompilerException("Variable has no defining token");

    sym_table.front()[node.id.get()->token.value().text] = 
        node.id.get()->ftype;
}

void TypeVisitor::visit(PrintNode& node)
{
    node.expr.get()->visit(*this);
}

FullType TypeVisitor::binop_type(Token op,
    FullType lhs, FullType rhs)
{
    switch (op.type) {
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
                else
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
                else
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

            if (lhs.is_list() and rhs.is_list())
                return FullType(ExprType::Section);
            break;
        case ProgramLexer::DIV_T:
            if (lhs.type == rhs.type) {
                if (lhs.type != ExprType::Path)
                    return lhs;
                else
                    break;
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
    node.lhs.get()->visit(*this);
    node.rhs.get()->visit(*this);

    auto ltype = node.lhs.get()->ftype;
    auto rtype = node.rhs.get()->ftype;

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

void TypeVisitor::visit(ProgramNode& node)
{
    for (auto& cmd : node.cmds)
        cmd.get()->visit(*this);
}

void TypeVisitor::visit(ScalarNode& node)
{}

void TypeVisitor::check_num(std::optional<Token> tok, FullType type)
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

void TypeVisitor::check_image(std::optional<Token> tok, FullType type)
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

void TypeVisitor::check_path(std::optional<Token> tok, FullType type)
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

void TypeVisitor::check_id(std::optional<Token> tok, FullType type)
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

void TypeVisitor::check_dimensions(std::optional<Token> tok, FullType type)
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

void TypeVisitor::check_section(std::optional<Token> tok, FullType type)
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
