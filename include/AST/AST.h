#ifndef __AST_H__
#define __AST_H__

#include <optional>
#include <string>
#include "Token.h"

class AST
{
public:
    AST() {}
    AST(Token _tok) : token(_tok) {}
    AST(Token::t_type _t) : token(Token(_t)) {}

    std::optional<Token> token;
};

#endif
