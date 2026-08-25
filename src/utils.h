#pragma once
#include <collision_component.h>
#include <node.h>

#include <glm/glm.hpp>
#include <string>

namespace Utils {
    long now();

    std::string vec2_to_string(glm::vec2 vec);

    void save_transform_origin(Base_Node* node);

    void clean_transform_origin(Base_Node* node);

    Custom::Transform get_transform_origin(Base_Node* node);

    void reset_to_origin(Base_Node* node);

    std::string get_root_path();

    template <typename T>
    inline T* get_component(Node* node, std::string component_name) {
        Base_Component* component = node->get_component_by_name(component_name);
        if (component == nullptr)
            return nullptr;
        return reinterpret_cast<T*>(component);
    }

    template <typename T>
    inline T* get_collision_owner_data(Collision_Component* collision) {
        void* data = collision->get_owner_data();
        if (data == nullptr)
            return nullptr;
        return reinterpret_cast<T*>(data);
    }
}  // namespace Utils