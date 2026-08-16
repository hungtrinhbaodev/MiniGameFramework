#pragma once
#include <image_node.h>
#include <image_ui_node.h>
#include <scene_node.h>

class Image_Test_Scene : public Scene_Node {
public:
    Image_Test_Scene();
    ~Image_Test_Scene();

private:
    Image_Node* image = nullptr;
    Image_Node* image2 = nullptr;
};