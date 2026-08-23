#include <defined.h>
#include <node.h>

#include <algorithm>
#include <iostream>

Node::Node() {}

Node::~Node() {
    for (Base_Action* action : actions) {
        delete (action);
    }
    for (Base_Component* component : components) {
        if (!component->is_removed()) {
            component->detach_from_node(this, nullptr);
        }
        delete (component);
    }
    for (Base_Component* component : detached_components) {
        component->detach_from_node(this, nullptr);
        delete (component);
    }
}

Base_Component* Node::get_component_by_name(std::string name) {
    for (Base_Component* component : components) {
        if (component->get_name() == name) {
            return component;
        }
    }
    return nullptr;
}

void Node::add_component(Base_Component* component) {
    this->components.push_back(component);
}

void Node::do_action(Base_Action* action, int tag, bool debug) {
    if (action == nullptr) {
        return;
    }
    action->set_tag(tag);
    action->set_debug(debug);
    actions.push_back(action);
}

void Node::schedule(
    const std::string& key,
    float duration_schedule,
    std::function<void(Base_Node*, void*)> caller,
    bool is_schedule_once
) {
    if (schedulers.find(key) != schedulers.end()) {
        std::cout << "Node warnning: scheduler key is duplicate, please check it again!" << std::endl;
    }
    Scheduler_Data scheduler{duration_schedule, 0, is_schedule_once, caller};
    schedulers[key] = scheduler;
}

void Node::schedule_once(
    const std::string& key, float duration_schedule, std::function<void(Base_Node*, void*)> caller
) {
    schedule(key, duration_schedule, caller, true);
}

void Node::remove_component(Base_Component* component) {
    for (int i = 0; i < components.size(); i++) {
        if (components[i] == component) {
            components[i]->set_removed(true);
            break;
        }
    }
}

void Node::remove_component(std::string component_name) {
    for (int i = 0; i < components.size(); i++) {
        if (components[i]->get_name() == component_name) {
            components[i]->set_removed(true);
        }
    }
}

void Node::stop_action(int tag) {
    Base_Action* action = nullptr;
    for (int i = 0; i < actions.size(); i++) {
        if (actions[i]->get_tag() == tag) {
            actions[i]->set_removed(true);
        }
    }
}

void Node::stop_all_action() {
    for (Base_Action* action : actions) {
        action->set_removed(true);
    }
}

void Node::unschedule(const std::string& key) {
    if (schedulers.find(key) != schedulers.end()) {
        schedulers[key].is_removed = true;
    }
}

bool Node::is_key_enabled(Custom::Key key) {
    Key_Input_Component* key_input_component = this->get_key_input_component();
    if (key_input_component == nullptr) {
        return false;
    }
    return key_input_component->is_key_listener_enabled(key);
}

bool Node::is_swallow_keys_enabled(Custom::Key key) {
    Key_Input_Component* key_input_component = this->get_key_input_component();
    if (key_input_component == nullptr) {
        return false;
    }
    return key_input_component->is_swallow_keys_enabled(key);
}

std::function<void(Key_Press_Detail, Base_Node*, void*)> Node::get_key_press_caller(Custom::Key key) {
    if (this->key_input_callers.find(key) == this->key_input_callers.end()) {
        return nullptr;
    }
    return this->key_input_callers[key];
}

void Node::add_key_press_listener(
    Custom::Key key, std::function<void(Key_Press_Detail, Base_Node*, void*)> caller, bool swallow_keys
) {
    this->key_input_callers[key] = caller;
    this->set_key_press_enabled(key, true);
    this->set_key_press_swallow_enabled(key, swallow_keys);
}

void Node::set_key_press_enabled(Custom::Key key, bool enabled) {
    Key_Input_Component* key_input_component = this->get_or_create_key_input_component();
    key_input_component->set_key_listener_enabled(key, enabled);
}

void Node::set_key_press_swallow_enabled(Custom::Key key, bool swallow_keys) {
    Key_Input_Component* key_input_component = this->get_key_input_component();
    if (key_input_component == nullptr) {
        return;
    }
    key_input_component->set_swallow_keys_enabled(key, swallow_keys);
}

void Node::on_key_pressed(Custom::Key key, Key_Press_Detail pressed_detail, void* global_data) {}

Node_Type Node::get_type() {
    return Node_Type::NODE;
}

void Node::attach(void* global_data) {}

void Node::detach() {}

void Node::fix_update(float delta_time, void* global_data) {}

void Node::flex_update(float delta_time) {}

void Node::handle_personal_task(float delta_time, void* global_data) {
    /**We handle logic of all components of node here */
    /**Handle it's task logic*/
    for (Base_Component* component : components) {
        if (!component->is_removed() && !component->is_setup()) {
            component->attach_from_node(this, global_data);
        }
        if (component->is_active()) {
            component->handle_task(this, delta_time, global_data);
        }
    }

    /**After that we handle actions need to do in node*/
    for (int i = 0; i < actions.size(); i++) {
        Base_Action* action = actions[i];
        if (action->is_removed())
            continue;
        bool is_finish_all = action->travel_action_2(this, delta_time, global_data);
        if (is_finish_all) {
            action->set_removed(true);
        }
    }

    /**After action we check the schedulers logic to handle it!*/
    /**Loop all scheduler check time of it and handle it!*/
    for (auto& [key, scheduler] : schedulers) {
        scheduler.current_duration += delta_time;
        if (scheduler.current_duration >= scheduler.duration_scheduler) {
            if (scheduler.caller) {
                scheduler.caller(this, global_data);
            }
            if (scheduler.is_scheduler_once) {
                scheduler.is_removed = true;
            } else {
                scheduler.current_duration = 0;
            }
        }
    }

    /**Final we call fix update to cascade update into extended node to update it logic if needed*/
    this->fix_update(delta_time, global_data);
}

void Node::set_world_transform_information(
    Custom::Transform world_transform, int draw_index, float delta_time, void* global_data
) {
    Base_Node::set_world_transform_information(world_transform, draw_index, delta_time, global_data);

    /**Assign target to component and invoke apply_from_node to loop */
    for (Base_Component* component : components) {
        if (component->is_removed()) {
            continue;
        }
        component->apply_from_node(this, delta_time, global_data);
    }
}

void Node::draw(Custom::Transform& world_transform, int& draw_index) {
    for (Base_Component* component : components) {
        if (component->is_active()) {
            component->draw(this, draw_index);
        }
    }
}

void Node::visit_cleanup(float delta_time, void* global_data) {
    Base_Node::visit_cleanup(delta_time, global_data);
    this->cleanup_stopped_actions();

    /**Remove the component mark removed */
    for (int i = 0; i < components.size(); i++) {
        if (components[i]->is_removed()) {
            components[i]->detach_from_node(this, global_data);
            components[i] = components.back();
            components.pop_back();
            i--;
        }
    }

    /**Clear all scheduler is mark at removed */
    std::vector<std::string> removed_keys;
    for (auto& [key, scheduler] : schedulers) {
        if (scheduler.is_removed) {
            removed_keys.push_back(key);
        }
    }
    for (auto& key : removed_keys) {
        schedulers.erase(key);
    }
}

void Node::update(float delta_time) {
    /**Cascade update flex into class children extended from node update it's graphic information */
    this->flex_update(delta_time);
}

void Node::enter(void* global_data) {
    /**Add list waitting component into list commponent again when node enter again!*/
    for (Base_Component* component : detached_components) {
        components.push_back(component);
    }
    detached_components.clear();
    this->attach(global_data);
}

void Node::exit(void* global_data) {
    /**Add list running component into list detach reserve wait to attach again!*/
    for (Base_Component* component : components) {
        component->detach_from_node(this, global_data);
        detached_components.push_back(component);
    }
    components.clear();
    schedulers.clear();
    this->detach();
    this->stop_all_action();
}

Key_Input_Component* Node::get_or_create_key_input_component() {
    Base_Component* component = this->get_component_by_name(Defined::COMPONENT_KEY_INPUT_NAME);
    if (component == nullptr) {
        component = new Key_Input_Component();
        component->set_name(Defined::COMPONENT_KEY_INPUT_NAME);
        this->add_component(component);
    }
    Key_Input_Component* key_input_component = reinterpret_cast<Key_Input_Component*>(component);
    return key_input_component;
}

Key_Input_Component* Node::get_key_input_component() {
    Base_Component* component = this->get_component_by_name(Defined::COMPONENT_KEY_INPUT_NAME);
    if (component == nullptr)
        return nullptr;
    return reinterpret_cast<Key_Input_Component*>(component);
}

void Node::cleanup_stopped_actions() {
    for (int i = 0; i < actions.size(); i++) {
        Base_Action* action = actions[i];
        if (action->is_removed()) {
            if (action->is_debug()) {
                action->show_debug();
            }
            delete (action);
            actions[i] = actions.back();
            actions.pop_back();
            i--;
        }
    }
}