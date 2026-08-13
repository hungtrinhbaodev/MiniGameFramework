#include <label_node.h>
#include <wrapper.h>

Label_Node::Label_Node() {}

Label_Node::Label_Node(std::string text, std::string font, int font_size) {
    this->text = text;
    this->font = font;
    this->font_size = font_size;
}

Label_Node::~Label_Node() {}

std::string Label_Node::get_text() {
    return this->text;
}

Custom::Color Label_Node::get_color() {
    return this->color;
}

int Label_Node::get_font_size() {
    return this->font_size;
}

Node_Type Label_Node::get_type() {
    return Node_Type::LABEL;
}

Custom::Size Label_Node::get_content_size() {
    Text_Info info = Libs_Wrapper::text_info(this->text, this->font, this->font_size);
    return info.size;
}

std::string Label_Node::get_font() {
    return this->font;
}

void Label_Node::set_font(std::string font) {
    this->font = font;
}

void Label_Node::set_font_size(int size) {
    this->font_size = size;
}

void Label_Node::set_text(std::string text) {
    this->text = text;
}

void Label_Node::set_color(Custom::Color color) {
    this->color = color;
}

void Label_Node::draw(Custom::Transform& world_transform, int& draw_index) {
    Node::draw(world_transform, draw_index);
    if (Libs_Wrapper::is_debug_mode()) {
        Custom::Size size = this->get_content_size();
        Custom::Rectangle rec = {size.width, size.height};
        Custom::Color color = {255, 0, 255};
        draw_index += rec.draw_border_rectangle(world_transform, this->anchor, draw_index, color);
    }
    Libs_Wrapper::draw_text(font, text, font_size, {world_transform, anchor, draw_index, color});
    draw_index++;
}