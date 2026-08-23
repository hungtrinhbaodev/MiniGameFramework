#pragma once
#include <custom.h>

#include <map>
#include <vector>

enum Key_Input_Type { PRESSED, HOLDING, RELEASE, IDLE, CANCEL };

struct Key_Press_Detail {
    Key_Input_Type type = Key_Input_Type::IDLE;
    float duration_pressed = 0;
};

struct Key_Listener_Information {
    bool is_listened;
    bool swallow_keys;
};

struct Key_Pressed_Information {
    int key_pressed_id;
    int priority;
    std::map<Custom::Key, Key_Listener_Information> listened_keys;
    bool has_key(Custom::Key key);
    bool is_key_listenning(Custom::Key key);
    bool operator<(const Key_Pressed_Information& other) {
        return priority > other.priority;
    }
};

class Key_Input_System {
public:
    static Key_Input_System* get();
    static void clear();

    Key_Input_System();
    ~Key_Input_System();

    int request_key_pressed_listener();
    void request_update_listener(int id, Key_Pressed_Information updated);
    void remove_key_press_listener(int id);
    std::map<Custom::Key, Key_Press_Detail> query_pressed_keys(int id);

    void handle_key_pressed(const std::map<Custom::Key, Key_Press_Detail>& keys_detail);

private:
    static Key_Input_System* instance;
    static int current_generated_id;

    std::map<int, std::map<Custom::Key, Key_Press_Detail>> handled_keys;
    std::vector<Key_Pressed_Information> keys;

    Key_Pressed_Information empty_listener;
    bool has_id(int id);
    Key_Pressed_Information& get_key_listener(int id);
};