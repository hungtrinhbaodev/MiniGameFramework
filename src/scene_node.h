#pragma once
#include <label_node.h>
#include <layer_node.h>

class Scene_Node : public Layer_Node {
public:
    Scene_Node();
    Scene_Node(void* global_data);
    ~Scene_Node();

    Node_Type get_type() override;
    void set_global_data(void* global_data);
    void process_frame(float delta_time);
    void set_initialize(bool initialize);
    void start();

protected:
    void fix_update(float delta_time, void* global_data) override;
    void enter(void* global_data) override;

private:
    Label_Node* label_fps = nullptr;
    float fps_rate = 0.f;
    int frame_count;
    void* global_data = nullptr;
    /**
     * @Note: before anything in loop happen
     * we need to confirm that the global data
     * is set to make the right flow in travel
     */
    bool initialize = false;
};