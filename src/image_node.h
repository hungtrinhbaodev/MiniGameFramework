#pragma once
#include <defined.h>
#include <node.h>
#include <touch_system.h>

#include <functional>
#include <string>

class Image_Node : public Node {
public:
    Image_Node();
    Image_Node(std::string image_path);
    ~Image_Node();

    std::string get_image();
    Custom::Size get_content_size();
    Custom::Size get_scaled_size();
    Node_Type get_type() override;
    std::function<void(glm::vec2, Base_Node*, void*)> get_touch_caller();
    bool is_enable_touched();
    bool is_swallow_touches();

    void set_image(std::string image_path, Defined::LOAD_MODE load_mode = Defined::LOAD_MODE::IMMEDIATE);
    /**Handle touch to Image */
    void set_touch_enabled(bool touch_enabled);
    void set_swallow_touches(bool swallow_touches);
    /**
     * @Note: the caller will call with target and touch
     * position argument but the target is form Base_Node
     * so if wanna use other type please manual cast it!
     * @Param glm::vec2: touch position.
     * @Param Base_Node*: target running this component.
     * @Param void*: global user data.
     * */
    void set_touched_caller(std::function<void(glm::vec2, Base_Node*, void*)> caller);
    void set_resource_loaded_caller(std::function<void()> caller);

    void set_enable_draw_rect(bool enable_draw_rect);
    void set_draw_rect(Custom::Rectangle_Area draw_rect);

protected:
    Defined::LOAD_MODE load_mode = Defined::LOAD_MODE::IMMEDIATE;
    void draw(Custom::Transform& world_transform, int& draw_index) override;
    void handle_personal_task(float delta_time, void* global_data) override;
    virtual void on_texture_loaded();

private:
    std::string image_path = "";

    /**Handle touches */
    bool touch_enabled = false;
    bool swallow_touches = false;
    std::function<void(glm::vec2, Base_Node*, void*)> touch_caller = nullptr;
    std::function<void()> resource_loaded_caller = nullptr;
    bool enable_draw_rect = false;
    Custom::Rectangle_Area draw_rect{0, 0, 0, 0};
    bool is_call_loaded_resource = false;
};