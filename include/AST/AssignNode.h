#ifndef __ASSIGN_NODE_H__
#define __ASSIGN_NODE_H__

#include <memory>
#include "IdNode.h"
#include "CommandNode.h"

class ASTVisitor;
class AssignNode : public CommandNode
{
public:
    AssignNode(Token _t, IdNode _id, ExprNode _expr);
    AssignNode(Token _t, IdNode _id, 
        std::unique_ptr<ExprNode> _expr);
    void visit(ASTVisitor& visitor);

    std::unique_ptr<IdNode> id;
    std::unique_ptr<ExprNode> expr;
};

#endif
