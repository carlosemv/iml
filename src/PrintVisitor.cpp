#include "PrintVisitor.h"
#include "ProgramLexer.h"
#include "CompilerExceptions.h"

PrintVisitor::PrintVisitor() : indent(0)
{}

void PrintVisitor::visit(CropNode& node)
{
    std::cout << "crop ";
    node.section.get()->visit(*this);
    std::cout << " from ";
    node.image.get()->visit(*this);
}

void PrintVisitor::visit(DimensionsNode& node)
{
    std::cout << "(";
    node.width.get()->visit(*this);
    std::cout << ", ";
    node.height.get()->visit(*this);
    std::cout << ")";
}

void PrintVisitor::visit(ExportNode& node)
{
    std::cout << "save ";
    node.image.get()->visit(*this);
    std::cout << " as ";
    node.path.get()->visit(*this);
}

void PrintVisitor::visit(FlipNode& node)
{
    std::cout << "flip ";
    node.image.get()->visit(*this);
    if (node.dir == FlipDirection::Vertical)
        std::cout << " vertically";
    else
        std::cout << " horizontally";
}

void PrintVisitor::visit(ForNode& node)
{
    indent++;

    std::cout << "for ";
    if (node.recursive)
        std::cout << "all ";
    node.iterator.get()->visit(*this);
    std::cout << " in ";
    node.path.get()->visit(*this);

    std::cout << " {" << std::endl;
    for (auto& cmd : node.cmds) {
        for (auto i = 0; i < indent; ++i)
            std::cout << "    ";
        cmd.get()->visit(*this);
        std::cout << std::endl;
    }
    for (auto i = 0; i < indent-1; ++i)
        std::cout << "    ";
    std::cout << "}";

    indent--;
}

void PrintVisitor::visit(ImportNode& node)
{
    std::cout << "image in ";
    node.path.get()->visit(*this);
}

void PrintVisitor::visit(ModifyNode& node)
{
    std::cout << "modify ";
    node.image.get()->visit(*this);
    std::cout << " " << node.mod_name() << " by ";
    node.factor.get()->visit(*this);
}

void PrintVisitor::visit(ResizeNode& node)
{
    std::cout << "resize ";
    node.image.get()->visit(*this);
    if (node.resize_type == ResizeType::Absolute)
        std::cout << " to ";
    else
        std::cout << " by ";
    node.resize.get()->visit(*this);
}

void PrintVisitor::visit(RotateNode& node)
{
    std::cout << "rotate ";
    node.image.get()->visit(*this);
    std::cout << " by ";
    node.rotation.get()->visit(*this);
}

void PrintVisitor::visit(SectionNode& node)
{
    std::cout << "(";
    node.left.get()->visit(*this);
    std::cout << ", ";
    node.upper.get()->visit(*this);
    std::cout << ", ";
    node.right.get()->visit(*this);
    std::cout << ", ";
    node.lower.get()->visit(*this);
    std::cout << ")";
}

void PrintVisitor::visit(UnOpNode& node)
{
    if (node.token) {
        auto type = node.token.value().type;
        if (type == ProgramLexer::UNMINUS_T) {
            std::cout << node.token.value().text;
            node.expr.get()->visit(*this);
        } else {
            node.expr.get()->visit(*this);
            std::cout << "(" << node.token.value().text << ")";
        }
    } else {
        throw CompilerException("Unary operation"\
            " has no defining token\n");
    }
}

void PrintVisitor::visit(AssignNode& node)
{
    node.id.get()->visit(*this);
    std::cout << " = ";
    node.expr.get()->visit(*this);
}

void PrintVisitor::visit(PrintNode& node)
{
    std::cout << "print ";
    node.expr.get()->visit(*this);
}

void PrintVisitor::visit(BinOpNode& node)
{
    node.lhs.get()->visit(*this);
    if (node.token) {
        std::cout << " ";
        std::cout << node.token.value().text;
        std::cout << " ";
    } else {
        throw CompilerException("Binary operation"\
            " has no defining token\n");
    }
    node.rhs.get()->visit(*this);
}

void PrintVisitor::visit(IdNode& node)
{
    if (node.token) {
        std::cout << node.token.value().text;
    } else {
        throw CompilerException("Id node has no token\n");
    }
}

void PrintVisitor::visit(ProgramNode& node)
{
    for (auto& cmd : node.cmds) {
        cmd.get()->visit(*this);
        std::cout << std::endl;
    }
}

void PrintVisitor::visit(ScalarNode& node)
{
    if (node.token) {
        if (node.ftype.type == ExprType::Path) {
            std::cout << '"';
            std::cout << node.token.value().text;
            std::cout << '"';
        } else {
            std::cout << node.token.value().text;
        }
    } else {
        throw CompilerException("Scalar node has no token\n");
    }  
}
