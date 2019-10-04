#include "AST/IfNode.h"

#include "AST/ASTVisitor.h"
#include <utility>

IfNode::IfNode(Token _t, std::unique_ptr<ExprNode> _condition,
    std::vector<std::unique_ptr<CommandNode>> _if_body,
    std::vector<std::unique_ptr<CommandNode>> _else_body) :
    ProgramNode(std::move(_t), std::move(_if_body)),
    condition(std::move(_condition)),
    else_body(std::move(_else_body))
{}

void IfNode::visit(ASTVisitor& visitor)
{
    visitor.visit(*this);
}
