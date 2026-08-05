#include <utils.h>
#include <random>
#include <chrono>

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

    long now() {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto mili_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        return mili_seconds.count();
    }
}