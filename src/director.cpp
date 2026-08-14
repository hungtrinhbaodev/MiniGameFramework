#include <director.h>

Director* Director::instance = nullptr;

Director* Director::get() {
    if (instance == nullptr) {
        instance = new Director();
    }
    return instance;
}

void Director::clear() {
    delete (instance);
}

Director::Director() {}

Director::~Director() {}

void Director::start(Scene_Node* scene, void* global_data) {
    this->running_scene = scene;
    scene->set_global_data(global_data);
}

Scene_Node* Director::change_scene(Scene_Node* scene, void* global_data, bool remove_last_scene) {
    this->next_scene = scene;
    this->next_global_data = global_data;
    if (remove_last_scene) {
        delete (scene);
        return nullptr;
    }
    return this->running_scene;
}

void Director::process_frame(float delta_time) {
    if (this->next_scene != nullptr) {
        this->running_scene = this->next_scene;
        this->running_scene->set_global_data(this->next_global_data);
        this->next_scene = nullptr;
    }
    this->running_scene->process_frame(delta_time);
}

void Director::end() {
    if (this->running_scene != nullptr) {
        delete (this->running_scene);
    }
    if (this->next_scene != nullptr) {
        delete (this->next_scene);
    }
}