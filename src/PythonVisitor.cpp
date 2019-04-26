#include "PythonVisitor.h"
#include "ProgramLexer.h"
#include "CompilerExceptions.h"

void PythonVisitor::visit(AssignNode& node)
{
    node.id.get()->visit(*this);
    output << " = ";
    node.expr.get()->visit(*this);
}

void PythonVisitor::visit(PrintNode& node)
{
    output << "print(";
    node.expr.get()->visit(*this);
    output << ")";
}

void PythonVisitor::visit(BinOpNode& node)
{
    if (not node.token)
        throw CompilerException("Binary operation"\
            " has no defining token\n");

    std::optional<std::string> func_call;
    std::optional<std::string> operation;
    bool invert = false;

    switch (node.token.value().type) {
        case ProgramLexer::DOT_T:
            if (node.lhs.get()->ftype.type == ExprType::List
                and node.rhs.get()->ftype.type == ExprType::List) {
                operation = "+";
            } else if (node.rhs.get()->ftype.type == ExprType::List) {
                func_call = "_scalar_list_dot";
            } else if (node.lhs.get()->ftype.type == ExprType::List) {
                func_call = "_scalar_list_dot";
                invert = true;
            } else if (node.lhs.get()->ftype.type == ExprType::Bool) {
                operation = "and";
            } else {
                operation = "*";
            }
            break;
        case ProgramLexer::PLUS_T:
            if (node.lhs.get()->ftype.type == ExprType::List
                and node.rhs.get()->ftype.type == ExprType::List) {
                func_call = "_list_sum";
            } else if (node.rhs.get()->ftype.type == ExprType::List) {
                func_call = "_scalar_list_sum";
            } else if (node.lhs.get()->ftype.type == ExprType::List) {
                func_call = "_scalar_list_sum";
                invert = true;
            } else if (node.lhs.get()->ftype.type == ExprType::Bool) {
                operation = "or";
            } else {
                operation = "+";
            }
            break;
        default:
            throw CompilerException("Binary operation at "
                + node.token.value().pos_string()
                + " has invalid token type");
    }

    if (func_call) {
        output << func_call.value() << "(";
        if (not invert) {
            node.lhs.get()->visit(*this);
            output << ", ";
            node.rhs.get()->visit(*this);
        } else {
            node.rhs.get()->visit(*this);
            output << ", ";
            node.lhs.get()->visit(*this);
        }
        output << ")";
    } else if (operation) {
        output << "(";
        node.lhs.get()->visit(*this);
        output << " " << operation.value() << " ";
        node.rhs.get()->visit(*this);
        output << ")";
    }
}

void PythonVisitor::visit(IdNode& node)
{
    if (node.token) {
        output << node.token.value().text;
    } else {
        throw CompilerException("Id node has no token\n");
    }
}

void PythonVisitor::visit(ProgramNode& node)
{
    output << prog_header;
    for (auto& cmd : node.cmds) {
        cmd.get()->visit(*this);
        output << std::endl;
    }
}

void PythonVisitor::visit(ListNode& node)
{
    output << "[";
    auto expr = node.exprs.begin();
    for (; expr != node.exprs.end(); ++expr) {
        (*expr)->visit(*this);
        if (expr+1 != node.exprs.end())
            output << ", ";
    }
    output << "]";
}

void PythonVisitor::visit(ScalarNode& node)
{
    if (not node.token)
        throw CompilerException("Scalar node has no token\n");

    if (node.ftype.type == ExprType::Bool) {
        if (node.token.value().text == "true")
            output << "True";
        else
            output << "False";
    } else {
        output << node.token.value().text;
    }
}

const char* PythonVisitor::prog_header = 
    "def _sum(a1, a2):\n"
    "    if isinstance(a1, list) and isinstance(a2, list):\n"
    "        return _list_sum(a1, a2)\n"
    "    elif isinstance(a1, list):\n"
    "        return _scalar_list_sum(a2, a1)\n"
    "    elif isinstance(a2, list):\n"
    "        return _scalar_list_sum(a1, a2)\n"
    "    else:\n"
    "        return _scalar_sum(a1, a2)\n"
    "\n"
    "def _list_sum(l1, l2):\n"
    "    return [_sum(_a, l2[_i]) if _i < len(l2) else _a\\\n"
    "        for _i, _a in enumerate(l1)]\n"
    "\n"
    "def _scalar_list_sum(s, l):\n"
    "    return [_sum(s, i) for i in l]\n"
    "\n"
    "def _scalar_sum(s1, s2):\n"
    "    if isinstance(s1, bool):\n"
    "        return s1 or s2\n"
    "    else:\n"
    "        return s1 + s2\n"
    "\n"
    "def _dot(a1, a2):\n"
    "    if isinstance(a2, list):\n"
    "        return _scalar_list_dot(a1, a2)\n"
    "    elif isinstance(a1, list):\n"
    "        return _scalar_list_dot(a2, a1)\n"
    "    else:\n"
    "        return _scalar_dot(a1, a2)\n"
    "\n"
    "def _scalar_list_dot(s, l):\n"
    "    return [_dot(s, i) for i in l]\n"
    "\n"
    "def _scalar_dot(s1, s2):\n"
    "    if isinstance(s1, bool):\n"
    "        return s1 or s2\n"
    "    else:\n"
    "        return s1 * s2\n"
    "\n"
    "\n";