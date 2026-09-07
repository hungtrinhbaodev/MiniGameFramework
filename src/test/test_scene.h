#pragma once
#include <actions.h>
#include <animation_node.h>
#include <button_node.h>
#include <collision_component.h>
#include <director.h>
#include <image_ui_node.h>
#include <label_node.h>
#include <layer_node.h>
#include <math_custom.h>
#include <meow_meow/animation/character_fire_ball_anmimation.h>
#include <meow_meow/animation/character_fire_run_animation.h>
#include <meow_meow/gui/lose_game_gui.h>
#include <meow_meow/gui/start_round_gui.h>
#include <meow_meow/gui/win_game_gui.h>
#include <progression_node.h>
#include <scene_node.h>

class Test_Scene : public Scene_Node {
public:
    Test_Scene();
    ~Test_Scene();
    void on_key_pressed(Custom::Key key, Key_Press_Detail pressed_detail, void* global_data) override;

protected:
    void fix_update(float delta_time, void* global_data) override;

private:
    int COLISION_ANIMATION_TAG = 57;
    Animation_Node* animation = nullptr;
    Animation_Node* animation_2 = nullptr;
    Layer_Node* layer = nullptr;
    Label_Node* label = nullptr;
    Image_UI_Node* ui = nullptr;
    Image_UI_Node* ui_child = nullptr;
    Image_Node* image = nullptr;
    Image_Node* image2 = nullptr;
    Progression_Node* progression = nullptr;
    Button_Node* btn = nullptr;
    Meow_Meow::Win_Game_GUI* win_game_gui = nullptr;
    Meow_Meow::Lose_Game_GUI* lose_game_gui = nullptr;
    Meow_Meow::Start_Round_GUI* start_round_gui = nullptr;
};