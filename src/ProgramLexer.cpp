#include "ProgramLexer.h"
#include "CompilerExceptions.h"
#include <iostream>

static std::map<Token::t_type, std::string> init_token_names()
{
    auto m = Lexer::token_names;
    std::map<Token::t_type, std::string> new_names = {
        /*[[[cog
        import cog
        import json

        map_item = "{{{}, \"{}\"}},"

        def to_id(token):
            return "ProgramLexer::"+(token + "_t").upper()

        with open('tokens.json') as f:
            tokens = json.load(f)
            for tok_dict in tokens:
                tok_id = tok_dict["id"]
                tok_name = tok_dict.get("name", tok_id)
                cog.outl(map_item.format(to_id(tok_id), tok_name))
        ]]]*/
        {ProgramLexer::ID_T, "ID"},
        {ProgramLexer::ASSIGN_T, "assign"},
        {ProgramLexer::COMMA_T, "comma"},
        {ProgramLexer::LPAREN_T, "left paren"},
        {ProgramLexer::RPAREN_T, "right paren"},
        {ProgramLexer::LBRACE_T, "left brace"},
        {ProgramLexer::RBRACE_T, "right brace"},
        {ProgramLexer::SEP_T, "separator"},
        {ProgramLexer::PLUS_T, "plus"},
        {ProgramLexer::MINUS_T, "minus"},
        {ProgramLexer::MULT_T, "mult."},
        {ProgramLexer::DIV_T, "division"},
        {ProgramLexer::INTEGER_T, "integer"},
        {ProgramLexer::FLOAT_T, "float"},
        {ProgramLexer::PATH_T, "path"},
        {ProgramLexer::PRINT_T, "print"},
        {ProgramLexer::ROTATE_T, "rotate"},
        {ProgramLexer::RESIZE_T, "resize"},
        {ProgramLexer::TO_T, "to"},
        {ProgramLexer::BY_T, "by"},
        {ProgramLexer::CROP_T, "crop"},
        {ProgramLexer::FROM_T, "from"},
        {ProgramLexer::FOR_T, "for"},
        {ProgramLexer::ALL_T, "all"},
        {ProgramLexer::IN_T, "in"},
        {ProgramLexer::IMAGE_T, "image"},
        {ProgramLexer::SAVE_T, "save"},
        {ProgramLexer::AS_T, "as"},
        {ProgramLexer::FLIP_T, "flip"},
        {ProgramLexer::HORIZONTALLY_T, "horizontally"},
        {ProgramLexer::VERTICALLY_T, "vertically"},
        {ProgramLexer::R_T, "R"},
        {ProgramLexer::G_T, "G"},
        {ProgramLexer::B_T, "B"},
        {ProgramLexer::MODIFY_T, "modify"},
        {ProgramLexer::SHARPNESS_T, "sharpness"},
        {ProgramLexer::BRIGHTNESS_T, "brightness"},
        {ProgramLexer::CONTRAST_T, "contrast"},
        {ProgramLexer::COLOR_T, "color"},
        {ProgramLexer::SECTION_T, "section"},
        {ProgramLexer::DIMENSIONS_T, "dimensions"},
        {ProgramLexer::PROG_T, "program"},
        //[[[end]]]
    };
    m.merge(new_names);
    return m;
}

std::map<Token::t_type, std::string> ProgramLexer::token_names =
    init_token_names();

std::map<std::string, Token::t_type> ProgramLexer::keywords = {
    /*[[[cog
    import cog
    import json

    map_item = "{{\"{}\", {}}},"

    def to_id(token):
        return (token + "_t").upper()

    with open('tokens.json') as f:
        tokens = json.load(f)
        for tok_dict in tokens:
            tok_id = tok_dict["id"]
            tok_name = tok_dict.get("name", tok_id)
            if tok_dict.get("keyword"):
                cog.outl(map_item.format(tok_name, to_id(tok_id)))
    ]]]*/
    {"print", PRINT_T},
    {"rotate", ROTATE_T},
    {"resize", RESIZE_T},
    {"to", TO_T},
    {"by", BY_T},
    {"crop", CROP_T},
    {"from", FROM_T},
    {"for", FOR_T},
    {"all", ALL_T},
    {"in", IN_T},
    {"image", IMAGE_T},
    {"save", SAVE_T},
    {"as", AS_T},
    {"flip", FLIP_T},
    {"horizontally", HORIZONTALLY_T},
    {"vertically", VERTICALLY_T},
    {"R", R_T},
    {"G", G_T},
    {"B", B_T},
    {"modify", MODIFY_T},
    {"sharpness", SHARPNESS_T},
    {"brightness", BRIGHTNESS_T},
    {"contrast", CONTRAST_T},
    {"color", COLOR_T},
    //[[[end]]]
};

bool ProgramLexer::is_letter()
{
    return std::isalpha(curr_char);
}

bool ProgramLexer::is_digit()
{
    return std::isdigit(curr_char);
}

bool ProgramLexer::is_linebreak()
{
    return (curr_char == '\n'
        or curr_char == '\r');
}

bool ProgramLexer::is_space()
{
    return (curr_char == ' '
        or curr_char == '\t');
}

Token ProgramLexer::id_or_keyword()
{
    auto p = pos;
    auto l = line;
    auto c = col;
    while (match(std::bind(&ProgramLexer::is_letter, this))) {}
    auto t = text_from(p);
    if (keywords.find(t) != keywords.end())
        return Token(keywords[t], t, l, c);
    else
        return Token(ID_T, t, l, c);
}

Token ProgramLexer::path()
{
    auto p = pos;
    auto l = line;
    auto c = col;

    bool escape = false;
    while ((escape or curr_char != '"') and (curr_char != EOF)) {
        if (curr_char == '\\')
            escape = not escape;
        else
            escape = false;
        skip();
    }

    auto t = text_from(p);

    if (not match('"'))
        return Token(Token::INVALID_T, t, l, c);

    return Token(PATH_T, t, l, c);
}

Token ProgramLexer::number()
{
    auto p = pos;
    auto l = line;
    auto c = col;

    while (match(std::bind(&ProgramLexer::is_digit, this))) {}
    bool is_float = match('.');
    while (match(std::bind(&ProgramLexer::is_digit, this))) {}

    auto t = text_from(p);
    if (is_float)
        return Token(FLOAT_T, t, l, c);
    else
        return Token(INTEGER_T, t, l, c);
}

std::optional<Token::t_type> ProgramLexer::char_type()
{
    switch (curr_char)
    {
        /*[[[cog
        import cog
        import json

        switch_item = "case '{}':\n\treturn {};"

        def to_id(token):
            return (token + "_t").upper()

        with open('tokens.json') as f:
            tokens = json.load(f)
            for tok_dict in tokens:
                symb = tok_dict.get("symbol")
                if symb:
                    tok_id = to_id(tok_dict["id"])
                    cog.outl(switch_item.format(symb, tok_id))
        ]]]*/
        case '=':
        	return ASSIGN_T;
        case '(':
        	return LPAREN_T;
        case ')':
        	return RPAREN_T;
        case '{':
        	return LBRACE_T;
        case '}':
        	return RBRACE_T;
        case ',':
        	return SEP_T;
        case '+':
        	return PLUS_T;
        case '-':
        	return MINUS_T;
        case '*':
        	return MULT_T;
        case '/':
        	return DIV_T;
        //[[[end]]]
        default:
            return std::nullopt;
    }
}

Token ProgramLexer::next_token()
{
    while (curr_char != EOF)
    {
        if (is_linebreak()) {
            line++;
            skip();
            col = 1;
            continue;
        }
        if (is_space()) {
            skip();
            continue;
        }

        if (auto t = char_type(); t) {
            auto tok = Token(t.value(),
                std::string(1, curr_char), line, col);
            match(curr_char);
            return tok;
        }

        if (match('"')) {
            return path();
        } else if (is_letter()) {
            return id_or_keyword();
        } else if (is_digit()) {
            return number();
        }

        auto tok = Token(Token::INVALID_T,
            std::string(1, curr_char), line, col);
        skip();
        return tok;
    }
    return Lexer::EOF_token(line, col);
}

std::string ProgramLexer::token_name(Token::t_type type)
{
    return get_token_name(type);
}

std::string ProgramLexer::get_token_name(Token::t_type type)
{
    auto name_p = ProgramLexer::token_names.find(type);
    if (name_p != ProgramLexer::token_names.end())
        return name_p->second;
    return std::to_string(type);
}