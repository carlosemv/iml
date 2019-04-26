#ifndef __RESIZE_NODE_H__
#define __RESIZE_NODE_H__

#include <memory>
#include "ExprNode.h"

enum class ResizeType
{
    Absolute,
    Relative
};

class ASTVisitor;
class ResizeNode : public ExprNode
{
public:
    ResizeNode(Token _t, ResizeType _type, ExprNode _img, ExprNode _resize);
    ResizeNode(Token _t, ResizeType _type, expr_ptr _img, expr_ptr _resize);
    void visit(ASTVisitor& visitor);

    ResizeType resize_type;
    expr_ptr image, resize;
};

#endif
