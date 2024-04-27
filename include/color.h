#ifndef HUEMASTER_COLOR_H
#define HUEMASTER_COLOR_H

#include <string>
#include <sstream>
#include <iomanip>
#include <opencv2/opencv.hpp>

class Color {
private:
    typedef enum {
        HEXRGB = 0,
        HEXRGBA,
        HEXARGB,
        RGB,
        RGBA,
        ARGB,
        CRGB,
        CRGBA,
        CARGB
    } StringFormat;

    static const std::string format_names[9];

public:
    Color() = default;
    explicit Color(const cv::Vec3f &color);
    Color(const cv::Vec3f &color, float proportion);

    [[nodiscard]] float calculate_luminance() const;
    [[nodiscard]] float calculate_luminance_difference(float other_luminance) const;
    [[nodiscard]] float calculate_contrast(const Color &other) const;
    [[nodiscard]] float calculate_distance(const Color &other) const;
    [[nodiscard]] float calculate_minimum_distance(const std::vector<Color> &colors) const;

    void adjust_minmax_luminance(float target_luminance, bool is_light);
    void adjust_min_contrast(float target_contrast, const Color &background_color, bool is_light);

    void adjust_contrast_color(const Color &background_color, bool is_light);

    void adjust_luminance(float amount);
    void adjust_alpha(float amount);
    void adjust_hue(float target_hue);

    static bool is_valid_format(const std::string &format_name);
    void set_format(const std::string &format_name);

    [[nodiscard]] cv::Vec3f get_color() const;
    [[nodiscard]] float get_proportion() const;

    [[nodiscard]] std::string to_string() const;

    [[nodiscard]] Color multiply(float amount);

private:
    static cv::Vec3f normalize_color(const cv::Vec3f &color);
    static float normalize_channel(float channel);

    std::string to_hex() const;
    std::string to_rgb() const;

    cv::Vec3f color;
    float alpha = 1.0f;
    StringFormat format{};
    float proportion{};
};

#endif //HUEMASTER_COLOR_H
