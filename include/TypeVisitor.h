#ifndef __TYPE_VISITOR_H__
#define __TYPE_VISITOR_H__

#include "AST/ASTVisitor.h"
#include <functional>
#include <forward_list>
#include <iostream>
#include <map>
#include <string>

class TypeVisitor : public ASTVisitor
{
public:
    TypeVisitor();

    void visit(AssignNode& node);
    void visit(BinOpNode& node);
    void visit(CallNode& node);
    void visit(CropNode& node);
    void visit(DimensionsNode& node);
    void visit(ExportNode& node);
    void visit(FlipNode& node);
    void visit(IfNode& node);
    void visit(FunctionNode& node);
    void visit(ForNode& node);
    void visit(IdNode& node);
    void visit(ImportNode& node);
    void visit(ModifyNode& node);
    void visit(PrintNode& node);
    void visit(ReturnNode& node);
    void visit(ProgramNode& node);
    void visit(ResizeNode& node);
    void visit(RotateNode& node);
    void visit(ScalarNode& node);
    void visit(SectionNode& node);
    void visit(UnOpNode& node);

private:
    std::forward_list<std::map<std::string, FullType>> sym_table;
    static void check_type(std::optional<Token> tok, const FullType& type,
        const std::function<bool(const FullType&)>& predicate,
        std::string expected);

    static void check_bool(std::optional<Token> tok, const FullType& type);
    static void check_num(std::optional<Token> tok, const FullType& type);
    static void check_image(std::optional<Token> tok, const FullType& type);
    static void check_path(std::optional<Token> tok, const FullType& type);
    static void check_id(std::optional<Token> tok, const FullType& type);
    static void check_dimensions(std::optional<Token> tok,
        const FullType& type);
    static void check_section(std::optional<Token> tok,
        const FullType& type);
    FullType binop_type(Token op, FullType lhs, FullType rhs);
};

#endif
