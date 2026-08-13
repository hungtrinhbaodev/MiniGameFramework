#pragma once
#include <glm/glm.hpp>
#include <string>

namespace Utils {
    long now();

    std::string vec2_to_string(glm::vec2 vec);

    std::string get_root_path();
}  // namespace Utils