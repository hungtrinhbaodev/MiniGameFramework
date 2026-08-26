#pragma once
#include <string>

namespace Defined {
    enum LOAD_MODE { IMMEDIATE, ASYNC };
    enum RESOURCE_LOADED_STATE { UNLOAD, LOADED, LOADING };
    /**Component defined name */
    inline std::string COMPONENT_TOUCH_NAME = "COMPONENT_TOUCH";
    inline std::string COMPONENT_COLLISION_NAME = "COMPONENT_COLLISION";
    inline std::string COMPONENT_KEY_INPUT_NAME = "COMPONENT_KEY_INPUT";
    inline std::string COMPONENT_STATE_MACHINE_NAME = "CHARACTER_STATE_MACHINE";
    inline std::string SCENE_LABEL_FPS_SCHEDULER_KEY = "SCENE_LABEL_FPS_SCHEDULER_KEY";
    inline std::string KEY_SAVE_NODE_ORIGIN = "KEY_SAVE_NODE_ORIGIN";
    inline int ACTION_MOVE_CAMERA_TAG = -57;
    inline std::string KEY_LAYER_SCHEDULE_CAMERA_MOVE = "LAYER_SCHEDULE_CAMERA_MOVE";
}  // namespace Defined