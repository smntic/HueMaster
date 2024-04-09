#include "configurator.h"
#include "writer.h"
#include "parser.h"

void Configurator::load_config(const std::string &config_path) {
    if (!std::filesystem::exists(config_path)) {
        throw std::runtime_error("Config file does not exist");
    }

    const auto data = toml::parse(config_path);

    if (!data.is_table()) {
        throw std::runtime_error("Config file must be a table");
    }

    if (data.as_table().empty()) {
        std::cerr << "Config file is empty" << std::endl;
    }

    for (const auto &section: data.as_table()) {
        const std::string &section_name = section.first;
        const auto &section_data = section.second;
        if (!section_data.is_table()) {
            throw std::runtime_error("Config file section must be a table (section: " + section_name + ")");
        }

        if (!section_data.contains("format_path") || !section_data.contains("real_path")) {
            throw std::runtime_error(
                    "Config file section must contain 'format_path' and 'real_path' fields (section: " + section_name +
                    ")");
        }

        if (section_data.size() != 2) {
            throw std::runtime_error(
                    "Config file section must only contain 'format_path' and 'real_path' fields (section: " +
                    section_name + ")");
        }

        format_paths.push_back(section_data.at("format_path").as_string());
        real_paths.push_back(section_data.at("real_path").as_string());
    }
}

void Configurator::configure(const ColorScheme &color_scheme) {
    Parser parser;
    Writer writer;

    for (size_t i = 0; i < format_paths.size(); ++i) {
        const std::string &format_path = format_paths[i];
        const std::string &real_path = real_paths[i];

        std::string parsed_config = Parser::parse(format_path, color_scheme);
        writer.write(real_path, parsed_config);
    }
}
