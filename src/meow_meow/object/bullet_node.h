#pragma once
#include <image_ui_node.h>
#include <meow_meow/const.h>
#include <meow_meow/object/game_object.h>

namespace Meow_Meow {
    class Bullet_Node : public Game_Object {
    public:
        Bullet_Node();
        Bullet_Node(int character_id, Const::DIRECTION direction);
        ~Bullet_Node();

    protected:
        Custom::Transformed_Rectangle get_bounding_box() override;
        void handle_boundary() override;
        void fix_update(float delta_time, void* global_data) override;

    private:
        void init_image_bullet();
        void init_bullet_component();

        const float VELOSITY = 800;
        const float ACCELARATE = 2;

        int character_id = -1;
        float velosity = VELOSITY;
        Const::DIRECTION horizontal_direction = Const::DIRECTION::LEFT;
        Image_UI_Node* image_bullet = nullptr;
    };
}  // namespace Meow_Meow