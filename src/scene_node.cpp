#include <defined.h>
#include <profiler.h>
#include <scene_node.h>
#include <wrapper.h>

#include <algorithm>
#include <cstdio>

Scene_Node::Scene_Node() {
    set_content_size({Libs_Wrapper::get_screen_width(), Libs_Wrapper::get_screen_height()});

    this->label_fps = new Label_Node("", "", 16);
    this->label_fps->set_position({10.f, 5.f});
    this->label_fps->set_anchor({0.f, 0.f});
    this->label_fps->set_color({125, 125, 125});
    this->label_fps->set_z_order(1000);
    this->add_child(label_fps);

    this->init_profiler_rows();
}

Scene_Node::Scene_Node(void* global_data) {
    Scene_Node();
    this->set_global_data(global_data);
}

Scene_Node::~Scene_Node() {}

Node_Type Scene_Node::get_type() {
    return Node_Type::SCENE;
}

void Scene_Node::set_global_data(void* global_data) {
    this->global_data = global_data;
    this->set_initialize(true);
}

void Scene_Node::process_frame(float delta_time) {
    if (!this->initialize) {
        return;
    }
    this->travel(delta_time, global_data);
}

void Scene_Node::set_initialize(bool initialize) {
    this->initialize = initialize;
}

void Scene_Node::start() {
    this->enter(this->global_data);
}

Base_Node* Scene_Node::get_layer_background() {
    return this->layer_background;
}

void Scene_Node::enter(void* global_data) {
    Node::enter(global_data);
    this->schedule(Defined::SCENE_LABEL_FPS_SCHEDULER_KEY, 0.2, [](Base_Node* target, void* global_data) {
        Scene_Node* scene = reinterpret_cast<Scene_Node*>(target);
        scene->label_fps->set_text("frame rate: " + std::to_string((float)scene->fps_rate / 1000));
        scene->update_profiler_rows();
    });
}

void Scene_Node::track_layer_background(Base_Node* background) {
    this->layer_background = background;
}

void Scene_Node::handle_personal_task(float delta_time, void* global_data) {
    Node::handle_personal_task(delta_time, global_data);
    this->fps_rate = (int)((1.f / delta_time) * 1000);
    this->label_fps->set_visible(!Libs_Wrapper::is_debug_mode());
}

void Scene_Node::init_profiler_rows() {
    float start_y = 25.f;
    float gap_y = 14.f;
    for (int i = 0; i < this->MAX_PROFILER_ROWS; i++) {
        Label_Node* row = new Label_Node("", "", 14);
        row->set_position({10.f, start_y + i * gap_y});
        row->set_anchor({0.f, 0.f});
        row->set_color({200, 200, 200});
        row->set_z_order(1000);
        row->set_visible(false);
        this->add_child(row);
        this->profiler_rows.push_back(row);
    }
}

void Scene_Node::update_profiler_rows() {
    if (!Libs_Wrapper::is_profiling_mode()) {
        for (Label_Node* row : this->profiler_rows) {
            row->set_visible(false);
        }
        return;
    }

    std::vector<std::pair<std::string, Profile_Entry>> entries(
        Profiler::get().get_entries().begin(), Profiler::get().get_entries().end()
    );
    std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
        return a.second.avg_ms > b.second.avg_ms;
    });

    for (int i = 0; i < this->profiler_rows.size(); i++) {
        Label_Node* row = this->profiler_rows[i];
        if (i >= (int)entries.size()) {
            row->set_visible(false);
            continue;
        }
        const std::string& tag = entries[i].first;
        const Profile_Entry& entry = entries[i].second;
        char buffer[128];
        snprintf(
            buffer,
            sizeof(buffer),
            "%-24s %6.2f ms avg  (peak %.2f, x%d)",
            tag.c_str(),
            entry.avg_ms,
            entry.max_ms,
            entry.call_count
        );
        row->set_text(buffer);
        row->set_visible(true);
        if (entry.avg_ms < 0.5) {
            row->set_color({120, 220, 120});
        } else if (entry.avg_ms < 2.0) {
            row->set_color({230, 200, 90});
        } else {
            row->set_color({230, 90, 90});
        }
    }
}