#pragma once
#include <base_component.h>
#include <custom.h>
#include <touch_system.h>

class Touch_Component : public Base_Component {
public:
    Touch_Component();
    ~Touch_Component();

    Touch_Information& modify_infomation();

protected:
    void attach(Base_Node* target, void* global_data) override;
    void detach(Base_Node* target, void* global_data = nullptr) override;
    void draw(Base_Node* target, int& draw_index) override;
    void update_information(Base_Node* target, float delta_time, void* global_data) override;
    void handle_task(Base_Node* target, float delta_time, void* global_data) override;

private:
    Touch_Information touch_information;
};