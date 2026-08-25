#include <meow_meow/data/global_data.h>
#include <meow_meow/layer/layer_battle.h>
#include <meow_meow/object/game_object.h>
#include <utils.h>

namespace Meow_Meow {

    bool is_out_layer(const glm::vec2& point, const Custom::Size& layer_size) {
        if (point.x < 0 || point.y < 0)
            return true;
        return point.x > layer_size.width || point.y > layer_size.height;
    }

    Game_Object::Game_Object() {}

    Game_Object::~Game_Object() {}

    void Game_Object::fix_update(float delta_time, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Battle_Layer* layer = data->get_battle_layer();
        Custom::Size layer_size = layer->get_content_size();
        Custom::Transformed_Rectangle rect = this->get_bounding_box();
        for (glm::vec2& point : rect.points) {
            if (is_out_layer(point, layer_size)) {
                this->handle_boundary(global_data);
                break;
            }
        }
    }

    Custom::Transformed_Rectangle Game_Object::get_bounding_box() {
        Custom::Rectangle rect{0, 0};
        return Custom::Transformed_Rectangle{rect.apply(this->get_transform(), this->get_anchor())};
    }

    void Game_Object::handle_boundary(void* global_data) {}

}  // namespace Meow_Meow