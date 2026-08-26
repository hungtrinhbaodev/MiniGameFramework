#pragma once
#include <meow_meow/config/config.h>
#include <meow_meow/data/player_data.h>
#include <meow_meow/layer/layer_battle.h>
#include <meow_meow/object/character_node.h>

namespace Meow_Meow {
    class Global_Data {
    public:
        static Global_Data* get();
        static void clear();

        Global_Data();
        ~Global_Data();

        const Config& get_config();
        Battle_Layer* get_battle_layer();
        Character_Node* get_character();
        Player_Data& get_player_data();

        void set_battle_layer(Battle_Layer* battle_layer);
        void set_character(Character_Node* character);

    private:
        static Global_Data* instance;
        Config config;
        Battle_Layer* battle_layer = nullptr;
        Character_Node* character = nullptr;
        Player_Data player_data;
    };
}  // namespace Meow_Meow