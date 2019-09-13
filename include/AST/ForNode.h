#ifndef __FOR_NODE_H__
#define __FOR_NODE_H__

#include "ExprNode.h"
#include "IdNode.h"
#include "ProgramNode.h"

#include <vector>
#include <memory>

class ASTVisitor;
class ForNode : public ProgramNode
{
public:
    ForNode(Token _t, bool _rec, const IdNode& _id,
    	std::unique_ptr<ExprNode> _path,
        std::vector<std::unique_ptr<CommandNode>> _cmds);
    void visit(ASTVisitor& visitor);

    bool recursive;
    std::unique_ptr<IdNode> iterator;
    std::unique_ptr<ExprNode> path;
};

#endif
