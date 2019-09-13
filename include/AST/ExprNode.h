#ifndef __EXPR_NODE_H__
#define __EXPR_NODE_H__

#include "CommandNode.h"
#include "FullType.h"
#include <memory>
#include <vector>

class ASTVisitor;
class ExprNode : public CommandNode
{
protected:
    using expr_ptr = std::unique_ptr<ExprNode>;
public:
    ExprNode(Token _t, ExprType _type, bool _command = false);
    virtual ~ExprNode() = default;
    void visit(ASTVisitor& visitor);

    FullType ftype;
    bool command;
};

#endif
