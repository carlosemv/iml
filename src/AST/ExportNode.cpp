#include "AST/ExportNode.h"

#include "AST/ASTVisitor.h"
#include <utility>

ExportNode::ExportNode(Token _t, const ExprNode& _img, const ExprNode& _path) :
    CommandNode(std::move(_t)), image(std::make_unique<ExprNode>(_img)),
    path(std::make_unique<ExprNode>(_path))
{}

ExportNode::ExportNode(Token _t, expr_ptr _img, expr_ptr _path) :
    CommandNode(std::move(_t)), image(std::move(_img)), path(std::move(_path))
{}

void ExportNode::visit(ASTVisitor& visitor)
{ 
    visitor.visit(*this);
}