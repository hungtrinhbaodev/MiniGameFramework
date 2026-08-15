#include <meow_meow/global_data.h>

namespace Meow_Meow {
    Global_Data* Global_Data::instance = nullptr;

    Global_Data* Global_Data::get() {
        if (instance == nullptr) {
            instance = new Global_Data();
        }
        return instance;
    }

    Global_Data::Global_Data() {}

    Global_Data::~Global_Data() {}

    const Config& Global_Data::get_config() {
        return config;
    }

    void Global_Data::clear() {
        if (instance != nullptr) {
            delete (instance);
        }
    }

}  // namespace Meow_Meow