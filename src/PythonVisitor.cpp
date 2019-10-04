#include "PythonVisitor.h"
#include "CompilerExceptions.h"
#include "ProgramLexer.h"

void PythonVisitor::visit(CropNode& node)
{
    if (node.command) {
        node.image->visit(*this);
        output << " = ";
    }

    output << "_crop(";
    node.image->visit(*this);
    output << ", ";
    node.section->visit(*this);
    output << ")";
}

void PythonVisitor::visit(DimensionsNode& node)
{
    output << "(";
    node.width->visit(*this);
    output << ", ";
    node.height->visit(*this);
    output << ")";
}

void PythonVisitor::visit(ExportNode& node)
{
    output << "_save(";
    node.image->visit(*this);
    output << ", ";
    node.path->visit(*this);
    output << ")";
}

void PythonVisitor::visit(FlipNode& node)
{
    if (node.command) {
        node.image->visit(*this);
        output << " = ";
    }

    node.image->visit(*this);
    output << ".transpose(Image.";
    if (node.dir == FlipDirection::Horizontal)
        output << "FLIP_LEFT_RIGHT)";
    else
        output << "FLIP_TOP_BOTTOM)";
}

void PythonVisitor::visit(IfNode& node)
{
    output << "if ";

    node.condition->visit(*this);
    output << ":\n";

    indent++;
    for (auto& cmd : node.cmds) {
        for (auto i = 0; i < indent; ++i)
            output << "    ";
        cmd->visit(*this);
        output << "\n";
    }
    indent--;

    if (not node.else_body.empty()) {
        for (auto i = 0; i < indent; ++i)
            output << "    ";
        output << "else:\n";

        indent++;
        for (auto& cmd : node.else_body) {
            for (auto i = 0; i < indent; ++i)
                output << "    ";
            cmd->visit(*this);
            output << "\n";
        }
        indent--;
    }
}

void PythonVisitor::visit(CallNode& node)
{
    node.func->visit(*this);
    output << "(";
    for (auto i = 0u; i < node.args.size(); ++i) {
        node.args[i]->visit(*this);
        if (i != node.args.size()-1)
            output << ", ";
    }
    output << ")";
}

void PythonVisitor::visit(FunctionNode& node)
{
    indent++;

    output << "def ";

    node.name->visit(*this);

    output << " (";
    for (auto p = 0u; p < node.params.size(); ++p) {
        node.params[p]->visit(*this);
        if (p != node.params.size()-1)
            output << ", ";
    }
    output << "):\n";

    for (auto& cmd : node.cmds) {
        for (auto i = 0; i < indent; ++i)
            output << "    ";
        cmd->visit(*this);
        output << "\n";
    }

    indent--;
}

void PythonVisitor::visit(ForNode& node)
{
    indent++;

    output << "for ";

    node.iterator->visit(*this);

    // std::string iter_name = "_i";
    // iter_name += std::to_string(indent);
    // output << iter_name << " in ";

    output << " in ";
    if (node.recursive) {
        output << "_chain(*[_f for _, _, _f in _walk(";
        node.path->visit(*this);
        output << ")]):";
    } else {
        output << "filter(_isfile, (";
        node.path->visit(*this);
        output << "/_f for _f in _listdir(";
        node.path->visit(*this);
        output << "))):";
    }
    output << std::endl;

    // for (auto i = 0; i < indent; ++i)
    //     output << "     ";
    // node.iterator->visit(*this);
    // output << " = " << iter_name;

    for (auto& cmd : node.cmds) {
        for (auto i = 0; i < indent; ++i)
            output << "    ";
        cmd->visit(*this);
        output << "\n";
    }

    indent--;
}

void PythonVisitor::visit(ImportNode& node)
{
    output << "Image.open(";
    node.path->visit(*this);
    output << ").convert(\"RGBA\")";
}

void PythonVisitor::visit(ModifyNode& node)
{
    if (node.command) {
        node.image->visit(*this);
        output << " = ";
    }

    output << "ImageEnhance." << node.mod_name() << "(";
    node.image->visit(*this);
    output << ").enhance(";
    node.factor->visit(*this);
    output << ")";
}

void PythonVisitor::visit(ResizeNode& node)
{
    if (node.command) {
        node.image->visit(*this);
        output << " = ";
    }

    output << "_resize(";
    node.image->visit(*this);
    output << ", ";
    if (node.resize_type == ResizeType::Absolute) {
        node.resize->visit(*this);
    } else {
        output << "tuple(";
        node.resize->visit(*this);
        output << " * dim for dim in ";
        node.image->visit(*this);
        output << ".size)";
    }
    output << ")";
}

void PythonVisitor::visit(RotateNode& node)
{
    if (node.command) {
        node.image->visit(*this);
        output << " = ";
    }

    node.image->visit(*this);
    output << ".rotate(";
    node.rotation->visit(*this);
    output << ")";
}

void PythonVisitor::visit(SectionNode& node)
{
    output << "(";
    node.left->visit(*this);
    output << ", ";
    node.upper->visit(*this);
    output << ", ";
    node.right->visit(*this);
    output << ", ";
    node.lower->visit(*this);
    output << ")";
}

void PythonVisitor::visit(UnOpNode& node)
{
    if (node.token) {
        auto op = node.token.value();
        if (op.type == ProgramLexer::UNMINUS_T) {
            output << op.text;
            node.expr->visit(*this);
        } else if (op.type == ProgramLexer::NOT_T) {
            output << op.text << " ";
            node.expr->visit(*this);
        } else if (op.type == ProgramLexer::DIMENSIONS_T) {
            node.expr->visit(*this);
            output << ".size";
        } else if (ProgramLexer::channel_token(op.type)) {
            node.expr->visit(*this);
            output << ".getchannel('" << op.text << "')";
        } else {
            throw CompilerException("Unary operation at "
                + op.pos_string() + " has invalid token type");
        }
    } else {
        throw CompilerException("Unary operation"\
            " has no defining token\n");
    }
}

void PythonVisitor::visit(AssignNode& node)
{
    node.id->visit(*this);
    output << " = ";
    node.expr->visit(*this);
}

void PythonVisitor::visit(PrintNode& node)
{
    if (node.expr->ftype.type == ExprType::Image) {
        node.expr->visit(*this);
        output << ".show()";
    } else {
        output << "print(";
        node.expr->visit(*this);
        output << ")";
    }
}

void PythonVisitor::visit(ReturnNode& node)
{
    output << "return ";
    node.expr->visit(*this);
}

void PythonVisitor::visit(BinOpNode& node)
{
    if (not node.token)
        throw CompilerException("Binary operation"\
            " has no defining token\n");

    auto lhs = node.lhs->ftype;
    auto rhs = node.rhs->ftype;
    std::optional<std::string> func_call;
    std::optional<std::string> operation;
    bool invert = false;
    bool invalid = false;

    switch (node.token.value().type) {
        case ProgramLexer::AND_T:
        case ProgramLexer::OR_T:
        case ProgramLexer::EQUALS_T:
        case ProgramLexer::NEQUALS_T:
        case ProgramLexer::LEQ_T:
        case ProgramLexer::GEQ_T:
        case ProgramLexer::LESS_T:
        case ProgramLexer::GREATER_T:
            operation = node.token.value().text;
            break;
        case ProgramLexer::PLUS_T:
            if (lhs.type == ExprType::Image
                    and rhs.type == ExprType::Image) {
                func_call = "_add";
            } else if (lhs.is_num() and rhs.is_num()) {
                operation = "+";
            } else if (lhs.type == rhs.type
                    and lhs.type == ExprType::Path) {
                func_call = "_path_sum";
            } else if (lhs.type == rhs.type
                    and lhs.is_list()) {
                output << "tuple(_a+_b for _a,_b in zip(";
                node.lhs->visit(*this);
                output << ",";
                node.rhs->visit(*this);
                output << "))";
            } else if (lhs.is_list() and rhs.is_num()) {
                output << "tuple(";
                node.rhs->visit(*this);
                output << " + _c for _c in ";
                node.lhs->visit(*this);
                output << ")";
            } else if (rhs.is_list() and lhs.is_num()) {
                output << "tuple(";
                node.lhs->visit(*this);
                output << " + _c for _c in ";
                node.rhs->visit(*this);
                output << ")";
            } else if (lhs.type == ExprType::Image
                    and rhs.is_num()) {
                node.lhs->visit(*this);
                output << ".point(lambda i: i + ";
                node.rhs->visit(*this);
                output << ")";
            } else if (rhs.type == ExprType::Image
                    and lhs.is_num()) {
                node.rhs->visit(*this);
                output << ".point(lambda i: i + ";
                node.lhs->visit(*this);
                output << ")";
            } else {
                invalid = true;
            }
            break;
        case ProgramLexer::MINUS_T:
            if (lhs.type == ExprType::Image
                    and rhs.type == ExprType::Image) {
                func_call = "_sub";
            } else if (lhs.is_num() and rhs.is_num()) {
                operation = "-";
            } else if (lhs.type == ExprType::Image
                and rhs.type == ExprType::Section) {
                output << "_rm_sec(";
                node.lhs->visit(*this);
                output << ", ";
                node.rhs->visit(*this);
                output << ")";
            } else if (rhs.type == ExprType::Image
                and lhs.type == ExprType::Section) {
                output << "_rm_sec(";
                node.rhs->visit(*this);
                output << ", ";
                node.lhs->visit(*this);
                output << ", reverse=True)";
            } else if (lhs.type == rhs.type
                and lhs.is_list()) {
                output << "tuple(abs(_a-_b) for _a,_b in zip(";
                node.lhs->visit(*this);
                output << ",";
                node.rhs->visit(*this);
                output << "))";
            } else {
                invalid = true;
            }
            break;
        case ProgramLexer::MULT_T:
            if (lhs.type == ExprType::Image
                and rhs.type == ExprType::Image) {
                func_call = "_mult";
            } else if (lhs.is_num() and rhs.is_num()) {
                operation = "*";
            } else if (lhs.type == ExprType::Image
                and rhs.is_num()) {
                node.lhs->visit(*this);
                output << ".point(lambda i: i * ";
                node.rhs->visit(*this);
                output << ")";
            } else if (rhs.type == ExprType::Image
                and lhs.is_num()) {
                node.rhs->visit(*this);
                output << ".point(lambda i: i * ";
                node.lhs->visit(*this);
                output << ")";
            } else if (lhs.is_list() and rhs.is_num()) {
                output << "tuple(";
                node.rhs->visit(*this);
                output << " * _c for _c in ";
                node.lhs->visit(*this);
                output << ")";
            } else if (rhs.is_list() and lhs.is_num()) {
                output << "tuple(";
                node.lhs->visit(*this);
                output << " * _c for _c in ";
                node.rhs->visit(*this);
                output << ")";
            } else if (lhs.type == rhs.type
                and lhs.is_list()) {
                output << "tuple(_a*_b for _a,_b in zip(";
                node.lhs->visit(*this);
                output << ",";
                node.rhs->visit(*this);
                output << "))";
            } else if (lhs.type == ExprType::Section
                and rhs.type == ExprType::Dimensions) {
                func_call = "_scale";
            } else if (rhs.type == ExprType::Section
                and lhs.type == ExprType::Dimensions) {
                func_call = "_scale";
                invert = true;
            } else {
                invalid = true;
            }
            break;
        case ProgramLexer::DIV_T:
            if (lhs.type == ExprType::Image
                and rhs.type == ExprType::Image) {
                func_call = "_div";
            } else if (lhs.is_num() and rhs.is_num()) {
                operation = "/";
            } else if (lhs.type == rhs.type
                and lhs.type == ExprType::Path) {
                func_call = "_path_div";
            } else if (lhs.type == ExprType::Image
                and rhs.is_num()) {
                node.lhs->visit(*this);
                output << ".point(lambda i: i / ";
                node.rhs->visit(*this);
                output << ")";
            } else if (lhs.is_list() and rhs.is_num()) {
                output << "tuple(_c / ";
                node.rhs->visit(*this);
                output << " for _c in ";
                node.lhs->visit(*this);
                output << ")";
            } else if (lhs.type == rhs.type
                and lhs.is_list()) {
                output << "tuple(_a/_b for _a,_b in zip(";
                node.lhs->visit(*this);
                output << ",";
                node.rhs->visit(*this);
                output << "))";
            } else {
                invalid = true;
            }
            break;
        default:
            throw CompilerException("Binary operation at "
                + node.token.value().pos_string()
                + " has invalid token type");
    }

    if (invalid)
        throw CompilerException("Undefined operation \""
            + node.token.value().text + "\" at "
            + node.token.value().pos_string()
            + " between " + lhs.to_string() + " and "
            + rhs.to_string());

    if (func_call) {
        output << func_call.value() << "(";
        if (not invert) {
            node.lhs->visit(*this);
            output << ", ";
            node.rhs->visit(*this);
        } else {
            node.rhs->visit(*this);
            output << ", ";
            node.lhs->visit(*this);
        }
        output << ")";
    } else if (operation) {
        output << "(";
        node.lhs->visit(*this);
        output << " " << operation.value() << " ";
        node.rhs->visit(*this);
        output << ")";
    }
}

void PythonVisitor::visit(IdNode& node)
{
    if (node.token) {
        output << node.token.value().text << "_";
    } else {
        throw CompilerException("Id node has no token\n");
    }
}

void PythonVisitor::visit(ProgramNode& node)
{
    output << prog_header;
    for (auto& cmd : node.cmds) {
        cmd->visit(*this);
        output << std::endl;
    }
}

void PythonVisitor::visit(ScalarNode& node)
{
    if (node.token) {
        if (node.ftype.type == ExprType::Path) {
            output << "_Path(\"";
            output << node.token.value().text;
            output << "\")";
        } else if (node.ftype.type == ExprType::Bool) {
            if (node.token.value().type == ProgramLexer::TRUE_T)
                output << "True";
            else
                output << "False";
        } else {
            output << node.token.value().text;
        }
    } else {
        throw CompilerException("Scalar node has no token\n");
    }
}

const char* PythonVisitor::prog_header =
    "#!/usr/bin/env python3\n"
    "from os import walk as _walk\n"
    "from os import listdir as _listdir\n"
    "from os.path import isfile as _isfile\n"
    "from itertools import chain as _chain\n"
    "from pathlib import Path as _Path\n"
    "from PIL import Image, ImageEnhance, ImageChops, ImageFile\n"
    "ImageFile.LOAD_TRUNCATED_IMAGES = True\n"
    "\n"
    "def _resize(img, dims):\n"
    "    return img.resize(tuple(int(c) for c in dims))\n"
    "\n"
    "def _crop(img, sec):\n"
    "    return img.crop(tuple(int(b) for b in sec))\n"
    "\n"
    "def _add(img1, img2):\n"
    "    result = img1.copy()\n"
    "    result.paste(img2, mask=img2)\n"
    "    return result\n"
    "\n"
    "def _sub(img1, img2):\n"
    "   return ImageChops.difference(img1, img2)\n"
    "\n"
    "def _mult(img1, img2):\n"
    "    return ImageChops.blend(img1, img2, 0.5)\n"
    "\n"
    "def _div(img1, img2):\n"
    "    return _mult(img1, _inv(img2))\n"
    "    \n"
    "def _inv(img):\n"
    "    *rgb, a = img.split()\n"
    "    img_rgb = Image.merge('RGB', rgb)\n"
    "    img_inv = ImageChops.invert(img_rgb)\n"
    "    img_inv.putalpha(a)\n"
    "\n"
    "    return img_inv\n"
    "\n"
    "def _rm_sec(img, sec, reverse=False):\n"
    "    left, upper, right, lower = sec\n"
    "    rect_sz = (int(right-left), int(lower-upper))\n"
    "    pos = (int(left), int(upper))\n"
    "\n"
    "    rect = Image.new(\"RGBA\", rect_sz, 4*(0,))\n"
    "    removed = img.copy()\n"
    "    removed.paste(rect, pos)\n"
    "\n"
    "    if not reverse:\n"
    "        return removed\n"
    "    else:\n"
    "        return _sub(img, removed)\n"
    "\n"
    "def _scale(section, dims):\n"
    "    return tuple(b*d for b, d in zip(section, 2*dims))\n"
    "\n"
    "def _save(img, path):\n"
    "    if path.suffix in ('.jpg', '.jpeg'):\n"
    "        img.convert(\"RGB\").save(path)\n"
    "    else:\n"
    "        img.save(path)\n"
    "\n"
    "def _path_sum(p1, p2):\n"
    "    name = p1.stem + p2.stem + p1.suffix\n"
    "    return p1.with_name(name)\n"
    "\n"
    "def _path_div(p1, p2):\n"
    "    if p1.suffix:\n"
    "        path = p1.parent / p2\n"
    "        if p2.suffix:\n"
    "            return path\n"
    "        else:\n"
    "            return path / p1.name\n"
    "    else:\n"
    "        return p1 / p2\n"
    "\n"
    "\n";
