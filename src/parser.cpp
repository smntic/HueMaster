#include "parser.h"

std::string Parser::parse(const std::string &format_path, const ColorScheme &color_scheme) {
    std::string parsed_config;

    std::ifstream file(format_path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + format_path);
    }

    std::string line;
    int line_number = 1;
    while (std::getline(file, line)) {
        std::string parsed_line = Parser::parse_line(format_path, color_scheme, line, line_number);
        parsed_config += parsed_line + '\n';
        line_number++;
    }

    file.close();

    return parsed_config;
}

std::string Parser::parse_line(const std::string &format_path, const ColorScheme &color_scheme, const std::string &line,
                               int line_number) {
    std::string parsed_line;
    std::string current_segment;
    bool placeholder = false;
    for (size_t i = 0; i < line.size(); i++) {
        if (line[i] == '$' && i + 1 < line.size() && line[i + 1] == '$') {
            if (placeholder) {
                parsed_line += parse_placeholder(format_path, color_scheme, current_segment, line_number);
            } else {
                parsed_line += current_segment;
            }
            placeholder = !placeholder;
            current_segment.clear();
            i++;
        } else {
            current_segment += line[i];
        }
    }

    if (placeholder) {
        std::stringstream error_message;
        error_message << "Placeholder missing closing '$$' in file: `" << format_path << "` at line: "
                      << line_number;
        throw std::runtime_error(error_message.str());
    }

    parsed_line += current_segment;

    return parsed_line;
}

std::string Parser::parse_placeholder(const std::string &format_path, const ColorScheme &color_scheme,
                                      const std::string &placeholder, int line_number) {
    if (placeholder.size() >= 5 && placeholder.substr(0, 5) == "LIGHT") {
        return parse_ternary_placeholder(format_path, color_scheme, placeholder, line_number);
    }

    ColorScheme::ConversionResult result = color_scheme.commands_to_color(placeholder);
    if (!result.success) {
        std::stringstream error_message;
        error_message << "Failed to parse color: `" << placeholder << "` in file: `" << format_path
                      << "` at line: " << line_number;
        throw std::runtime_error(error_message.str());
    }
    return result.result.to_string();
}

std::string Parser::parse_ternary_placeholder(const std::string &format_path, const ColorScheme &color_scheme,
                                              const std::string &placeholder, int line_number) {
    bool light_theme = color_scheme.is_light();
    if (placeholder.size() <= 5 || placeholder[5] != '?') {
        throw std::runtime_error("Invalid placeholder (missing '?'): `" + placeholder + "` in file: `" + format_path
                                 + "` at line: " + std::to_string(line_number));
    }

    std::string options = placeholder.substr(6);
    size_t colon_index = options.find(':');
    if (colon_index == std::string::npos) {
        throw std::runtime_error("Invalid placeholder (missing ':'): `" + placeholder + "` in file: `" + format_path
                                 + "` at line: " + std::to_string(line_number));
    }

    std::string string_1 = options.substr(0, colon_index);
    std::string string_2;
    if (colon_index + 1 < options.size()) {
        string_2 = options.substr(colon_index + 1);
    }

    if (light_theme) {
        return string_1;
    } else {
        return string_2;
    }
}
