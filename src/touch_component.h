#pragma once
#include <base_component.h>
#include <custom.h>
#include <touch_system.h>

class Touch_Component : public Base_Component {
public:
    Touch_Component();
    ~Touch_Component();

    Touch_Information& modify_infomation();
    void enter() override;
    void exit() override;
    void draw(int& draw_index) override;
    void update_information() override;
    void handle_task() override;

private:
    Touch_Information touch_information;
};