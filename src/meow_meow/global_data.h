#pragma once
#include <meow_meow/config/config.h>

namespace Meow_Meow {
    class Global_Data {
    public:
        static Global_Data* get();
        static void clear();

        Global_Data();
        ~Global_Data();

        const Config& get_config();

    private:
        static Global_Data* instance;
        Config config;
    };
}  // namespace Meow_Meow