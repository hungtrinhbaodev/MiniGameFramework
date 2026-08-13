#pragma once

#include <glm/glm.hpp>

namespace Math {

    int random_int(int min = 1, int max = 10);

    float random_float(float min = 0.0f, float max = 1.0f);

    glm::vec2 rotate(float angle, glm::vec2 position);

    bool right_side_edge(glm::vec2 start_point, glm::vec2 end_point, glm::vec2 check_point);

    bool is_in_convex(const std::vector<glm::vec2>& convex, glm::vec2 point);

    glm::vec2 get_convex_center(const std::vector<glm::vec2>& convex);

}  // namespace Math