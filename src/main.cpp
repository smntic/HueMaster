#include <iostream>
#include "image.h"
#include "color_scheme.h"
#include "configurator.h"

int main(int argc, char **argv) {
    try {
        if (argc != 2) {
            throw std::runtime_error("Usage: " + std::string(argv[0]) + " <image_path>");
        }

        Configurator configurator;
        std::string config_path = std::string(getenv("HOME")) + "/.config/huemaster/config.toml";
        configurator.load_config(config_path);

        Image image(argv[1]);
        image.resize(256, 256);

        ColorScheme color_scheme;
        color_scheme.generate(image);

        configurator.configure(color_scheme);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

