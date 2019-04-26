import json

tok_type = "static constexpr Token::t_type"
tok_assign = " {} = Lexer::LAST_T + {};"

def to_id(token):
    return (token + "_t").upper()

with open('tokens.json') as f:
    tokens = list(json.load(f))
    for i, tok_dict in enumerate(tokens):
        print(tok_type, end='')
        print(tok_assign.format(to_id(tok_dict["id"]), i+1))
    print(tok_type, end='')
    print(" LAST_T = {};".format(to_id(tokens[-1]["id"])))