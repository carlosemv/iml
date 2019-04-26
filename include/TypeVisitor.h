#ifndef __TYPE_VISITOR_H__
#define __TYPE_VISITOR_H__

#include <iostream>
#include <map>
#include "ASTVisitor.h"

class TypeVisitor : public ASTVisitor
{
public:
    void visit(AssignNode& node);
    void visit(PrintNode& node);
    void visit(BinOpNode& node);
    void visit(IdNode& node);
    void visit(ProgramNode& node);
    void visit(ListNode& node);
    void visit(ScalarNode& node);
private:
    std::map<std::string, FullType> sym_table;
    FullType op_type(Token op, FullType lhs, FullType rhs);
};

#endif