#pragma once
#include <image_node.h>
#include <layer_node.h>
#include <meow_meow/data/enemy_data.h>
#include <meow_meow/object/character_node.h>
#include <meow_meow/object/enemy_node.h>

namespace Meow_Meow {
    class Battle_Layer : public Layer_Node {
    public:
        Battle_Layer();
        ~Battle_Layer();

        Image_Node* get_bg();
        Character_Node* get_character_node();
        Layer_Node* get_effect_layer();

        void spawn_enemy(Enemy_Data emeny_data);
        void remove_enemy_by(int enemy_id);
        std::vector<Enemy_Node*>& get_enemy_nodes();

    private:
        void init_bg();
        void init_character();
        void init_effect_layer();

        void show_label_attacked(float delay, float damage, glm::vec2 position);

        Character_Node* character = nullptr;
        Layer_Node* effect_layer = nullptr;
        Image_Node* bg = nullptr;
        std::vector<Enemy_Node*> enemies;
    };
}  // namespace Meow_Meow