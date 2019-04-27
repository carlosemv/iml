#include <optional>

#include "IO.h"
#include "CompilerExceptions.h"
#include "ProgramParser.h"
#include "AST/ProgramNode.h"
#include "TypeVisitor.h"
#include "PythonVisitor.h"

std::optional<ProgramNode> parse(std::string input)
{
    ProgramParser pp(input);
    try {
        auto node = pp.parse();
        std::cout << "Successfully parsed\n";
        return std::move(node);
    } catch (LexicalException& e) {
        std::cerr << "[Lexical Error] ";
        std::cerr << e.what() << std::endl;
    } catch (SyntacticException& e) {
        std::cerr << "[Syntactic Error] ";
        std::cerr << e.what() << std::endl;
    } catch (CompilerException& e) {
        std::cerr << "[Compiler Exception] ";
        std::cerr << e.what() << std::endl;
    }
    return std::nullopt;    
}

bool generate(ProgramNode& root, std::string out_file)
{
    TypeVisitor tv;
    PythonVisitor pv;

    try {
        tv.visit(root);
        pv.visit(root);
        std::cout << "Successfully compiled\n";
        IO::write_file(out_file, pv.output);
        std::cout << "Output written to " << out_file
            << std::endl;
        return true;
    } catch (SemanticException& e) {
        std::cerr << "[Semantic Error] ";
        std::cerr << e.what() << std::endl;
    } catch (CompilerException& e) {
        std::cerr << "[Compiler Exception] ";
        std::cerr << e.what() << std::endl;
    }

    return false;
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cerr << "Invalid number of arguments, correct format is: ";
        std::cerr << "executable <input file> <output file>\n";
        return EXIT_FAILURE;
    }

    std::string input = IO::read_file(argv[1]);
    std::string output = argv[2];

    auto prog_root = parse(input);
    if (prog_root) {
        if (generate(prog_root.value(), output))
            return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
