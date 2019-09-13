#ifndef __ROTATE_NODE_H__
#define __ROTATE_NODE_H__

#include "ExprNode.h"
#include <memory>

class ASTVisitor;
class RotateNode : public ExprNode
{
public:
    RotateNode(Token _t, const ExprNode& _img, const ExprNode& _rot);
    RotateNode(Token _t, expr_ptr _img, expr_ptr _rot);
    void visit(ASTVisitor& visitor);

    expr_ptr image, rotation;
};

#endif
