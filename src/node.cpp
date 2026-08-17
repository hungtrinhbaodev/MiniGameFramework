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
            component->exit();
        }
        delete (component);
    }
    for (Base_Component* component : dettached_components) {
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
    action->set_target(this);
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
            components[i]->exit();
            break;
        }
    }
}

void Node::remove_component(std::string component_name) {
    for (int i = 0; i < components.size(); i++) {
        if (components[i]->get_name() == component_name) {
            components[i]->set_removed(true);
            components[i]->exit();
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
        cleanup_actions.push_back(action);
    }
}

void Node::unschedule(const std::string& key) {
    if (schedulers.find(key) != schedulers.end()) {
        schedulers[key].is_removed = true;
    }
}

Node_Type Node::get_type() {
    return Node_Type::NODE;
}

void Node::attach(void* global_data) {}

void Node::detach() {}

void Node::fix_update(float delta_time, void* global_data) {}

void Node::flex_update(float delta_time) {}

void Node::handle_personal_task(float delta_time, void* global_data) {
    /**We handle logic of all components of node here */
    /**First: remove the component mark removed */
    for (int i = 0; i < components.size(); i++) {
        if (components[i]->is_removed()) {
            components[i] = components.back();
            components.pop_back();
            i--;
        }
    }
    /**Second: handle it's task logic*/
    for (Base_Component* component : components) {
        if (component->is_active() && component->has_target()) {
            if (!component->has_global_data()) {
                component->set_global_data(global_data);
            }
            component->handle_task();
        }
    }

    /**After that we handle actions need to do in node*/
    for (int i = 0; i < actions.size(); i++) {
        Base_Action* action = actions[i];
        if (action->is_removed())
            continue;
        bool is_finish_all = action->travel_action(this, delta_time, global_data);
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

void Node::set_world_transform_information(Custom::Transform world_transform, int draw_index) {
    Base_Node::set_world_transform_information(world_transform, draw_index);

    /**Assign target to component and invoke enter to start loop */
    for (Base_Component* component : components) {
        if (component->is_removed()) {
            continue;
        }
        if (!component->has_target()) {
            component->assign_target(this);
            component->enter();
        }
        component->update_information();
    }
}

void Node::draw(Custom::Transform& world_transform, int& draw_index) {
    for (Base_Component* component : components) {
        if (component->is_active() && component->has_target()) {
            component->draw(draw_index);
        }
    }
}

void Node::visit_cleanup(float delta_time, void* global_data) {
    Base_Node::visit_cleanup(delta_time, global_data);
    this->cleanup_stopped_actions();

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
    for (Base_Component* component : dettached_components) {
        component->assign_target(nullptr);
        components.push_back(component);
    }
    dettached_components.clear();
    this->attach(global_data);
}

void Node::exit() {
    /**Add list running component into list detach reserve wait to attach again!*/
    for (Base_Component* component : components) {
        component->exit();
        dettached_components.push_back(component);
    }
    components.clear();
    schedulers.clear();
    this->detach();
    this->stop_all_action();
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