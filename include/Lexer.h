#ifndef __LEXER_H__
#define __LEXER_H__

#include <string>
#include <functional>
#include <map>
#include "Token.h"

class Lexer
{
public:
    // static constexpr char EOF = char_traits<char>::eof();
    static constexpr Token::t_type EOF_T = 0;
    static constexpr Token::t_type LAST_T = EOF_T;
    static std::map<Token::t_type, std::string> token_names;

    static Token EOF_token(unsigned line=0, unsigned col=0);

    virtual Token next_token() = 0;
    virtual std::string token_name(Token::t_type type) = 0;

protected:
    std::string input;
    unsigned pos;
    unsigned line, col;
    char curr_char;

    explicit Lexer(std::string _in);
    void skip();
    bool match(char x);
    bool match(std::function<bool()> predicate);
    std::string text_from(unsigned pos_begin) const;
};

#endif
