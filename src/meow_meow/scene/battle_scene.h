#pragma once
#include <layer_node.h>
#include <meow_meow/layer/layer_battle.h>
#include <meow_meow/layer/layer_stats.h>
#include <scene_node.h>

namespace Meow_Meow {
    class Battle_Scene : public Scene_Node {
    public:
        Battle_Scene();
        ~Battle_Scene();

    protected:
        void attach(void* global_data) override;

    private:
        Battle_Layer* battle_layer = nullptr;
        Layer_Stats* stats_layer = nullptr;
    };
}  // namespace Meow_Meow