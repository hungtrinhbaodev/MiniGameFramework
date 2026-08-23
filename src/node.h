#pragma once
#include <base_action.h>
#include <base_component.h>
#include <base_node.h>
#include <key_input_component.h>
#include <key_input_system.h>

#include <map>
#include <string>

class Node : public Base_Node {
public:
    Node();
    ~Node();

    Base_Component* get_component_by_name(std::string name);

    void add_component(Base_Component* component);
    void do_action(Base_Action* action, int tag = -1, bool debug = false);
    void schedule(
        const std::string& key,
        float duration_schedule,
        std::function<void(Base_Node*, void*)> caller,
        bool is_schedule_once = false
    );
    void schedule_once(const std::string& key, float duration_schedule, std::function<void(Base_Node*, void*)> caller);

    void remove_component(Base_Component* component);
    void remove_component(std::string component_name);
    void stop_action(int tag);
    void stop_all_action();
    void unschedule(const std::string& key);

    bool is_key_enabled(Custom::Key key);
    bool is_swallow_keys_enabled(Custom::Key key);
    std::function<void(Key_Press_Detail, Base_Node*, void*)> get_key_press_caller(Custom::Key key);
    void add_key_press_listener(
        Custom::Key key,
        std::function<void(Key_Press_Detail, Base_Node*, void*)> caller = nullptr,
        bool swallow_keys = false
    );
    void set_key_press_enabled(Custom::Key key, bool enabled);
    void set_key_press_swallow_enabled(Custom::Key key, bool swallow_keys);
    virtual void on_key_pressed(Custom::Key key, Key_Press_Detail pressed_detail, void* global_data);

    Node_Type get_type() override;

protected:
    std::vector<Base_Action*> actions;
    void handle_personal_task(float delta_time, void* global_data) override;
    void set_world_transform_information(
        Custom::Transform world_transform, int draw_index, float delta_time, void* global_data
    ) override;
    void draw(Custom::Transform& world_transform, int& draw_index) override;
    void visit_cleanup(float delta_time, void* global_data) override;
    void update(float delta_time) override;
    void exit(void* global_data) override;
    void enter(void* global_data) override;

    /**
     * @Note: Override function if need to custom class from Node
     * and use something work when node enter and exit!
     */
    virtual void attach(void* global_data);
    virtual void detach();

    /**
     * @Note: Override function if need custom class from Node
     * use to handle loop update logic or update ui
     * update logic will call width fix frame rate time
     * but ui will depend on strength of your machine!
     */
    /**
     * @Note: fix update will call with fix frame rate use to handle logic of node
     * */
    virtual void fix_update(float delta_time, void* global_data);
    /**
     * @Note: flex update will call with ability delta time which depend on strength of machine
     * */
    virtual void flex_update(float delta_time);

private:
    struct Scheduler_Data {
        float duration_scheduler = 0.f;
        float current_duration = 0.f;
        bool is_scheduler_once = false;
        std::function<void(Base_Node*, void*)> caller = nullptr;
        bool is_removed = false;
    };
    std::vector<Base_Action*> cleanup_actions;
    std::vector<Base_Component*> components;

    /**
     * Key input handler will be here
     */
    std::map<Custom::Key, std::function<void(Key_Press_Detail, Base_Node* target, void* global_data)>>
        key_input_callers;
    Key_Input_Component* get_or_create_key_input_component();
    Key_Input_Component* get_key_input_component();

    /**
     * @Note: In some case not is remove from parent but the
     * components is not remove we keep it in this vector
     * and if node enter again we add it again!
     */
    std::vector<Base_Component*> detached_components;
    std::map<std::string, Scheduler_Data> schedulers;
    void cleanup_stopped_actions();
};