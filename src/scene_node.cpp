#include <defined.h>
#include <scene_node.h>
#include <wrapper.h>

Scene_Node::Scene_Node() {
    set_content_size({Libs_Wrapper::get_screen_width(), Libs_Wrapper::get_screen_height()});

    this->label_fps = new Label_Node("", "", 16);
    this->label_fps->set_position({10.f, 5.f});
    this->label_fps->set_anchor({0.f, 0.f});
    this->label_fps->set_color({125, 125, 125});
    this->add_child(label_fps);

    this->schedule(Defined::SCENE_LABEL_FPS_SCHEDULER_KEY, 0.2, [](Base_Node* target, void* global_data) {
        Scene_Node* scene = reinterpret_cast<Scene_Node*>(target);
        scene->label_fps->set_text("frame rate: " + std::to_string((float)scene->fps_rate / 1000));
    });
}

Scene_Node::~Scene_Node() {}

Node_Type Scene_Node::get_type() {
    return Node_Type::SCENE;
}

void Scene_Node::process_frame(float delta_time) {
    this->travel(delta_time, global_data);
}

void Scene_Node::fix_update(float delta_time, void* global_data) {
    Node::update(delta_time);
    this->fps_rate = (int)((1.f / delta_time) * 1000);
}