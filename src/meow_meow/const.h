#pragma once
#include <string>
#include <vector>

namespace Meow_Meow {

    struct Const_Character_Information {
        int id;
        std::string name = "";
    };

    class Const {
    public:
        inline static int BASE_CHARACTER_ID = 0;
        inline static int NUMBER_CHARACTER = 3;
        inline static Const_Character_Information MEOW_FANCY{0, "FANCY MEOW"};
        inline static Const_Character_Information MEOW_SAD{1, "SAD MEOW"};
        inline static Const_Character_Information MEO_CONFIDENT{2, "OVERCONFIDENT MEOW"};
        inline static std::string CHARACTER_DEFAULT_PATH = "res/meow_meow/Characters/C";
        inline static std::vector<std::string> CHARACTER_ANIMATION_NAMES = {"IDLE", "SHOOT"};
        inline static std::vector<std::string> CHARACTER_ANIMATION_FOLDER_NAMES = {"Idle", "Shoot"};
        inline static std::vector<int> CHARACTER_ANIMATION_NUMBER_FRAMES = {20, 9};
        inline static std::vector<float> CHARACTER_ANIMATION_DURATION_PER_FRAMES = {0.04, 0.06};
        inline static std::vector<int> CHARACTER_MAX_LEVELS = {5, 5, 5};
        inline static std::vector<Const_Character_Information> CHARACTERS_INFO{{MEOW_FANCY, MEOW_SAD, MEO_CONFIDENT}};

        inline static int BASE_ENEMY_ID = 1000;
        inline static int NUMBER_ENEMY = 8;
        inline static std::string ENEMY_DEFAULT_PATH = "res/meow_meow/Enemies/Enemy Reg ";
        inline static std::vector<std::string> ENEMY_ANIMATION_NAMES = {"IDLE", "ATTACK", "DEAD", "WALK"};
        inline static std::vector<std::string> ENEMY_ANIMATION_FOLDER_NAMES = {"Idle", "Attack", "Dead", "Walk"};
        inline static std::vector<int> ENEMY_ANIMATION_NUMBER_FRAMES = {20, 25, 60, 35};
        inline static std::vector<float> ENEMY_ANIMATION_DURATION_PER_FRAMES = {0.04, 0.04, 0.03, 0.04};
        inline static std::vector<int> ENEMY_MAX_LEVELS = std::vector<int>(NUMBER_ENEMY, 1);
    };
}  // namespace Meow_Meow