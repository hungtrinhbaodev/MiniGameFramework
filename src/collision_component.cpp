#include <collision_component.h>
#include <defined.h>
#include <wrapper.h>

Collision_Component::Collision_Component() {}

Collision_Component::~Collision_Component() {}

Collision_Component* Collision_Component::make(
    int tag,
    Custom::Size box_size,
    int track_layer,
    glm::vec2 delta_position,
    void* owner_data,
    std::function<void(Base_Node* target, std::vector<Collision_Information>)> caller
) {
    Collision_Component* collision = new Collision_Component();
    collision->set_name(Defined::COMPONENT_COLLISION_NAME);
    collision->set_tag(tag);
    collision->set_box_size(box_size);
    collision->set_track_layer(track_layer);
    collision->set_owner_data(owner_data);
    collision->set_collision_handler(caller);
    return collision;
}

int Collision_Component::get_tag() {
    return this->tag;
}

void* Collision_Component::get_owner_data() {
    return this->owner_data;
}

Custom::Size Collision_Component::get_box_size() {
    return this->box_size;
}

glm::vec2 Collision_Component::get_delta_position() {
    return this->delta_position;
}

std::vector<Collision_Information> Collision_Component::get_collisioneds() {
    return Collision_System::get()->query_collisions(this->collision_id);
}

void Collision_Component::set_tag(int tag) {
    this->tag = tag;
}

void Collision_Component::set_owner_data(void* owner_data) {
    this->owner_data = owner_data;
}

void Collision_Component::set_box_size(Custom::Size size) {
    this->box_size = size;
}

void Collision_Component::set_track_layer(int track_layer) {
    this->track_layer = track_layer;
}

void Collision_Component::set_delta_position(glm::vec2 position) {
    this->delta_position = delta_position;
}

void Collision_Component::set_collision_handler(
    std::function<void(Base_Node* target, std::vector<Collision_Information>)> handler
) {
    this->collision_handler = handler;
}

void Collision_Component::enter() {
    this->collision_id = Collision_System::get()->request_collision();
}

void Collision_Component::exit() {
    Collision_System::get()->remove_collision(this->collision_id);
}

void Collision_Component::draw(int& draw_index) {
    if (!Libs_Wrapper::is_debug_mode())
        return;
    Custom::Transform local_transform{};
    Custom::Transform world_transform = this->target->get_world_transform();
    Custom::Anchor_Point anchor = this->target->get_anchor();
    local_transform = local_transform.set_position(delta_position);
    world_transform.forward(local_transform, false, {false, false});
    draw_index += Custom::Rectangle{box_size.width, box_size.height}.draw_rectangle(
        world_transform, anchor, draw_index, {160, 0, 20}, 120
    );
}

void Collision_Component::update_information() {
    if (this->target == nullptr) {
        return;
    }
    Custom::Transform local_transform{};
    Custom::Transform world_transform = this->target->get_world_transform();
    Custom::Anchor_Point anchor = this->target->get_anchor();
    local_transform = local_transform.set_position(delta_position);
    world_transform.forward(local_transform, false, {false, false});
    Collision_System::get()->request_update_information(
        {this->collision_id,
         this->tag,
         this->track_layer,
         this->target,
         this->owner_data,
         Custom::Transformed_Rectangle{
             Custom::Rectangle{box_size.width, box_size.height}.apply(world_transform, anchor)
         }}
    );
}

void Collision_Component::handle_task() {
    if (this->is_active() && this->has_target()) {
        if (this->collision_handler != nullptr) {
            std::vector<Collision_Information> collsioneds = this->get_collisioneds();
            if (collsioneds.size() > 0) {
                this->collision_handler(this->target, this->get_collisioneds());
            }
        }
    }
}