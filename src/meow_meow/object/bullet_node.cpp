#include <collision_component.h>
#include <defined.h>
#include <meow_meow/animation/explosion_animation.h>
#include <meow_meow/config/bullet_behavior_config.h>
#include <meow_meow/data/bullet_collision_data.h>
#include <meow_meow/data/enemy_collision_data.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/layer/layer_battle.h>
#include <meow_meow/object/bullet_node.h>
#include <utils.h>

namespace Meow_Meow {

    Bullet_Node::Bullet_Node() {}

    Bullet_Node::Bullet_Node(int character_id, Const::DIRECTION direction, float damage) {
        this->character_id = character_id;
        this->horizontal_direction = direction;
        this->damage = damage;
        this->init_image_bullet();
        this->init_components();
    }

    Bullet_Node::~Bullet_Node() {
        Collision_Component* collision =
            Utils::get_component<Collision_Component>(this, Defined::COMPONENT_COLLISION_NAME);
        Bullet_Collision_Data* collision_data = Utils::get_collision_owner_data<Bullet_Collision_Data>(collision);
        delete (collision_data);
    }

    Custom::Transformed_Rectangle Bullet_Node::get_bounding_box(void* global_data) {
        if (this->image_bullet == nullptr) {
            return Game_Object::get_bounding_box(global_data);
        }
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Bullet_Behavior_Config& behavior_config = data->get_config().get_bullet_behavior_config();
        Custom::Size image_size = behavior_config.get_bullet_bounding_box();
        return Custom::Transformed_Rectangle{
            Custom::Rectangle{image_size.width, image_size.height}.apply(this->get_transform(), this->get_anchor())
        };
    }

    void Bullet_Node::update_movemenet(float delta_time) {
        int direction = horizontal_direction == Const::DIRECTION::LEFT ? -1 : 1;
        this->velosity -= delta_time * this->accelarate;
        this->set_position(this->get_position() + glm::vec2(this->velosity, 0) * (delta_time * direction));
    }

    void Bullet_Node::update_collision_data() {
        Collision_Component* collision =
            Utils::get_component<Collision_Component>(this, Defined::COMPONENT_COLLISION_NAME);
        Bullet_Collision_Data* collision_data = Utils::get_collision_owner_data<Bullet_Collision_Data>(collision);
        collision_data->set_bullet_direction(this->horizontal_direction);
    }

    void Bullet_Node::fix_update(float delta_time, void* global_data) {
        this->handle_collision(delta_time, global_data);
        this->update_movemenet(delta_time);
        this->update_collision_data();
        Game_Object::fix_update(delta_time, global_data);
    }

    void Bullet_Node::attach(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Bullet_Behavior_Config& behavior_config = data->get_config().get_bullet_behavior_config();
        Collision_Component* collision =
            Utils::get_component<Collision_Component>(this, Defined::COMPONENT_COLLISION_NAME);
        collision->set_box_size(behavior_config.get_bullet_bounding_box());
        this->velosity = behavior_config.get_bullet_velosity();
        this->accelarate = behavior_config.get_bullet_accelarate();
    }

    void Bullet_Node::handle_boundary(void* global_data) {
        this->remove_from_parent();
    }

    void Bullet_Node::init_image_bullet() {
        std::string path = Const::PATH_BULLET_IMAGE + std::to_string(this->character_id) + ".png";
        this->image_bullet = new Image_UI_Node();
        this->image_bullet->set_image(path);
        this->add_child(this->image_bullet);
    }

    void Bullet_Node::init_components() {
        Collision_Component* collision = new Collision_Component();
        collision->set_name(Defined::COMPONENT_COLLISION_NAME);
        collision->set_anchor({0.5, 0.5});
        Bullet_Collision_Data* collision_data = new Bullet_Collision_Data();
        collision_data->set_damage_deal(this->damage);
        collision->set_owner_data(collision_data);
        collision->set_track_layer(Const::BATTLE_LAYER_COLLISION);
        collision->set_tag(Const::BULLET_COLLISION_TAG);
        this->add_component(collision);
    }

    void Bullet_Node::change_to_explore(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Battle_Layer* battle_layer = data->get_battle_layer();
        this->remove_from_parent();
        if (battle_layer == nullptr)
            return;
        Explosion_Animation* explosion_animation = new Explosion_Animation(this->get_position());
        battle_layer->add_child(explosion_animation);
    }

    void Bullet_Node::handle_collision(float delta_time, void* global_data) {
        Collision_Component* collision_component =
            Utils::get_component<Collision_Component>(this, Defined::COMPONENT_COLLISION_NAME);

        Bullet_Collision_Data* bullet_collision_data =
            Utils::get_collision_owner_data<Bullet_Collision_Data>(collision_component);
        if (bullet_collision_data->is_hitted()) {
            this->change_to_explore(global_data);
            return;
        }
    }

}  // namespace Meow_Meow