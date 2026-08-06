#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Base_Node {
public:
    struct Transform {
        glm::vec2 position{0.0f, 0.0f};
        glm::vec2 scale{1.0f, 1.0f};
        glm::vec2 anchor{0.0f, 0.0f};
        float rotation = 0.0f;
        int z_order = 0;
        unsigned char opacity = 255;
        glm::u8vec3 color{255, 255, 255};

        void forward(const Transform& other, bool is_cascade_opacity);
        void inverse(const Transform& other, unsigned char inverse_opacity);
        bool operator<(const Transform& other) const;
    };

    Base_Node();
    ~Base_Node();

    float get_x();
    float get_y();
    float get_scale_x();
    float get_scale_y();
    float get_rotation();
    float get_anchor_x();
    float get_anchor_y();
    glm::vec2 get_position();
    glm::vec2 get_scale();
    glm::vec2 get_anchor();
    int get_opacity();
    int get_z_order();
    int get_tag();
    std::string get_name();
    bool is_visible();
    bool is_cascade_opacity();
    Base_Node* get_parent();

    std::vector<Base_Node*>& get_children();
    Base_Node* get_child_by_tag(int tag);
    Base_Node* get_child_by_name(std::string name);

    void set_x(float x);
    void set_y(float y);
    void set_scale_x(float scale_x);
    void set_scale_y(float scale_y);
    void set_rotation(float roation);
    void set_anchor_x(float anchor_x);
    void set_anchor_y(float anchor_y);
    void set_position(glm::vec2 position);
    void set_scale(glm::vec2 scale);
    void set_anchor(glm::vec2 anchor);
    void set_opacity(int opacity);
    void set_z_order(int z_order);
    void set_visible(bool visible);
    void set_cascade_opacity(bool cascade);

    void set_tag(int tag);
    void set_name(std::string name);
    void add_child(Base_Node* child);
    void travel(float delta_time);
    void remove_from_parent(bool is_cleanup = true);
    bool remove_child(Base_Node* child, bool is_cleanup = false);

protected:
    Transform transform;
    std::vector<Base_Node*> children;
    Base_Node* parent = nullptr;

    void visit(Transform& world_transform, float delta_time, int& draw_index);
    virtual void before_draw_children(Transform& world_transform, int& draw_index);
    virtual void draw(Transform& world_transform, int& draw_index);
    virtual void after_draw_children(Transform& world_transform, int& draw_index);
    virtual void update(float delta_time);

    // Call when node attach from parent
    virtual void enter();
    // Call when node detech from parent
    virtual void exit();

private:
    int tag = -1;
    std::string name = "";
    bool visible = true;
    bool casecade_opacity = false;
    int total_node = 1;
    std::vector<Base_Node*> cleanup_children;
    // Invalid happen when node in list cleanup_children
    // with this node we don't keep track it anymore
    bool is_valid = true;

    void sort_nodes();
    void cleanup_invalid_children();
};