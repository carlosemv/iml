#ifndef __IO_H__
#define __IO_H__

#include <fstream>
#include <sstream>
#include <string>

namespace IO {
    std::string read_file(const std::string& filename);
    void write_file(const std::string& filename, const std::string& output);
    void write_file(const std::string& filename, std::ostringstream& output);
} // namespace IO

#endif
