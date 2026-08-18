#pragma once
#include <custom.h>

#include <map>
#include <vector>

enum Key_Input_Type { PRESSED, HOLDING, RELEASE, IDLE, CANCEL };

struct Key_Pressed_Detail {
    Key_Input_Type type;
    Custom::Key key;
};

struct Key_Listener_Information {
    Custom::Key key;
    bool is_listened;
    bool swallow_keys;
};

struct Key_Pressed_Information {
    int key_pressed_id;
    int priority;
    std::map<Custom::Key, Key_Listener_Information> listened_keys;
    bool operator<(const Key_Pressed_Information& other) {
        return priority < other.priority;
    }
};

class Key_Input_System {
public:
    static Key_Input_System* get();
    static void clear();

    Key_Input_System();
    ~Key_Input_System();

    int request_key_pressed_listener();
    void request_add_key_listener(int id, Custom::Key key);
    void request_press_enabled(int id, Custom::Key key, bool enabled);
    void request_swallow_enabled(int id, Custom::Key key, bool enabled);
    void request_remove_key(int id);
    void handle_key_pressed(const std::map<Custom::Key, Key_Pressed_Detail>& keys_detail);

private:
    static Key_Input_System* instance;
    static int current_generated_id;

    std::map<int, std::map<Custom::Key, Key_Input_Type>> handled_keys;
    std::vector<Key_Pressed_Information> keys;

    Key_Pressed_Information empty_listener;
    bool has_id(int id);
    Key_Pressed_Information& get_key_listener(int id);
};