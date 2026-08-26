#include <ThreadPool.h>
#include <collision_system.h>
#include <director.h>
#include <key_input_system.h>
#include <label_node.h>
#include <math_custom.h>
#include <touch_system.h>
#include <utils.h>
#include <wrapper.h>

#include <algorithm>
#include <glm/glm.hpp>
#include <iostream>
#include <mutex>
#include <queue>
#include <stack>
#include <stdexcept>
#include <thread>

#include "rlgl.h"

int MAX_CLIPPING_POINTS = 64;

namespace Libs_Wrapper {
    enum RayLib_Draw_Type { IMAGE, TEXT, START_CLIPPING, END_CLIPPING, LINE, RECTANGLE };

    struct RayLib_Base_Draw_Resouce {
        virtual std::map<std::string, std::string> get_trace() {
            return {};
        }
    };

    struct RayLib_Draw_Image_Resource : public RayLib_Base_Draw_Resouce {
        std::string image_path = "";
        bool enable_force_color = false;
        Custom::Color force_color;
        Defined::LOAD_MODE load_mode;

        RayLib_Draw_Image_Resource(
            std::string image_path,
            bool enable_force_color = false,
            Custom::Color force_color = {},
            Defined::LOAD_MODE load_mode = Defined::LOAD_MODE::IMMEDIATE
        ) {
            this->image_path = image_path;
            this->enable_force_color = enable_force_color;
            this->force_color = force_color;
            this->load_mode = load_mode;
        }

        std::map<std::string, std::string> get_trace() {
            std::map<std::string, std::string> extra_information;
            extra_information.insert({std::string("Image texture"), image_path});
            extra_information.insert({std::string("Enable force color"), std::to_string(enable_force_color)});
            if (enable_force_color) {
                extra_information.insert({std::string("Force color"), this->force_color.to_string()});
            }
            return extra_information;
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

        std::map<std::string, std::string> get_trace() {
            std::map<std::string, std::string> extra_information;
            extra_information.insert({std::string("Text font"), font_path});
            extra_information.insert({std::string("Text"), text});
            extra_information.insert({std::string("Font size"), std::to_string(font_size)});
            return extra_information;
        }
    };

    struct RayLib_Draw_Line_Resource : public RayLib_Base_Draw_Resouce {
        Vector2 start;
        Vector2 end;
        Color color;
        float thin;
        bool is_dashed;

        RayLib_Draw_Line_Resource(
            float start_x, float start_y, float end_x, float end_y, Custom::Color color, float thin, bool is_dashed
        ) {
            this->start = {start_x, start_y};
            this->end = {end_x, end_y};
            this->color = {color.r, color.g, color.b, 255};
            this->thin = thin;
            this->is_dashed = is_dashed;
        }

        std::map<std::string, std::string> get_trace() {
            std::map<std::string, std::string> extra_information;
            extra_information.insert({std::string("Start x"), std::to_string(start.x)});
            extra_information.insert({std::string("Start y"), std::to_string(start.y)});
            extra_information.insert({std::string("End x"), std::to_string(end.x)});
            extra_information.insert({std::string("End y"), std::to_string(end.y)});
            return extra_information;
        }
    };

    struct RayLib_Draw_Rectangle_Resource : public RayLib_Base_Draw_Resouce {
        float width;
        float height;

        RayLib_Draw_Rectangle_Resource(float width, float height) {
            this->width = width;
            this->height = height;
        }

        std::map<std::string, std::string> get_trace() {
            std::map<std::string, std::string> extra_information;
            extra_information.insert({std::string("width"), std::to_string(width)});
            extra_information.insert({std::string("height"), std::to_string(height)});
            return extra_information;
        }
    };

    struct RayLib_Draw_Clipping_Resource : public RayLib_Base_Draw_Resouce {
        float width = 0.f;
        float height = 0.f;

        RayLib_Draw_Clipping_Resource(float width, float height) {
            this->width = width;
            this->height = height;
        }

        std::map<std::string, std::string> get_trace() {
            std::map<std::string, std::string> extra_information;
            extra_information.insert({std::string("Clipping width"), std::to_string(width)});
            extra_information.insert({std::string("Clipping height"), std::to_string(height)});
            return extra_information;
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
        Defined::RESOURCE_LOADED_STATE loaded_state = Defined::RESOURCE_LOADED_STATE::UNLOAD;
        Defined::LOAD_MODE load_mode = Defined::LOAD_MODE::IMMEDIATE;
        Image inner_image = {};
        bool is_loaded_texture = false;
    };

    struct RayLib_Trace_Draw_Command {
        RayLib_Draw_Type command_type;
        Custom::Draw_Attributes attributes;
        std::map<std::string, std::string> trace_extra_information;
        friend std::ostream& operator<<(std::ostream& os, const RayLib_Trace_Draw_Command& trace) {
            std::string type = "";
            switch (trace.command_type) {
                case IMAGE: {
                    type = "IMAGE";
                    break;
                }
                case TEXT: {
                    type = "TEXT";
                    break;
                }
                case START_CLIPPING: {
                    type = "START_CLIPPING";
                    break;
                }
                case END_CLIPPING: {
                    type = "END_CLIPPING";
                    break;
                }
                case LINE: {
                    type = "LINE";
                    break;
                }
                case RECTANGLE: {
                    type = "RECTANGLE";
                    break;
                }
            }
            os << "Trace command: " << type << std::endl << trace.attributes;
            if (trace.trace_extra_information.size() > 0) {
                os << "Extra information: ";
                for (auto& [key, value] : trace.trace_extra_information) {
                    os << key << ": " << value << " ";
                }
            }
            os << std::endl << std::endl;
            return os;
        }
    };

    /**Draw storages data information */
    std::map<std::string, RayLib_Texture_Info> rl_textures_storage;
    std::map<std::string, Font> rl_fonts_storage;
    std::priority_queue<RayLib_Draw_Command> rl_queue_commands;

    /**Debug information */
    bool is_debug = false;
    std::vector<RayLib_Trace_Draw_Command> trace_commands;

    /**Shader support information */
    Shader clipping_shader;
    int clipping_shader_points_location = 0;
    int clipping_shader_point_count_localtion = 0;
    int shader_enable_force_color_location = 0;
    int shader_force_color_location = 0;
    std::vector<Vector2> clipping_points;
    bool enable_force_color_texture = false;
    Custom::Color force_color_texture{};

    /**Event touch handler information */
    Touch_Type touch_state = Touch_Type::END;
    Vector2 current_touched_position;

    /**Event key press handler information */
    std::map<Custom::Key, Key_Press_Detail> keys_detail;

    ThreadPool thread_pool{std::thread::hardware_concurrency()};
    std::mutex resource_mutex;
    /**
     * @Note: one frame we load one async texture
     * from image to make the game smooth!
     */
    bool is_load_texture_async_in_frame = false;

    void load_async_texture(std::string path, RayLib_Texture_Info* info) {
        Image image = LoadImage(path.data());
        {
            std::unique_lock<std::mutex> lock(resource_mutex);
            info->loaded_state = Defined::RESOURCE_LOADED_STATE::LOADED;
            info->info.state = info->loaded_state;
            info->inner_image = image;
        }
    }

    RayLib_Texture_Info load_raylib_texture(
        std::string path, Defined::LOAD_MODE load_mode = Defined::LOAD_MODE::IMMEDIATE
    ) {
        path = Utils::get_root_path() + path;

        if (rl_textures_storage.find(path) != rl_textures_storage.end()) {
            RayLib_Texture_Info& texture_info = rl_textures_storage[path];
            if (texture_info.load_mode != Defined::LOAD_MODE::ASYNC) {
                return texture_info;
            }
            {
                std::unique_lock<std::mutex> lock(resource_mutex);
                if (texture_info.loaded_state == Defined::RESOURCE_LOADED_STATE::LOADED &&
                    !texture_info.is_loaded_texture && !is_load_texture_async_in_frame) {
                    texture_info.data = LoadTextureFromImage(texture_info.inner_image);
                    texture_info.info = {
                        (float)texture_info.data.width, (float)texture_info.data.height, texture_info.loaded_state
                    };
                    texture_info.is_loaded_texture = true;
                    is_load_texture_async_in_frame = true;
                }
            }
            return texture_info;
        }

        rl_textures_storage[path] = {};
        rl_textures_storage[path].load_mode = load_mode;
        if (load_mode == Defined::LOAD_MODE::IMMEDIATE) {
            Texture2D texture = LoadTexture(path.data());
            rl_textures_storage[path] = {
                texture,
                {(float)texture.width, (float)texture.height, Defined::RESOURCE_LOADED_STATE::LOADED},
                Defined::RESOURCE_LOADED_STATE::LOADED
            };
            rl_textures_storage[path].is_loaded_texture = true;
        } else {
            rl_textures_storage[path].loaded_state = Defined::RESOURCE_LOADED_STATE::LOADING;
            rl_textures_storage[path].info = {0.f, 0.f, rl_textures_storage[path].loaded_state};
            thread_pool.enqueue(
                [](std::string path, RayLib_Texture_Info* info) { load_async_texture(path, info); },
                path,
                &rl_textures_storage[path]
            );
            rl_textures_storage[path].is_loaded_texture = false;
        }

        return rl_textures_storage[path];
    }

    Font load_raylib_font(std::string path) {
        path = Utils::get_root_path() + path;
        if (rl_fonts_storage.find(path) != rl_fonts_storage.end()) {
            return rl_fonts_storage[path];
        }

        Font font = LoadFont(path.data());
        if (!IsFontValid(font)) {
            font = GetFontDefault();
        }
        rl_fonts_storage[path] = font;

        return font;
    }

    bool is_debug_mode() {
        return is_debug;
    }

    float get_screen_width() {
        return (float)GetScreenWidth();
    }

    float get_screen_height() {
        return (float)GetScreenHeight();
    }

    KeyboardKey form_key(Custom::Key key) {
        switch (key) {
            case Custom::Key::A: {
                return KEY_A;
            }
            case Custom::Key::W: {
                return KEY_W;
            }
            case Custom::Key::S: {
                return KEY_S;
            }
            case Custom::Key::D: {
                return KEY_D;
            }
            case Custom::Key::B: {
                return KEY_B;
            }
            case Custom::Key::C: {
                return KEY_C;
            }
            case Custom::Key::V: {
                return KEY_V;
            }
            case Custom::Key::LEFT: {
                return KEY_LEFT;
            }
            case Custom::Key::RIGHT: {
                return KEY_RIGHT;
            }
            case Custom::Key::UP: {
                return KEY_UP;
            }
            case Custom::Key::DOWN: {
                return KEY_DOWN;
            }
            case Custom::Key::SPACE: {
                return KEY_SPACE;
            }
            default: {
                return KEY_NULL;
            }
        }
    }

    void init_keys_pressed_information() {
        for (int i = 0; i < static_cast<int>(Custom::Key::COUNT); i++) {
            Custom::Key current = static_cast<Custom::Key>(i);
            keys_detail[current] = {Key_Input_Type::IDLE, 0};
        }
    }

    void flush_uniform_clipping_data() {
        int point_count = static_cast<int>(clipping_points.size());
        SetShaderValue(clipping_shader, clipping_shader_point_count_localtion, &point_count, SHADER_UNIFORM_INT);
        SetShaderValueV(
            clipping_shader, clipping_shader_points_location, clipping_points.data(), SHADER_UNIFORM_VEC2, point_count
        );
    };

    void flush_uniform_force_color_texture_data() {
        int is_force = enable_force_color_texture ? 1 : 0;
        Custom::Color color = force_color_texture;
        Vector4 in_color = {(float)color.r / 255, (float)color.g / 255, (float)color.b / 255, 1.f};
        SetShaderValue(clipping_shader, shader_enable_force_color_location, &is_force, SHADER_UNIFORM_INT);
        SetShaderValue(clipping_shader, shader_force_color_location, &in_color, SHADER_UNIFORM_VEC4);
    };

    void reload_shader() {
        EndShaderMode();
        flush_uniform_clipping_data();
        flush_uniform_force_color_texture_data();
        BeginShaderMode(clipping_shader);
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
        reload_shader();
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
        reload_shader();
    }

    void process_start_force_color_to_texture(Custom::Color color) {
        enable_force_color_texture = true;
        force_color_texture = color;
        reload_shader();
    }

    void process_end_force_color_to_texture() {
        enable_force_color_texture = false;
        reload_shader();
    }

    void handle_touch_inputs() {
        /**
         * @Note: handle touch, from now work with just one touch, multi touches handle later
         * */
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            switch (touch_state) {
                case Touch_Type::BEGIN: {
                    touch_state = Touch_Type::MOVING;
                    break;
                }
                case Touch_Type::MOVING: {
                    touch_state = Touch_Type::MOVING;
                    break;
                }
                case Touch_Type::END: {
                    touch_state = Touch_Type::BEGIN;
                    break;
                }
                default: {
                    break;
                }
            }
            current_touched_position = GetMousePosition();
        } else {
            switch (touch_state) {
                case Touch_Type::BEGIN: {
                    std::cout << "Warning handle touch logic: some state fail please check logic!" << std::endl;
                    break;
                }
                case Touch_Type::MOVING: {
                    touch_state = Touch_Type::END;
                    break;
                }
                default: {
                    break;
                }
            }
        }
        Touch_System::get()->on_touched(
            {touch_state, {current_touched_position.x, get_screen_height() - current_touched_position.y}}
        );
    }

    void handle_key_inputs(float delta_time) {
        for (auto& [key, detail] : keys_detail) {
            if (detail.type == Key_Input_Type::RELEASE) {
                detail.type = Key_Input_Type::IDLE;
                detail.duration_pressed = 0;
            }
        }
        for (int i = 0; i < static_cast<int>(Custom::Key::COUNT); i++) {
            Custom::Key current = static_cast<Custom::Key>(i);
            KeyboardKey key = form_key(current);
            if (key == KeyboardKey::KEY_NULL) {
                std::cout << "Key inputs warning: Unsported key, please check {from_key} function again!" << std::endl;
                continue;
            }
            Key_Press_Detail& detail = keys_detail[current];
            if (IsKeyPressed(key)) {
                if (detail.type == Key_Input_Type::IDLE) {
                    detail.duration_pressed = 0;
                    detail.type = Key_Input_Type::PRESSED;
                }
            } else if (IsKeyReleased(key) || IsKeyUp(key)) {
                if (detail.type == Key_Input_Type::PRESSED || detail.type == Key_Input_Type::HOLDING) {
                    detail.duration_pressed = 0;
                    detail.type = Key_Input_Type::RELEASE;
                }
            } else {
                if (detail.type == Key_Input_Type::PRESSED) {
                    detail.type = Key_Input_Type::HOLDING;
                    detail.duration_pressed += delta_time;
                } else if (detail.type == Key_Input_Type::HOLDING) {
                    detail.duration_pressed += delta_time;
                }
            }
        }
        Key_Input_System::get()->handle_key_pressed(keys_detail);
    }

    void init_libs() {
        /**Init shader custom to clipping and force draw color*/
        clipping_shader = LoadShader(0, (Utils::get_root_path() + "shader/clipping.fs").data());
        if (!IsShaderValid(clipping_shader)) {
            throw std::runtime_error("Fail to load clipping shader please, try again!");
        }
        clipping_shader_points_location = GetShaderLocation(clipping_shader, "u_clip_points");
        clipping_shader_point_count_localtion = GetShaderLocation(clipping_shader, "u_point_count");
        shader_enable_force_color_location = GetShaderLocation(clipping_shader, "u_enable_force_color");
        shader_force_color_location = GetShaderLocation(clipping_shader, "u_force_color");

        /**Init key pressed event handler information*/
        init_keys_pressed_information();
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
        Custom::Draw_Attributes attributes,
        bool enable_force_color,
        Custom::Color force_color,
        Defined::LOAD_MODE load_mode
    ) {
        RayLib_Draw_Command command{
            RayLib_Draw_Type::IMAGE,
            attributes,
            new RayLib_Draw_Image_Resource(image_path, enable_force_color, force_color, load_mode)
        };
        rl_queue_commands.push(command);
    }

    void draw_text(std::string font_path, std::string text, int font_size, Custom::Draw_Attributes attributes) {
        RayLib_Draw_Command command{
            RayLib_Draw_Type::TEXT, attributes, new RayLib_Draw_Text_Resource(font_path, text, font_size)
        };
        rl_queue_commands.push(command);
    }

    void draw_line(
        float start_x,
        float start_y,
        float end_x,
        float end_y,
        int draw_index,
        Custom::Color color,
        float thin,
        bool is_dash
    ) {
        Custom::Draw_Attributes attr{};
        attr.draw_index = draw_index;
        RayLib_Draw_Command command{
            RayLib_Draw_Type::LINE,
            attr,
            new RayLib_Draw_Line_Resource(start_x, start_y, end_x, end_y, color, thin, is_dash)
        };
        rl_queue_commands.push(command);
    }

    void draw_rectangle(float width, float height, Custom::Draw_Attributes attributes) {
        RayLib_Draw_Command command{
            RayLib_Draw_Type::RECTANGLE, attributes, new RayLib_Draw_Rectangle_Resource(width, height)
        };
        rl_queue_commands.push(command);
    }

    Image_Info image_info(std::string path, Defined::LOAD_MODE load_mode) {
        RayLib_Texture_Info texture_info = load_raylib_texture(path, load_mode);
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

    void handle_frame(float delta_time) {
        handle_touch_inputs();
        handle_key_inputs(delta_time);
        Collision_System::get()->handle_collisions();
    }

    void draw_frame() {
        if (is_debug_mode()) {
            Label_Node* label = new Label_Node("", "", 10);
            label->set_anchor({0, 0});
            label->set_color({120, 120, 120});
            int distance_x = 50, distance_y = 50;
            Base_Node* background = Director::get()->get_running_scene_background();
            int draw_index = -1;
            Custom::Color color = {120, 120, 120};
            if (background != nullptr) {
                draw_index = background->get_draw_index();
                color = {180, 180, 180};
            }
            for (int i = 0; i <= get_screen_width() / 50; i++) {
                int x = i * distance_x;
                draw_line(x, 0, x, get_screen_height(), draw_index, color, 1.0f, true);
                std::string text = std::to_string(x);
                label->set_position({x + 5, 8});
                label->set_text(text);
                Libs_Wrapper::draw_text(
                    "", text, 10, {label->get_transform(), label->get_anchor(), draw_index, label->get_color()}
                );
            }
            for (int i = 0; i <= get_screen_height() / 50; i++) {
                int y = i * distance_y;
                draw_line(0, y, get_screen_width(), y, draw_index, color, 1.0f, true);
                std::string text = std::to_string(y);
                label->set_position({10, y});
                label->set_text(text);
                Libs_Wrapper::draw_text(
                    "", text, 10, {label->get_transform(), label->get_anchor(), draw_index, label->get_color()}
                );
            }
            delete (label);
        }

        trace_commands.clear();
        BeginDrawing();

        ClearBackground({230, 230, 230, 255});
        reload_shader();

        is_load_texture_async_in_frame = false;
        while (!rl_queue_commands.empty()) {
            RayLib_Draw_Command command = rl_queue_commands.top();
            Custom::Draw_Attributes attributes = command.attributes;
            Custom::Transform transform = attributes.transform;
            Custom::Anchor_Point anchor = attributes.anchor;
            float screen_height = get_screen_height();
            float x = transform.position.x;
            float y = screen_height - transform.position.y;

            // Save command to trace and debug when need
            if (is_debug && command.type != RayLib_Draw_Type::LINE) {
                std::map<std::string, std::string> extra_trace_information;
                if (command.resource != nullptr) {
                    extra_trace_information = command.resource->get_trace();
                }
                trace_commands.push_back({command.type, command.attributes, extra_trace_information});
            }

            switch (command.type) {
                case RayLib_Draw_Type::START_CLIPPING: {
                    process_start_clipping(command);
                    command.clean();
                    break;
                }
                case RayLib_Draw_Type::IMAGE: {
                    RayLib_Draw_Image_Resource* resource =
                        reinterpret_cast<RayLib_Draw_Image_Resource*>(command.resource);
                    RayLib_Texture_Info texture_info = load_raylib_texture(resource->image_path, resource->load_mode);
                    /**
                     * If texture is not loaded success we ignore it to draw!
                     */
                    if (texture_info.loaded_state != Defined::RESOURCE_LOADED_STATE::LOADED) {
                        command.clean();
                        break;
                    }
                    Texture2D texture = texture_info.data;
                    glm::vec2 rect_scale = attributes.rect_scale;
                    auto need_rounded_pixel = [&transform, &rect_scale]() {
                        return transform.rotation <= 3 && transform.scale.x / rect_scale.x <= 1.02f &&
                               transform.scale.y / rect_scale.y <= 1.02f;
                    };
                    Rectangle source = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
                    if (attributes.is_use_rect_texture) {
                        Custom::Rectangle_Area& rect = attributes.rect_texture;
                        /*@Hack: if draw area have size <= 0 we ignore it don't draw anything*/
                        if (rect.width <= 0 || rect.height <= 0) {
                            command.clean();
                            continue;
                        }
                        source = {rect.x, (float)texture.height - (rect.y + rect.height), rect.width, rect.height};
                        if (need_rounded_pixel()) {
                            source = {
                                std::ceil(source.x),
                                std::ceil(source.y),
                                std::ceil(source.width),
                                std::ceil(source.height)
                            };
                        }
                    }

                    float tex_width = source.width * std::abs(transform.scale.x);
                    float tex_height = source.height * std::abs(transform.scale.y);
                    source.width *= (transform.flipped.x ? -1.f : 1.f);
                    source.height *= (transform.flipped.y ? -1.f : 1.f);
                    Rectangle dest = {x, y, tex_width, tex_height};
                    Vector2 origin = {anchor.x * tex_width, (1 - anchor.y) * tex_height};
                    if (attributes.is_use_rect_texture) {
                        if (need_rounded_pixel()) {
                            dest = {
                                std::ceil(dest.x), std::ceil(dest.y), std::round(dest.width), std::round(dest.height)
                            };
                        }
                    }
                    if (resource->enable_force_color) {
                        process_start_force_color_to_texture(resource->force_color);
                    }
                    DrawTexturePro(
                        texture,
                        source,
                        dest,
                        origin,
                        transform.rotation,
                        {attributes.tint.r, attributes.tint.g, attributes.tint.b, transform.opacity}
                    );
                    if (resource->enable_force_color) {
                        process_end_force_color_to_texture();
                    }
                    command.clean();
                    break;
                }
                case RayLib_Draw_Type::RECTANGLE: {
                    RayLib_Draw_Rectangle_Resource* resource =
                        reinterpret_cast<RayLib_Draw_Rectangle_Resource*>(command.resource);
                    float width = resource->width * transform.scale.x;
                    float height = resource->height * transform.scale.y;
                    Rectangle source{x, y, width, height};
                    Vector2 origin = {anchor.x * width, (1 - anchor.y) * height};
                    DrawRectanglePro(
                        source,
                        origin,
                        transform.rotation,
                        {attributes.tint.r, attributes.tint.g, attributes.tint.b, transform.opacity}
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
                    Vector2 origin = {text_size.x * anchor.x, text_size.y * (1 - anchor.y)};
                    DrawTextPro(
                        font,
                        resource->text.data(),
                        position,
                        origin,
                        transform.rotation,
                        font_size,
                        1.0f,
                        {attributes.tint.r, attributes.tint.g, attributes.tint.b, transform.opacity}
                    );
                    command.clean();
                    break;
                }
                case RayLib_Draw_Type::LINE: {
                    RayLib_Draw_Line_Resource* resource =
                        reinterpret_cast<RayLib_Draw_Line_Resource*>(command.resource);
                    resource->start.y = screen_height - resource->start.y;
                    resource->end.y = screen_height - resource->end.y;
                    if (!resource->is_dashed) {
                        DrawLineEx(resource->start, resource->end, resource->thin, resource->color);
                    } else {
                        rlSetLineWidth(3.0f);
                        DrawLineDashed(resource->start, resource->end, 5, 5, resource->color);
                        rlSetLineWidth(1.0f);
                    }
                    command.clean();
                    break;
                }
                case RayLib_Draw_Type::END_CLIPPING: {
                    process_end_clipping(true);
                    command.clean();
                    break;
                }
                default: {
                    break;
                }
            }
            rl_queue_commands.pop();
        }

        EndShaderMode();
        EndDrawing();

        if (IsKeyPressed(KEY_K)) {
            if (is_debug) {
                for (auto& trace : trace_commands) {
                    std::cout << trace;
                }
            }
        }
        if (IsKeyPressed(KEY_B)) {
            is_debug = !is_debug;
            if (is_debug) {
                std::cout << "Open mode debug!" << std::endl;
            } else {
                std::cout << "Exit mode debug!" << std::endl;
            }
        }
    }

    void clear_libs() {
        for (const auto& [_, texture_info] : rl_textures_storage) {
            UnloadTexture(texture_info.data);
        }
        Collision_System::clear();
        Touch_System::clear();
        Key_Input_System::clear();
    }

}  // namespace Libs_Wrapper