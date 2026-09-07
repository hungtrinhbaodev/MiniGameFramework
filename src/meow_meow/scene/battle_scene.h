#pragma once
#include <layer_node.h>
#include <meow_meow/gui/lose_game_gui.h>
#include <meow_meow/gui/start_round_gui.h>
#include <meow_meow/gui/win_game_gui.h>
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
        void fix_update(float delta_time, void* global_data) override;

    private:
        Battle_Layer* battle_layer = nullptr;
        Layer_Stats* stats_layer = nullptr;
        float battle_countdown_wave = 0.f;

        Win_Game_GUI* win_game_gui = nullptr;
        Lose_Game_GUI* lose_game_gui = nullptr;
        Start_Round_GUI* start_round_gui = nullptr;
        void generate_new_enemies_wave(void* global_data);
    };
}  // namespace Meow_Meow