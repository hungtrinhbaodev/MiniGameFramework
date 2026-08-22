#include <key_input_component.h>
#include <node.h>

Node* convert_target_to_node(Base_Node* target) {
    Node_Type type = target->get_type();
    Node* node = nullptr;
    switch (type) {
        case Node_Type::BASE_NODE: {
            std::cout << "Key input component warning: Please use Node instead of Base Node to use this component!"
                      << std::endl;
            break;
        }
        default: {
            node = reinterpret_cast<Node*>(target);
            break;
        }
    }
    return node;
}

Key_Input_Component::Key_Input_Component() {}

Key_Input_Component::~Key_Input_Component() {}

void Key_Input_Component::attach(Base_Node* target, void* global_data) {
    this->listener_id = Key_Input_System::get()->request_key_pressed_listener();
}

void Key_Input_Component::detach(Base_Node* target, void* global_data) {
    Key_Input_System::get()->remove_key_press_listener(this->listener_id);
}

std::map<Custom::Key, Key_Input_Type> Key_Input_Component::get_key_inputs() {
    std::map<Custom::Key, Key_Input_Type> keys = Key_Input_System::get()->query_pressed_keys(listener_id);
    return keys;
}

bool Key_Input_Component::is_key_listener_enabled(Custom::Key key) {
    if (this->keys_listener.find(key) == this->keys_listener.end()) {
        return false;
    }
    return this->keys_listener[key].is_listened;
}

bool Key_Input_Component::is_swallow_keys_enabled(Custom::Key key) {
    if (this->keys_listener.find(key) == this->keys_listener.end()) {
        return false;
    }
    return this->keys_listener[key].swallow_keys;
}

void Key_Input_Component::set_key_listener_enabled(Custom::Key key, bool enabled, bool swallow_keys) {
    this->keys_listener[key] = {enabled, swallow_keys};
}

void Key_Input_Component::set_swallow_keys_enabled(Custom::Key key, bool swallow_keys) {
    if (this->keys_listener.find(key) == this->keys_listener.end()) {
        return;
    }
    this->keys_listener[key].swallow_keys = swallow_keys;
}

void Key_Input_Component::update_information(Base_Node* target, float delta_time, void* global_data) {
    Node* node = convert_target_to_node(target);
    if (node == nullptr)
        return;
    Key_Input_System::get()->request_update_listener(
        listener_id, {listener_id, node->get_draw_index(), this->keys_listener}
    );
}

void Key_Input_Component::handle_task(Base_Node* target, float delta_time, void* global_data) {
    Node* node = convert_target_to_node(target);
    if (node == nullptr)
        return;
    std::map<Custom::Key, Key_Input_Type> handled_keys = Key_Input_System::get()->query_pressed_keys(listener_id);
    for (auto& [key, type] : handled_keys) {
        if (type == Key_Input_Type::IDLE) {
            continue;
        }
        auto caller = node->get_key_press_caller(key);
        if (caller != nullptr) {
            caller(type, target, global_data);
        } else {
            node->on_key_pressed(key, type, global_data);
        }
    }
}
