#include <utils.h>
#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#endif
#include <chrono>
#include <filesystem>
#include <stdexcept>

namespace Utils {

    std::string inline DEFAULT_PATH = "";

    long now() {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto mili_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        return mili_seconds.count();
    }

    std::filesystem::path get_root() {
#if defined(_WIN32)
        wchar_t buffer[MAX_PATH];
        DWORD length = GetModuleFileNameW(NULL, buffer, MAX_PATH);
        if (length <= 0) {
            throw std::runtime_error("Fail to get root path with win32!");
        }
        return std::filesystem::path{buffer}.parent_path();
#elif defined(__APPLE__)
        uint32_t buffer_size = 0;
        _NSGetExecutablePath(nullptr, &buffer_size);
        std::vector<char> raw_path(buffer_size);
        if (_NSGetExecutablePath(raw_path.data(), &buffer_size) != 0) {
            throw std::runtime_error("Fail to get root path with apple!");
        }
        return std::filesystem::path{raw_path.data()}.parent_path();
#else
        return std::filesystem::canonical("/proc/self/exe").parent_path();
#endif
    }

    std::string vec2_to_string(glm::vec2 vec2) {
        return std::string("{") + std::to_string(vec2.x) + ", " + std::to_string(vec2.y) + "}";
    }

    std::string get_root_path() {
        if (DEFAULT_PATH == "") {
            DEFAULT_PATH = get_root().string();
        }
        return DEFAULT_PATH + "\\";
    }
}  // namespace Utils