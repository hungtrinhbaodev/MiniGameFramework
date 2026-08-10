#include <layer_node.h>
#include <math_custom.h>
#include <utils.h>
#include <wrapper.h>

#include <algorithm>
#include <glm/glm.hpp>
#include <iostream>
#include <queue>
#include <stack>
#include <stdexcept>

int MAX_CLIPPING_POINTS = 64;

namespace Libs_Wrapper {
    enum RayLib_Draw_Type { IMAGE, TEXT, START_CLIPPING, END_CLIPPING, LINE };

    struct RayLib_Base_Draw_Resouce {};

    struct RayLib_Draw_Image_Resource : public RayLib_Base_Draw_Resouce {
        std::string image_path = "";

        RayLib_Draw_Image_Resource(std::string image_path) {
            this->image_path = image_path;
        }
    };

    struct RayLib_Draw_Text_Resource : public RayLib_Base_Draw_Resouce {
        std::string font_path = "";
        std::string text = "";
        int font_size = 0;

        RayLib_Draw_Text_Resource(std::string font_path, std::string text, int font_size) {
            this->font_path = font_path;
            this->text = text;
            this->font_size = font_size;
        }
    };

    struct RayLib_Draw_Line_Resource : public RayLib_Base_Draw_Resouce {
        Vector2 start;
        Vector2 end;
        Color color;
        float thin;

        RayLib_Draw_Line_Resource(float start_x, float start_y, float end_x, float end_y, glm::vec3 color, float thin) {
            this->start = {start_x, start_y};
            this->end = {end_x, end_y};
            this->color = {
                (unsigned char)std::max(std::min((int)color.x, 255), 0),
                (unsigned char)std::max(std::min((int)color.y, 255), 0),
                (unsigned char)std::max(std::min((int)color.z, 255), 0),
                255
            };
            this->thin = thin;
        }
    };

    struct RayLib_Draw_Clipping_Resource : public RayLib_Base_Draw_Resouce {
        float width = 0.f;
        float height = 0.f;

        RayLib_Draw_Clipping_Resource(float width, float height) {
            this->width = width;
            this->height = height;
        }
    };

    struct RayLib_Draw_Command {
        RayLib_Draw_Type type;
        Custom::Draw_Attributes attributes;
        RayLib_Base_Draw_Resouce* resource = nullptr;

        bool operator<(const RayLib_Draw_Command& other) const {
            return this->attributes.draw_index > other.attributes.draw_index;
        }

        void clean() {
            if (resource != nullptr) {
                delete (resource);
            }
        }
    };

    struct RayLib_Texture_Info {
        Texture data;
        Image_Info info;
    };

    std::map<std::string, RayLib_Texture_Info> rl_textures_storage;
    std::map<std::string, Font> rl_fonts_storages;
    std::priority_queue<RayLib_Draw_Command> rl_queue_commands;

    Shader clipping_shader;
    int clipping_shader_points_location = 0;
    int clipping_shader_point_count_localtion = 0;
    std::vector<Vector2> clipping_points;
    /**@Hack using stack like the verify "(())()" backets sequence to handle clipping  */
    std::stack<int> stack_clipping;

    RayLib_Texture_Info load_raylib_texture(std::string path) {
        path = Utils::get_root_path() + path;
        if (rl_textures_storage.find(path) != rl_textures_storage.end()) {
            RayLib_Texture_Info texture_info = rl_textures_storage[path];
            return texture_info;
        }

        Texture2D texture = LoadTexture(path.data());
        RayLib_Texture_Info texture_info{texture, {(float)texture.width, (float)texture.height}};
        rl_textures_storage[path] = texture_info;

        return texture_info;
    }

    Font load_raylib_font(std::string path) {
        if (path.empty()) {
            return GetFontDefault();
        }

        path = Utils::get_root_path() + path;
        if (rl_fonts_storages.find(path) != rl_fonts_storages.end()) {
            return rl_fonts_storages[path];
        }

        Font font = LoadFont(path.data());
        if (!IsFontValid(font)) {
            font = GetFontDefault();
        }
        rl_fonts_storages[path] = font;

        return font;
    }

    float get_screen_width() {
        return GetScreenWidth();
    }

    float get_screen_height() {
        return GetScreenHeight();
    }

    void process_start_clipping(RayLib_Draw_Command& command) {
        Custom::Draw_Attributes attributes = command.attributes;
        Custom::Transform transfrom = attributes.transform;
        Custom::Anchor_Point anchor = attributes.anchor;
        RayLib_Draw_Clipping_Resource* resource = reinterpret_cast<RayLib_Draw_Clipping_Resource*>(command.resource);
        Custom::Rectangle rec{resource->width, resource->height};
        std::array<glm::vec2, 4> current_clipping_points = rec.apply(transfrom, anchor);
        for (int i = 0; i < 4; i++) {
            glm::vec2 point = current_clipping_points[i];
            glm::vec2 next_point = current_clipping_points[(i + 1) % 4];
            clipping_points.push_back({point.x, point.y});
        }

        int point_count = static_cast<int>(clipping_points.size());
        SetShaderValue(clipping_shader, clipping_shader_point_count_localtion, &point_count, SHADER_UNIFORM_INT);
        SetShaderValueV(
            clipping_shader, clipping_shader_points_location, clipping_points.data(), SHADER_UNIFORM_VEC2, point_count
        );
        BeginShaderMode(clipping_shader);
    }

    void process_resume_clipping() {
        int point_count = static_cast<int>(clipping_points.size());
        SetShaderValue(clipping_shader, clipping_shader_point_count_localtion, &point_count, SHADER_UNIFORM_INT);
        SetShaderValueV(
            clipping_shader, clipping_shader_points_location, clipping_points.data(), SHADER_UNIFORM_VEC2, point_count
        );
        BeginShaderMode(clipping_shader);
    }

    void process_end_clipping(bool clean_current_points = false) {
        if (clean_current_points) {
            for (int i = 0; i < 4; i++) {
                if (clipping_points.size() <= 0) {
                    throw std::runtime_error("End clipping: logic clipping is wrong, please verify it again!");
                }
                clipping_points.pop_back();
            }
        }
        EndShaderMode();
    }

    void init_libs() {
        clipping_shader = LoadShader(0, (Utils::get_root_path() + "shader/clipping.fs").data());
        if (!IsShaderValid(clipping_shader)) {
            throw std::runtime_error("Fail to load clipping shader please, try again!");
        }
        clipping_shader_points_location = GetShaderLocation(clipping_shader, "u_clip_points");
        clipping_shader_point_count_localtion = GetShaderLocation(clipping_shader, "u_point_count");
    }

    void open_window(int width, int height, int FPS, std::string window_name, void* window) {
        InitWindow(width, height, window_name.data());
        SetTargetFPS(FPS);
    }

    bool window_should_close(void* window) {
        return WindowShouldClose();
    }

    void close_window(void* window) {
        CloseWindow();
    }
    void draw_image(std::string image_path, Custom::Draw_Attributes attributes) {
        RayLib_Draw_Command command{RayLib_Draw_Type::IMAGE, attributes, new RayLib_Draw_Image_Resource(image_path)};
        rl_queue_commands.push(command);
    }

    void draw_text(std::string font_path, std::string text, int font_size, Custom::Draw_Attributes attributes) {
        RayLib_Draw_Command command{
            RayLib_Draw_Type::TEXT, attributes, new RayLib_Draw_Text_Resource(font_path, text, font_size)
        };
        rl_queue_commands.push(command);
    }

    void draw_line(
        float start_x, float start_y, float end_x, float end_y, int draw_index, glm::vec3 color, float thin
    ) {
        Custom::Draw_Attributes attr{};
        attr.draw_index = draw_index;
        RayLib_Draw_Command command{
            RayLib_Draw_Type::LINE, attr, new RayLib_Draw_Line_Resource(start_x, start_y, end_x, end_y, color, thin)
        };
        rl_queue_commands.push(command);
    }

    Image_Info image_info(std::string path) {
        RayLib_Texture_Info texture_info = load_raylib_texture(path);
        return texture_info.info;
    }

    Text_Info text_info(std::string text, std::string font_path, int font_size) {
        Font font = load_raylib_font(font_path);
        Vector2 text_size = MeasureTextEx(font, text.data(), (float)font_size, 1.0f);
        return {text_size.x, text_size.y};
    }

    void start_draw_clipping(float width, float height, Custom::Draw_Attributes attributes) {
        RayLib_Draw_Command command{
            RayLib_Draw_Type::START_CLIPPING, attributes, new RayLib_Draw_Clipping_Resource(width, height)
        };
        rl_queue_commands.push(command);
    }

    void end_draw_clipping(int draw_index) {
        Custom::Draw_Attributes attributes{};
        attributes.draw_index = draw_index;
        RayLib_Draw_Command command{RayLib_Draw_Type::END_CLIPPING, attributes};
        rl_queue_commands.push(command);
    }

    void draw_frame() {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        while (!rl_queue_commands.empty()) {
            RayLib_Draw_Command command = rl_queue_commands.top();
            Custom::Draw_Attributes attributes = command.attributes;
            Custom::Transform transform = attributes.transform;
            Custom::Anchor_Point anchor = attributes.anchor;
            int screen_height = get_screen_height();
            float x = transform.position.x;
            float y = (float)screen_height - transform.position.y;

            switch (command.type) {
                case RayLib_Draw_Type::START_CLIPPING: {
                    if (stack_clipping.size() <= 0) {
                        process_start_clipping(command);
                    } else {
                        process_end_clipping(false);
                        process_start_clipping(command);
                    }
                    stack_clipping.push(1);
                    command.clean();
                    break;
                }
                case RayLib_Draw_Type::IMAGE: {
                    RayLib_Draw_Image_Resource* resource =
                        reinterpret_cast<RayLib_Draw_Image_Resource*>(command.resource);
                    RayLib_Texture_Info texture_info = load_raylib_texture(resource->image_path);
                    Texture2D texture = texture_info.data;

                    Rectangle source = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
                    source.width *= (attributes.flipped.x ? -1.f : 1.f);
                    source.height *= (attributes.flipped.y ? -1.f : 1.f);
                    float tex_width = (float)texture.width * transform.scale.x;
                    float tex_height = (float)texture.height * transform.scale.y;
                    Rectangle dest = {x, y, tex_width, tex_height};
                    Vector2 origin = {anchor.x * tex_width, anchor.y * tex_height};

                    DrawTexturePro(
                        texture,
                        source,
                        dest,
                        origin,
                        transform.rotation,
                        {attributes.tint.g, attributes.tint.b, attributes.tint.r, transform.opacity}
                    );

                    command.clean();
                    break;
                }
                case RayLib_Draw_Type::TEXT: {
                    RayLib_Draw_Text_Resource* resource =
                        reinterpret_cast<RayLib_Draw_Text_Resource*>(command.resource);
                    Font font = load_raylib_font(resource->font_path);

                    Vector2 position = {x, y};
                    float font_size = (float)resource->font_size * std::min(transform.scale.x, transform.scale.y);
                    Vector2 text_size = MeasureTextEx(font, resource->text.data(), font_size, 1.0f);
                    Vector2 origin = {text_size.x * anchor.x, text_size.y * anchor.y};

                    DrawTextPro(
                        font,
                        resource->text.data(),
                        position,
                        origin,
                        transform.rotation,
                        font_size,
                        1.0f,
                        {attributes.tint.g, attributes.tint.b, attributes.tint.r, transform.opacity}
                    );

                    command.clean();
                    break;
                }
                case RayLib_Draw_Type::LINE: {
                    RayLib_Draw_Line_Resource* resource =
                        reinterpret_cast<RayLib_Draw_Line_Resource*>(command.resource);
                    resource->start.y = screen_height - resource->start.y;
                    resource->end.y = screen_height - resource->end.y;
                    DrawLineEx(resource->start, resource->end, resource->thin, resource->color);
                    command.clean();
                    break;
                }
                case RayLib_Draw_Type::END_CLIPPING: {
                    process_end_clipping(true);
                    if (stack_clipping.size() > 0) {
                        stack_clipping.pop();
                    }
                    if (stack_clipping.size() > 0) {
                        process_resume_clipping();
                    }
                    command.clean();
                    break;
                }
                default: {
                    break;
                }
            }
            rl_queue_commands.pop();
        }

        EndDrawing();

        if (stack_clipping.size() > 0 || clipping_points.size() > 0) {
            throw std::runtime_error("Drawing: logic clipping wrong, please review it again!");
        }
    }

    void clear_libs() {
        for (const auto& [_, texture_info] : rl_textures_storage) {
            UnloadTexture(texture_info.data);
        }
    }

}  // namespace Libs_Wrapper