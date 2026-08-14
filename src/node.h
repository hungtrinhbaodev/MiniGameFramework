#pragma once
#include <base_action.h>
#include <base_component.h>
#include <base_node.h>

#include <map>
#include <string>

class Node : public Base_Node {
public:
    Node();
    ~Node();

    Base_Component* get_component_by_name(std::string name);

    void add_component(Base_Component* component);
    void do_action(Base_Action* action, int tag = -1);
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

    Node_Type get_type() override;

protected:
    std::vector<Base_Action*> actions;
    void handle_personal_task(float delta_time, void* global_data) override;
    void set_world_transform_information(Custom::Transform world_transform, int draw_index) override;
    void draw(Custom::Transform& world_transform, int& draw_index) override;
    void update(float delta_time) override;
    void exit() override;
    void enter() override;

    /**
     * @Note: Override function if need to custom class from Node
     * and use something work when node enter and exit!
     */
    virtual void attach();
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
     * @Note: In some case not is remove from parent but the
     * components is not remove we keep it in this vector
     * and if node enter again we add it again!
     */
    std::vector<Base_Component*> dettached_components;
    std::map<std::string, Scheduler_Data> schedulers;
    void cleanup_stopped_actions();
};