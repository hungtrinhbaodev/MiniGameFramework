#include <math_custom.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/layer/layer_battle.h>

namespace Meow_Meow {
    Battle_Layer::Battle_Layer() {
        this->init_bg();
        this->init_character();
        this->init_effect_layer();
        this->set_name("Battle_Layer");
    }

    Battle_Layer::~Battle_Layer() {}

    void Battle_Layer::init_bg() {
        this->bg = new Image_Node("res/meow_meow/Area/Area1.png");
        this->set_content_size(this->bg->get_content_size());
        this->add_child(this->bg);

        Custom::Size layer_size = this->get_content_size();
        this->bg->set_position({layer_size.width / 2, layer_size.height / 2});
    }

    void Battle_Layer::init_character() {
        this->character = new Character_Node();
        this->set_camera_focus_on(this->character, 0.2f);
        this->add_child(this->character);

        Custom::Size layer_size = this->get_content_size();
        this->character->set_position(
            {Math::random_float(0, layer_size.width), Math::random_float(0, layer_size.height)}
        );
    }

    void Battle_Layer::init_effect_layer() {
        this->effect_layer = new Layer_Node();
        this->effect_layer->set_content_size(this->get_content_size());
        this->effect_layer->set_z_order(1);
        this->add_child(this->effect_layer);
        this->effect_layer->set_name("Battle_Layer::effect_layer");
    }

    void Battle_Layer::show_label_attacked(float delay, float damage, glm::vec2 position) {}

    bool Battle_Layer::remove_enemy(std::vector<Enemy_Node*>& enemies, int enemy_id) {
        for (int i = 0; i < enemies.size(); i++) {
            if (enemies[i]->get_enemy_id() == enemy_id) {
                if (enemies[i]->get_parent() != nullptr) {
                    enemies[i]->remove_from_parent();
                }
                enemies[i] = enemies.back();
                enemies.pop_back();
                i--;
                return true;
            }
        }
        return false;
    }

    bool Battle_Layer::remove_boss(std::vector<Boss_Node*>& bosses, int boss_id) {
        for (int i = 0; i < bosses.size(); i++) {
            if (bosses[i]->get_enemy_id() == boss_id) {
                if (bosses[i]->get_parent() != nullptr) {
                    bosses[i]->remove_from_parent();
                }
                bosses[i] = bosses.back();
                bosses.pop_back();
                i--;
                return true;
            }
        }
        return false;
    }

    Image_Node* Battle_Layer::get_bg() {
        return this->bg;
    }

    Character_Node* Battle_Layer::get_character_node() {
        return this->character;
    }

    Layer_Node* Battle_Layer::get_effect_layer() {
        return this->effect_layer;
    }

    const std::vector<Enemy_Node*>& Battle_Layer::get_all_enemy_nodes() {
        return this->all_enemies;
    }

    const std::vector<Enemy_Node*>& Battle_Layer::get_enemies() {
        return this->enemies;
    }

    const std::vector<Boss_Node*>& Battle_Layer::get_bosses() {
        return this->bosses;
    }

    Boss_Node* Battle_Layer::get_boss_by(int boss_id) {
        for (Boss_Node* boss : this->bosses) {
            if (boss->get_enemy_id() == boss_id) {
                return boss;
            }
        }
        return nullptr;
    }

    void Battle_Layer::spawn_enemy(Enemy_Data emeny_data) {
        Enemy_Node* enemy = new Enemy_Node(emeny_data.get_enemy_id(), emeny_data.get_enemy_animation_id());
        glm::vec2 player_position = this->character->get_position();
        enemy->set_position(
            player_position + glm::vec2{Math::random_float(-1000, 1000), Math::random_float(-400, 400)}
        );
        this->add_child(enemy);
        this->enemies.push_back(enemy);
        this->all_enemies.push_back(enemy);
    }

    void Battle_Layer::remove_enemy_by(int enemy_id) {
        if (this->remove_enemy(this->enemies, enemy_id)) {
            this->remove_enemy(this->all_enemies, enemy_id);
            this->removed_enemies_id.push_back(enemy_id);
        }
    }

    void Battle_Layer::spawn_boss(Enemy_Data boss_data) {
        Boss_Node* boss = new Boss_Node(boss_data.get_enemy_id(), 2003);
        glm::vec2 player_position = this->character->get_position();
        boss->set_position(player_position + glm::vec2{Math::random_float(-1000, 1000), Math::random_float(-400, 400)});
        this->add_child(boss);
        this->bosses.push_back(boss);
        this->all_enemies.push_back(boss);
    }

    void Battle_Layer::remove_boss_by(int boss_id) {
        if (this->remove_boss(this->bosses, boss_id)) {
            this->remove_enemy(this->all_enemies, boss_id);
            this->removed_bosses_id.push_back(boss_id);
        }
    }

    void Battle_Layer::fix_update(float delta_time, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        for (int id : this->removed_enemies_id) {
            data->remove_enemy_by(id);
        }
        for (int id : this->removed_bosses_id) {
            data->remove_boss_by(id);
        }
        this->removed_enemies_id.clear();
        this->removed_bosses_id.clear();
    }
}  // namespace Meow_Meow