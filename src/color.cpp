#include "../include/color.h"

Color::Color(const cv::Vec3f &color) : color(color) { }

Color::Color(const cv::Vec3f &color, float proportion) : color(color), proportion(proportion) { }

float Color::calculate_luminance() const {
    cv::Vec3f normalized_color = normalize_color(color);
    return 0.2126f * normalized_color[2] + 0.7152f * normalized_color[1] + 0.0722f * normalized_color[0];
}

float Color::calculate_luminance_difference(float other_luminance) const {
    float luminance = calculate_luminance();
    return std::abs(luminance - other_luminance);
}

float Color::calculate_contrast(const Color &other) const {
    float luminance = calculate_luminance();
    float other_luminance = other.calculate_luminance();
    if (luminance < other_luminance) {
        std::swap(luminance, other_luminance);
    }
    return (luminance + 0.05f) / (other_luminance + 0.05f);
}

float Color::calculate_distance(const Color &other) const {
    cv::Mat lab_color1, lab_color2;
    cv::cvtColor(cv::Mat(1, 1, CV_32FC3, color / 255.0f), lab_color1, cv::COLOR_RGB2Lab);
    cv::cvtColor(cv::Mat(1, 1, CV_32FC3, other.get_color() / 255.0f), lab_color2, cv::COLOR_RGB2Lab);
    return (float) cv::norm(lab_color1, lab_color2);
}

float Color::calculate_minimum_distance(const std::vector<Color> &colors) const {
    float min_dist = 1e9f; // not infinite but large enough
    for (const Color &other: colors) {
        float dist = calculate_distance(other);
        if (dist < min_dist) {
            min_dist = dist;
        }
    }
    return min_dist;
}

void Color::adjust_minmax_luminance(float target_luminance, bool is_light) {
    cv::Mat lab_color;
    cv::cvtColor(cv::Mat(1, 1, CV_32FC3, color / 255.0f), lab_color, cv::COLOR_RGB2Lab);

    float current_luminance = lab_color.at<cv::Vec3f>(0, 0)[0];
    if ((is_light && current_luminance < target_luminance)
        || (!is_light && current_luminance > target_luminance)) {
        lab_color.at<cv::Vec3f>(0, 0)[0] = target_luminance;
    }

    cv::cvtColor(lab_color, lab_color, cv::COLOR_Lab2RGB);
    color = lab_color.at<cv::Vec3f>(0, 0) * 255.0f;
}

void Color::adjust_min_contrast(float target_contrast, const Color &background_color, bool is_light) {
    float current_contrast = calculate_contrast(background_color);
    cv::Vec3f adjusted_color = color;

    while (current_contrast < target_contrast) {
        cv::Mat hls_color;
        cv::cvtColor(cv::Mat(1, 1, CV_32FC3, adjusted_color / 255.0f), hls_color, cv::COLOR_RGB2HLS);

        float luminance_multiplier = is_light ? 1.1f : 0.9f;
        float saturation_multiplier = is_light ? 1.1f : 0.9f;
        hls_color.at<cv::Vec3f>(0, 0)[1] *= luminance_multiplier;
        hls_color.at<cv::Vec3f>(0, 0)[2] *= saturation_multiplier;

        if (hls_color.at<cv::Vec3f>(0, 0)[2] > 1.0f) {
            hls_color.at<cv::Vec3f>(0, 0)[2] = 1.0f;
        }

        if (hls_color.at<cv::Vec3f>(0, 0)[1] > 1.0f) {
            hls_color.at<cv::Vec3f>(0, 0)[1] = 1.0f;
            break;
        }

        if (std::abs(hls_color.at<cv::Vec3f>(0, 0)[1]) < 1e-6) {
            break;
        }

        cv::cvtColor(hls_color, hls_color, cv::COLOR_HLS2RGB);
        adjusted_color = hls_color.at<cv::Vec3f>(0, 0) * 255.0f;

        color = adjusted_color;
        current_contrast = calculate_contrast(background_color);
    }
}

void Color::adjust_contrast_color(const Color &background_color, bool is_light) {
    adjust_minmax_luminance(is_light ? 80.0f : 20.0f, !is_light); // avoid (black/white)
    adjust_min_contrast(is_light ? 7.0f : 3.0f, background_color, is_light);
}

void Color::adjust_luminance(float amount) {
    cv::Mat lab_color;
    cv::cvtColor(cv::Mat(1, 1, CV_32FC3, color / 255.0f), lab_color, cv::COLOR_RGB2Lab);

    lab_color.at<cv::Vec3f>(0, 0)[0] += amount;

    cv::cvtColor(lab_color, lab_color, cv::COLOR_Lab2RGB);
    color = lab_color.at<cv::Vec3f>(0, 0) * 255.0f;
}

void Color::adjust_alpha(float amount) {
    alpha += amount;
    if (alpha < 0.0f) {
        alpha = 0.0f;
    } else if (alpha > 1.0f) {
        alpha = 1.0f;
    }
    alpha_changed = true;
}

void Color::adjust_hue(float target_hue) {
    cv::Mat hls_color;
    cv::cvtColor(cv::Mat(1, 1, CV_32FC3, color / 255.0f), hls_color, cv::COLOR_RGB2HLS);

    hls_color.at<cv::Vec3f>(0, 0)[0] = target_hue;
    if (hls_color.at<cv::Vec3f>(0, 0)[2] < 0.1f) {
        hls_color.at<cv::Vec3f>(0, 0)[2] = 1.0f;
    }

    cv::cvtColor(hls_color, hls_color, cv::COLOR_HLS2RGB);
    color = hls_color.at<cv::Vec3f>(0, 0) * 255.0f;
}

cv::Vec3f Color::get_color() const {
    return color;
}

float Color::get_proportion() const {
    return proportion;
}

std::string Color::to_hex() const {
    std::stringstream stream;
    stream << "#" << std::hex << std::setfill('0') << std::setw(2) << (int) color[0]
           << std::hex << std::setfill('0') << std::setw(2) << (int) color[1]
           << std::hex << std::setfill('0') << std::setw(2) << (int) color[2];

    if (alpha_changed) {
        stream << std::hex << std::setfill('0') << std::setw(2) << (int) (alpha * 255.0f);
    }

    return stream.str();
}

Color Color::multiply(float amount) {
    Color product;
    product.color = color * amount;
    product.proportion = proportion;
    return product;
}

cv::Vec3f Color::normalize_color(const cv::Vec3f &color) {
    cv::Vec3f normalized_color = {
            normalize_channel(color[0]),
            normalize_channel(color[1]),
            normalize_channel(color[2])
    };
    return normalized_color;
}

float Color::normalize_channel(float channel) {
    float srgb = channel / 255.0f;
    if (srgb <= 0.03928) {
        return srgb / 12.92f;
    } else {
        return (float) std::pow((srgb + 0.055) / 1.055, 2.4);
    }
}
