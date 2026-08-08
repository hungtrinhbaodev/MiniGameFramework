#pragma once
#include <label_node.h>
#include <layer_node.h>

class Scene_Node : public Layer_Node {
public:
    Scene_Node();
    ~Scene_Node();

protected:
    void update(float delta_time) override;

private:
    Label_Node* label_fps;
    int frame_count;
};