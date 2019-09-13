#ifndef __AST_H__
#define __AST_H__

#include "Token.h"
#include <optional>
#include <string>

class AST
{
public:
    AST() = default;
    AST(Token _tok) : token(_tok) {}
    AST(Token::t_type _t) : token(Token(_t)) {}

    std::optional<Token> token;
};

#endif
