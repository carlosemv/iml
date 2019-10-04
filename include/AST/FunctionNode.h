#ifndef __FUNCTION_NODE_H__
#define __FUNCTION_NODE_H__

#include "IdNode.h"
#include "ProgramNode.h"

#include <vector>
#include <memory>

class ASTVisitor;
class FunctionNode : public ProgramNode
{
public:
    FunctionNode(Token _t, const IdNode& _id,
    	std::vector<std::unique_ptr<IdNode>> _params,
        std::vector<std::unique_ptr<CommandNode>> _cmds);
    void visit(ASTVisitor& visitor);

    std::unique_ptr<IdNode> name;
    std::vector<std::unique_ptr<IdNode>> params;
};

#endif
