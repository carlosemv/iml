#ifndef __TYPE_VISITOR_H__
#define __TYPE_VISITOR_H__

#include <iostream>
#include <map>
#include "ASTVisitor.h"

class TypeVisitor : public ASTVisitor
{
public:
    void visit(AssignNode& node);
    void visit(BinOpNode& node);
    void visit(CropNode& node);
    void visit(DimensionsNode& node);
    void visit(ExportNode& node);
    void visit(FlipNode& node);
    void visit(ForNode& node);
    void visit(IdNode& node);
    void visit(ImportNode& node);
    void visit(ModifyNode& node);
    void visit(PrintNode& node);
    void visit(ProgramNode& node);
    void visit(ResizeNode& node);
    void visit(RotateNode& node);
    void visit(ScalarNode& node);
    void visit(SectionNode& node);
    void visit(UnOpNode& node);
private:
    std::map<std::string, FullType> sym_table;
    FullType op_type(Token op, FullType lhs, FullType rhs);
};

#endif