#include <image_node.h>
#include <progression_node.h>
#include <scene_node.h>

namespace Meow_Meow {
    class Loading_Scene : public Scene_Node {
    public:
        Loading_Scene();
        ~Loading_Scene();

    protected:
        void attach() override;

    private:
        void init_bg();
        void init_loading();
        void init_logo();

        /**
         * Defined constants here!
         * */
        glm::vec2 ORIGIN_SCALE_LOGO = {0.3, 0.3};
        Custom::Rectangle_Area CAP_INSETS_LOADING_BAR = {30, 10, 74, 31};
        Custom::Size RENDERER_SIZE_LOADING_BAR = {360, 43};
        Custom::Color COLOR_PROGRESSION_BAR = {20, 160, 20};
        glm::vec2 DELTA_POSITION_INNER_PROGRESSION = {6, 8};
        float POSITION_Y_PRORESSION = 80.f;
        float RATIO_Y_LOGO = 0.55f;

        Image_Node* bg;
        Image_Node* logo;
        Progression_Node* loading;
    };
}  // namespace Meow_Meow