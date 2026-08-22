#pragma once
#include <image_node.h>

#include <map>
#include <string>

class Animation_Node : public Image_Node {
public:
    Node_Type get_type() override;

    void make_animation(
        std::string name,
        std::string folder_path,
        int number_frame,
        float duration_loop,
        std::string extend_format = ".png"
    );
    void play_animation(std::string name, float speed = 1.0f, bool is_reset = false);
    void set_speed(float speed);
    void clear_all_animation();
    float get_amimation_duration(std::string name);

protected:
    void flex_update(float delta_time) override;

private:
    struct Animation_Data {
        std::string name;
        std::string folder_path;
        int number_frame;
        float duration_loop;
        std::string extend_format;
    };

    std::string current_animation;
    int current_frame = 0;
    float speed_ratio;
    float total_delta_time = 0;
    std::map<std::string, Animation_Data> animations;

    bool is_valid_animation(std::string);
};