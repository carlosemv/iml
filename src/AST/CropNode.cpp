#include "AST/CropNode.h"
#include "AST/ASTVisitor.h"

CropNode::CropNode(Token _t, ExprNode _section, ExprNode _img) :
    ExprNode(_t, ExprType::Image), section(std::make_unique<ExprNode>(_section)),
    image(std::make_unique<ExprNode>(_img))
{}

CropNode::CropNode(Token _t, expr_ptr _section, expr_ptr _img) :
    ExprNode(_t, ExprType::Image), section(std::move(_section)),
    image(std::move(_img))
{}

void CropNode::visit(ASTVisitor& visitor)
{ 
    visitor.visit(*this);
}