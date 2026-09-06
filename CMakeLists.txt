cmake_minimum_required(VERSION 3.20)
project(GameEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RAYLIB_DIR "D:/Dev/raylib/raylib/")
set(GLM_DIR "D:/Dev/glm/")
add_subdirectory(${RAYLIB_DIR} ${CMAKE_BINARY_DIR}/raylib_build)
add_subdirectory(${GLM_DIR} ${CMAKE_BINARY_DIR}/glm_build)
target_compile_definitions(raylib PUBLIC SUPPORT_FILEFORMAT_JPG)

file(GLOB_RECURSE SOURCES "main.cpp" "src/*.cpp")
add_executable(MiniGameFramework ${SOURCES})

target_include_directories(MiniGameFramework PRIVATE src vendor)
target_link_libraries(MiniGameFramework PRIVATE raylib glm::glm)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND MSVC)
    
    # 1. Ép CMake chuyển toàn bộ cấu hình Runtime từ Debug (/MDd) sang Release (/MD) toàn cục
    # Điều này giúp ngăn chặn việc liên kết nhầm với msvcprtd.lib (gây ra lỗi _malloc_dbg)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    
    # Kích hoạt macro debug để giữ nguyên các tính năng kiểm tra lỗi lập trình
    add_compile_definitions(_DEBUG)

    # 2. Thêm cờ biên dịch ASan cho toàn bộ dự án
    add_compile_options(/fsanitize=address /Zi)
    
    # 3. Thêm thư viện liên kết ASan tương ứng cho Linker (cho kiến trúc x64)
    add_link_options(
        "clang_rt.asan_dynamic-x86_64.lib"
        "clang_rt.asan_dynamic_runtime_thunk-x86_64.lib"
    )
    
endif()

find_program(PYTHON_EXEC
    NAMES python python3
)

if(PYTHON_EXEC)
    add_custom_target(sync_resource ALL
        COMMAND ${PYTHON_EXEC}
            "${CMAKE_SOURCE_DIR}/scripts/copy_res.py"
            "${CMAKE_SOURCE_DIR}/res"
            "$<TARGET_FILE_DIR:MiniGameFramework>/res"
        COMMAND ${PYTHON_EXEC}
            "${CMAKE_SOURCE_DIR}/scripts/copy_res.py"
            "${CMAKE_SOURCE_DIR}/shader"
            "$<TARGET_FILE_DIR:MiniGameFramework>/shader"
        COMMENT "Syncing res and shader/ to build output..."
    )
    add_dependencies(sync_resource MiniGameFramework)
else()
    message(WARNING "Python not found — res/ and shader/ will not be copied to build output.")
endif()
