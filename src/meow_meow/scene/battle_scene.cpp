#include <meow_meow/config/battle_config.h>
#include <meow_meow/config/character_behavior_config.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/data/player_data.h>
#include <meow_meow/scene/battle_scene.h>

namespace Meow_Meow {
    Battle_Scene::Battle_Scene() {
        this->battle_layer = new Battle_Layer();
        this->add_child(this->battle_layer);
        this->track_layer_background(this->battle_layer->get_bg());

        this->stats_layer = new Layer_Stats();
        this->stats_layer->set_position({0.f, 0.f});
        this->add_child(this->stats_layer);

        this->start_round_gui = new Start_Round_GUI();
        this->add_child(this->start_round_gui);
    }

    Battle_Scene::~Battle_Scene() {}

    void Battle_Scene::attach(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        data->set_battle_layer(this->battle_layer);
        data->set_character(this->battle_layer->get_character_node());
        data->set_effect_layer(this->battle_layer->get_effect_layer());

        const Character_Behavior_Config& behavior_config = data->get_config().get_character_behavior_config();
        Player_Data& player_data = data->get_player_data();
        player_data.set_max_health(behavior_config.get_max_health());
        player_data.set_current_health(behavior_config.get_max_health());
        player_data.set_player_damage(behavior_config.get_damage());
    }

    void Battle_Scene::fix_update(float delta_time, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        int current_battle_level = data->get_current_battle_level();
        int current_battle_wave = data->get_current_battle_wave();
        const Battle_Config& battle_config = data->get_config().get_battle_config_at(current_battle_level);
        if (current_battle_wave < battle_config.get_number_wave()) {
            float duration_wave_countdown = battle_config.get_duration_generate_enemy_at(current_battle_wave);
            if (this->battle_countdown_wave >= duration_wave_countdown) {
                data->generate_enemies_at(current_battle_wave);
                this->generate_new_enemies_wave(global_data);
                data->set_current_battle_wave(current_battle_wave + 1);
                this->battle_countdown_wave = 0.f;
                this->start_round_gui->show();
                return;
            }
            this->battle_countdown_wave += delta_time;
            data->set_current_battle_duration(data->get_current_battle_duration() + delta_time);
        }
        if (data->is_player_win()) {
            if (this->win_game_gui == nullptr) {
                this->battle_layer->set_paused(true);
                this->stats_layer->set_paused(true);
                this->win_game_gui = new Win_Game_GUI();
                this->add_child(this->win_game_gui);
            }
            return;
        }
        if (data->is_player_lose()) {
            if (this->lose_game_gui == nullptr) {
                this->battle_layer->set_paused(true);
                this->stats_layer->set_paused(true);
                this->lose_game_gui = new Lose_Game_GUI();
                this->add_child(this->lose_game_gui);
            }
            return;
        }
    }

    void Battle_Scene::generate_new_enemies_wave(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        std::vector<int> new_enemies_id = data->get_new_enemies_id_generated();
        for (int i = 0; i < new_enemies_id.size(); i++) {
            int enemy_id = new_enemies_id[i];
            Enemy_Data& enemy_data = data->get_enemy_data_by(enemy_id);
            this->battle_layer->spawn_enemy(enemy_data);
        }

        std::vector<int> new_bossed_id = data->get_new_bosses_id_generated();
        for (int i = 0; i < new_bossed_id.size(); i++) {
            int boss_id = new_bossed_id[i];
            Enemy_Data& enemy_data = data->get_boss_data_by(boss_id);
            this->battle_layer->spawn_boss(enemy_data);
        }
    }
}  // namespace Meow_Meow