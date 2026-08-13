#define _USE_MATH_DEFINES
#include <math.h>
#include <math_custom.h>

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

}  // namespace Math