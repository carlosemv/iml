#ifndef __IO_H__
#define __IO_H__

#include <sstream>
#include <fstream>

namespace IO {
    std::string read_file(std::string filename);
    void write_file(std::string filename, std::string output);
    void write_file(std::string filename, std::ostringstream& output);
}

#endif