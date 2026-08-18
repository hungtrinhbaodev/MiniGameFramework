#include <key_input_system.h>

#include <algorithm>

Key_Input_System* Key_Input_System::instance = nullptr;

int Key_Input_System::current_generated_id = 0;

Key_Input_System* Key_Input_System::get() {
    if (instance == nullptr) {
        instance = new Key_Input_System();
    }
    return instance;
}

void Key_Input_System::clear() {
    if (instance) {
        delete (instance);
    }
}

Key_Input_System::Key_Input_System() {}

Key_Input_System::~Key_Input_System() {}

int Key_Input_System::request_key_pressed_listener() {
    Key_Pressed_Information key{current_generated_id++, 0, {}};
    return key.key_pressed_id;
}

void Key_Input_System::request_add_key_listener(int id, Custom::Key key) {
    if (!has_id(id))
        return;
    Key_Pressed_Information& key_info = get_key_listener(id);
    if (key_info.listened_keys.find(key) != key_info.listened_keys.end())
        return;
    key_info.listened_keys[key] = {key, true, false};
}

void Key_Input_System::request_press_enabled(int id, Custom::Key key, bool enabled) {
    if (!has_id(id))
        return;
    Key_Pressed_Information& key_info = get_key_listener(id);
    if (key_info.listened_keys.find(key) != key_info.listened_keys.end()) {
        Key_Listener_Information& listener = key_info.listened_keys[key];
        listener.is_listened = enabled;
    }
}

void Key_Input_System::request_swallow_enabled(int id, Custom::Key key, bool enabled) {
    if (!has_id(id))
        return;
    Key_Pressed_Information& key_info = get_key_listener(id);
    if (key_info.listened_keys.find(key) != key_info.listened_keys.end()) {
        Key_Listener_Information& listener = key_info.listened_keys[key];
        listener.swallow_keys = enabled;
    }
}

void Key_Input_System::handle_key_pressed(const std::map<Custom::Key, Key_Pressed_Detail>& keys_detail) {
    std::sort(keys.begin(), keys.end());
    for (auto& key_info : keys) {
        int key_id = key_info.key_pressed_id;
        std::map<Custom::Key, Key_Listener_Information>& key_listeners = key_info.listened_keys;
        for (const auto& [key, key_listener] : key_listeners) {
            if (handled_keys.find(key_id) != handled_keys.end()) {
                std::map<Custom::Key, Key_Input_Type>& listener_keys_detail = handled_keys[key_id];
                if (listener_keys_detail.find(key) == listener_keys_detail.end()) {
                    continue;
                }
                Key_Input_Type& type = listener_keys_detail[key];
                if (type == Key_Input_Type::CANCEL || type == Key_Input_Type::RELEASE) {
                    type = Key_Input_Type::IDLE;
                }
            }
        }
    }

    for (auto& [key, detail] : keys_detail) {
        bool swallow_keys = false;
        for (int i = 0; i < keys.size(); i++) {
            Key_Pressed_Information& key_info = keys[i];
            int key_id = keys[i].key_pressed_id;
            std::map<Custom::Key, Key_Listener_Information>& listeners = key_info.listened_keys;
            if (listeners.find(key) == listeners.end()) {
                continue;
            }
            Key_Listener_Information& listener_info = listeners[key];
            if (!listener_info.is_listened) {
                continue;
            }
            if (handled_keys.find(key_id) == handled_keys.end()) {
                handled_keys[key_id] = {};
            }
            std::map<Custom::Key, Key_Input_Type>& listener_keys_detail = handled_keys[key_id];
            if (listener_keys_detail.find(key) == listener_keys_detail.end()) {
                if (detail.type == Key_Input_Type::PRESSED) {
                    listener_keys_detail[key] = detail.type;
                }
            } else {
                if (detail.type == Key_Input_Type::PRESSED) {
                    if (listener_keys_detail[key] == Key_Input_Type::IDLE) {
                        listener_keys_detail[key] = detail.type;
                    }
                } else if (detail.type == Key_Input_Type::HOLDING) {
                    if (listener_keys_detail[key] == Key_Input_Type::PRESSED) {
                        listener_keys_detail[key] = detail.type;
                    }
                } else if (detail.type == Key_Input_Type::RELEASE) {
                    if (listener_keys_detail[key] == Key_Input_Type::HOLDING) {
                        listener_keys_detail[key] = detail.type;
                    }
                } else {
                    listener_keys_detail[key] = Key_Input_Type::CANCEL;
                }
            }
            if (listener_info.swallow_keys) {
                for (int j = i + 1; j < keys.size(); j++) {
                    Key_Pressed_Information& key_swallowed_info = keys[j];
                    std::map<Custom::Key, Key_Listener_Information>& swallowed_listeners = key_info.listened_keys;
                    if (swallowed_listeners.find(key) == swallowed_listeners.end()) {
                        continue;
                    }
                    Key_Listener_Information& swallowed_listener_info = swallowed_listeners[key];
                    if (!swallowed_listener_info.is_listened) {
                        continue;
                    }
                    int swallowed_id = key_swallowed_info.key_pressed_id;
                    std::map<Custom::Key, Key_Input_Type>& listener_swallowed_keys_detail = handled_keys[swallowed_id];
                    if (listener_swallowed_keys_detail.find(key) != listener_swallowed_keys_detail.end()) {
                        Key_Input_Type& swallowed_key_type = listener_swallowed_keys_detail[key];
                        if (swallowed_key_type == Key_Input_Type::PRESSED ||
                            swallowed_key_type == Key_Input_Type::HOLDING) {
                            swallowed_key_type = Key_Input_Type::CANCEL;
                        }
                    }
                }
                break;
            }
        }
    }
}

bool Key_Input_System::has_id(int id) {
    for (const auto& listener : keys) {
        if (listener.key_pressed_id == id) {
            return true;
        }
    }
    return false;
}

Key_Pressed_Information& Key_Input_System::get_key_listener(int id) {
    for (auto& listener : keys) {
        if (listener.key_pressed_id == id) {
            return listener;
        }
    }
    return empty_listener;
}