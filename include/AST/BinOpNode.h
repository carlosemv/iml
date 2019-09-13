#ifndef __BIN_OP_NODE_H__
#define __BIN_OP_NODE_H__

#include "ExprNode.h"
#include <memory>

class ASTVisitor;
class BinOpNode : public ExprNode
{
public:
    BinOpNode(ExprNode _lhs, Token op_tok, ExprNode _rhs);
    BinOpNode(std::unique_ptr<ExprNode> _lhs, Token op_tok,
        std::unique_ptr<ExprNode> _rhs);

    void visit(ASTVisitor& visitor);

    std::unique_ptr<ExprNode> lhs, rhs;
};

#endif
