#ifndef __IF_NODE_H__
#define __IF_NODE_H__

#include "ExprNode.h"
#include "ProgramNode.h"

#include <vector>
#include <memory>

class ASTVisitor;
class IfNode : public ProgramNode
{
public:
    IfNode(Token _t, std::unique_ptr<ExprNode> _condition,
        std::vector<std::unique_ptr<CommandNode>> _if_body,
        std::vector<std::unique_ptr<CommandNode>> _else_body = {});
    void visit(ASTVisitor& visitor);

    std::unique_ptr<ExprNode> condition;
    std::vector<std::unique_ptr<CommandNode>> else_body;
};

#endif
