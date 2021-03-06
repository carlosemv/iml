#ifndef __PROGRAM_LEXER_H__
#define __PROGRAM_LEXER_H__

#include "Lexer.h"
#include <map>
#include <optional>
#include <string>
#include <utility>

class ProgramLexer : public Lexer
{
public:
    /*[[[cog
    import cog
    import json

    tok_type = "static constexpr Token::t_type"
    tok_assign = " {} = Lexer::LAST_T + {};"

    def to_id(tok_dict):
        name = tok_dict["id"]
        name = tok_dict.get("token", name)
        return (name + "_t").upper()

    with open('tokens.json') as f:
        tokens = list(json.load(f))
        for i, tok_dict in enumerate(tokens):
            cog.out(tok_type)
            cog.outl(tok_assign.format(to_id(tok_dict), i+1))
        cog.out(tok_type)
        cog.outl(" LAST_T = {};".format(to_id(tokens[-1])))
    ]]]*/
    static constexpr Token::t_type ID_T = Lexer::LAST_T + 1;
    static constexpr Token::t_type COMMENT_T = Lexer::LAST_T + 2;
    static constexpr Token::t_type ASSIGN_T = Lexer::LAST_T + 3;
    static constexpr Token::t_type COMMA_T = Lexer::LAST_T + 4;
    static constexpr Token::t_type LPAREN_T = Lexer::LAST_T + 5;
    static constexpr Token::t_type RPAREN_T = Lexer::LAST_T + 6;
    static constexpr Token::t_type LBRACE_T = Lexer::LAST_T + 7;
    static constexpr Token::t_type RBRACE_T = Lexer::LAST_T + 8;
    static constexpr Token::t_type SEP_T = Lexer::LAST_T + 9;
    static constexpr Token::t_type COLON_T = Lexer::LAST_T + 10;
    static constexpr Token::t_type PLUS_T = Lexer::LAST_T + 11;
    static constexpr Token::t_type MINUS_T = Lexer::LAST_T + 12;
    static constexpr Token::t_type MULT_T = Lexer::LAST_T + 13;
    static constexpr Token::t_type DIV_T = Lexer::LAST_T + 14;
    static constexpr Token::t_type INTEGER_LIT_T = Lexer::LAST_T + 15;
    static constexpr Token::t_type FLOAT_LIT_T = Lexer::LAST_T + 16;
    static constexpr Token::t_type PATH_LIT_T = Lexer::LAST_T + 17;
    static constexpr Token::t_type EQUALS_T = Lexer::LAST_T + 18;
    static constexpr Token::t_type NEQUALS_T = Lexer::LAST_T + 19;
    static constexpr Token::t_type LEQ_T = Lexer::LAST_T + 20;
    static constexpr Token::t_type GEQ_T = Lexer::LAST_T + 21;
    static constexpr Token::t_type LESS_T = Lexer::LAST_T + 22;
    static constexpr Token::t_type GREATER_T = Lexer::LAST_T + 23;
    static constexpr Token::t_type NOT_T = Lexer::LAST_T + 24;
    static constexpr Token::t_type AND_T = Lexer::LAST_T + 25;
    static constexpr Token::t_type OR_T = Lexer::LAST_T + 26;
    static constexpr Token::t_type TRUE_T = Lexer::LAST_T + 27;
    static constexpr Token::t_type FALSE_T = Lexer::LAST_T + 28;
    static constexpr Token::t_type IF_T = Lexer::LAST_T + 29;
    static constexpr Token::t_type ELSE_T = Lexer::LAST_T + 30;
    static constexpr Token::t_type PRINT_T = Lexer::LAST_T + 31;
    static constexpr Token::t_type ROTATE_T = Lexer::LAST_T + 32;
    static constexpr Token::t_type RESIZE_T = Lexer::LAST_T + 33;
    static constexpr Token::t_type TO_T = Lexer::LAST_T + 34;
    static constexpr Token::t_type BY_T = Lexer::LAST_T + 35;
    static constexpr Token::t_type CROP_T = Lexer::LAST_T + 36;
    static constexpr Token::t_type FROM_T = Lexer::LAST_T + 37;
    static constexpr Token::t_type FUNCTION_T = Lexer::LAST_T + 38;
    static constexpr Token::t_type RETURN_T = Lexer::LAST_T + 39;
    static constexpr Token::t_type FOR_T = Lexer::LAST_T + 40;
    static constexpr Token::t_type ALL_T = Lexer::LAST_T + 41;
    static constexpr Token::t_type IN_T = Lexer::LAST_T + 42;
    static constexpr Token::t_type IMAGE_OP_T = Lexer::LAST_T + 43;
    static constexpr Token::t_type SAVE_T = Lexer::LAST_T + 44;
    static constexpr Token::t_type AS_T = Lexer::LAST_T + 45;
    static constexpr Token::t_type FLIP_T = Lexer::LAST_T + 46;
    static constexpr Token::t_type HORIZONTALLY_T = Lexer::LAST_T + 47;
    static constexpr Token::t_type VERTICALLY_T = Lexer::LAST_T + 48;
    static constexpr Token::t_type R_T = Lexer::LAST_T + 49;
    static constexpr Token::t_type G_T = Lexer::LAST_T + 50;
    static constexpr Token::t_type B_T = Lexer::LAST_T + 51;
    static constexpr Token::t_type MODIFY_T = Lexer::LAST_T + 52;
    static constexpr Token::t_type SHARPNESS_T = Lexer::LAST_T + 53;
    static constexpr Token::t_type BRIGHTNESS_T = Lexer::LAST_T + 54;
    static constexpr Token::t_type CONTRAST_T = Lexer::LAST_T + 55;
    static constexpr Token::t_type COLOR_T = Lexer::LAST_T + 56;
    static constexpr Token::t_type IMAGE_T = Lexer::LAST_T + 57;
    static constexpr Token::t_type DIMS_T = Lexer::LAST_T + 58;
    static constexpr Token::t_type SECTION_T = Lexer::LAST_T + 59;
    static constexpr Token::t_type FLOAT_T = Lexer::LAST_T + 60;
    static constexpr Token::t_type INT_T = Lexer::LAST_T + 61;
    static constexpr Token::t_type PATH_T = Lexer::LAST_T + 62;
    static constexpr Token::t_type NONE_T = Lexer::LAST_T + 63;
    static constexpr Token::t_type TYPE_T = Lexer::LAST_T + 64;
    static constexpr Token::t_type CALL_T = Lexer::LAST_T + 65;
    static constexpr Token::t_type UNMINUS_T = Lexer::LAST_T + 66;
    static constexpr Token::t_type SECTION_LIT_T = Lexer::LAST_T + 67;
    static constexpr Token::t_type DIMS_LIT_T = Lexer::LAST_T + 68;
    static constexpr Token::t_type DIMENSIONS_T = Lexer::LAST_T + 69;
    static constexpr Token::t_type PROG_T = Lexer::LAST_T + 70;
    static constexpr Token::t_type LAST_T = PROG_T;
    // [[[end]]]

    static std::map<Token::t_type, std::string> token_names;
    static std::map<std::string, Token::t_type> keywords;

    explicit ProgramLexer(std::string _in) : Lexer(std::move(_in)) {}

    Token next_token();
    std::string token_name(Token::t_type type);
    static std::string get_token_name(Token::t_type type);

    static bool channel_token(Token::t_type type);

protected:
    bool is_char();
    bool is_letter();
    bool is_digit();
    bool is_linebreak();
    bool is_space();

    Token id_or_keyword();
    Token number();
    Token path();

    std::optional<Token::t_type> char_type();
    std::optional<Token::t_type> comp_type();
};

#endif
