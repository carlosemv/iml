#include "PythonVisitor.h"
#include "ProgramLexer.h"
#include "CompilerExceptions.h"

PythonVisitor::PythonVisitor() : indent(0)
{}

void PythonVisitor::visit(CropNode& node)
{
    if (node.command) {
        node.image.get()->visit(*this);
        output << " = ";
    }
    
    output << "_crop(";
    node.image.get()->visit(*this);
    output << ", ";
    node.section.get()->visit(*this);
    output << ")";
}

void PythonVisitor::visit(DimensionsNode& node)
{
    output << "(";
    node.width.get()->visit(*this);
    output << ", ";
    node.height.get()->visit(*this);
    output << ")";
}

void PythonVisitor::visit(ExportNode& node)
{
    node.image.get()->visit(*this);
    output << ".save(";
    node.path.get()->visit(*this);
    output << ")";
}

void PythonVisitor::visit(FlipNode& node)
{
    if (node.command) {
        node.image.get()->visit(*this);
        output << " = ";
    }

    node.image.get()->visit(*this);
    output << ".transpose(Image.";
    if (node.dir == FlipDirection::Horizontal)
        output << "FLIP_LEFT_RIGHT)";
    else
        output << "FLIP_TOP_BOTTOM)"; 
}

void PythonVisitor::visit(ForNode& node)
{
    indent++;

    output << "for ";

    node.iterator.get()->visit(*this);

    // std::string iter_name = "_i";
    // iter_name += std::to_string(indent);
    // output << iter_name << " in ";

    output << " in ";
    if (node.recursive) {
        output << "_chain(*[_f for _, _, _f in _walk(";
        node.path.get()->visit(*this);
        output << ")]):";
    } else {
        output << "(_f for _f in _listdir(";
        node.path.get()->visit(*this);
        output << ") if _isfile(";
        node.path.get()->visit(*this);
        output << "+_f)):";
    }
    output << std::endl;

    // for (auto i = 0; i < indent; ++i)
    //     output << "     ";
    // node.iterator.get()->visit(*this);
    // output << " = " << iter_name;

    for (auto& cmd : node.cmds) {
        for (auto i = 0; i < indent; ++i)
            output << "    ";
        cmd.get()->visit(*this);
        output << "\n";
    }

    indent--;
}

void PythonVisitor::visit(ImportNode& node)
{
    output << "Image.open(";
    node.path.get()->visit(*this);
    output << ").convert(\"RGBA\")";
}

void PythonVisitor::visit(ModifyNode& node)
{
    if (node.command) {
        node.image.get()->visit(*this);
        output << " = ";
    }

    output << "ImageEnhance." << node.mod_name() << "(";
    node.image.get()->visit(*this);
    output << ").enhance(";
    node.factor.get()->visit(*this);
    output << ")";
}

void PythonVisitor::visit(ResizeNode& node)
{
    if (node.command) {
        node.image.get()->visit(*this);
        output << " = ";
    }

    output << "_resize(";
    node.image.get()->visit(*this);
    output << ", ";
    if (node.resize_type == ResizeType::Absolute) {
        node.resize.get()->visit(*this);
    } else {
        output << "tuple(";
        node.resize.get()->visit(*this);
        output << " * dim for dim in ";
        node.image.get()->visit(*this);
        output << ".size)";
    }
    output << ")";
}

void PythonVisitor::visit(RotateNode& node)
{
    if (node.command) {
        node.image.get()->visit(*this);
        output << " = ";
    }

    node.image.get()->visit(*this);
    output << ".rotate(";
    node.rotation.get()->visit(*this);
    output << ")";
}

void PythonVisitor::visit(SectionNode& node)
{
    output << "(";
    node.left.get()->visit(*this);
    output << ", ";
    node.upper.get()->visit(*this);
    output << ", ";
    node.right.get()->visit(*this);
    output << ", ";
    node.lower.get()->visit(*this);
    output << ")";
}

void PythonVisitor::visit(UnOpNode& node)
{
    if (node.token) {
        auto op = node.token.value();
        if (op.type == ProgramLexer::UNMINUS_T) {
            output << op.text;
            node.expr.get()->visit(*this);
        } else if (ProgramLexer::channel_token(op.type)) {
            node.expr.get()->visit(*this);
            output << ".getchannel(" << op.text << ")";
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
    node.id.get()->visit(*this);
    output << " = ";
    node.expr.get()->visit(*this);
}

void PythonVisitor::visit(PrintNode& node)
{
    if (node.expr.get()->ftype.type == ExprType::Image) {
        node.expr.get()->visit(*this);
        output << ".show()";
    } else {
        output << "print(";
        node.expr.get()->visit(*this);
        output << ")";
    }
}

void PythonVisitor::visit(BinOpNode& node)
{
    if (not node.token)
        throw CompilerException("Binary operation"\
            " has no defining token\n");

    auto lhs = node.lhs.get()->ftype;
    auto rhs = node.rhs.get()->ftype;
    std::optional<std::string> func_call;
    std::optional<std::string> operation;
    bool invert = false;
    bool invalid = false;

    switch (node.token.value().type) {
       case ProgramLexer::PLUS_T:
            if (lhs.type == ExprType::Image
                and rhs.type == ExprType::Image) {
                func_call = "_add";
            } else if (lhs.is_num() and rhs.is_num()) {
                operation = "+";
            } else if (lhs.type == rhs.type
                and lhs.type == ExprType::Path) {
                operation = "+";
            } else if (lhs.type == rhs.type
                and lhs.is_list()) {
                output << "tuple(_a+_b for _a,_b in zip(";
                node.lhs.get()->visit(*this);
                output << ",";
                node.rhs.get()->visit(*this);
                output << "))";
            } else if (lhs.is_list() and rhs.is_num()) {
                output << "tuple(";
                node.rhs.get()->visit(*this);
                output << " + _c for _c in ";
                node.lhs.get()->visit(*this);
                output << ")";
            } else if (rhs.is_list() and lhs.is_num()) {
                output << "tuple(";
                node.lhs.get()->visit(*this);
                output << " + _c for _c in ";
                node.rhs.get()->visit(*this);
                output << ")";
            } else if (lhs.type == ExprType::Image
                and rhs.is_num()) {
                node.lhs.get()->visit(*this);
                output << ".point(lambda i: i + ";
                node.rhs.get()->visit(*this);
                output << ")";
            } else if (rhs.type == ExprType::Image
                and lhs.is_num()) {
                node.rhs.get()->visit(*this);
                output << ".point(lambda i: i + ";
                node.lhs.get()->visit(*this);
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
                node.lhs.get()->visit(*this);
                output << ", ";
                node.rhs.get()->visit(*this);
                output << ")";
            } else if (rhs.type == ExprType::Image
                and lhs.type == ExprType::Section) {
                output << "_rm_sec(";
                node.rhs.get()->visit(*this);
                output << ", ";
                node.lhs.get()->visit(*this);
                output << ", reverse=True)";
            } else if (lhs.type == rhs.type
                and lhs.is_list()) {
                output << "tuple(abs(_a-_b) for _a,_b in zip(";
                node.lhs.get()->visit(*this);
                output << ",";
                node.rhs.get()->visit(*this);
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
                node.lhs.get()->visit(*this);
                output << ".point(lambda i: i * ";
                node.rhs.get()->visit(*this);
                output << ")";
            } else if (rhs.type == ExprType::Image
                and lhs.is_num()) {
                node.rhs.get()->visit(*this);
                output << ".point(lambda i: i * ";
                node.lhs.get()->visit(*this);
                output << ")";
            } else if (lhs.is_list() and rhs.is_num()) {
                output << "tuple(";
                node.rhs.get()->visit(*this);
                output << " * _c for _c in ";
                node.lhs.get()->visit(*this);
                output << ")";
            } else if (rhs.is_list() and lhs.is_num()) {
                output << "tuple(";
                node.lhs.get()->visit(*this);
                output << " * _c for _c in ";
                node.rhs.get()->visit(*this);
                output << ")";
            } else if (lhs.type == rhs.type
                and lhs.is_list()) {
                output << "tuple(_a*_b for _a,_b in zip(";
                node.lhs.get()->visit(*this);
                output << ",";
                node.rhs.get()->visit(*this);
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
            } else if (lhs.type == ExprType::Image
                and rhs.is_num()) {
                node.lhs.get()->visit(*this);
                output << ".point(lambda i: i / ";
                node.rhs.get()->visit(*this);
                output << ")";
            } else if (lhs.is_list() and rhs.is_num()) {
                output << "tuple(";
                node.rhs.get()->visit(*this);
                output << " / _c for _c in ";
                node.lhs.get()->visit(*this);
                output << ")";
            } else if (lhs.type == rhs.type
                and lhs.is_list()) {
                output << "tuple(_a/_b for _a,_b in zip(";
                node.lhs.get()->visit(*this);
                output << ",";
                node.rhs.get()->visit(*this);
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

void PythonVisitor::visit(ScalarNode& node)
{
    if (node.token) {
        if (node.ftype.type == ExprType::Path) {
            output << '"';
            output << node.token.value().text;
            output << '"';
        } else {
            output << node.token.value().text;
        }
    } else {
        throw CompilerException("Scalar node has no token\n");
    }
}

const char* PythonVisitor::prog_header = 
    "from os import walk as _walk\n"
    "from os import listdir as _listdir\n"
    "from os.path import isfile as _isfile\n"
    "from itertools import chain as _chain\n"
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
    "        return _inv(_sub(img, removed))\n"
    "\n"
    "def _scale(section, dims):\n"
    "    return tuple(b*d for b, d in zip(section, 2*dims))\n"
    "\n"
    "\n";