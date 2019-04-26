#include <iostream>
#include <sstream>
#include <fstream>
#include <optional>

#include "ProgramLexer.h"
#include "CompilerExceptions.h"
#include "ProgramParser.h"
#include "AST/ProgramNode.h"

std::string read_file(std::string filename)
{
    std::ifstream in(filename, std::ios::in);
    std::stringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

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

// bool generate_py(ProgramNode& root, std::string out_file)
// {
//     TypeVisitor tv;
//     PythonVisitor pv;

//     try {
//         tv.visit(root);
//         pv.visit(root);
//         std::cout << "Successfully compiled\n";
//         std::ofstream out(out_file, std::ios::out);
//         out << pv.output.str();
//         std::cout << "Output written to " << out_file
//             << std::endl;
//         return true;
//     } catch (SemanticException& e) {
//         std::cerr << "[Semantic Error] ";
//         std::cerr << e.what() << std::endl;
//     } catch (CompilerException& e) {
//         std::cerr << "[Compiler Exception] ";
//         std::cerr << e.what() << std::endl;
//     }

//     return false;
// }

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cerr << "Invalid number of arguments, correct format is: ";
        std::cerr << "executable <input file> <output file>\n";
        return EXIT_FAILURE;
    }

    std::string input = read_file(argv[1]);
    std::string output = argv[2];

    // ProgramLexer pl(input);
    // Token t;
    // do {
    //     t = pl.next_token();
    //     std::cout << "<" << pl.token_name(t.type) << "> ";
    // } while (t.type != ProgramLexer::EOF_T);
    auto parsed = parse(input);

    return EXIT_SUCCESS;
}
