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

        def to_id(tok_dict):
            name = tok_dict["id"]
            name = tok_dict.get("token", name)
            return "ProgramLexer::"+(name + "_t").upper()

        with open('tokens.json') as f:
            tokens = json.load(f)
            for tok_dict in tokens:
                tok_id = tok_dict["id"]
                tok_name = tok_dict.get("name", tok_id)
                cog.outl(map_item.format(to_id(tok_dict), tok_name))
        ]]]*/
        {ProgramLexer::ID_T, "ID"},
        {ProgramLexer::COMMENT_T, "comment"},
        {ProgramLexer::ASSIGN_T, "assign"},
        {ProgramLexer::COMMA_T, "comma"},
        {ProgramLexer::LPAREN_T, "left paren"},
        {ProgramLexer::RPAREN_T, "right paren"},
        {ProgramLexer::LBRACE_T, "left brace"},
        {ProgramLexer::RBRACE_T, "right brace"},
        {ProgramLexer::SEP_T, "separator"},
        {ProgramLexer::COLON_T, "colon"},
        {ProgramLexer::PLUS_T, "plus"},
        {ProgramLexer::MINUS_T, "minus"},
        {ProgramLexer::MULT_T, "mult."},
        {ProgramLexer::DIV_T, "division"},
        {ProgramLexer::INTEGER_LIT_T, "integer_lit"},
        {ProgramLexer::FLOAT_LIT_T, "float_lit"},
        {ProgramLexer::PATH_LIT_T, "path_lit"},
        {ProgramLexer::EQUALS_T, "equals"},
        {ProgramLexer::NEQUALS_T, "not equals"},
        {ProgramLexer::LEQ_T, "<="},
        {ProgramLexer::GEQ_T, ">="},
        {ProgramLexer::LESS_T, "less"},
        {ProgramLexer::GREATER_T, "greater"},
        {ProgramLexer::NOT_T, "not"},
        {ProgramLexer::AND_T, "and"},
        {ProgramLexer::OR_T, "or"},
        {ProgramLexer::TRUE_T, "true"},
        {ProgramLexer::FALSE_T, "false"},
        {ProgramLexer::IF_T, "if"},
        {ProgramLexer::ELSE_T, "else"},
        {ProgramLexer::PRINT_T, "print"},
        {ProgramLexer::ROTATE_T, "rotate"},
        {ProgramLexer::RESIZE_T, "resize"},
        {ProgramLexer::TO_T, "to"},
        {ProgramLexer::BY_T, "by"},
        {ProgramLexer::CROP_T, "crop"},
        {ProgramLexer::FROM_T, "from"},
        {ProgramLexer::FUNCTION_T, "function"},
        {ProgramLexer::RETURN_T, "return"},
        {ProgramLexer::FOR_T, "for"},
        {ProgramLexer::ALL_T, "all"},
        {ProgramLexer::IN_T, "in"},
        {ProgramLexer::IMAGE_OP_T, "image"},
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
        {ProgramLexer::IMAGE_T, "Image"},
        {ProgramLexer::DIMS_T, "Dims"},
        {ProgramLexer::SECTION_T, "Section"},
        {ProgramLexer::FLOAT_T, "Float"},
        {ProgramLexer::INT_T, "Int"},
        {ProgramLexer::PATH_T, "Path"},
        {ProgramLexer::NONE_T, "None"},
        {ProgramLexer::TYPE_T, "type"},
        {ProgramLexer::CALL_T, "call"},
        {ProgramLexer::UNMINUS_T, "unminus"},
        {ProgramLexer::SECTION_LIT_T, "section_lit"},
        {ProgramLexer::DIMS_LIT_T, "dimensions"},
        {ProgramLexer::DIMENSIONS_T, "dimensions operator"},
        {ProgramLexer::PROG_T, "program"},
        // [[[end]]]
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

    def to_id(tok_dict):
        name = tok_dict["id"]
        name = tok_dict.get("token", name)
        return (name + "_t").upper()

    with open('tokens.json') as f:
        tokens = json.load(f)
        for tok_dict in tokens:
            tok_id = tok_dict["id"]
            if tok_dict.get("keyword"):
                cog.outl(map_item.format(tok_id, to_id(tok_dict)))
    ]]]*/
    {"not", NOT_T},
    {"and", AND_T},
    {"or", OR_T},
    {"true", TRUE_T},
    {"false", FALSE_T},
    {"if", IF_T},
    {"else", ELSE_T},
    {"print", PRINT_T},
    {"rotate", ROTATE_T},
    {"resize", RESIZE_T},
    {"to", TO_T},
    {"by", BY_T},
    {"crop", CROP_T},
    {"from", FROM_T},
    {"function", FUNCTION_T},
    {"return", RETURN_T},
    {"for", FOR_T},
    {"all", ALL_T},
    {"in", IN_T},
    {"image", IMAGE_OP_T},
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
    {"Image", IMAGE_T},
    {"Dims", DIMS_T},
    {"Section", SECTION_T},
    {"Float", FLOAT_T},
    {"Int", INT_T},
    {"Path", PATH_T},
    {"None", NONE_T},
    {"dimensions", DIMENSIONS_T},
    // [[[end]]]
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

bool ProgramLexer::channel_token(Token::t_type type)
{
    return (type == ProgramLexer::R_T
        or type == ProgramLexer::G_T
        or type == ProgramLexer::B_T);
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
    return Token(PATH_LIT_T, t, l, c);
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
    return Token(is_float? FLOAT_LIT_T : INTEGER_LIT_T, t, l, c);
}

std::optional<Token::t_type> ProgramLexer::char_type()
{
    switch (curr_char)
    {
        /*[[[cog
        import cog
        import json

        switch_item = "case '{}':\n    return {};"

        def to_id(tok_dict):
            name = tok_dict["id"]
            name = tok_dict.get("token", name)
            return (name + "_t").upper()

        with open('tokens.json') as f:
            tokens = json.load(f)
            for tok_dict in tokens:
                symb = tok_dict.get("symbol")
                if symb:
                    tok_id = to_id(tok_dict)
                    cog.outl(switch_item.format(symb, tok_id))
        ]]]*/
        case '#':
            return COMMENT_T;
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
        case ':':
            return COLON_T;
        case '+':
            return PLUS_T;
        case '-':
            return MINUS_T;
        case '*':
            return MULT_T;
        case '/':
            return DIV_T;
        case '<':
            return LESS_T;
        case '>':
            return GREATER_T;
        // [[[end]]]
        default:
            return std::nullopt;
    }
}

std::optional<Token::t_type> ProgramLexer::comp_type()
{
    if (la() == '=') {
        switch (curr_char)
        {
            case '=':
                return EQUALS_T;
            case '!':
                return NEQUALS_T;
            case '<':
                return LEQ_T;
            case '>':
                return GEQ_T;
        }
    }

    return std::nullopt;
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

        if (auto comp = comp_type(); comp) {
            auto l = line;
            auto c = col;
            auto p = pos;

            skip(); skip();
            return Token(comp.value(), text_from(p), l, c);
        }

        if (auto t = char_type(); t) {
            auto ch = curr_char;
            auto l = line;
            auto c = col;

            if (t == COMMENT_T) {
                while (not is_linebreak() and
                    curr_char != EOF) {
                    skip();
                }
            } else {
                match(curr_char);
            }

            return Token(t.value(),
                std::string(1, ch), l, c);
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
