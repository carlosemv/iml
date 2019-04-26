#ifndef __TYPE_VISITOR_H__
#define __TYPE_VISITOR_H__

#include <iostream>
#include <forward_list>
#include <map>
#include "AST/ASTVisitor.h"

class TypeVisitor : public ASTVisitor
{
public:
    TypeVisitor();

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
    std::forward_list<std::map<std::string, FullType>> sym_table;
    static void check_num(std::optional<Token> tok, FullType type);
    static void check_image(std::optional<Token> tok, FullType type);
    static void check_path(std::optional<Token> tok, FullType type);
    static void check_id(std::optional<Token> tok, FullType type);
    static void check_dimensions(std::optional<Token> tok, FullType type);
    static void check_section(std::optional<Token> tok, FullType type);
    FullType binop_type(Token op, FullType lhs, FullType rhs);
};

#endif