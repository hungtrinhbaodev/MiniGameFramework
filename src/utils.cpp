#include <utils.h>
#include <random>

namespace Utils
{
    std::random_device rd;

    std::mt19937 gen(rd());

    int random_int(int min, int max)
    {
        std::uniform_int_distribution<int> distrib(min, max);

        return distrib(gen);
    }

    float random_float(float min, float max)
    {
        std::uniform_real_distribution<float> distrib(min, max);

        return distrib(gen);
    }
}