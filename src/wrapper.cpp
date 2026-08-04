#include <wrapper.h>
#include "raylib.h"
#include <stdexcept>

namespace Libs_Wrapper
{
    struct RayLib_Draw_Attributes
    {
        int x;
        int y;
        float scale_x;
        float scale_y;
        int rotation;
        int z_order;
    };

    struct RayLib_Draw_Image_Command
    {
        std::string image_path;
        RayLib_Draw_Attributes attributes;
    };

    struct RayLib_Draw_Font_Command
    {
        std::string font;
        RayLib_Draw_Attributes attributes;
    };

    struct RayLib_Texture_Info
    {
        Texture data;

        Image_Info info;
    };

    std::map<std::string, RayLib_Texture_Info>
        rl_textures_storage;

    std::queue<RayLib_Draw_Image_Command> rl_queue_image_commands;

    std::queue<RayLib_Draw_Font_Command> rl_queue_font_commands;

    void init_libs()
    {
    }

    void open_window(int width, int height, int FPS, std::string window_name, void *window)
    {
        InitWindow(width, height, window_name.data());
        SetTargetFPS(FPS);
    }

    bool window_should_close(void *window)
    {
        return WindowShouldClose();
    }

    void close_window(void *window)
    {
        CloseWindow();
    }

    RayLib_Texture_Info load_raylib_texture(std::string path)
    {
        if (rl_textures_storage.find(path) != rl_textures_storage.end())
        {
            RayLib_Texture_Info texture_info = rl_textures_storage[path];
            return texture_info;
        }

        Texture2D texture = LoadTexture(path.data());
        RayLib_Texture_Info texture_info{texture, {texture.width, texture.width}};
        rl_textures_storage[path] = texture_info;

        return texture_info;
    }

    void draw_image(std::string path, int x, int y, float scale_x, float scale_y, int rotation, int z_order)
    {
        rl_queue_image_commands.push({path, {x, y, scale_x, scale_y, rotation, z_order}});
    }

    void draw_font(std::string path, int x, int y, float scale_x, float scale_y, int rotation, int z_order)
    {
        rl_queue_font_commands.push({path, {x, y, scale_x, scale_y, rotation, z_order}});
    }

    Image_Info image_info(std::string path)
    {
        RayLib_Texture_Info texture_info = load_raylib_texture(path);
        return texture_info.info;
    }

    void draw_frame()
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        while (!rl_queue_image_commands.empty())
        {
            RayLib_Draw_Image_Command command = rl_queue_image_commands.front();
            RayLib_Texture_Info texture_info = load_raylib_texture(command.image_path);

            Texture2D texture = texture_info.data;
            Rectangle source = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
            Rectangle dest = {(float)command.attributes.x, (float)command.attributes.y, (float)texture.width * command.attributes.scale_x, (float)texture.height * command.attributes.scale_y};
            Vector2 origin = {0.0f, 0.0f};

            DrawTexturePro(texture, source, dest, origin, command.attributes.rotation, WHITE);
            rl_queue_image_commands.pop();
        }

        EndDrawing();
    }

    void clear_libs()
    {
        for (const auto &[_, texture_info] : rl_textures_storage)
        {
            UnloadTexture(texture_info.data);
        }
    }

}