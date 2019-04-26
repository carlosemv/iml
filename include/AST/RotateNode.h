#ifndef __ROTATE_NODE_H__
#define __ROTATE_NODE_H__

#include <memory>
#include "ExprNode.h"

class ASTVisitor;
class RotateNode : public ExprNode
{
public:
    RotateNode(Token _t, ExprNode _img, ExprNode _rot);
    RotateNode(Token _t, expr_ptr _img, expr_ptr _rot);
    void visit(ASTVisitor& visitor);

    expr_ptr image, rotation;
};

#endif
