#pragma once
#include <image_node.h>
#include <layer_node.h>
#include <meow_meow/data/enemy_data.h>
#include <meow_meow/object/boss_node.h>
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
        void spawn_boss(Enemy_Data boss_data);
        void remove_boss_by(int boss_id);
        const std::vector<Enemy_Node*>& get_all_enemy_nodes();
        const std::vector<Enemy_Node*>& get_enemies();
        const std::vector<Boss_Node*>& get_bosses();
        Boss_Node* get_boss_by(int boss_id);

    protected:
        void fix_update(float delta_time, void* global_data) override;

    private:
        void init_bg();
        void init_character();
        void init_effect_layer();

        void show_label_attacked(float delay, float damage, glm::vec2 position);
        bool remove_enemy(std::vector<Enemy_Node*>& enemies, int enemy_id);
        bool remove_boss(std::vector<Boss_Node*>& bosses, int boss_id);

        Character_Node* character = nullptr;
        Layer_Node* effect_layer = nullptr;
        Image_Node* bg = nullptr;
        std::vector<Enemy_Node*> enemies;
        std::vector<Boss_Node*> bosses;
        std::vector<Enemy_Node*> all_enemies;
        std::vector<int> removed_bosses_id;
        std::vector<int> removed_enemies_id;
    };
}  // namespace Meow_Meow