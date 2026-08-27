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
        void spawn_enemy(Enemy_Data emeny_data);
        void remove_enemy_by(int enemy_id);

    protected:
        void attach(void* global_data) override;

    private:
        void init_bg();
        void init_character();

        void show_label_attacked(float delay, float damage, glm::vec2 position);

        Character_Node* character = nullptr;
        std::vector<Enemy_Node*> enemies;
        Image_Node* bg = nullptr;
    };
}  // namespace Meow_Meow