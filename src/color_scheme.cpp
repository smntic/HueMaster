#include "color_scheme.h"

void ColorScheme::generate(const Image &image) {
    bool light_image = image.is_light();
    dominant_colors = image.get_dominant_colors();

    background_color = find_background_color(light_image);
    used_colors.push_back(background_color);
    text_color = find_text_color(!light_image);
    used_colors.push_back(text_color);

    Color color0 = find_background_color(light_image);
    const Color &color8 = color0;

    used_colors.push_back(color0);
    scheme_colors.push_back(color0);

    for (int color = 1; color <= 6; color++) {
        Color contrasting_color = find_contrasting_color(!light_image);
        used_colors.push_back(contrasting_color);
        scheme_colors.push_back(contrasting_color);
    }

    Color color15 = find_text_color(!light_image);
    Color color7 = color15.multiply(0.75f);

    used_colors.push_back(color7);
    used_colors.push_back(color15);

    scheme_colors.push_back(color7);
    scheme_colors.push_back(color8);

    for (int color = 9; color <= 14; color++) {
        scheme_colors.push_back(scheme_colors[color - 8]);
    }

    scheme_colors.push_back(color15);
}

void ColorScheme::print_Xresources() {
    std::cout << "! special" << std::endl;
    std::cout << "*.foreground:\t" << text_color.to_hex() << std::endl;
    std::cout << "*.background:\t" << background_color.to_hex() << std::endl;
    std::cout << "*.cursorColor:\t" << text_color.to_hex() << std::endl;

    for (size_t i = 0; i < Xresources_headers.size(); i++) {
        std::cout << std::endl;
        std::cout << "! " << Xresources_headers[i] << std::endl;
        std::cout << "*.color" << i << ":\t" << scheme_colors[i].to_hex() << std::endl;
        std::cout << "*.color" << i + 8 << ":\t" << scheme_colors[i + 8].to_hex() << std::endl;
    }
}

Color ColorScheme::find_background_color(bool find_light) {
    Color color;
    float max_score = 0.0f;
    float opposite_background = find_light ? 0.0f : 1.0f;
    for (const Color &dominant_color: dominant_colors) {
        float min_dist = dominant_color.calculate_minimum_distance(used_colors);
        float dif = dominant_color.calculate_luminance_difference(opposite_background);

        float current_score = dominant_color.get_proportion() * std::pow(dif, 2.0f) * min_dist;
        if (current_score > max_score) {
            max_score = current_score;
            color = dominant_color;
        }
    }

    return color;
}

Color ColorScheme::find_text_color(bool find_light) {
    Color color;
    float max_score = 0.0f;
    for (const Color &dominant_color: dominant_colors) {
        Color current_color = dominant_color;
        current_color.adjust_luminance(find_light ? 0.9f : 0.1f, find_light);

        float min_dist = current_color.calculate_minimum_distance(used_colors);
        float contrast = current_color.calculate_contrast(background_color);

        float current_score = dominant_color.get_proportion() * contrast * min_dist;
        if (current_score > max_score) {
            max_score = current_score;
            color = current_color;
        }
    }

    return color;
}

Color ColorScheme::find_contrasting_color(bool find_light) {
    Color color;
    float max_score = 0.0f;
    for (const Color &dominant_color: dominant_colors) {
        Color current_color = dominant_color;

        float contrast = current_color.calculate_contrast(background_color);
        current_color.adjust_contrast(find_light ? 7.0f : 3.0f, background_color, find_light);
        float min_dist = current_color.calculate_minimum_distance(used_colors);

        float current_score = dominant_color.get_proportion() * contrast * min_dist;
        if (current_score > max_score) {
            max_score = current_score;
            color = current_color;
        }
    }

    return color;
}
