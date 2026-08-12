#include <touch_component.h>

Touch_Component::Touch_Component() {}

Touch_Component::~Touch_Component() {}

bool Touch_Component::is_swallow_touches() const {
    return this->swallow_touches;
}

bool Touch_Component::is_in_touch_area(glm::vec2 point) const {
    return this->touch_area.is_in_area(point);
}

bool Touch_Component::is_active() const {
    return this->active;
}

int Touch_Component::get_touch_id() const {
    return this->touch_id;
}

int Touch_Component::get_priority() const {
    return this->priority;
}

void Touch_Component::set_touch_id(int touch_id) {
    this->touch_id = touch_id;
}

void Touch_Component::set_touch_area(Custom::Transformed_Rectangle touch_area) {
    this->touch_area = touch_area;
}

void Touch_Component::set_priority(int priority) {
    this->priority = priority;
}

void Touch_Component::set_swallow_touches(bool swallow_touches) {
    this->swallow_touches = swallow_touches;
}

void Touch_Component::set_active(bool active) {
    this->active = active;
}