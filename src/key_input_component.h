#pragma once
#include <base_component.h>
#include <key_input_system.h>

#include <map>

class Key_Input_Component : public Base_Component {
public:
    Key_Input_Component();
    ~Key_Input_Component();

    void enter() override;
    void exit() override;
    void update_information() override;
    void handle_task() override;

    bool is_key_listener_enabled(Custom::Key key);
    bool is_swallow_keys_enabled(Custom::Key key);
    void set_key_listener_enabled(Custom::Key key, bool enabled, bool swallow_keys = false);
    void set_swallow_keys_enabled(Custom::Key key, bool swallow_keys);

private:
    std::map<Custom::Key, Key_Listener_Information> keys_listener;
    int listener_id = -1;
};