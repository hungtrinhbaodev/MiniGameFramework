#pragma once
#include <meow_meow/config/config.h>
#include <meow_meow/data/enemy_data.h>
#include <meow_meow/data/player_data.h>
#include <meow_meow/data/skill_data.h>
#include <meow_meow/layer/layer_battle.h>
#include <meow_meow/object/character_node.h>

#include <map>

namespace Meow_Meow {
    class Global_Data {
    public:
        static Global_Data* get();
        static void clear();

        Global_Data();
        ~Global_Data();

        const Config& get_config();
        Battle_Layer* get_battle_layer();
        Character_Node* get_character_node();
        Player_Data& get_player_data();
        Enemy_Data& get_enemy_data_by(int enemy_id);
        Enemy_Data& get_boss_data_by(int boss_id);
        int get_current_battle_level();
        int get_current_battle_wave();
        std::vector<int> get_new_enemies_id_generated();
        std::vector<int> get_new_bosses_id_generated();
        Layer_Node* get_effect_layer();
        bool is_character_level_up();
        std::vector<Skill_Data>& get_characters_skill_data();
        Skill_Data* get_skill_data_by(std::string skill_name);

        void set_current_battle_wave(int current_battle_wave);
        void set_battle_layer(Battle_Layer* battle_layer);
        void set_character(Character_Node* character);
        void set_enemey_dead(int enemy_id);
        void set_effect_layer(Layer_Node* layer);
        void generate_enemies_at(int wave);
        void character_level_up();

    private:
        static Global_Data* instance;
        Config config{};

        Player_Data player_data;

        Enemy_Data default_enemy{};
        std::map<int, Enemy_Data> enemies;
        std::map<int, Enemy_Data> bosses;
        std::vector<int> new_enemies_id_generated;
        std::vector<int> new_bosses_id_generated;

        int current_battle_level = 0;
        int current_battle_wave = 0;
        int current_enemy_generated_id = 0;

        Battle_Layer* battle_layer = nullptr;
        Character_Node* character = nullptr;
        Layer_Node* effect_layer = nullptr;

        std::vector<Skill_Data> character_skills_data;

        void init_skills_data();
    };
}  // namespace Meow_Meow