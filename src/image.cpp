#include "image.h"

Image::Image(const std::string &path) {
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("File does not exist");
    }

    image = cv::imread(path, cv::IMREAD_COLOR);
    if (image.empty()) {
        throw std::runtime_error("Could not read image");
    }

    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
}

std::vector<Color> Image::get_dominant_colors() const {
    const int num_colors = 16;

    cv::Mat reshaped = image.reshape(1, image.cols * image.rows);
    cv::Mat reshaped32f;
    reshaped.convertTo(reshaped32f, CV_32F);

    cv::Mat labels, centers;
    cv::kmeans(reshaped32f, num_colors, labels,
               cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 1.0), 3, cv::KMEANS_PP_CENTERS,
               centers);

    int total_pixels = image.rows * image.cols;

    std::vector<Color> dominant_colors;
    for (int i = 0; i < centers.rows; i++) {
        cv::Vec3f color = centers.at<cv::Vec3f>(i);
        int num_pixels = cv::countNonZero(labels == i);
        float proportion = (float) num_pixels / (float) total_pixels;
        dominant_colors.emplace_back(color, proportion);
    }

    return dominant_colors;
}

float Image::calculate_mean_luminance() const {
    cv::Mat lab_image;
    cv::cvtColor(image, lab_image, cv::COLOR_RGB2Lab);
    cv::Scalar mean_lab = cv::mean(lab_image);
    return (float) mean_lab[0] / 255.0f;
}

bool Image::is_light() const {
    float mean_luminance = calculate_mean_luminance();
    const float light_threshold = 0.5f;
    return mean_luminance >= light_threshold;
}
