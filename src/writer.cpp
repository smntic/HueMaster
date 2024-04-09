#include "writer.h"

void Writer::write(const std::string &real_path, const std::string &parsed_config) {
    std::ofstream file(real_path);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + real_path);
    }

    file << parsed_config;

    file.close();
}
