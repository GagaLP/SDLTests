cmake_minimum_required(VERSION 3.7)
project(SDLTests)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
set(CMAKE_C_STANDARD 99)


set(SOURCE_FILES main.c)
add_executable(SDLTests ${SOURCE_FILES})
target_link_libraries(SDLTests ${SDL2_LIBRARY})



