#include "AST/ExportNode.h"
#include "AST/ASTVisitor.h"

ExportNode::ExportNode(Token _t, ExprNode _img, ExprNode _path) :
    CommandNode(_t), image(std::make_unique<ExprNode>(_img)),
    path(std::make_unique<ExprNode>(_path))
{}

ExportNode::ExportNode(Token _t, expr_ptr _img, expr_ptr _path) :
    CommandNode(_t), image(std::move(_img)), path(std::move(_path))
{}

void ExportNode::visit(ASTVisitor& visitor)
{ 
    visitor.visit(*this);
}