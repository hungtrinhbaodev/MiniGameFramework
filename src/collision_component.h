#pragma once
#include <base_component.h>
#include <collision_system.h>

#include <functional>

class Collision_Component : public Base_Component {
public:
    static Collision_Component* make(
        int tag,
        Custom::Size box_size,
        int track_layer = 0,
        glm::vec2 delta_position = {0.f, 0.f},
        void* owner_data = nullptr,
        std::function<void(Base_Node* target, std::vector<Collision_Information>)> caller = nullptr
    );
    Collision_Component();
    ~Collision_Component();

    int get_tag();
    void* get_owner_data();
    Custom::Size get_box_size();
    glm::vec2 get_delta_position();
    std::vector<Collision_Information> get_collisioneds();

    void set_tag(int tag);
    void set_owner_data(void* owner_data);
    void set_box_size(Custom::Size size);
    void set_delta_position(glm::vec2 delta_position);
    void set_track_layer(int track_layer);
    void set_collision_handler(std::function<void(Base_Node* target, std::vector<Collision_Information>)> handler);

    void enter() override;
    void exit() override;
    void draw(int& draw_index) override;
    void update_information() override;
    void handle_task() override;

private:
    /** Using this tag to cast inner data right away!*/
    int tag;
    /** Using this data to cast when collision happen */
    void* owner_data;
    /**Note: only objects with same track layer can be collision with each other!*/
    int track_layer = -1;
    int collision_id = -1;
    Custom::Size box_size;
    /**
     * Note: the middle box will we place at anchor of object with
     * with case wanna custom position to box add it to here the
     * box will move more from anchor of object with this detla!
     */
    glm::vec2 delta_position{0.f, 0.f};
    /**
     * Note: caller will be applied in handle_personal_task of Node
     * so if you don't wanna use functional you can extend object
     * from node and call get_collisioneds in handle_personal_task
     * that overrided from the base node to manully use!
     */
    std::function<void(Base_Node* target, std::vector<Collision_Information>)> collision_handler = nullptr;
};