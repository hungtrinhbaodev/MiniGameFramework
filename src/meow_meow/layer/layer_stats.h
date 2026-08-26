#include <layer_node.h>
#include <meow_meow/ui/battle_character_stats_ui.h>

namespace Meow_Meow {
    class Layer_Stats : public Layer_Node {
    public:
        Layer_Stats();
        ~Layer_Stats();

    private:
        void init_stats_ui();

        Battle_Character_Stats_UI* stats_ui = nullptr;
    };
}  // namespace Meow_Meow