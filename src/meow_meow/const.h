#pragma once
#include <custom.h>

#include <string>
#include <vector>

namespace Meow_Meow {

    struct Const_Character_Information {
        int id;
        std::string name = "";
    };

    class Const {
    public:
        enum DIRECTION { LEFT, RIGHT, UP, DOWN, NONE /** Don't update with NONE state */ };

        const inline static int BASE_CHARACTER_ID = 0;
        const inline static int NUMBER_CHARACTER = 3;
        const inline static Const_Character_Information MEOW_FANCY{0, "FANCY MEOW"};
        const inline static Const_Character_Information MEOW_SAD{1, "SAD MEOW"};
        const inline static Const_Character_Information MEO_CONFIDENT{2, "OVERCONFIDENT MEOW"};
        const inline static std::string CHARACTER_DEFAULT_PATH = "res/meow_meow/Characters/C";
        const inline static std::vector<std::string> CHARACTER_ANIMATION_NAMES = {"IDLE", "SHOOT"};
        const inline static std::vector<std::string> CHARACTER_ANIMATION_FOLDER_NAMES = {"Idle", "Shoot"};
        const inline static std::vector<int> CHARACTER_ANIMATION_NUMBER_FRAMES = {20, 10};
        const inline static std::vector<float> CHARACTER_ANIMATION_DURATION_PER_FRAMES = {0.04, 0.06};
        const inline static std::vector<int> CHARACTER_MAX_LEVELS = {5, 5, 5};
        const inline static std::vector<Const_Character_Information> CHARACTERS_INFO{
            {MEOW_FANCY, MEOW_SAD, MEO_CONFIDENT}
        };
        const inline static Custom::Size CHARACTER_BOUNDING_BOX = {100, 120};

        const inline static int BASE_ENEMY_ID = 1000;
        const inline static int NUMBER_ENEMY = 8;
        const inline static std::string ENEMY_DEFAULT_PATH = "res/meow_meow/Enemies/Enemy Reg ";
        const inline static std::vector<std::string> ENEMY_ANIMATION_NAMES = {"IDLE", "ATTACK", "DEAD", "WALK"};
        const inline static std::vector<std::string> ENEMY_ANIMATION_FOLDER_NAMES = {"Idle", "Attack", "Dead", "Walk"};
        const inline static std::vector<int> ENEMY_ANIMATION_NUMBER_FRAMES = {20, 25, 60, 35};
        const inline static std::vector<float> ENEMY_ANIMATION_DURATION_PER_FRAMES = {0.04, 0.04, 0.03, 0.03};
        const inline static std::vector<int> ENEMY_MAX_LEVELS = std::vector<int>(NUMBER_ENEMY, 1);
        const inline static Custom::Size ENEMY_BOUNDING_BOX = {120, 140};

        /**
         * State machine config to character
         */
        const inline static std::string TRACK_CONTROLL = "CONTROLL";
        const inline static std::string STATE_MOVE = "MOVE";
        const inline static std::string STATE_IDLE = "IDLE";
        const inline static std::string STATE_ATTACK = "ATTACK";
        const inline static std::string TRACK_EFFECTED = "EFFECTED";
        const inline static std::string STATE_ATTACKED = "ATTACKED";
        const inline static std::string STATE_WALK = "WALK";
        const inline static std::string STATE_JUMP = "JUMP";
        const inline static std::string STATE_INVINCIBLE = "INVINCIBLE";
        const inline static std::string STATE_UNEFFECTED = "UNEFFECTED";
        const inline static std::string STATE_DEATH = "DEATH";

        /**
         * Bullet
         */
        const inline static std::string PATH_BULLET_IMAGE = "res/meow_meow/Bullets/bullet";
        const inline static Custom::Size BULLET_BOUNDING_BOX = {50, 30};
        const inline static float BULLET_VELOSITY = 700;
        const inline static float BULLET_ACCELARATE = 5;

        /**
         * Shoot animation
         */
        const inline static int SHOOT_ANIMATION_NODE_TAG = 0;

        /**
         * Collision
         */
        const inline static int CHARACTER_COLLISION_TAG = 0;
        const inline static int BULLET_COLLISION_TAG = 1;
        const inline static int ENEMY_COLLISION_TAG = 2;
        const inline static int BATTLE_LAYER_COLLISION = 0;

        /**
         * Character behavior
         */
        const inline static float CHARACTER_ATTACKED_DURATION = 0.55f;
        const inline static float CHARACTER_INVINCIBLE_DURATION = 1.5f;
        const inline static float CHARACTER_VELOSITY = 150.f;
        const inline static float CHARACTER_ACCELARATE = 35.f;
        const inline static float CHARACTER_ATTACK_DURATION = 0.5f;
        const inline static float CHARACTER_BONUS_VELOSITY_RATE = 0.25f;
        const inline static float CHARACTER_BONUS_VELOSITY = 10.f;
        const inline static float CHARACTER_MAX_BONUS_VELOSITY = 60.f;
        const inline static float CHARACTER_HEALTH = 300.f;
        const inline static float CHARACTER_DAMAGE = 20.f;
        const inline static float CHARACTER_DEAD_DURATION = 1.f;

        /**
         * Enemy behavior
         */
        const inline static float ENEMY_JUMP_DISTANCE = 150.f;
        const inline static float ENEMY_VELOSITY = 80.f;
        const inline static float ENEMY_ATTACK_DAMGE = 30.f;
        const inline static float ENEMY_ATTACK_RANGE = 50.f;
        const inline static float ENEMY_DURATION_WALK = 1.f;
        const inline static float ENEMY_DURATION_JUMP = 0.5f;
        const inline static float ENEMY_DURATION_ATTACK = 0.35f;
        const inline static float ENEMY_ATTACK_COUNTDOWN = 1.25f;
        const inline static float ENEMY_JUMP_COUNTDOWN = 5.0f;
        const inline static float ENEMY_ATTACKED_DURATION = 0.35f;
        const inline static float ENEMY_ATTACKED_VELOSITY = 20.f;
        const inline static float ENEMY_HEALTH = 250.f;

        /**
         * Progression health bar
         */
        const inline static std::string PATH_HEALTH_BAR = "res/meow_meow/GemsBarBg.png";
        const inline static Custom::Rectangle_Area HEALTH_BAR_CAP_INSETS = {30, 15, 169, 32};
        const inline static Custom::Size HEALTH_BAR_SIZE = {160, 36};
        const inline static Custom::Color HEALTH_BAR_COLOR = {10, 128, 10};
        const inline static glm::vec2 HEALTH_BAR_PADDING = {1.f, 4.f};
        const inline static glm::vec2 HEALTH_BAR_DELTA_POSITION = {1.f, 0.f};

        /**
         * Custom component name
         */
        const inline static std::string ENEMY_BEHAVIOR_COMPONENT_NAME = "ENEMY_BEHAVIOR_COMPONENT";
    };
}  // namespace Meow_Meow