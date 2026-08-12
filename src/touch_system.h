#pragma once
#include <touch_component.h>

#include <glm/glm.hpp>
#include <map>
#include <vector>

enum Touch_Type { BEGIN, MOVING, END, CANCCELED, INVALID };

struct Touch_Detail {
    Touch_Type type;
    glm::vec2 touch_position;
};

struct Touch_Information {
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

    int request_touch_listenner();
    void request_update_touch(int touch_id, Touch_Information update_info);
    Touch_Detail query_touch_information(int touch_id);
    void on_touched(Touch_Detail touch_detail);

private:
    std::vector<Touch_Component> touches;
    std::map<int, Touch_Detail> handled_touches_information;
};