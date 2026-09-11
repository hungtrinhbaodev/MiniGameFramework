#include <profiler.h>
#include <wrapper.h>

#include <algorithm>

Profiler& Profiler::get() {
    static Profiler instance;
    return instance;
}

bool Profiler::is_enabled() const {
    return this->enabled;
}

void Profiler::begin_frame() {
    this->enabled = Libs_Wrapper::is_profiling_mode();
    for (auto& [tag, entry] : this->entries) {
        entry.avg_ms = entry.avg_ms * 0.9 + entry.frame_ms * 0.1;
        entry.max_ms = std::max(entry.max_ms * 0.98, entry.frame_ms);
        entry.frame_ms = 0.0;
        entry.call_count = 0;
    }
}

void Profiler::record(const std::string& tag, double ms) {
    if (!this->enabled)
        return;
    Profile_Entry& entry = this->entries[tag];
    entry.frame_ms += ms;
    entry.call_count++;
}

const std::map<std::string, Profile_Entry>& Profiler::get_entries() const {
    return this->entries;
}

Profile_Scope::Profile_Scope(const std::string& tag) : active(Profiler::get().is_enabled()) {
    if (this->active) {
        this->tag = tag;
        this->start = std::chrono::high_resolution_clock::now();
    }
}

Profile_Scope::~Profile_Scope() {
    if (!this->active)
        return;
    auto elapsed = std::chrono::high_resolution_clock::now() - this->start;
    Profiler::get().record(this->tag, std::chrono::duration<double, std::milli>(elapsed).count());
}
