#ifndef HUEMASTER_COLOR_SCHEME_H
#define HUEMASTER_COLOR_SCHEME_H

#include "image.h"

class ColorScheme {
public:
    ColorScheme() = default;

    void generate(const Image &image);

    void print_Xresources();

    struct ConversionResult {
        bool success{};
        std::string result;
    };
    [[nodiscard]] ConversionResult name_to_hex(const std::string &name) const;

private:
    Color find_background_color(bool find_light);
    Color find_text_color(bool find_light);
    Color find_contrasting_color(bool find_light);

    const std::vector<std::string> Xresources_headers = {
            "black", "red", "green", "yellow", "blue", "magenta", "cyan", "white"
    };

    Color text_color;
    Color background_color;

    std::vector<Color> scheme_colors;
    std::vector<Color> dominant_colors;
    std::vector<Color> used_colors;
};

#endif //HUEMASTER_COLOR_SCHEME_H
