#ifndef HUEMASTER_COLOR_H
#define HUEMASTER_COLOR_H

#include <string>
#include <sstream>
#include <iomanip>
#include <opencv2/opencv.hpp>

class Color {
public:
    Color() = default;
    explicit Color(const cv::Vec3f &color);
    Color(const cv::Vec3f &color, float proportion);

    [[nodiscard]] float calculate_luminance() const;
    [[nodiscard]] float calculate_luminance_difference(float other_luminance) const;
    [[nodiscard]] float calculate_contrast(const Color &other) const;
    [[nodiscard]] float calculate_distance(const Color &other) const;
    [[nodiscard]] float calculate_minimum_distance(const std::vector<Color> &colors) const;

    void adjust_luminance(float target_luminance, bool is_light);
    void adjust_contrast(float target_contrast, const Color &background_color, bool is_light);

    [[nodiscard]] cv::Vec3f get_color() const;
    [[nodiscard]] float get_proportion() const;

    [[nodiscard]] std::string to_hex() const;

    [[nodiscard]] Color multiply(float amount);

private:
    static cv::Vec3f normalize_color(const cv::Vec3f &color);
    static float normalize_channel(float channel);

    cv::Vec3f color;
    float proportion{};
};

#endif //HUEMASTER_COLOR_H
