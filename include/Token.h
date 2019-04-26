#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>

class Token
{
public:
    using t_type = int;

    static constexpr t_type INVALID_T = -1;

    t_type type;
    std::string text;
    unsigned line, col;

    Token() : type(INVALID_T), text(""), line(0), col(0) {}
    Token(t_type _type) : type(_type), text(""), line(0), col() {}
    Token(t_type _type, std::string _text, unsigned _line, unsigned _col);
    std::string pos_string();
};

#endif
