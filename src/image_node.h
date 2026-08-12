#pragma once
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
    Node_Type get_type() override;

    void set_image(std::string image_path);

    /**Handle touch to Image */
    void set_touch_enabled(bool touch_enabled);
    void set_swallow_touches(bool swallow_touches);
    /**
     * Notes: the caller will call with target and touch
     * position argument but the target is form Base_Node
     * so if wanna use other type please manual cast it!
     * */
    void set_touched_caller(std::function<void(glm::vec2, Base_Node*)> caller);

protected:
    void handle_personal_task() override;
    void update_world_transform_information(Custom::Transform& world_transform, int draw_index) override;
    void draw(Custom::Transform& world_transform, int& draw_index) override;

    /**Handle touch to Image */
    virtual Custom::Size get_touch_size();

private:
    std::string image_path = "";

    /**Handle touches */
    int touch_component_id = -1;
    Touch_Information touch_info;
    std::function<void(glm::vec2, Base_Node*)> caller = nullptr;
};