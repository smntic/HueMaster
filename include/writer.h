#ifndef HUEMASTER_WRITER_H
#define HUEMASTER_WRITER_H

#include <string>
#include <fstream>

class Writer {
public:
    static void write(const std::string &real_path, const std::string &parsed_config);
};

#endif //HUEMASTER_WRITER_H
