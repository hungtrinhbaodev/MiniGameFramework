#include <test/image_test.h>

Image_Test_Scene::Image_Test_Scene() {
    image = new Image_Node("res/meow_meow/AddonBtnyellow.png");
    image->set_position({602, 307});
    image->set_scale({1.2f, 1.1f});
    image->set_rotation(36.5);
    image->set_anchor({0.3, 0.7});
    this->add_child(image);

    image2 = new Image_Node("res/meow_meow/BtnGreen.png");
    image2->set_position({257, 150});
    image2->set_scale({1.1f, 1.2f});
    image2->set_rotation(15);
    image2->set_anchor({0.5, 0.7});
    image2->set_enable_draw_rect(true);
    image2->set_draw_rect({20, 30, 220, 40});
    this->add_child(image2);
}

Image_Test_Scene::~Image_Test_Scene() {}