#pragma once
#include <base_node.h>
#include <custom.h>

#include <map>
#include <vector>

struct Collision_Information {
    int collision_id = -1;
    int tag = -1;
    /**Note: only objects with same track layer can be collision with each other!*/
    int track_layer = -1;
    Base_Node* onwner = nullptr;
    void* owner_data = nullptr;
    Custom::Transformed_Rectangle collision_area;
};

class Collision_System {
public:
    static Collision_System* get();
    static void clear();
    Collision_System();
    ~Collision_System();

    int request_collision();
    void request_update_information(Collision_Information collision);
    std::vector<Collision_Information> query_collisions(int collision_id);
    void remove_collision(int collision_id);

    void handle_collisions();

private:
    static Collision_System* instance;
    static int current_generated_id;

    std::vector<Collision_Information> collisions;
    std::map<int, Collision_Information> collision_lookup;
    std::map<int, std::vector<Collision_Information>> handled_collisions;
};