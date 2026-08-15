#pragma once
#include <base_node.h>

#include <glm/glm.hpp>
#include <string>

namespace Utils {
    long now();

    std::string vec2_to_string(glm::vec2 vec);

    void save_transform_origin(Base_Node* node);

    Custom::Transform get_transform_origin(Base_Node* node);

    void clean_transform_origin(Base_Node* node);

    std::string get_root_path();
}  // namespace Utils