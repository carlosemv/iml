#ifndef __AST_VISITOR_H__
#define __AST_VISITOR_H__

#include "ProgramNode.h"
#include "AssignNode.h"
#include "PrintNode.h"
#include "BinOpNode.h"
#include "IdNode.h"
#include "ScalarNode.h"
#include "CropNode.h"
#include "DimensionsNode.h"
#include "ExportNode.h"
#include "FlipNode.h"
#include "ForNode.h"
#include "ImportNode.h"
#include "ResizeNode.h"
#include "RotateNode.h"
#include "SectionNode.h"
#include "UnOpNode.h"
#include "ModifyNode.h"

class ProgramNode;
class ASTVisitor
{
public:
    void visit(ExprNode& node);
    void visit(CommandNode& node);
    virtual void visit(AssignNode& node) = 0;
    virtual void visit(BinOpNode& node) = 0;
    virtual void visit(CropNode& node) = 0;
    virtual void visit(DimensionsNode& node) = 0;
    virtual void visit(ExportNode& node) = 0;
    virtual void visit(FlipNode& node) = 0;
    virtual void visit(ForNode& node) = 0;
    virtual void visit(IdNode& node) = 0;
    virtual void visit(ImportNode& node) = 0;
    virtual void visit(ModifyNode& node) = 0;
    virtual void visit(PrintNode& node) = 0;
    virtual void visit(ProgramNode& node) = 0;
    virtual void visit(ResizeNode& node) = 0;
    virtual void visit(RotateNode& node) = 0;
    virtual void visit(ScalarNode& node) = 0;
    virtual void visit(SectionNode& node) = 0;
    virtual void visit(UnOpNode& node) = 0;

protected:
    template <class T, class N>
    void cast_visit(N& node, std::string name)
    {
        try {
            visit(dynamic_cast<T&>(node));
        } catch (std::bad_cast& e) {
            std::cerr << e.what() << " to "+name+"\n";
        }
    }
};

#endif
