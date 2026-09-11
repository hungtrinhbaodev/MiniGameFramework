#pragma once
#include <defined.h>
#include <image_node.h>

#include <functional>
#include <map>
#include <string>

class Animation_Node : public Image_Node {
public:
    enum ANIMATION_LOAD_MODE { IMMEDIATE, ASYNC, SMOOTH };
    static void preload_animation();

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
        bool is_preload_finish = false;
    };
    std::map<std::string, Animation_Data> animations;

    void handle_personal_task(float delta_time, void* global_data) override;
    void flex_update(float delta_time) override;
    bool is_load_all_smooth_frame(std::string animation_name);

private:
    struct Animation_Preload_Information {
        std::string name = "";
        std::string folder_path = "";
        bool is_checked_preload_in_frame = false;
    };
    static std::map<std::string, std::map<std::string, std::vector<std::string>>> animations_need_preload;
    static void add_animation_need_preload(const Animation_Data& animation);
    static std::string get_image_animation_path(const Animation_Data& animation, int frame_index);
    static bool is_animation_preload_finish(const Animation_Data& animation);

    std::string current_animation;
    int current_frame = 0;
    float speed_ratio;
    float total_delta_time = 0;
    std::string get_image_path(std::string animation_name, int current_frame);

    bool is_valid_animation(std::string animation_name);
};