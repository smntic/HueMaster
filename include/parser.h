#ifndef HUEMASTER_PARSER_H
#define HUEMASTER_PARSER_H

#include <string>
#include <fstream>
#include "color_scheme.h"

class Parser {
public:
    static std::string parse(const std::string &format_path, const ColorScheme &color_scheme);
};

#endif //HUEMASTER_PARSER_H
