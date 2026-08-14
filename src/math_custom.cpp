#define _USE_MATH_DEFINES
#include <math.h>
#include <math_custom.h>

#include <cmath>
#include <random>

namespace Math {

    std::random_device rd;

    std::mt19937 gen(rd());

    float to_radians(float angle) {
        return angle * M_PI / 180.f;
    }

    int random_int(int min, int max) {
        std::uniform_int_distribution<int> distrib(min, max);
        return distrib(gen);
    }

    float random_float(float min, float max) {
        std::uniform_real_distribution<float> distrib(min, max);
        return distrib(gen);
    }

    glm::vec2 rotate(float angle, glm::vec2 position) {
        /**@HACK: RULE C++ positive angle produce counter lockwise we must reverse it! */
        angle = to_radians(-angle);
        return {position.x * cos(angle) - position.y * sin(angle), position.x * sin(angle) + position.y * cos(angle)};
    }

    bool right_side_edge(glm::vec2 start_point, glm::vec2 end_point, glm::vec2 check_point) {
        return ((check_point.x - start_point.x) * (end_point.y - start_point.y) -
                (check_point.y - start_point.y) * (end_point.x - start_point.x)) > 0;
    }

    bool is_in_convex(const std::vector<glm::vec2>& convex, glm::vec2 point) {
        for (int i = 0; i < convex.size(); i++) {
            int current = i;
            int next = (current + 1) % convex.size();
            if (right_side_edge(convex[current], convex[next], point)) {
                return false;
            }
        }
        return true;
    }

    glm::vec2 get_convex_center(const std::vector<glm::vec2>& convex) {
        glm::vec2 center{0, 0};
        for (auto& point : convex) {
            center += point;
        }
        center /= (float)convex.size();
        return center;
    }

    glm::vec2 get_bezier_point(glm::vec2 start_point, glm::vec2 middle_point, glm::vec2 end_point, float rate) {
        return {
            std::pow(1.f - rate, 2) * start_point.x + 2 * (1.f - rate) * rate * middle_point.x +
                std::pow(rate, 2) * end_point.x,
            std::pow(1.f - rate, 2) * start_point.y + 2 * (1.f - rate) * rate * middle_point.y +
                std::pow(rate, 2) * end_point.y
        };
    }

    glm::vec2 get_middle_bezier_point(
        glm::vec2 start_point, glm::vec2 end_point, float high, float delta_to_middle, int sign
    ) {
        glm::vec2 middle_position{0, 0};
        middle_position.x = start_point.x + (end_point.x - start_point.x) * delta_to_middle;
        middle_position.y = start_point.y + (end_point.y - start_point.y) * delta_to_middle;

        auto vec_x = (end_point.x - start_point.x);
        auto vec_y = (end_point.y - end_point.y);
        auto distance = std::sqrt(vec_x * vec_x + vec_y * vec_y);
        auto per_norm_vec = glm::vec2(vec_y / distance, -vec_x / distance);
        auto temp_point = glm::vec2(per_norm_vec.x + start_point.x, per_norm_vec.y + end_point.y);
        if (!sign) {
            sign = 1;
            if (!right_side_edge(start_point, temp_point, end_point))
                sign = -1;

            if (start_point.x > end_point.x)
                sign *= -1;
        }
        middle_position.x += per_norm_vec.x * high * sign;
        middle_position.y += per_norm_vec.y * high * sign;
        if (std::isnan(middle_position.x) || std::isnan(middle_position.y)) {
            return glm::vec2(start_point.x, start_point.y + high);
        }
        return middle_position;
    }

}  // namespace Math