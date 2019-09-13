#ifndef __MODIFY_NODE_H__
#define __MODIFY_NODE_H__

#include "ExprNode.h"
#include <memory>
#include <string>

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
    ModifyNode(Token _t, Modification _mod,
        const ExprNode& _img, const ExprNode& _factor);
    ModifyNode(Token _t, Modification _mod,
        expr_ptr _img, expr_ptr _factor);
    void visit(ASTVisitor& visitor);

    std::string mod_name();
    Modification mod;
    expr_ptr image, factor;
};

#endif
