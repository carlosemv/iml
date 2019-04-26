#ifndef __AST_VISITOR_H__
#define __AST_VISITOR_H__

#include "ProgramNode.h"
#include "AssignNode.h"
#include "PrintNode.h"
#include "BinOpNode.h"
#include "IdNode.h"
#include "ScalarNode.h"

class ProgramNode;
class ASTVisitor
{
public:
    void visit(ExprNode& node);
    void visit(CommandNode& node);
    virtual void visit(AssignNode& node) = 0;
    virtual void visit(PrintNode& node) = 0;
    virtual void visit(BinOpNode& node) = 0;
    virtual void visit(IdNode& node) = 0;
    virtual void visit(ProgramNode& node) = 0;
    virtual void visit(ScalarNode& node) = 0;
};

#endif
