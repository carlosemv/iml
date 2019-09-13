#ifndef __LEXER_H__
#define __LEXER_H__

#include "Token.h"
#include <functional>
#include <map>
#include <string>

class Lexer
{
public:
    static constexpr Token::t_type EOF_T = 0;
    static constexpr Token::t_type LAST_T = EOF_T;
    static std::map<Token::t_type, std::string> token_names;

    static Token EOF_token(unsigned line = 0, unsigned col = 0);

    virtual Token next_token() = 0;
    virtual std::string token_name(Token::t_type type) = 0;
    virtual ~Lexer() = default;

protected:
    std::string input;
    unsigned pos;
    unsigned line, col;
    char curr_char;

    explicit Lexer(std::string _in);
    void skip();
    bool match(char x);
    bool match(const std::function<bool()>& predicate);
    [[nodiscard]] std::string text_from(unsigned pos_begin) const;
};

#endif
