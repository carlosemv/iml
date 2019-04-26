#ifndef __PRINT_VISITOR_H__
#define __PRINT_VISITOR_H__

#include <iostream>
#include "ASTVisitor.h"

class PrintVisitor : public ASTVisitor
{
public:
    void visit(AssignNode& node);
    void visit(PrintNode& node);
    void visit(BinOpNode& node);
    void visit(IdNode& node);
    void visit(ProgramNode& node);
    void visit(ListNode& node);
    void visit(ScalarNode& node);
};

#endif