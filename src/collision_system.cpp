#include <collision_system.h>

#include <algorithm>

Collision_System* Collision_System::instance = nullptr;

int Collision_System::current_generated_id = 0;

Collision_System* Collision_System::get() {
    if (instance == nullptr) {
        instance = new Collision_System();
    }
    return instance;
}

void Collision_System::clear() {
    delete (instance);
}

Collision_System::Collision_System() {}

Collision_System::~Collision_System() {}

int Collision_System::request_collision() {
    Collision_Information collision{};
    collision.collision_id = ++current_generated_id;
    collisions.push_back(collision);
    collision_lookup[collision.collision_id] = collision;
    return collision.collision_id;
}

void Collision_System::request_update_information(Collision_Information collision) {
    for (Collision_Information& current_collision : collisions) {
        if (current_collision.collision_id == collision.collision_id) {
            current_collision = collision;
            collision_lookup[collision.collision_id] = collision;
        }
    }
}

std::vector<Collision_Information> Collision_System::query_collisions(int collision_id) {
    if (handled_collisions.find(collision_id) == handled_collisions.end()) {
        return {};
    }
    return handled_collisions[collision_id];
}

void Collision_System::remove_collision(int collision_id) {
    for (int i = 0; i < collisions.size(); i++) {
        if (collisions[i].collision_id == collision_id) {
            collisions[i] = collisions.back();
            collisions.pop_back();
            i--;
        }
    }
    handled_collisions.erase(collision_id);
}

void Collision_System::handle_collisions() {
    /**
     * TODO: implement better algorithm in future to handle collision
     *  like quad tree, from now just use n^2 check bonding first
     */
    handled_collisions.clear();
    for (int i = 0; i < collisions.size(); i++) {
        handled_collisions[collisions[i].collision_id] = {};
    }

    int size = collisions.size();
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            Collision_Information& one = collisions[i];
            Collision_Information& two = collisions[j];
            if (one.track_layer != two.track_layer) {
                continue;
            }
            if (one.collision_area.is_collision_with(two.collision_area)) {
                handled_collisions[one.collision_id].push_back(two);
                handled_collisions[two.collision_id].push_back(one);
            }
        }
    }
    for (auto& [collision_id, collisioneds] : handled_collisions) {
        Collision_Information& collision = collision_lookup[collision_id];
        std::sort(
            collisioneds.begin(),
            collisioneds.end(),
            /**
             * @Note: sometime the collision will be have many other collision with it, so we
             * sort it by nearest center to handle phase logic easier!
             */
            [&collision](const Collision_Information& a, const Collision_Information& b) {
                return a.collision_area.distance_from(collision.collision_area) >
                       b.collision_area.distance_from(collision.collision_area);
            }
        );
    }
}