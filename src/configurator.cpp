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

    bool wallpaper_section = false;
    for (const auto &section: data.as_table()) {
        const std::string &section_name = section.first;
        const toml::value &section_data = section.second;
        if (!section_data.is_table()) {
            throw std::runtime_error("Config file section must be a table (section: " + section_name + ")");
        }

        if (section_name == "Wallpaper") {
            wallpaper_section = true;
            load_wallpaper_path(section_name, section_data);
        } else {
            load_format(section_name, section_data);
        }
    }

    if (!wallpaper_section) {
        throw std::runtime_error("Config file must contain a 'Wallpaper' section with a 'path' field");
    }
}

void Configurator::configure(const ColorScheme &color_scheme) {
    for (size_t i = 0; i < format_paths.size(); ++i) {
        const std::string &format_path = format_paths[i];
        const std::string &real_path = real_paths[i];

        std::string parsed_config = Parser::parse(format_path, color_scheme);
        Writer::write(real_path, parsed_config);
    }
}

std::string Configurator::get_wallpaper_path() {
    return wallpaper_path;
}

void Configurator::load_format(const std::string &section_name, const toml::value &section_data) {
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

void Configurator::load_wallpaper_path(const std::string &section_name, const toml::value &section_data) {
    if (!section_data.contains("path")) {
        throw std::runtime_error("Config file section must contain 'path' field (section: " + section_name + ")");
    }

    if (section_data.size() != 1) {
        throw std::runtime_error("Config file section must only contain 'path' field (section: " + section_name + ")");
    }

    wallpaper_path = section_data.at("path").as_string();
}


