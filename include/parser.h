#ifndef HUEMASTER_PARSER_H
#define HUEMASTER_PARSER_H

#include <string>
#include <fstream>
#include "color_scheme.h"

class Parser {
public:
    static std::string parse(const std::string &format_path, const ColorScheme &color_scheme);
    static std::string parse_line(const std::string &format_path, const ColorScheme &color_scheme,
                                  const std::string &line, int line_number);
    static std::string parse_placeholder(const std::string &format_path, const ColorScheme &color_scheme,
                                         const std::string &placeholder, int line_number);
    static std::string parse_ternary_placeholder(const std::string &format_path, const ColorScheme &color_scheme,
                                                 const std::string &placeholder, int line_number);
};

#endif //HUEMASTER_PARSER_H
