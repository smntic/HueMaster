#ifndef HUEMASTER_CONFIGURATOR_H
#define HUEMASTER_CONFIGURATOR_H

#include <string>
#include <toml.hpp>
#include "color_scheme.h"

class Configurator {
public:
    void load_config(const std::string &config_path);
    void configure(const ColorScheme &color_scheme);

    std::string get_wallpaper_path();
private:
    std::vector<std::string> format_paths;
    std::vector<std::string> real_paths;
    std::string wallpaper_path;

    void load_format(const std::string &section_name, const toml::value &section_data);
    void load_wallpaper_path(const std::string &section_name, const toml::value &section_data);
};

#endif //HUEMASTER_CONFIGURATOR_H
