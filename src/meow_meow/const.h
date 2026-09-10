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

        /**
         * Character animation
         */
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

        /**
         * Enemy animation
         */
        const inline static int BASE_ENEMY_ID = 1000;
        const inline static int NUMBER_ANIMATION_ENEMY = 8;
        const inline static std::string ENEMY_DEFAULT_PATH = "res/meow_meow/Enemies/Enemy Reg ";
        const inline static std::vector<std::string> ENEMY_ANIMATION_NAMES = {"IDLE", "ATTACK", "DEAD", "WALK"};
        const inline static std::vector<std::string> ENEMY_ANIMATION_FOLDER_NAMES = {"Idle", "Attack", "Dead", "Walk"};
        const inline static std::vector<int> ENEMY_ANIMATION_NUMBER_FRAMES = {20, 25, 60, 35};
        const inline static std::vector<float> ENEMY_ANIMATION_DURATION_PER_FRAMES = {0.04, 0.04, 0.03, 0.06};
        const inline static std::vector<int> ENEMY_MAX_LEVELS = std::vector<int>(NUMBER_ANIMATION_ENEMY, 1);
        const inline static Custom::Size ENEMY_BOUNDING_BOX = {120, 140};

        /**
         * Boss animation
         */
        const inline static int BASE_BOSS_ID = 2000;
        const inline static int NUMBER_ANIMATION_BOSS = 7;
        const inline static std::string BOSS_DEFAULT_PATH = "res/meow_meow/Enemies/Enemy Boss ";
        const inline static std::vector<int> BOSS_ANIMATION_NUMBER_FRAMES = {20, 40, 50, 25};
        const inline static std::vector<float> BOSS_ANIMATION_DURATION_PER_FRAMES = {0.04, 0.03, 0.03, 0.04};
        const inline static Custom::Size BOSS_BOUNDING_BOX = {160, 160};

        /**
         * Shoot animation
         */
        const inline static int SHOOT_ANIMATION_NODE_TAG = 0;

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
        const inline static std::string STATE_DASHING = "DASHING";
        const inline static std::string STATE_SKILL_CHANNELLING = "SKILL_CHANNELING";
        const inline static std::string STATE_STUN = "STUN";
        const inline static std::string STATE_SKILL_FLASH = "SKILL_FLASH";
        const inline static std::string STATE_FLIGHT = "FLIGHT";

        const inline static int CHARACTER_ATTACKED_FROM_EMEMY = 0;
        const inline static int CHARACTER_CHANNLING_FROM_SKILL_THUNDER = 1;
        const inline static int CHARACTER_FLIGHT_FROM_BOSS_SKILL_FLASH = 2;
        const inline static int CHARACTER_STUN_FROM_BOSS_SKILL_FLASH = 3;
        const inline static int ENEMY_ATTACKED_FROM_BULLET = 4;
        const inline static int ENEMY_STUN_FROM_CHARACTER_SKILL_THUNDER = 5;
        const inline static int BOSS_CHANNELING_FROM_SKILL_FLASH = 6;
        const inline static int ENEMY_HITTED_WHEN_STUNED = 7;

        /**
         * Bullet
         */
        const inline static std::string PATH_BULLET_IMAGE = "res/meow_meow/Bullets/bullet";
        const inline static Custom::Size BULLET_BOUNDING_BOX = {50, 30};
        const inline static float BULLET_VELOSITY = 700;
        const inline static float BULLET_ACCELARATE = 5;

        /**
         * Collision
         */
        const inline static int CHARACTER_COLLISION_TAG = 0;
        const inline static int BULLET_COLLISION_TAG = 1;
        const inline static int ENEMY_COLLISION_TAG = 2;
        const inline static int BOSS_COLLISION_TAG = 3;
        const inline static int BATTLE_LAYER_COLLISION = 0;

        /**
         * Character behavior
         */
        const inline static float CHARACTER_ATTACKED_DURATION = 0.55f;
        const inline static float CHARACTER_INVINCIBLE_DURATION = 1.5f;
        const inline static float CHARACTER_VELOSITY = 150.f;
        const inline static float CHARACTER_ACCELARATE = 35.f;
        const inline static float CHARACTER_ATTACK_DURATION = 0.2f;
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
        const inline static float ENEMY_DURATION_WALK = 0.5f;
        const inline static float ENEMY_DURATION_JUMP = 0.5f;
        const inline static float ENEMY_DURATION_ATTACK = 0.35f;
        const inline static float ENEMY_ATTACK_COUNTDOWN = 1.25f;
        const inline static float ENEMY_JUMP_COUNTDOWN = 5.0f;
        const inline static float ENEMY_ATTACKED_DURATION = 0.35f;
        const inline static float ENEMY_ATTACKED_VELOSITY = 20.f;
        const inline static float ENEMY_HEALTH = 250.f;
        const inline static float ENEMY_KILLED_EXP = 15;
        const inline static bool ENEMY_ENABLE_JUMP = true;

        /**
         * Boss behavior
         */
        const inline static float BOSS_VELOSITY = 60.f;
        const inline static float BOSS_ATTACK_DAMGE = 60.f;
        const inline static float BOSS_DURATION_ATTACK = 0.75f;
        const inline static float BOSS_HEALTH = 1500.f;
        const inline static bool BOSS_ENABLE_JUMP = true;
        const inline static float BOSS_ATTACK_RANGE = 100.f;

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

        /**
         * Battle level config
         */
        const inline static int BATTLE_NUMBER_WAVE = 3;
        const inline static std::vector<float> BATTLE_DURATION_GENERATE_ENEMY = {0, 30, 40};
        const inline static std::vector<int> BATTLE_NUMBER_ENEMY_GENERATED = {5, 7, 5};
        const inline static std::vector<int> BATTLE_NUMBER_BOSS_GENERATED = {0, 0, 1};

        /**
         * Character level config
         */
        const inline static int CHARACTER_MAX_LEVEL_NUMBER = 5;
        const inline static std::vector<int> EXP_AT_LEVELS = {0, 30, 45, 70, 100};
        const inline static std::vector<float> DAMAGE_BONUS_AT_LEVELS = {0.f, 15.f, 15.f, 20.f, 25.f};

        /**
         * Skill dash config
         */
        const inline static std::string CHARACTER_SKILL_DASH_COMPONENT_NAME = "SKILL_DASH_COMPONENT";
        const inline static std::string CHARACTER_SKILL_DASH_ID = "SKILL_DASH";
        const inline static float CHARACTER_SKILL_DASH_COUNTDOWN = 2.5f;
        const inline static float CHARACTER_SKILL_DASH_DURATION = 0.5f;
        const inline static float CHARACTER_SKILL_DASH_DISTANCE = 300.f;

        /**
         * Skill thunder config
         */
        const inline static std::string CHARACTER_SKILL_THUNDER_COMPONENT_NAME = "SKILL_THURNDER_COMPONENT";
        const inline static std::string CHARACTER_SKILL_THUNDER_ID = "SKILL_THUNDER";
        const inline static float CHARACTER_SKILL_THUNDER_COUNTDOWN = 5.f;
        const inline static float CHARACTER_SKILL_THUNDER_DAMAGE_TAKEN = 50.f;
        const inline static float CHARACTER_SKILL_THUNDER_DURATION_STUN = 1.75f;
        const inline static float CHARACTER_SKILL_THUNDER_TAKEN_RADIUS = 250.f;
        const inline static int CHARACTER_SKILL_THUNDER_NUMBER_TAKEN = 5;
        const inline static float CHARACTER_SKILL_THUNDER_CHANNELLING_DURATION = 0.35f;

        /**
         * Skill jump config (enemy)
         */
        const inline static std::string ENEMY_SKILL_JUMP_COMPONENT_NAME = "SKILL_JUMP_COMPONENT";
        const inline static std::string ENEMY_SKILL_JUMP_ID = "SKILL_JUMP";
        const inline static float ENEMY_SKILL_JUMP_COUNTDOWN = 1.0f;
        const inline static float ENEMY_SKILL_JUMP_DURATION = 0.5f;
        const inline static float ENEMY_SKILL_JUMP_DISTANCE = 150.f;

        /**
         * Skill flash config (boss)
         */
        const inline static std::string BOSS_SKILL_FLASH_COMPONENT_NAME = "SKILL_FLASH_COMPONENT";
        const inline static std::string BOSS_SKILL_FLASH_ID = "FLASH_JUMP";
        const inline static float BOSS_SKILL_FLASH_COUNTDOWN = 20.0f;
        const inline static float BOSS_SKILL_FLASH_DURATION = 0.5f;
        const inline static float BOSS_SKILL_FLASH_CHANNELLING_DURATION = 0.75f;
        const inline static float BOSS_SKILL_FLASH_DAMAGE = 100.f;
        const inline static float BOSS_SKILL_FLASH_DISTANCE = 500.f;
        const inline static float BOSS_SKILL_FLASH_DURATION_FLY = 1.f;
        const inline static float BOSS_SKILL_FLASH_DURATION_STUN = 0.5f;

        /**
         * Character skill ui countdown config
         * @Note: the skill id need to be same as skill id
         * in skill config to sync ui countdown
         */
        const inline static std::vector<std::string> CHARACTER_SKILL_IDS = {
            CHARACTER_SKILL_DASH_ID, CHARACTER_SKILL_THUNDER_ID
        };
        const inline static std::vector<std::string> CHARACTER_SKILL_NAMES = {"Dash: C", "Fire: V"};
        const inline static std::vector<std::string> CHARACTER_SKILL_IMAGE_PATHS = {
            "res/meow_meow/AddonIcon1.png", "res/meow_meow/AddonIcon3.png"
        };
    };
}  // namespace Meow_Meow