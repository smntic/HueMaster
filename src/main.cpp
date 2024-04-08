#include <iostream>
#include "image.h"
#include "color_scheme.h"

int main(int argc, char **argv) {
    try {
        if (argc != 2) {
            throw std::runtime_error("Usage: " + std::string(argv[0]) + " <image_path>");
        }

        Image image(argv[1]);
        ColorScheme color_scheme;
        color_scheme.generate(image);
        color_scheme.print_Xresources();
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

