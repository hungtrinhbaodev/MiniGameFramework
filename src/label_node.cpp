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

glm::vec3 Label_Node::get_color() {
    return this->color;
}

int Label_Node::get_font_size() {
    return this->font_size;
}

glm::vec2 Label_Node::get_content_size() {
    Text_Info info = Libs_Wrapper::text_info(this->text, this->font, this->font_size);
    return {info.width, info.height};
}

std::string Label_Node::get_font() {
    return this->font;
}

void Label_Node::set_font(std::string font) {
    this->font = font;
}

void Label_Node::set_font_size(int size) {
    this->font_size = font_size;
}

void Label_Node::set_text(std::string text) {
    this->text = text;
}

void Label_Node::set_color(glm::vec3 color) {
    this->color = color;
}

void Label_Node::draw(Base_Node::Transform& world_transform, int& draw_index) {
    Libs_Wrapper::draw_text(
        font,
        text,
        font_size,
        {world_transform.position.x,
         world_transform.position.y,
         world_transform.scale.x,
         world_transform.scale.y,
         world_transform.rotation,
         draw_index,
         this->transform.anchor.x,
         this->transform.anchor.y,
         {(unsigned char)color.x, (unsigned char)color.y, (unsigned char)color.z, 255},
         world_transform.opacity}
    );
    draw_index++;
}