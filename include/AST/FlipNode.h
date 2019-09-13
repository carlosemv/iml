#ifndef __FLIP_NODE_H__
#define __FLIP_NODE_H__

#include "ExprNode.h"
#include <memory>

enum class FlipDirection
{
    Vertical,
    Horizontal
};

class ASTVisitor;
class FlipNode : public ExprNode
{
public:
    FlipNode(Token _t, FlipDirection _dir, const ExprNode& _img);
    FlipNode(Token _t, FlipDirection _dir, expr_ptr _img);
    void visit(ASTVisitor& visitor);

    FlipDirection dir;
    expr_ptr image;
};

#endif
