#include "AST/ImportNode.h"
#include "AST/ASTVisitor.h"

ImportNode::ImportNode(Token _t, ExprNode _path) :
    ExprNode(_t, ExprType::Image), path(std::make_unique<ExprNode>(_path))
{}

ImportNode::ImportNode(Token _t, std::unique_ptr<ExprNode> _path) :
    ExprNode(_t, ExprType::Image), path(std::move(_path))
{}


void ImportNode::visit(ASTVisitor& visitor)
{ 
    visitor.visit(*this);
}