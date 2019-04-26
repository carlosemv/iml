#include "TypeVisitor.h"
#include "ProgramLexer.h"
#include "CompilerExceptions.h"

void TypeVisitor::visit(AssignNode& node)
{
    node.expr.get()->visit(*this);
    node.id.get()->ftype = node.expr.get()->ftype;

    if (not node.id.get()->token)
        throw CompilerException("Variable has no defining token");

    sym_table[node.id.get()->token.value().text] = 
        node.id.get()->ftype;
}

void TypeVisitor::visit(PrintNode& node)
{
    node.expr.get()->visit(*this);
}

FullType TypeVisitor::op_type(Token op,
    FullType lhs, FullType rhs)
{
    switch (op.type) {
        case ProgramLexer::PLUS_T:
        case ProgramLexer::DOT_T:
        {
            if (lhs.type == rhs.type and (lhs.type == ExprType::Integer
                or lhs.type == ExprType::Bool)) {
                return lhs;
            } else if (lhs.type == ExprType::List
                and rhs.type == ExprType::List) {
                FullType res(lhs);

                if (op.type == ProgramLexer::PLUS_T) {
                    auto i = 0u;
                    for (auto rt : rhs.list_types) {
                        if (i >= res.list_types.size())
                            break;
                        try {
                            res.list_types[i] = op_type(
                                op, res.list_types[i], rt);
                        } catch (const SemanticException& err) {
                            throw err;
                        }
                        ++i;
                    }
                } else {
                    for (auto rt : rhs.list_types)
                        res.list_types.push_back(rt);
                }

                return res;
            } else if (lhs.type == ExprType::List) {
                FullType res(lhs);
                
                auto pos = 0u;
                for (auto& t : res.list_types) {
                    try {
                        t = op_type(op, t, rhs.type);
                    } catch (const SemanticException& err) {
                        throw err;
                    }
                    pos++;
                }
                return res;
            } else if (rhs.type == ExprType::List) {
                FullType res(rhs);

                auto pos = 0u;
                for (auto& t : res.list_types) {
                    try {
                        t = op_type(op, lhs.type, t);
                    } catch (const SemanticException& err) {
                        throw err;
                    }
                    pos++;
                }
                return res;
            } else {
                throw SemanticException("Invalid operation \'"
                    + op.text + "\' at "
                    + op.pos_string() + ", between " + lhs.to_string()
                    + " and " + rhs.to_string());
            }
        }
        default:
            throw CompilerException("Binary operation at "
                + op.pos_string()
                + " has invalid token type");
    }
}

void TypeVisitor::visit(BinOpNode& node)
{
    node.lhs.get()->visit(*this);
    node.rhs.get()->visit(*this);

    auto ltype = node.lhs.get()->ftype;
    auto rtype = node.rhs.get()->ftype;

    if (node.token) {
        node.ftype = op_type(node.token.value(), ltype, rtype);
    } else {
        throw CompilerException("Binary operation"\
            " has no defining token\n");
    }
}

void TypeVisitor::visit(IdNode& node)
{
    if (not node.token)
        throw CompilerException("Variable has no defining token");

    try {
        node.ftype = sym_table.at(node.token.value().text);
    } catch (const std::out_of_range& e) {
        throw SemanticException("Reference to undefined variable \'"
            + node.token.value().text + "\' in "
            + node.token.value().pos_string());
    }
}

void TypeVisitor::visit(ProgramNode& node)
{
    for (auto& cmd : node.cmds)
        cmd.get()->visit(*this);
}

void TypeVisitor::visit(ListNode& node)
{
    for (auto& expr : node.exprs) {
        expr.get()->visit(*this);
        node.ftype.list_types.push_back(expr.get()->ftype);
    }
}

void TypeVisitor::visit(ScalarNode& node)
{}
