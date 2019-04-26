#include "TypeVisitor.h"
#include "ProgramLexer.h"
#include "CompilerExceptions.h"

TypeVisitor::TypeVisitor() : sym_table(1)
{}

void TypeVisitor::visit(CropNode& node)
{
    node.section.get()->visit(*this);
    node.image.get()->visit(*this);
}

void TypeVisitor::visit(DimensionsNode& node)
{
    node.width.get()->visit(*this);
    node.height.get()->visit(*this);
}

void TypeVisitor::visit(ExportNode& node)
{
    node.image.get()->visit(*this);
    node.path.get()->visit(*this);
}

void TypeVisitor::visit(FlipNode& node)
{
    node.image.get()->visit(*this);
}

void TypeVisitor::visit(ForNode& node)
{
    sym_table.emplace_front();

    node.iterator.get()->visit(*this);
    node.path.get()->visit(*this);

    for (auto& cmd : node.cmds) {
        cmd.get()->visit(*this);
    }

    sym_table.pop_front();
}

void TypeVisitor::visit(ImportNode& node)
{
    node.path.get()->visit(*this);
    auto path_type = node.path.get()->ftype;
    if (path_type.type != ExprType::Path)
        throw SemanticException("Expression has invalid type "
            + path_type.to_string() + "; expected a Path");

    node.ftype.type = ExprType::Image;
}

void TypeVisitor::visit(ModifyNode& node)
{
    node.image.get()->visit(*this);
    node.factor.get()->visit(*this);
}

void TypeVisitor::visit(ResizeNode& node)
{
    node.image.get()->visit(*this);
    node.resize.get()->visit(*this);
}

void TypeVisitor::visit(RotateNode& node)
{
    node.image.get()->visit(*this);
    node.rotation.get()->visit(*this);
}

void TypeVisitor::visit(SectionNode& node)
{
    node.left.get()->visit(*this);
    node.upper.get()->visit(*this);
    node.right.get()->visit(*this);
    node.lower.get()->visit(*this);
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
