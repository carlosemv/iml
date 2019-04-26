#ifndef __PRINT_NODE_H__
#define __PRINT_NODE_H__

#include <memory>
#include "CommandNode.h"
#include "ExprNode.h"

class ASTVisitor;
class PrintNode : public CommandNode
{
public:
    PrintNode(Token _t, ExprNode _expr);
    PrintNode(Token _t, std::unique_ptr<ExprNode> _expr);
    void visit(ASTVisitor& visitor);

    std::unique_ptr<ExprNode> expr;
};

#endif
