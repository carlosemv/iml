#ifndef __PYTHON_VISITOR_H__
#define __PYTHON_VISITOR_H__

#include <iostream>
#include <sstream>
#include "ASTVisitor.h"

class PythonVisitor : public ASTVisitor
{
public:
    std::ostringstream output;

    void visit(AssignNode& node);
    void visit(PrintNode& node);
    void visit(BinOpNode& node);
    void visit(IdNode& node);
    void visit(ProgramNode& node);
    void visit(ListNode& node);
    void visit(ScalarNode& node);
private:
    static const char* prog_header;
};

#endif