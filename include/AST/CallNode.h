#ifndef __CALL_NODE_H__
#define __CALL_NODE_H__

#include "ExprNode.h"
#include "IdNode.h"
#include <memory>

class ASTVisitor;
class CallNode : public ExprNode
{
public:
    CallNode(Token _t, const IdNode& _func,
    	std::vector<expr_ptr> _args);
    void visit(ASTVisitor& visitor);

    std::unique_ptr<IdNode> func;
    std::vector<expr_ptr> args;
};

#endif
