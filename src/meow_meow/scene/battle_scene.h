#pragma once
#include <layer_node.h>
#include <meow_meow/layer/layer_battle.h>
#include <scene_node.h>

namespace Meow_Meow {
    class Battle_Scene : public Scene_Node {
    public:
        Battle_Scene();
        ~Battle_Scene();

    protected:
        void attach(void* global_data) override;

    private:
        Layer_Node* layer_ui = nullptr;
        Battle_Layer* battle_layer = nullptr;
    };
}  // namespace Meow_Meow