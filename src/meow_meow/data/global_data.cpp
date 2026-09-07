#include <meow_meow/config/battle_config.h>
#include <meow_meow/data/global_data.h>

namespace Meow_Meow {
    Global_Data* Global_Data::instance = nullptr;

    Global_Data* Global_Data::get() {
        if (instance == nullptr) {
            instance = new Global_Data();
        }
        return instance;
    }

    Global_Data::Global_Data() {
        this->init_skills_data();
    }

    Global_Data::~Global_Data() {}

    void Global_Data::init_skills_data() {
        this->character_skills_data.clear();
        const Character_Skill_UI_Config& skills_config = this->get_config().get_character_skill_ui_config();
        for (int i = 0; i < skills_config.skills.size(); i++) {
            const Skill_Information& skill = skills_config.skills[i];
            this->character_skills_data.push_back({skill.skill_id, 0});
        }
    }

    const Config& Global_Data::get_config() {
        return config;
    }

    Battle_Layer* Global_Data::get_battle_layer() {
        return this->battle_layer;
    }

    Character_Node* Global_Data::get_character_node() {
        return this->character;
    }

    Player_Data& Global_Data::get_player_data() {
        return this->player_data;
    }

    Enemy_Data& Global_Data::get_enemy_data_by(int enemy_id) {
        if (this->enemies.find(enemy_id) == this->enemies.end()) {
            return default_enemy;
        }
        return this->enemies[enemy_id];
    }

    Enemy_Data& Global_Data::get_boss_data_by(int boss_id) {
        if (this->bosses.find(boss_id) == this->bosses.end()) {
            return default_enemy;
        }
        return this->bosses[boss_id];
    }

    int Global_Data::get_current_battle_level() {
        return this->current_battle_level;
    }

    int Global_Data::get_current_battle_wave() {
        return this->current_battle_wave;
    }

    std::vector<int> Global_Data::get_new_enemies_id_generated() {
        return this->new_enemies_id_generated;
    }

    std::vector<int> Global_Data::get_new_bosses_id_generated() {
        return this->new_bosses_id_generated;
    }

    Layer_Node* Global_Data::get_effect_layer() {
        return this->effect_layer;
    }

    bool Global_Data::is_character_level_up() {
        int current_exp = player_data.get_current_exp();
        int current_level = player_data.get_character_level();
        const Character_Level_Config& level_config = this->config.get_character_level_config();
        if (current_level >= level_config.get_number_max_level()) {
            return false;
        }
        int exp_required = level_config.get_exp_next_level(current_level);
        return current_exp >= exp_required;
    }

    std::vector<Skill_Data>& Global_Data::get_characters_skill_data() {
        return this->character_skills_data;
    }

    Skill_Data* Global_Data::get_skill_data_by(std::string skill_name) {
        for (int i = 0; i < this->character_skills_data.size(); i++) {
            Skill_Data* skill_data = &this->character_skills_data[i];
            if (skill_data->skill_id == skill_name) {
                return skill_data;
            }
        }
        return nullptr;
    }

    float Global_Data::get_current_battle_duration() {
        return this->current_battle_duration;
    }

    bool Global_Data::is_player_win() {
        const Battle_Config& battle_config = this->get_config().get_battle_config_at(this->current_battle_level);
        if (this->current_battle_wave >= battle_config.get_number_wave()) {
            if (this->enemies.size() <= 0 && this->bosses.size() <= 0) {
                return true;
            }
        }
        return false;
    }

    bool Global_Data::is_player_lose() {
        return this->get_player_data().is_dead();
    }

    void Global_Data::character_level_up() {
        if (!is_character_level_up())
            return;
        int current_exp = player_data.get_current_exp();
        int current_level = player_data.get_character_level();
        const Character_Level_Config& level_config = this->config.get_character_level_config();
        int exp_required = level_config.get_exp_next_level(current_level);
        this->player_data.set_chracter_level(++current_level);
        this->player_data.set_current_exp(current_exp % exp_required);
        this->player_data.set_player_damage(
            this->player_data.get_player_damage() + level_config.get_bonus_damage_at_level(current_level)
        );
    }

    void Global_Data::set_current_battle_duration(float duration) {
        this->current_battle_duration = duration;
    }

    void Global_Data::set_current_battle_wave(int current_battle_wave) {
        this->current_battle_wave = current_battle_wave;
    }

    void Global_Data::set_battle_layer(Battle_Layer* battle_layer) {
        this->battle_layer = battle_layer;
    }

    void Global_Data::set_character(Character_Node* character) {
        this->character = character;
    }

    void Global_Data::set_enemey_dead(int enemy_id) {
        if (this->enemies.find(enemy_id) == this->enemies.end()) {
            return;
        }
        this->enemies[enemy_id].set_dead(true);
    }

    void Global_Data::set_effect_layer(Layer_Node* layer) {
        this->effect_layer = layer;
    }

    void Global_Data::generate_enemies_at(int wave) {
        const Battle_Config& battle_config = this->get_config().get_battle_config_at(this->current_battle_level);
        const Enemy_Behavior_Config& enemy_behavior_config = this->get_config().get_enemy_behavior_config();
        const Enemy_Behavior_Config& boss_behavior_config = this->get_config().get_boss_behavior_config();
        const Character_Animation_Config& enemy_animation_config = this->get_config().get_enemy_animation_config();
        const Character_Animation_Config& boss_animation_config = this->get_config().get_boss_animation_config();

        int number_generated_enemy_at_wave = battle_config.get_number_enemies_at(wave);
        int number_generated_boss_at_wave = battle_config.get_number_bosses_at(wave);

        new_enemies_id_generated.clear();
        new_bosses_id_generated.clear();

        for (int i = 0; i < number_generated_enemy_at_wave; i++) {
            int enemy_id = current_enemy_generated_id++;
            this->enemies[enemy_id] = {
                enemy_id,
                enemy_animation_config.get_random_animation_id(),
                enemy_behavior_config.get_enemy_health(),
                enemy_behavior_config.get_enemy_health(),
                false
            };
            new_enemies_id_generated.push_back(enemy_id);
        }

        for (int i = 0; i < number_generated_boss_at_wave; i++) {
            int enemy_id = current_enemy_generated_id++;
            this->bosses[enemy_id] = {
                enemy_id,
                boss_animation_config.get_random_animation_id(),
                boss_behavior_config.get_enemy_health(),
                boss_behavior_config.get_enemy_health(),
                false
            };
            new_bosses_id_generated.push_back(enemy_id);
        }
    }

    void Global_Data::remove_boss_by(int boss_id) {
        if (this->bosses.find(boss_id) != this->bosses.end()) {
            this->bosses.erase(boss_id);
        }
    }

    void Global_Data::remove_enemy_by(int enemy_id) {
        if (this->enemies.find(enemy_id) != this->enemies.end()) {
            this->enemies.erase(enemy_id);
        }
    }

    void Global_Data::clear() {
        if (instance != nullptr) {
            delete (instance);
        }
    }

}  // namespace Meow_Meow