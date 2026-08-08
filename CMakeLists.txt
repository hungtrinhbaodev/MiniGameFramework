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

target_include_directories(MiniGameFramework PRIVATE src)
target_link_libraries(MiniGameFramework PRIVATE raylib glm::glm)

find_program(PYTHON_EXEC
    NAMES python python3
)# Use your actual path here

if(PYTHON_EXEC)
    add_custom_target(sync_resources ALL
        COMMAND ${PYTHON_EXEC}
            "${CMAKE_SOURCE_DIR}/scripts/copy_res.py"
            "${CMAKE_SOURCE_DIR}/res"
            "$<TARGET_FILE_DIR:MiniGameFramework>/res"
        COMMAND ${PYTHON_EXEC}
            "${CMAKE_SOURCE_DIR}/scripts/copy_res.py"
            "${CMAKE_SOURCE_DIR}/shader"
            "$<TARGET_FILE_DIR:MiniGameFramework>/shader"
        COMMENT "Syncing res/ and shader/ to build output..."
    )
    add_dependencies(sync_resources MiniGameFramework)
else()
    message(WARNING "Python not found — res/ and shader/ will not be copied to build output.")
endif()
