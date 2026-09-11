#pragma once
#include <chrono>
#include <map>
#include <string>

struct Profile_Entry {
    double frame_ms = 0.0;
    double avg_ms = 0.0;
    double max_ms = 0.0;
    int call_count = 0;
};

/**
 * @Note: Lightweight dev-only profiler, gated on Libs_Wrapper::is_debug_mode()
 * (the existing KEY_B debug toggle) so it costs nothing outside debug mode.
 */
class Profiler {
public:
    static Profiler& get();

    bool is_enabled() const;
    void begin_frame();
    void record(const std::string& tag, double ms);

    const std::map<std::string, Profile_Entry>& get_entries() const;

private:
    bool enabled = false;
    std::map<std::string, Profile_Entry> entries;
};

class Profile_Scope {
public:
    Profile_Scope(const std::string& tag);
    ~Profile_Scope();

private:
    std::string tag;
    bool active = false;
    std::chrono::high_resolution_clock::time_point start;
};

#define PROFILE_CONCAT_INNER(a, b) a##b
#define PROFILE_CONCAT(a, b) PROFILE_CONCAT_INNER(a, b)
#define PROFILE_SCOPE(name) Profile_Scope PROFILE_CONCAT(_profile_scope_, __LINE__)(name)
