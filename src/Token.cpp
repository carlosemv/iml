#include "Token.h"

Token::Token(t_type _type, std::string _text, unsigned _line, unsigned _col) :
    type(_type), text(_text), line(_line), col(_col)
{}

std::string Token::pos_string()
{
    std::string out = "line " + std::to_string(line);
    out += ", column " + std::to_string(col);
    return out;
}