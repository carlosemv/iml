#ifndef __RETURN_NODE_H__
#define __RETURN_NODE_H__

#include "CommandNode.h"
#include "ExprNode.h"
#include <memory>

class ASTVisitor;
class ReturnNode : public CommandNode
{
public:
    ReturnNode(Token _t, FullType _func_type,
    	std::unique_ptr<ExprNode> _expr);
    void visit(ASTVisitor& visitor);

    FullType function_type;
    std::unique_ptr<ExprNode> expr;
};

#endif
