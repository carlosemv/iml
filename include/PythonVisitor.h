#ifndef __PYTHON_VISITOR_H__
#define __PYTHON_VISITOR_H__

#include "AST/ASTVisitor.h"
#include <iostream>
#include <sstream>

class PythonVisitor : public ASTVisitor
{
public:
    std::ostringstream output;

    PythonVisitor();

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
    static const char* prog_header;
    int indent{0};
};

#endif