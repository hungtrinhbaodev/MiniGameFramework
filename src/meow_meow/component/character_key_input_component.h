#pragma once
#include <key_input_component.h>

namespace Meow_Meow {
    class Character_Key_Input_Component : public Key_Input_Component {
    public:
        Character_Key_Input_Component();
        ~Character_Key_Input_Component();

        Custom::Key get_key_need_handled();
        Key_Input_Type get_key_type_pressed();
        float get_duration_hold();

    protected:
        void handle_task(Base_Node* target, float delta_time, void* global_data) override;

    private:
        Custom::Key key_need_handled = Custom::Key::NONE;
        Key_Input_Type key_type_pressed = Key_Input_Type::IDLE;
        float duration_hold = 0;

        bool is_pressed_key(Custom::Key key);
        bool is_holding_key(Custom::Key key);
        bool is_released_key(Custom::Key key);
        bool is_cancel_key(Custom::Key key);
        void set_pressed_information(
            Custom::Key key_need_handled, Key_Input_Type key_type_pressed, float duration_hold
        );
    };
}  // namespace Meow_Meow
