#include <math_custom.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/layer/layer_battle.h>

namespace Meow_Meow {
    Battle_Layer::Battle_Layer() {
        this->init_bg();
        this->init_character();
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

    void Battle_Layer::show_label_attacked(float delay, float damage, glm::vec2 position) {}

    Image_Node* Battle_Layer::get_bg() {
        return this->bg;
    }

    void Battle_Layer::spawn_enemy(Enemy_Data emeny_data) {
        Enemy_Node* enemy = new Enemy_Node(emeny_data.get_enemy_id(), emeny_data.get_enemy_animation_id());
        glm::vec2 player_position = this->character->get_position();
        enemy->set_position(
            player_position + glm::vec2{Math::random_float(-1000, 1000), Math::random_float(-400, 400)}
        );
        this->add_child(enemy);
        this->enemies.push_back(enemy);
    }

    void Battle_Layer::remove_enemy_by(int enemy_id) {
        for (int i = 0; i < this->enemies.size(); i++) {
            if (this->enemies[i]->get_enemy_id() == enemy_id) {
                this->enemies[i]->remove_from_parent();
                this->enemies[i] = this->enemies.back();
                this->enemies.pop_back();
                i--;
            }
        }
    }

    void Battle_Layer::attach(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        data->set_character(this->character);
    }
}  // namespace Meow_Meow