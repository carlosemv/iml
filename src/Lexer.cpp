#include "Lexer.h"

#include <utility>

std::map<Token::t_type, std::string> Lexer::token_names = {
    {Token::INVALID_T, "INVALID"},
    {EOF_T, "EOF"}
};

Lexer::Lexer(std::string _in) : input(std::move(_in)), pos(0), line(1), col(1)
{
    if (input.empty())
        curr_char = EOF;
    else
        curr_char = input[0];
}

void Lexer::skip()
{
    if (curr_char == EOF) return;

    pos++; col++;
    if (pos >= input.length())
        curr_char = EOF;
    else
        curr_char = input[pos];
}

bool Lexer::match(char x)
{
    if (curr_char != x) return false;

    skip();
    return true;
}

bool Lexer::match(const std::function<bool()>& predicate)
{
    if (not predicate()) return false;

    skip();
    return true;
}

std::string Lexer::text_from(unsigned pos_begin) const
{
    return input.substr(pos_begin, pos-pos_begin);
}

Token Lexer::EOF_token(unsigned line, unsigned col)
{
    return Token(EOF_T, "<<EOF>>", line, col);
}
