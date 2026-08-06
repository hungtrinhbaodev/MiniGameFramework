#pragma once
#include <string>

namespace Utils {
    int random_int(int min = 1, int max = 10);

    float random_float(float min = 0.0f, float max = 1.0f);

    long now();

    std::string get_root_path();
}  // namespace Utils