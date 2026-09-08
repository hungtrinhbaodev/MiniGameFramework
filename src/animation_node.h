#pragma once
#include <defined.h>
#include <image_node.h>

#include <functional>
#include <map>
#include <string>

class Animation_Node : public Image_Node {
public:
    enum ANIMATION_LOAD_MODE { IMMEDIATE, ASYNC, SMOOTH };

    Animation_Node();
    ~Animation_Node();
    Node_Type get_type() override;

    void make_animation(
        std::string name,
        std::string folder_path,
        int number_frame,
        float duration_loop,
        ANIMATION_LOAD_MODE load_mode = ANIMATION_LOAD_MODE::IMMEDIATE,
        std::string extend_format = ".png"
    );
    void play_animation(std::string name, float speed = 1.0f, bool is_reset = false);
    void set_speed(float speed);
    void on_finish_animation_callback(
        std::string name, std::function<void(Animation_Node* target, void* global_data)> finish_callback
    );
    void set_preload_animation(std::string animation_name);
    void clear_all_animation();
    float get_amimation_duration(std::string name);

protected:
    void handle_personal_task(float delta_time, void* global_data) override;
    void flex_update(float delta_time) override;

private:
    struct Animation_Data {
        std::string name;
        std::string folder_path;
        int number_frame;
        float duration_loop;
        std::string extend_format;
        ANIMATION_LOAD_MODE animation_load_mode = ANIMATION_LOAD_MODE::IMMEDIATE;
        std::function<void(Animation_Node* target, void* global_data)> finish_callback;
        bool is_finish_cycle = false;
        bool need_preload = false;
    };

    std::string current_animation;
    int current_frame = 0;
    float speed_ratio;
    float total_delta_time = 0;
    std::map<std::string, Animation_Data> animations;
    std::string get_image_path(std::string animation_name, int current_frame);

    bool is_valid_animation(std::string animation_name);
    bool is_load_all_smooth_frame(std::string animation_name);
};