#include "AST/ModifyNode.h"
#include "AST/ASTVisitor.h"

ModifyNode::ModifyNode(Token _t, Modification _mod,
    ExprNode _img, ExprNode _factor) :
    ExprNode(_t, ExprType::Image), mod(_mod),
    image(std::make_unique<ExprNode>(_img)),
    factor(std::make_unique<ExprNode>(_factor))
{}

ModifyNode::ModifyNode(Token _t, Modification _mod,
    expr_ptr _img, expr_ptr _factor) :
    ExprNode(_t, ExprType::Image), mod(_mod),
    image(std::move(_img)),
    factor(std::move(_factor))
{}

std::string ModifyNode::mod_name()
{
    switch (mod) {
        case Sharpness:
            return "sharpness";
        case Brightness:
            return "brightness";
        case Contrast:
            return "contrast";
        case Color:
            return "color";
        default:
            return "?";
    }
}

void ModifyNode::visit(ASTVisitor& visitor)
{ 
    visitor.visit(*this);
}