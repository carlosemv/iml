#include "IO.h"
#include <iostream>

namespace IO {

    std::string read_file(std::string filename)
    {
        std::ifstream in(filename, std::ios::in);
        try {
            in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        } catch (const std::ios::failure& e) {
            std::cerr << "Error reading " << filename << std::endl;
            throw e;
        }

        std::stringstream sstr;
        sstr << in.rdbuf();
        return sstr.str();
    }

    void write_file(std::string filename, std::string output)
    {
        std::ofstream out(filename, std::ios::out);
        out << output;
    }

    void write_file(std::string filename, std::ostringstream& output)
    {
        write_file(filename, output.str());
    }
}