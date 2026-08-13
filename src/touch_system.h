#pragma once

#include <custom.h>

#include <glm/glm.hpp>
#include <map>
#include <vector>

enum Touch_Type { BEGIN, MOVING, END, CANCCELED, INVALID };

struct Touch_Detail {
    Touch_Type type;
    glm::vec2 touch_position;
};

struct Touch_Information {
    int touch_id;
    int priority;
    Custom::Transformed_Rectangle touch_area;
    bool swallow_touches;
    /**Use this when want to disable touch */
    bool is_listening = false;
};

class Touch_System {
public:
    static int current_generated_id;
    static Touch_System* instance;
    static Touch_System* get();
    static void clear();

    Touch_System();
    ~Touch_System();

    Touch_Information request_touch_listenner();
    Touch_Detail query_touch_information(int touch_id);

    void request_update_touch(Touch_Information update_info);
    void on_touched(Touch_Detail touch_detail);
    void remove_touch_listener(int touch_id);

private:
    std::vector<Touch_Information> touches;
    std::map<int, Touch_Detail> handled_touches_information;
};