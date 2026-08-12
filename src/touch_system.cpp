#include <touch_system.h>

#include <algorithm>

int Touch_System::current_generated_id = 0;

Touch_System* Touch_System::instance = nullptr;

Touch_System* Touch_System::get() {
    if (instance == nullptr) {
        instance = new Touch_System();
    }
    return instance;
}

void Touch_System::clear() {
    if (instance != nullptr) {
        delete (instance);
    }
}

Touch_System::Touch_System() {}

Touch_System::~Touch_System() {}

int Touch_System::request_touch_listenner() {
    Touch_Component touch{};
    touch.set_touch_id(++current_generated_id);
    this->touches.push_back(touch);
    return touch.get_touch_id();
}

void Touch_System::request_update_touch(int touch_id, Touch_Information update_info) {
    for (Touch_Component& touch : touches) {
        if (touch.get_touch_id() == touch_id) {
            touch.set_priority(update_info.priority);
            touch.set_touch_area(update_info.touch_area);
            touch.set_swallow_touches(update_info.swallow_touches);
            touch.set_active(update_info.is_listening);
        }
    }
}

Touch_Detail Touch_System::query_touch_information(int touch_id) {
    if (handled_touches_information.find(touch_id) == handled_touches_information.end()) {
        return {Touch_Type::INVALID};
    }
    return handled_touches_information[touch_id];
}

void Touch_System::on_touched(Touch_Detail touch_detail) {
    std::sort(touches.begin(), touches.end(), [](const Touch_Component& a, const Touch_Component& b) {
        return a.get_priority() > b.get_priority();
    });

    /**Canceled and End touch state just contain in one frame after that we removed it! */
    std::vector<int> removed_last_canceled_touches;
    for (auto& [id, touch_detail] : handled_touches_information) {
        if (touch_detail.type == Touch_Type::CANCCELED || touch_detail.type == Touch_Type::END) {
            removed_last_canceled_touches.push_back(id);
        }
    }
    for (int id : removed_last_canceled_touches) {
        handled_touches_information.erase(id);
    }

    for (int i = 0; i < touches.size(); i++) {
        Touch_Component& touch = touches[i];
        int touch_id = touch.get_touch_id();
        if (touch.is_in_touch_area(touch_detail.touch_position)) {
            handled_touches_information[touch_id] = touch_detail;
            if (touch.is_swallow_touches()) {
                for (int j = i + 1; j < touches.size(); j++) {
                    Touch_Component& touch = touches[j];
                    int cancel_id = touch.get_touch_id();
                    if (handled_touches_information.find(cancel_id) != handled_touches_information.end()) {
                        handled_touches_information[cancel_id].type = Touch_Type::CANCCELED;
                    }
                }
                break;
            }
        } else {
            if (handled_touches_information.find(touch_id) != handled_touches_information.end()) {
                handled_touches_information[touch_id].type = Touch_Type::CANCCELED;
            }
        }
    }
}