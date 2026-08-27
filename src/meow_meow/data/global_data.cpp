#include <meow_meow/config/battle_config.h>
#include <meow_meow/data/global_data.h>

namespace Meow_Meow {
    Global_Data* Global_Data::instance = nullptr;

    Global_Data* Global_Data::get() {
        if (instance == nullptr) {
            instance = new Global_Data();
        }
        return instance;
    }

    Global_Data::Global_Data() {}

    Global_Data::~Global_Data() {}

    const Config& Global_Data::get_config() {
        return config;
    }

    Battle_Layer* Global_Data::get_battle_layer() {
        return this->battle_layer;
    }

    Character_Node* Global_Data::get_character_node() {
        return this->character;
    }

    Player_Data& Global_Data::get_player_data() {
        return this->player_data;
    }

    Enemy_Data& Global_Data::get_enemy_data_by(int enemy_id) {
        if (this->enemies.find(enemy_id) == this->enemies.end()) {
            return default_enemy;
        }
        return this->enemies[enemy_id];
    }

    int Global_Data::get_current_battle_level() {
        return this->current_battle_level;
    }

    int Global_Data::get_current_battle_wave() {
        return this->current_battle_wave;
    }

    void Global_Data::set_current_battle_wave(int current_battle_wave) {
        this->current_battle_wave = current_battle_wave;
    }

    void Global_Data::set_battle_layer(Battle_Layer* battle_layer) {
        this->battle_layer = battle_layer;
    }

    void Global_Data::set_character(Character_Node* character) {
        this->character = character;
    }

    void Global_Data::set_enemey_dead(int enemy_id) {
        if (this->enemies.find(enemy_id) == this->enemies.end()) {
            return;
        }
        this->enemies[enemy_id].set_dead(true);
    }

    void Global_Data::generate_enemies_at(int wave) {
        const Battle_Config& battle_config = this->get_config().get_battle_config_at(this->current_battle_level);
        const Enemy_Behavior_Config& enemy_behavior_config = this->get_config().get_enemy_behavior_config();
        const Character_Animation_Config& enemy_animation_config = this->get_config().get_enemy_animation_config();
        int number_generated_enemy_at_wave = battle_config.get_number_enemies_at(wave);
        new_enemies_id_generated.clear();
        for (int i = 0; i < number_generated_enemy_at_wave; i++) {
            int enemy_id = current_enemy_generated_id++;
            this->enemies[enemy_id] = {
                enemy_id,
                enemy_animation_config.get_random_animation_id(),
                enemy_behavior_config.get_enemy_health(),
                enemy_behavior_config.get_enemy_health(),
                false
            };
            new_enemies_id_generated.push_back(enemy_id);
        }
        std::cout << "WHAT is number_generated_enemy_at_wave 2 " << new_enemies_id_generated.size() << std::endl;
    }

    std::vector<int> Global_Data::get_new_enemies_id_generated() {
        return this->new_enemies_id_generated;
    }

    void Global_Data::clear() {
        if (instance != nullptr) {
            delete (instance);
        }
    }

}  // namespace Meow_Meow