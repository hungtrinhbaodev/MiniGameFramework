#pragma once
#include <custom.h>

class Touch_Component {
public:
    Touch_Component();
    ~Touch_Component();

    bool is_swallow_touches() const;
    bool is_in_touch_area(glm::vec2 point) const;
    bool is_active() const;
    int get_touch_id() const;
    int get_priority() const;

    void set_touch_id(int touch_id);
    void set_touch_area(Custom::Transformed_Rectangle touch_area);
    void set_priority(int priority);
    void set_swallow_touches(bool swallow_touches);
    void set_active(bool active);

private:
    int touch_id;
    int priority = 0;
    bool swallow_touches = false;
    bool active = true;
    Custom::Transformed_Rectangle touch_area;
};