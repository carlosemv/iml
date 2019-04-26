#ifndef __DIMENSIONS_NODE_H__
#define __DIMENSIONS_NODE_H__

#include <memory>

#include "ExprNode.h"

class ASTVisitor;
class DimensionsNode : public ExprNode
{
private:
    using expr_ptr = std::unique_ptr<ExprNode>;        
public:
    DimensionsNode(Token::t_type _t, ExprNode _width, ExprNode _height);
    DimensionsNode(Token::t_type _t, expr_ptr _width, expr_ptr _height);
    void visit(ASTVisitor& visitor);

    expr_ptr width, height;
};
#endif
