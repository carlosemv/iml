#ifndef __UN_OP_NODE_H__
#define __UN_OP_NODE_H__

#include "ExprNode.h"
#include <memory>

class ASTVisitor;
class UnOpNode : public ExprNode
{
public:
    UnOpNode(Token op_tok, const ExprNode& _expr);
    UnOpNode(Token op_tok, std::unique_ptr<ExprNode> _expr);

    void visit(ASTVisitor& visitor);
    std::unique_ptr<ExprNode> expr;
};

#endif
