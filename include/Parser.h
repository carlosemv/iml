#ifndef __PARSER_H__
#define __PARSER_H__

#include <iostream>
#include <initializer_list>
#include <memory>
#include <vector>
#include <string>

#include "Lexer.h"
#include "AST/ProgramNode.h"

class Parser
{
public:
    explicit Parser(std::unique_ptr<Lexer> _lex);
    Token curr_token;

    Token peek(unsigned la);
    virtual ProgramNode parse() = 0;

protected:
    std::unique_ptr<Lexer> lex;
    std::vector<Token> input;
    unsigned pos;

    void skip();
    bool match(Token::t_type type);
    bool match(std::initializer_list<Token::t_type> types);
    bool match(std::function<bool()> predicate);

    std::string error_msg(std::string expected, Token found);
};

#endif
