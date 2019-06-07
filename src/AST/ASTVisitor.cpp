#include "AST/ASTVisitor.h"
#include "ProgramLexer.h"
#include "CompilerExceptions.h"
#include <iostream>

void ASTVisitor::visit(ExprNode& node)
{
    if (node.token) {
        switch (node.token.value().type) {
            case ProgramLexer::INTEGER_T:
            case ProgramLexer::FLOAT_T:
            case ProgramLexer::PATH_T:
                cast_visit<ScalarNode&>(node, "ScalarNode");
                break;
            case ProgramLexer::SECTION_T:
                cast_visit<SectionNode&>(node, "SectionNode");
                break;
            case ProgramLexer::DIMS_T:
                cast_visit<DimensionsNode&>(node, "DimensionsNode");
                break;
            case ProgramLexer::ID_T:
                cast_visit<IdNode&>(node, "IdNode");
                break;
            case ProgramLexer::PLUS_T:
            case ProgramLexer::MINUS_T:
            case ProgramLexer::MULT_T:
            case ProgramLexer::DIV_T:
                cast_visit<BinOpNode&>(node, "BinOpNode");
                break;
            case ProgramLexer::UNMINUS_T:
            case ProgramLexer::DIMENSIONS_T:
            case ProgramLexer::R_T:
            case ProgramLexer::G_T:
            case ProgramLexer::B_T:
                cast_visit<UnOpNode&>(node, "UnOpNode");
                break;
            case ProgramLexer::IMAGE_T:
                cast_visit<ImportNode&>(node, "ImportNode");
                break;
            case ProgramLexer::ROTATE_T:
                cast_visit<RotateNode&>(node, "RotateNode");
                break;
            case ProgramLexer::RESIZE_T:
                cast_visit<ResizeNode&>(node, "ResizeNode");
                break;
            case ProgramLexer::CROP_T:
                cast_visit<CropNode&>(node, "CropNode");
                break;
            case ProgramLexer::FLIP_T:
                cast_visit<FlipNode&>(node, "FlipNode");
                break;
            case ProgramLexer::MODIFY_T:
                cast_visit<ModifyNode&>(node, "ModifyNode");
                break;
            default:
            {
                auto name = ProgramLexer::get_token_name(
                    node.token.value().type);
                throw CompilerException("Expression has "\
                    "unkown associated token type \""+name+"\"\n");
            }
        }
    } else {
        throw CompilerException("Expression "\
            " has no defining token\n");
    }
}

void ASTVisitor::visit(CommandNode& node)
{
    if (node.token) {
        switch (node.token.value().type) {
            case ProgramLexer::ASSIGN_T:
                cast_visit<AssignNode&>(node, "AssignNode");
                break;
            case ProgramLexer::PRINT_T:
                cast_visit<PrintNode&>(node, "PrintNode");
                break;
            case ProgramLexer::SAVE_T:
                cast_visit<ExportNode&>(node, "ExportNode");
                break;
            case ProgramLexer::FOR_T:
                cast_visit<ForNode&>(node, "ForNode");
                break;
            case ProgramLexer::ROTATE_T:
            case ProgramLexer::RESIZE_T:
            case ProgramLexer::CROP_T:
            case ProgramLexer::FLIP_T:
            case ProgramLexer::MODIFY_T:
                try {
                    auto& expr = dynamic_cast<ExprNode&>(node);
                    expr.command = true;
                    visit(expr);
                } catch (std::bad_cast& e) {
                    std::cerr << e.what() << " to ExprNode\n";
                }
                break;
            default:
                auto name = ProgramLexer::get_token_name(
                    node.token.value().type);
                throw CompilerException("Command has "\
                    "unkown associated token type \""+name+"\"\n");
        }
    } else {
        throw CompilerException("Command "\
            " has no defining token\n");
    }
}
