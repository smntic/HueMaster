#include "color_scheme.h"

ColorScheme::ColorScheme() {
    scheme_colors.assign(16, {});
}

void ColorScheme::generate(const Image &image) {
    light_theme = image.is_light();
    dominant_colors = image.get_dominant_colors();

    background_color = find_background_color(light_theme);
    used_colors.push_back(background_color);
    text_color = find_text_color(!light_theme);
    used_colors.push_back(text_color);

    Color color0 = find_background_color(light_theme);
    const Color &color8 = color0;

    used_colors.push_back(color0);
    scheme_colors[0] = color0;

    for (int color = 1; color <= 6; color++) {
        Color contrasting_color = find_contrasting_color(!light_theme);
        used_colors.push_back(contrasting_color);
        scheme_colors[color] = contrasting_color;
    }

    Color color15 = find_text_color(!light_theme);
    Color color7 = color15.multiply(0.75f);

    used_colors.push_back(color7);
    used_colors.push_back(color15);

    scheme_colors[7] = color7;
    scheme_colors[8] = color8;

    for (int color = 9; color <= 14; color++) {
        scheme_colors[color] = scheme_colors[color - 8];
    }

    scheme_colors[15] = color15;
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

ColorScheme::ConversionResult ColorScheme::commands_to_color(const std::string &commands) const {
    std::vector<std::string> segments = split_commands(commands);

    if (segments.empty()) {
        return {false, {}};
    }

    ConversionResult result = name_to_color(segments[0]);
    if (!result.success) {
        return {false, {}};
    }

    Color color = result.result;
    for (size_t i = 1; i < segments.size(); i++) {
        size_t modifier_end = segments[i].find('(');
        if (modifier_end == std::string::npos) {
            return {false, {}};
        }

        std::string modifier = segments[i].substr(0, modifier_end);
        if (segments[i].size() < modifier_end + 2 || segments[i].back() != ')') {
            return {false, {}};
        }

        std::string argument = segments[i].substr(modifier_end + 1, segments[i].size() - modifier_end - 2);
        float amount;
        try {
            amount = std::stof(argument);
        } catch (const std::invalid_argument &e) {
            return {false, {}};
        } catch (const std::out_of_range &e) {
            return {false, {}};
        }

        if (modifier == "lighten") {
            color.adjust_luminance(amount * (light_theme ? -1.0f : 1.0f));
        } else if (modifier == "darken") {
            color.adjust_luminance(-amount * (light_theme ? -1.0f : 1.0f));
        } else if (modifier == "alpha") {
            color.adjust_alpha(amount / 100.0f);
        } else {
            return {false, {}};
        }
    }

    return {true, color};
}

ColorScheme::ConversionResult ColorScheme::name_to_color(const std::string &name) const {
    if (name == "BACKGROUND") {
        return {true, background_color};
    } else if (name == "FOREGROUND") {
        return {true, text_color};
    } else if (name == "ACCENT") {
        return {true, scheme_colors[1]};
    } else if (name == "GOOD") {
        return {true, scheme_colors[3]};
    } else if (name == "WARNING") {
        return {true, scheme_colors[4]};
    } else if (name == "ERROR") {
        return {true, scheme_colors[5]};
    } else if (name == "INFO") {
        return {true, scheme_colors[2]};
    } else {
        if (name.size() < 6 || name.substr(0, 5) != "COLOR") {
            return {false, {}};
        }

        std::string color_number = name.substr(5);
        try {
            int value = std::stoi(color_number);
            if (value < 0 || value > 15) {
                return {false, {}};
            }

            return {true, scheme_colors[value]};
        } catch (const std::invalid_argument &e) {
            return {false, {}};
        } catch (const std::out_of_range &e) {
            return {false, {}};
        }
    }
}

std::string ColorScheme::lightness() const {
    return light_theme ? "light" : "dark";
}

Color ColorScheme::find_background_color(bool find_light) {
    Color color;
    float max_score = 0.0f;
    float opposite_background = find_light ? 0.0f : 1.0f;
    for (const Color &dominant_color: dominant_colors) {
        Color current_color = dominant_color;
        current_color.adjust_minmax_luminance(find_light ? 80.0f : 20.0f, find_light);

        float min_dist = current_color.calculate_minimum_distance(used_colors);
        float dif = current_color.calculate_luminance_difference(opposite_background);

        float current_score = current_color.get_proportion() * std::pow(dif, 2.0f) * min_dist;
        if (current_score > max_score) {
            max_score = current_score;
            color = current_color;
        }
    }

    return color;
}

Color ColorScheme::find_text_color(bool find_light) {
    Color color;
    float max_score = 0.0f;
    for (const Color &dominant_color: dominant_colors) {
        Color current_color = dominant_color;
        current_color.adjust_minmax_luminance(find_light ? 90.0f : 10.0f, find_light);

        float min_dist = current_color.calculate_minimum_distance(used_colors);
        float contrast = current_color.calculate_contrast(background_color);

        float current_score = current_color.get_proportion() * contrast * min_dist;
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

        // adjust color away from the extremes (avoid black/white)
        current_color.adjust_minmax_luminance(find_light ? 80.0f : 20.0f, !find_light);

        float contrast = current_color.calculate_contrast(background_color);
        current_color.adjust_min_contrast(find_light ? 7.0f : 3.0f, background_color, find_light);
        float min_dist = current_color.calculate_minimum_distance(used_colors);

        float current_score = contrast * min_dist;
        if (current_score > max_score) {
            max_score = current_score;
            color = current_color;
        }
    }

    return color;
}

std::vector<std::string> ColorScheme::split_commands(const std::string &name) const {
    std::vector<std::string> segments;
    std::string current_segment;
    for (char c: name) {
        if (c == '.') {
            segments.push_back(current_segment);
            current_segment.clear();
        } else {
            current_segment += c;
        }
    }
    segments.push_back(current_segment);
    return segments;
}
