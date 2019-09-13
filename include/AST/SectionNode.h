#ifndef __SECTION_NODE_H__
#define __SECTION_NODE_H__

#include <memory>

#include "ExprNode.h"

class ASTVisitor;
class SectionNode : public ExprNode
{
private:
    using expr_ptr = std::unique_ptr<ExprNode>;  
public:
    SectionNode(Token::t_type _t, const ExprNode& _left, const ExprNode& _upper,
        const ExprNode& _right, const ExprNode& _lower);
    SectionNode(Token::t_type _t, expr_ptr _left, expr_ptr _upper,
        expr_ptr _right, expr_ptr _lower);
    void visit(ASTVisitor& visitor);

    // leftmost, uppermost, rightmost, and lowermost bounds
    expr_ptr left, upper, right, lower;
};
#endif
