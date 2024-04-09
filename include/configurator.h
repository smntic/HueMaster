#ifndef HUEMASTER_CONFIGURATOR_H
#define HUEMASTER_CONFIGURATOR_H

#include <string>
#include <toml.hpp>
#include "color_scheme.h"

class Configurator {
public:
    void load_config(const std::string &config_path);
    void configure(const ColorScheme &color_scheme);
private:
    std::vector<std::string> format_paths;
    std::vector<std::string> real_paths;
};

#endif //HUEMASTER_CONFIGURATOR_H
