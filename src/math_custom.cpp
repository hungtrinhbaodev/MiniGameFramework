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

}  // namespace Math