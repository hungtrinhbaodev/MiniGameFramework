#include <meow_meow/layer/layer_stats.h>
#include <wrapper.h>

namespace Meow_Meow {
    Layer_Stats::Layer_Stats() {
        this->set_content_size({Libs_Wrapper::get_screen_width(), Libs_Wrapper::get_screen_height()});
        this->init_stats_ui();
        this->set_name("Layer_Stats");
    }

    Layer_Stats::~Layer_Stats() {}

    void Layer_Stats::init_stats_ui() {
        this->stats_ui = new Battle_Character_Stats_UI();
        Custom::Size ui_size = this->stats_ui->get_content_size();
        this->stats_ui->set_position({0, this->get_content_size().height - ui_size.height});
        this->add_child(this->stats_ui);
    }
}  // namespace Meow_Meow