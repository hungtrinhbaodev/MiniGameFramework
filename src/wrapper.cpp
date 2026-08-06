#include <wrapper.h>

#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <stdexcept>

namespace Libs_Wrapper {
    enum RayLib_Draw_Type { IMAGE, TEXT, START_CLIPPING, END_CLIPPING };

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
        Draw_Attributes attributes;
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
    int clipping_shader_quads_location = -1;
    int clipping_shader_count_location = -1;

    RayLib_Texture_Info load_raylib_texture(std::string path) {
        if (rl_textures_storage.find(path) != rl_textures_storage.end()) {
            RayLib_Texture_Info texture_info = rl_textures_storage[path];
            return texture_info;
        }

        Texture2D texture = LoadTexture(path.data());
        RayLib_Texture_Info texture_info{texture, {texture.width, texture.width}};
        rl_textures_storage[path] = texture_info;

        return texture_info;
    }

    Font load_raylib_font(std::string path) {
        if (rl_fonts_storages.find(path) != rl_fonts_storages.end()) {
            return rl_fonts_storages[path];
        }

        Font font = LoadFont(path.data());
        rl_fonts_storages[path] = font;

        return font;
    }

    int get_screen_width() {
        return GetScreenWidth();
    }

    int get_screen_height() {
        return GetScreenHeight();
    }

    void init_libs() {
        clipping_shader = LoadShader(0, "res/shader/clipping.fs");
        if (!IsShaderValid(clipping_shader)) {
            throw std::runtime_error("Fail to load clipping shader please, try again!");
        }
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

    void draw_image(std::string image_path, Draw_Attributes attributes) {
        RayLib_Draw_Command command{
            RayLib_Draw_Type::IMAGE, attributes, new RayLib_Draw_Image_Resource(image_path)
        };
        rl_queue_commands.push(command);
    }

    void draw_text(
        std::string font_path, std::string text, int font_size, Draw_Attributes attributes
    ) {
        RayLib_Draw_Command command{
            RayLib_Draw_Type::TEXT,
            attributes,
            new RayLib_Draw_Text_Resource(font_path, text, font_size)
        };
        rl_queue_commands.push(command);
    }

    Image_Info image_info(std::string path) {
        RayLib_Texture_Info texture_info = load_raylib_texture(path);
        return texture_info.info;
    }

    void start_draw_clipping(float width, float height, Draw_Attributes attributes) {
        RayLib_Draw_Command command{
            RayLib_Draw_Type::START_CLIPPING,
            attributes,
            new RayLib_Draw_Clipping_Resource(width, height)
        };
        rl_queue_commands.push(command);
    }

    void end_draw_clipping() {
        RayLib_Draw_Command command{RayLib_Draw_Type::END_CLIPPING};
        rl_queue_commands.push(command);
    }

    void draw_frame() {
        std::stack<RayLib_Draw_Command> stack_clipping_command;

        BeginDrawing();

        ClearBackground(RAYWHITE);

        while (!rl_queue_commands.empty()) {
            RayLib_Draw_Command command = rl_queue_commands.top();
            Draw_Attributes attributes = command.attributes;
            int screen_height = get_screen_height();
            float x = attributes.x;
            float y = (float)screen_height - attributes.y;

            switch (command.type) {
                case RayLib_Draw_Type::START_CLIPPING: {
                    break;
                }
                case RayLib_Draw_Type::IMAGE: {
                    RayLib_Draw_Image_Resource* resource =
                        reinterpret_cast<RayLib_Draw_Image_Resource*>(command.resource);
                    RayLib_Texture_Info texture_info = load_raylib_texture(resource->image_path);
                    Texture2D texture = texture_info.data;

                    Rectangle source = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
                    float tex_width = (float)texture.width * attributes.scale_x;
                    float tex_height = (float)texture.height * attributes.scale_y;
                    Rectangle dest = {x, y, tex_width, tex_height};
                    Vector2 origin = {
                        attributes.anchor_x * tex_width, attributes.anchor_y * tex_height
                    };

                    DrawTexturePro(
                        texture,
                        source,
                        dest,
                        origin,
                        command.attributes.rotation,
                        {attributes.tint.g,
                         attributes.tint.b,
                         attributes.tint.r,
                         attributes.opacity}
                    );

                    command.clean();
                    break;
                }
                case RayLib_Draw_Type::TEXT: {
                    RayLib_Draw_Text_Resource* resource =
                        reinterpret_cast<RayLib_Draw_Text_Resource*>(command.resource);
                    Font font = load_raylib_font(resource->font_path);

                    Vector2 position = {x, y};
                    float font_size = (float)resource->font_size *
                                      std::min(attributes.scale_y, attributes.scale_x);
                    Vector2 text_size = MeasureTextEx(font, resource->text.data(), font_size, 1.0f);
                    Vector2 origin = {
                        text_size.x * attributes.anchor_x, text_size.y * attributes.anchor_y
                    };

                    DrawTextPro(
                        font,
                        resource->text.data(),
                        position,
                        origin,
                        attributes.rotation,
                        font_size,
                        1.0f,
                        {attributes.tint.g,
                         attributes.tint.b,
                         attributes.tint.r,
                         attributes.opacity}
                    );

                    command.clean();
                    break;
                }
                case RayLib_Draw_Type::END_CLIPPING: {
                    break;
                }
                default: {
                    break;
                }
            }
            rl_queue_commands.pop();
        }

        EndDrawing();

        while (stack_clipping_command.size()) {
            stack_clipping_command.top().clean();
            stack_clipping_command.pop();
        }
    }

    void clear_libs() {
        for (const auto& [_, texture_info] : rl_textures_storage) {
            UnloadTexture(texture_info.data);
        }
    }

}  // namespace Libs_Wrapper