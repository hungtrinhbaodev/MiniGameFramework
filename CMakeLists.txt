cmake_minimum_required(VERSION 3.20)
project(GameEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RAYLIB_DIR "C:/raylib/raylib/")
set(GLM_DIR "D:/Dev/glm/")
add_subdirectory(${RAYLIB_DIR} ${CMAKE_BINARY_DIR}/raylib_build)
add_subdirectory(${GLM_DIR} ${CMAKE_BINARY_DIR}/glm_build)
target_compile_definitions(raylib PUBLIC SUPPORT_FILEFORMAT_JPG)

file(GLOB_RECURSE SOURCES "main.cpp" "src/*.cpp")
add_executable(MiniGameFramework ${SOURCES})

target_include_directories(MiniGameFramework PRIVATE src)
target_link_libraries(MiniGameFramework PRIVATE raylib glm::glm)
