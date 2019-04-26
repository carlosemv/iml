#ifndef __PROGRAM_PARSER_H__
#define __PROGRAM_PARSER_H__

#include <optional>
#include <memory>
#include <functional>
#include <string>

#include "Parser.h"
#include "ProgramLexer.h"

#include "AST/ProgramNode.h"
#include "AST/CommandNode.h"
#include "AST/AssignNode.h"
#include "AST/PrintNode.h"
#include "AST/ExportNode.h"
#include "AST/ForNode.h"
#include "AST/ExprNode.h"
#include "AST/ModifyNode.h"

class ProgramParser : public Parser
{
public:
    explicit ProgramParser(std::unique_ptr<Lexer> _lex);
    explicit ProgramParser(std::string _in);

    ProgramNode parse();

protected:
    void throw_unexpected(std::string expected);
    void throw_unexpected(Token::t_type expected);

    static std::optional<Modification> get_modification(Token tok);

    std::unique_ptr<AssignNode> assignment();
    std::unique_ptr<PrintNode> print_stmt();
    std::unique_ptr<ExportNode> export_stmt();
    std::unique_ptr<ForNode> for_stmt();

    std::unique_ptr<ExprNode> expression();
    std::unique_ptr<ExprNode> img_expr();
    std::unique_ptr<ExprNode> shape_expr();
    std::unique_ptr<ExprNode> arith_expr();
    std::unique_ptr<ExprNode> term();
    std::unique_ptr<ExprNode> factor();
    std::unique_ptr<ExprNode> atom();
    std::unique_ptr<ExprNode> primary();

    std::optional<std::function<std::unique_ptr<CommandNode>()>> get_command();
};

#endif
