#include <key_input_system.h>

#include <algorithm>

bool Key_Pressed_Information::has_key(Custom::Key key) {
    return this->listened_keys.find(key) != this->listened_keys.end();
}

bool Key_Pressed_Information::is_key_listenning(Custom::Key key) {
    if (!this->has_key(key))
        return false;
    return this->listened_keys[key].is_listened;
}

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
    this->keys.push_back(key);
    return key.key_pressed_id;
}

void Key_Input_System::request_update_listener(int id, Key_Pressed_Information updated) {
    if (!has_id(id))
        return;
    Key_Pressed_Information& key_listener = get_key_listener(id);
    key_listener = updated;
    if (handled_keys.find(id) != handled_keys.end()) {
        std::vector<Custom::Key> removed_keys;
        std::map<Custom::Key, Key_Input_Type> handled_listeners;
        for (auto& [key, handled_listener] : handled_listeners) {
            if (!key_listener.is_key_listenning(key)) {
                removed_keys.push_back(key);
            }
        }
        for (auto removed_key : removed_keys) {
            handled_listeners.erase(removed_key);
        }
    }
}

std::map<Custom::Key, Key_Press_Detail> Key_Input_System::query_pressed_keys(int id) {
    if (!has_id(id))
        return {};
    if (handled_keys.find(id) == handled_keys.end())
        return {};
    return handled_keys[id];
}

void Key_Input_System::remove_key_press_listener(int id) {
    if (!has_id(id))
        return;
    for (int i = 0; i < keys.size(); i++) {
        if (keys[i].key_pressed_id == id) {
            keys[i] = keys.back();
            keys.pop_back();
            i--;
        }
    }
    handled_keys.erase(id);
}

void Key_Input_System::handle_key_pressed(const std::map<Custom::Key, Key_Press_Detail>& keys_detail) {
    std::sort(keys.begin(), keys.end());
    for (auto& key_info : keys) {
        int key_id = key_info.key_pressed_id;
        std::map<Custom::Key, Key_Listener_Information>& key_listeners = key_info.listened_keys;
        for (const auto& [key, key_listener] : key_listeners) {
            if (handled_keys.find(key_id) != handled_keys.end()) {
                std::map<Custom::Key, Key_Press_Detail>& listener_keys_detail = handled_keys[key_id];
                if (listener_keys_detail.find(key) == listener_keys_detail.end()) {
                    continue;
                }
                Key_Input_Type& type = listener_keys_detail[key].type;
                float& duration_press = listener_keys_detail[key].duration_pressed;
                if (type == Key_Input_Type::CANCEL || type == Key_Input_Type::RELEASE) {
                    type = Key_Input_Type::IDLE;
                    duration_press = 0;
                }
            }
        }
    }
    for (auto& [key, detail] : keys_detail) {
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
            std::map<Custom::Key, Key_Press_Detail>& listener_keys_detail = handled_keys[key_id];
            if (listener_keys_detail.find(key) == listener_keys_detail.end()) {
                if (detail.type == Key_Input_Type::PRESSED) {
                    listener_keys_detail[key].type = detail.type;
                }
                listener_keys_detail[key].duration_pressed = detail.duration_pressed;
            } else {
                if (detail.type == Key_Input_Type::PRESSED) {
                    if (listener_keys_detail[key].type == Key_Input_Type::IDLE) {
                        listener_keys_detail[key].type = detail.type;
                    }
                } else if (detail.type == Key_Input_Type::HOLDING) {
                    if (listener_keys_detail[key].type == Key_Input_Type::PRESSED) {
                        listener_keys_detail[key].type = detail.type;
                    }
                } else if (detail.type == Key_Input_Type::RELEASE) {
                    if (listener_keys_detail[key].type == Key_Input_Type::HOLDING) {
                        listener_keys_detail[key].type = detail.type;
                    }
                } else {
                    listener_keys_detail[key].type = detail.type;
                }
                listener_keys_detail[key].duration_pressed = detail.duration_pressed;
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
                    std::map<Custom::Key, Key_Press_Detail>& listener_swallowed_keys_detail =
                        handled_keys[swallowed_id];
                    float& listener_swallowed_duration_press = listener_keys_detail[key].duration_pressed;
                    if (listener_swallowed_keys_detail.find(key) != listener_swallowed_keys_detail.end()) {
                        Key_Input_Type& swallowed_key_type = listener_swallowed_keys_detail[key].type;
                        if (swallowed_key_type == Key_Input_Type::PRESSED ||
                            swallowed_key_type == Key_Input_Type::HOLDING) {
                            swallowed_key_type = Key_Input_Type::CANCEL;
                            listener_swallowed_duration_press = 0;
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