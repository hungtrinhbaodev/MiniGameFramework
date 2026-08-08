#pragma once
#include <node.h>

#include <string>

class Label_Node : public Node {
public:
    Label_Node();
    Label_Node(std::string text, std::string font, int font_size);
    ~Label_Node();

    std::string get_text();
    std::string get_font();
    glm::vec3 get_color();
    int get_font_size();
    glm::vec2 get_content_size();

    void set_font(std::string font);
    void set_font_size(int size);
    void set_text(std::string text);
    void set_color(glm::vec3 color);

protected:
    void draw(Transform& world_transform, int& draw_index);

private:
    std::string text = "";
    std::string font = "";
    int font_size = 18;
    glm::vec3 color;
};