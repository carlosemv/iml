#include "AST/ModifyNode.h"

#include "AST/ASTVisitor.h"
#include <utility>

ModifyNode::ModifyNode(Token _t, Modification _mod,
    const ExprNode& _img, const ExprNode& _factor) :
    ExprNode(std::move(_t), ExprType::Image), mod(_mod),
    image(std::make_unique<ExprNode>(_img)),
    factor(std::make_unique<ExprNode>(_factor))
{}

ModifyNode::ModifyNode(Token _t, Modification _mod,
    expr_ptr _img, expr_ptr _factor) :
    ExprNode(std::move(_t), ExprType::Image), mod(_mod),
    image(std::move(_img)),
    factor(std::move(_factor))
{}

std::string ModifyNode::mod_name()
{
    switch (mod) {
        case Modification::Sharpness:
            return "Sharpness";
        case Modification::Brightness:
            return "Brightness";
        case Modification::Contrast:
            return "Contrast";
        case Modification::Color:
            return "Color";
        default:
            return "";
    }
}

void ModifyNode::visit(ASTVisitor& visitor)
{ 
    visitor.visit(*this);
}