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

Touch_Information Touch_System::request_touch_listenner() {
    Touch_Information touch{};
    touch.touch_id = ++current_generated_id;
    this->touches.push_back(touch);
    return touch;
}

void Touch_System::request_update_touch(Touch_Information update_info) {
    for (Touch_Information& touch : touches) {
        if (touch.touch_id == update_info.touch_id) {
            touch = update_info;
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
    std::sort(touches.begin(), touches.end(), [](const Touch_Information& a, const Touch_Information& b) {
        return a.priority > b.priority;
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

    /** Loop and handle touch by priority */
    for (int i = 0; i < touches.size(); i++) {
        Touch_Information& touch = touches[i];
        int touch_id = touch.touch_id;
        if (touch.touch_area.is_in_area(touch_detail.touch_position)) {
            if (handled_touches_information.find(touch_id) != handled_touches_information.end()) {
                handled_touches_information[touch_id] = touch_detail;
            } else {
                if (touch_detail.type != Touch_Type::END) {
                    handled_touches_information[touch_id] = touch_detail;
                }
            }
            if (touch.swallow_touches) {
                for (int j = i + 1; j < touches.size(); j++) {
                    Touch_Information& touch = touches[j];
                    int cancel_id = touch.touch_id;
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

void Touch_System::remove_touch_listener(int touch_id) {
    for (int i = 0; i < touches.size(); i++) {
        if (touches[i].touch_id == touch_id) {
            touches[i] = touches.back();
            touches.pop_back();
            i--;
        }
    }
    handled_touches_information.erase(touch_id);
}