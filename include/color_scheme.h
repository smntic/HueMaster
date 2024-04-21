#ifndef HUEMASTER_COLOR_SCHEME_H
#define HUEMASTER_COLOR_SCHEME_H

#include "image.h"

class ColorScheme {
public:
    ColorScheme();

    void generate(const Image &image);

    void print_Xresources();

    struct ConversionResult {
        bool success{};
        Color result;
    };
    [[nodiscard]] ConversionResult commands_to_color(const std::string &commands) const;
    [[nodiscard]] ConversionResult name_to_color(const std::string &name) const;
    [[nodiscard]] bool is_light() const;

private:
    Color find_background_color(bool find_light);
    Color find_text_color(bool find_light);
    Color find_contrasting_color(bool find_light);

    void generate_special_colors();

    std::vector<std::string> split_commands(const std::string &name) const;

    const std::vector<std::string> Xresources_headers = {
            "black", "red", "green", "yellow", "blue", "magenta", "cyan", "white"
    };

    bool light_theme;

    Color text_color;
    Color background_color;

    std::vector<Color> scheme_colors;
    std::vector<Color> dominant_colors;
    std::vector<Color> used_colors;

    Color error_color, good_color, warning_color, info_color, accent_color;
};

#endif //HUEMASTER_COLOR_SCHEME_H
