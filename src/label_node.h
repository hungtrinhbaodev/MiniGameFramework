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
    Custom::Color get_color();
    Custom::Size get_content_size();
    int get_font_size();

    void set_font(std::string font);
    void set_font_size(int size);
    void set_text(std::string text);
    void set_color(Custom::Color color);

protected:
    void draw(Custom::Transform& world_transform, int& draw_index) override;

private:
    std::string text = "";
    std::string font = "";
    int font_size = 18;
    Custom::Color color;
};