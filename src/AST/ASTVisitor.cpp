#include "AST/ASTVisitor.h"
#include "ProgramLexer.h"
#include <iostream>

void ASTVisitor::visit(ExprNode& node)
{
    if (node.token) {
        switch (node.token.value().type) {
            case ProgramLexer::INTEGER_T:
            case ProgramLexer::FLOAT_T:
            case ProgramLexer::PATH_T:
                try {
                    visit(dynamic_cast<ScalarNode&>(node));
                } catch (std::bad_cast& e) {
                    std::cerr << e.what() << " to ScalarNode\n";
                }
                break;
            case ProgramLexer::ID_T:
                try {
                    visit(dynamic_cast<IdNode&>(node));
                } catch (std::bad_cast& e) {
                    std::cerr << e.what() << " to IdNode\n";
                }
                break;
            case ProgramLexer::PLUS_T:
            case ProgramLexer::MINUS_T:
            case ProgramLexer::MULT_T:
            case ProgramLexer::DIV_T:
                try {
                    visit(dynamic_cast<BinOpNode&>(node));
                } catch (std::bad_cast& e) {
                    std::cerr << e.what() << " to BinOpNode\n";
                }
                break;
            default:
                std::cerr << ProgramLexer::get_token_name(
                    node.token.value().type) << std::endl;
                throw std::logic_error("Expression has "\
                    "unkown associated token type\n");
        }
    } else {
        throw std::logic_error("Expression "\
            " has no defining token\n");
    }
}

void ASTVisitor::visit(CommandNode& node)
{
    if (node.token) {
        switch (node.token.value().type) {
            case ProgramLexer::ASSIGN_T:
                try {
                    visit(dynamic_cast<AssignNode&>(node));
                } catch (std::bad_cast& e) {
                    std::cerr << e.what() << " to AssignNode\n";
                }
                break;
            case ProgramLexer::PRINT_T:
                try {
                    visit(dynamic_cast<PrintNode&>(node));
                } catch (std::bad_cast& e) {
                    std::cerr << e.what() << " to PrintNode\n";
                }
                break;
            default:
                std::cerr << ProgramLexer::get_token_name(
                    node.token.value().type) << std::endl;
                throw std::logic_error("Command has "\
                    "unkown associated token type\n");
        }
    } else {
        throw std::logic_error("Command "\
            " has no defining token\n");
    }
}
