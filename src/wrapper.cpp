#include <wrapper.h>

#include <algorithm>
#include <iostream>
#include <queue>

namespace Libs_Wrapper {
    enum RayLib_Draw_Type {
        IMAGE,
        TEXT
    };

    struct RayLib_Draw_Resouce {
        std::string resource_path = "";
        std::string text = "";
        int font_size = 0;

        static RayLib_Draw_Resouce make_image(std::string image_path) {
            return {image_path, "", 0};
        }

        static RayLib_Draw_Resouce make_text(std::string font_path, std::string text, int font_size) {
            return {font_path, text, font_size};
        }
    };

    struct RayLib_Draw_Command {
        RayLib_Draw_Type type;
        RayLib_Draw_Resouce resouces;
        Draw_Attributes attributes;

        bool operator<(const RayLib_Draw_Command& other) const {
            return this->attributes.z_order > other.attributes.z_order;
        }
    };

    struct RayLib_Texture_Info {
        Texture data;
        Image_Info info;
    };

    std::map<std::string, RayLib_Texture_Info>
        rl_textures_storage;

    std::map<std::string, Font> rl_fonts_storages;

    std::priority_queue<RayLib_Draw_Command> rl_queue_commands;

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

    void draw_image(
        std::string image_path,
        Draw_Attributes attributes) {
        RayLib_Draw_Command command{
            RayLib_Draw_Type::IMAGE,
            RayLib_Draw_Resouce::make_image(image_path),
            attributes};
        rl_queue_commands.push(command);
    }

    void draw_text(
        std::string font_path,
        std::string text,
        int font_size,
        Draw_Attributes attributes) {
        RayLib_Draw_Command command{
            RayLib_Draw_Type::TEXT,
            RayLib_Draw_Resouce::make_text(font_path, text, font_size),
            attributes};
        rl_queue_commands.push(command);
    }

    Image_Info image_info(std::string path) {
        RayLib_Texture_Info texture_info = load_raylib_texture(path);
        return texture_info.info;
    }

    void draw_frame() {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        while (!rl_queue_commands.empty()) {
            RayLib_Draw_Command command = rl_queue_commands.top();
            RayLib_Draw_Resouce resources = command.resouces;
            Draw_Attributes attributes = command.attributes;
            int screen_height = get_screen_height();
            float x = attributes.x;
            float y = (float)screen_height - attributes.y;

            switch (command.type) {
                case RayLib_Draw_Type::IMAGE: {
                    RayLib_Texture_Info texture_info = load_raylib_texture(resources.resource_path);
                    Texture2D texture = texture_info.data;

                    Rectangle source = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
                    float tex_width = (float)texture.width * attributes.scale_x;
                    float tex_height = (float)texture.height * attributes.scale_y;
                    Rectangle dest = {x, y, tex_width, tex_height};
                    Vector2 origin = {attributes.anchor_x * tex_width, attributes.anchor_y * tex_height};

                    DrawTexturePro(texture, source, dest, origin, command.attributes.rotation, attributes.tint);
                    break;
                }
                case RayLib_Draw_Type::TEXT: {
                    Font font = load_raylib_font(resources.resource_path);

                    Vector2 position = {x, y};
                    float font_size = (float)resources.font_size * std::min(attributes.scale_y, attributes.scale_x);
                    Vector2 text_size = MeasureTextEx(font, resources.text.data(), font_size, 1.0f);
                    Vector2 origin = {text_size.x * attributes.anchor_x, text_size.y * attributes.anchor_y};

                    DrawTextPro(font, resources.text.data(), position, origin, attributes.rotation, font_size, 1.0f, attributes.tint);
                    break;
                }
                default: {
                    break;
                }
            }
            rl_queue_commands.pop();
        }

        EndDrawing();
    }

    void clear_libs() {
        for (const auto& [_, texture_info] : rl_textures_storage) {
            UnloadTexture(texture_info.data);
        }
    }

}  // namespace Libs_Wrapper