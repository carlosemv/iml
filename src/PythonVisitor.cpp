#include "PythonVisitor.h"
#include "ProgramLexer.h"
#include "CompilerExceptions.h"

void TypeVisitor::visit(CropNode& node)
{
    node.section.get()->visit(*this);
    node.image.get()->visit(*this);
}

void TypeVisitor::visit(DimensionsNode& node)
{
    node.width.get()->visit(*this);
    node.height.get()->visit(*this);
}

void TypeVisitor::visit(ExportNode& node)
{
    node.image.get()->visit(*this);
    node.path.get()->visit(*this);
}

void TypeVisitor::visit(FlipNode& node)
{
    node.image.get()->visit(*this);
}

void TypeVisitor::visit(ForNode& node)
{
    sym_table.emplace_front();

    node.iterator.get()->visit(*this);
    node.path.get()->visit(*this);

    for (auto& cmd : node.cmds) {
        cmd.get()->visit(*this);
    }

    sym_table.pop_front();
}

void TypeVisitor::visit(ImportNode& node)
{
    node.path.get()->visit(*this);
    auto path_type = node.path.get()->ftype;
    if (path_type.type != ExprType::Path)
        throw SemanticException("Expression has invalid type "
            + path_type.to_string() + "; expected a Path");

    node.ftype.type = ExprType::Image;
}

void TypeVisitor::visit(ModifyNode& node)
{
    node.image.get()->visit(*this);
    node.factor.get()->visit(*this);
}

void TypeVisitor::visit(ResizeNode& node)
{
    node.image.get()->visit(*this);
    node.resize.get()->visit(*this);
}

void TypeVisitor::visit(RotateNode& node)
{
    node.image.get()->visit(*this);
    node.rotation.get()->visit(*this);
}

void TypeVisitor::visit(SectionNode& node)
{
    node.left.get()->visit(*this);
    node.upper.get()->visit(*this);
    node.right.get()->visit(*this);
    node.lower.get()->visit(*this);
}

void TypeVisitor::visit(UnOpNode& node)
{
    if (node.token) {
        auto op = node.token.value();
        if (op.type == ProgramLexer::UNMINUS_T) {
            output << op.text;
            node.expr.get()->visit(*this);
        } else if (ProgramLexer::channel_token(op.type)) {
            node.expr.get()->visit(*this);
            int channel = 0;
            if (op.text == "G") channel = 1;
            else if (op.text == "B") channel = 2;
            output << "[" << channel << "]";
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

    std::optional<std::string> func_call;
    std::optional<std::string> operation;
    bool invert = false;

    switch (node.token.value().type) {
        case ProgramLexer::DOT_T:
            break;
        case ProgramLexer::PLUS_T:
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

void PythonVisitor::visit(ScalarNode& node)
{
    if (node.token) {
        if (node.ftype.type == ExprType::Path) {
            std::cout << '"';
            std::cout << node.token.value().text;
            std::cout << '"';
        } else {
            std::cout << node.token.value().text;
        }
    } else {
        throw CompilerException("Scalar node has no token\n");
    }
}

const char* PythonVisitor::prog_header = 
    "from PIL import Image, ImageEnhance, ImageChops, ImageFile\n"
    "ImageFile.LOAD_TRUNCATED_IMAGES = True\n"
    "\n"
    "def _add(img1, img2):\n"
    "   result = img1.copy()\n"
    "   result.paste(img2)\n"
    "   return result\n"
    "\n"
    "def _sub(img1, img2):\n"
    "   return ImageChops.difference(img1, img2)\n"
    "\n"
    "def _mult(img1, img2):\n"
    "   return ImageChops.blend(img1, img2, 0.5)\n"
    "\n"
    "def _div(img1, img2):\n"
    "   return _mult(img1, _inv(img2))\n"
    "   \n"
    "def _inv(img):\n"
    "    *rgb, a = img.split()\n"
    "    img_rgb = Image.merge('RGB', rgb)\n"
    "    img_inv = ImageChops.invert(img_rgb)\n"
    "    img_inv.putalpha(a)\n"
    "\n"
    "    return img_inv\n"
    "\n"
    "\n";