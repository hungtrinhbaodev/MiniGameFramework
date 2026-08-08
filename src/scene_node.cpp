#include <scene_node.h>
#include <wrapper.h>

Scene_Node::Scene_Node() {
    set_content_size({Libs_Wrapper::get_screen_width(), Libs_Wrapper::get_screen_height()});
    this->label_fps = new Label_Node("", "", 16);
    this->label_fps->set_position({20.f, 15.f});
    this->label_fps->set_anchor({0.f, 0.f});
    this->label_fps->set_color({125, 125, 125});
    this->add_child(label_fps);
    frame_count = 0;
}

Scene_Node::~Scene_Node() {}

void Scene_Node::update(float delta_time) {
    Node::update(delta_time);
    int fps = (int)((1.f / delta_time) * 1000);
    frame_count = (frame_count + 1) % 60;
    if (frame_count % 10 == 0) {
        this->label_fps->set_text("frame rate: " + std::to_string((float)fps / 1000));
    }
}