#ifndef __CROP_NODE_H__
#define __CROP_NODE_H__

#include <memory>
#include "ExprNode.h"

class ASTVisitor;
class CropNode : public ExprNode
{
public:
    CropNode(Token _t, ExprNode _section, ExprNode _img);
    CropNode(Token _t, expr_ptr _section, expr_ptr _img);
    void visit(ASTVisitor& visitor);

    expr_ptr section, image;
};

#endif
