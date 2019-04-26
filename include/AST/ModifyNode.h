#ifndef __MODIFY_NODE_H__
#define __MODIFY_NODE_H__

#include <memory>
#include "ExprNode.h"

enum class Modification
{
    Sharpness,
    Brightness,
    Contrast,
    Color
};

class ASTVisitor;
class ModifyNode : public ExprNode
{
public:
    ModifyNode(Token _t, Modification _mod, ExprNode _img, ExprNode _factor);
    ModifyNode(Token _t, Modification _mod, expr_ptr _img, expr_ptr _factor);
    void visit(ASTVisitor& visitor);

    Modification mod;
    expr_ptr image, factor;
};

#endif
