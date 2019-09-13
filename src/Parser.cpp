#include "Parser.h"

Parser::Parser(std::unique_ptr<Lexer> _lex) : lex(std::move(_lex)), pos(0)
{
    Token t;
    do {
        t = lex->next_token();
        input.push_back(t);
    } while (t.type != Lexer::EOF_T);


    if (input.empty())
        curr_token = Lexer::EOF_token(0);
    else
        curr_token = input[0];
}

void Parser::skip()
{
    if (curr_token.type == Lexer::EOF_T) return;

    pos++;
    try {
        curr_token = input.at(pos);
    } catch (std::out_of_range& e) {
        std::cerr << "Reached end of input,"
            << " but no <<EOF>> token was found.\n";
        curr_token = Lexer::EOF_token();
    }
}

bool Parser::match(Token::t_type type)
{
    if (curr_token.type != type) return false;

    skip();
    return true;
}

bool Parser::match(std::initializer_list<Token::t_type> types)
{
    for (auto t : types) {
        if (curr_token.type == t) {
            skip();
            return true;
        }
    }

    return false;
}

bool Parser::match(const std::function<bool()>& predicate)
{
    if (not predicate()) return false;

    skip();
    return true;
}

Token Parser::peek(unsigned la)
{
    if (pos + la < input.size())
        return input.at(pos+la);
    else
        return input.back();
}

std::string Parser::error_msg(const std::string& expected, const Token& found)
{
    std::string err("Expected \"");
    err += expected + "\" but found \"" + lex->token_name(found.type);
    err += "\" in line " + std::to_string(found.line);
    err += ", column " + std::to_string(found.col);
    return err;
}
