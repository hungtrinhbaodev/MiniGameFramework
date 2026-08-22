#pragma once
#include <custom.h>
#include <node.h>

namespace Meow_Meow {
    class Game_Object : public Node {
    public:
        Game_Object();
        ~Game_Object();

    protected:
        void fix_update(float delta_time, void* global_data) override;
        virtual Custom::Transformed_Rectangle get_bounding_box();
        virtual void handle_boundary();
    };
}  // namespace Meow_Meow