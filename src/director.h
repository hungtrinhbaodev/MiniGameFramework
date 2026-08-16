#pragma once
#include <scene_node.h>

class Director {
public:
    static Director* get();
    static void clear();

    Director();
    ~Director();

    void start(Scene_Node* scene, void* global_data = nullptr);
    Scene_Node* change_scene(Scene_Node* scene, void* global_data = nullptr, bool remove_last_scene = true);
    void start_frame();
    void process_frame(float delta_time);
    void end_frame();
    void end();

    Base_Node* get_running_scene_background();

private:
    static Director* instance;
    Scene_Node* next_scene = nullptr;
    Scene_Node* running_scene = nullptr;
    void* next_global_data = nullptr;
    bool remove_last_scene = false;
};