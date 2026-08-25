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

    Character_Node* Global_Data::get_character() {
        return this->character;
    }

    void Global_Data::set_battle_layer(Battle_Layer* battle_layer) {
        this->battle_layer = battle_layer;
    }

    void Global_Data::set_character(Character_Node* character) {
        this->character = character;
    }

    void Global_Data::clear() {
        if (instance != nullptr) {
            delete (instance);
        }
    }

}  // namespace Meow_Meow