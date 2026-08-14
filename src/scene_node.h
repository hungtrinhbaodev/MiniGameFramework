#pragma once
#include <label_node.h>
#include <layer_node.h>

class Scene_Node : public Layer_Node {
public:
    Scene_Node();
    ~Scene_Node();

    Node_Type get_type() override;
    void process_frame(float delta_time);

protected:
    void fix_update(float delta_time, void* global_data) override;

private:
    Label_Node* label_fps = nullptr;
    float fps_rate = 0.f;
    int frame_count;
    void* global_data = nullptr;
};