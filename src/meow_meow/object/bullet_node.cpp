#include <meow_meow/object/bullet_node.h>

namespace Meow_Meow {

    Bullet_Node::Bullet_Node() {}

    Bullet_Node::Bullet_Node(int character_id, Const::DIRECTION direction) {
        this->character_id = character_id;
        this->horizontal_direction = direction;
        this->init_image_bullet();
        this->init_bullet_component();
    }

    Bullet_Node::~Bullet_Node() {}

    Custom::Transformed_Rectangle Bullet_Node::get_bounding_box() {
        if (this->image_bullet == nullptr) {
            return Game_Object::get_bounding_box();
        }
        Custom::Size image_size = this->image_bullet->get_renderer_size();
        return Custom::Transformed_Rectangle{
            Custom::Rectangle{image_size.width, image_size.height}.apply(this->get_transform(), this->get_anchor())
        };
    }

    void Bullet_Node::fix_update(float delta_time, void* global_data) {
        int direction = horizontal_direction == Const::DIRECTION::LEFT ? -1 : 1;
        velosity -= delta_time * ACCELARATE;
        this->set_position(this->get_position() + glm::vec2(velosity, 0) * (delta_time * direction));
        Game_Object::fix_update(delta_time, global_data);
    }

    void Bullet_Node::handle_boundary() {
        this->remove_from_parent();
    }

    void Bullet_Node::init_image_bullet() {
        std::string path = Const::PATH_BULLET_IMAGE + std::to_string(this->character_id) + ".png";
        this->image_bullet = new Image_UI_Node();
        this->image_bullet->set_image(path);
        this->add_child(this->image_bullet);
    }

    void Bullet_Node::init_bullet_component() {}

}  // namespace Meow_Meow