#include "PrintVisitor.h"
#include "CompilerExceptions.h"
#include "ProgramLexer.h"

void PrintVisitor::visit(CropNode& node)
{
    std::cout << "crop ";
    node.section->visit(*this);
    std::cout << " from ";
    node.image->visit(*this);
}

void PrintVisitor::visit(DimensionsNode& node)
{
    std::cout << "(";
    node.width->visit(*this);
    std::cout << ", ";
    node.height->visit(*this);
    std::cout << ")";
}

void PrintVisitor::visit(ExportNode& node)
{
    std::cout << "save ";
    node.image->visit(*this);
    std::cout << " as ";
    node.path->visit(*this);
}

void PrintVisitor::visit(FlipNode& node)
{
    std::cout << "flip ";
    node.image->visit(*this);
    if (node.dir == FlipDirection::Vertical)
        std::cout << " vertically";
    else
        std::cout << " horizontally";
}

void PrintVisitor::visit(IfNode& node)
{
    indent++;

    std::cout << "if ";
    node.condition->visit(*this);

    std::cout << " {" << std::endl;
    for (auto& cmd : node.cmds) {
        for (auto i = 0; i < indent; ++i)
            std::cout << "    ";
        cmd->visit(*this);
        std::cout << std::endl;
    }
    for (auto i = 0; i < indent-1; ++i)
        std::cout << "    ";
    std::cout << "}";

    if (not node.else_body.empty()) {
        std::cout << " else ";
        std::cout << " {" << std::endl;
        for (auto& cmd : node.else_body) {
            for (auto i = 0; i < indent; ++i)
                std::cout << "    ";
            cmd->visit(*this);
            std::cout << std::endl;
        }
        for (auto i = 0; i < indent-1; ++i)
            std::cout << "    ";
        std::cout << "}";
    }

    indent--;
}

void PrintVisitor::visit(FunctionNode& node)
{
    std::cout << node.name->token.value().text;
}

void PrintVisitor::visit(CallNode& node)
{
    std::cout << node.func->token.value().text;
}

void PrintVisitor::visit(ForNode& node)
{
    indent++;

    std::cout << "for ";
    if (node.recursive)
        std::cout << "all ";
    node.iterator->visit(*this);
    std::cout << " in ";
    node.path->visit(*this);

    std::cout << " {" << std::endl;
    for (auto& cmd : node.cmds) {
        for (auto i = 0; i < indent; ++i)
            std::cout << "    ";
        cmd->visit(*this);
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
    node.path->visit(*this);
}

void PrintVisitor::visit(ModifyNode& node)
{
    std::cout << "modify ";
    node.image->visit(*this);
    std::cout << " " << node.mod_name() << " by ";
    node.factor->visit(*this);
}

void PrintVisitor::visit(ResizeNode& node)
{
    std::cout << "resize ";
    node.image->visit(*this);
    if (node.resize_type == ResizeType::Absolute)
        std::cout << " to ";
    else
        std::cout << " by ";
    node.resize->visit(*this);
}

void PrintVisitor::visit(RotateNode& node)
{
    std::cout << "rotate ";
    node.image->visit(*this);
    std::cout << " by ";
    node.rotation->visit(*this);
}

void PrintVisitor::visit(SectionNode& node)
{
    std::cout << "(";
    node.left->visit(*this);
    std::cout << ", ";
    node.upper->visit(*this);
    std::cout << ", ";
    node.right->visit(*this);
    std::cout << ", ";
    node.lower->visit(*this);
    std::cout << ")";
}

void PrintVisitor::visit(UnOpNode& node)
{
    if (node.token) {
        auto type = node.token.value().type;
        if (type == ProgramLexer::UNMINUS_T) {
            std::cout << node.token.value().text;
            node.expr->visit(*this);
        } else {
            node.expr->visit(*this);
            std::cout << "(" << node.token.value().text << ")";
        }
    } else {
        throw CompilerException("Unary operation"\
            " has no defining token\n");
    }
}

void PrintVisitor::visit(AssignNode& node)
{
    node.id->visit(*this);
    std::cout << " = ";
    node.expr->visit(*this);
}

void PrintVisitor::visit(PrintNode& node)
{
    std::cout << "print ";
    node.expr->visit(*this);
}

void PrintVisitor::visit(BinOpNode& node)
{
    node.lhs->visit(*this);
    if (node.token) {
        std::cout << " ";
        std::cout << node.token.value().text;
        std::cout << " ";
    } else {
        throw CompilerException("Binary operation"\
            " has no defining token\n");
    }
    node.rhs->visit(*this);
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
        cmd->visit(*this);
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
