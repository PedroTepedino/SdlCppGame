cmake_minimum_required(VERSION 3.19)
project(SdlGame)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED sdl2)
find_package(SDL2_image REQUIRED SDL2_image >= 2.0.0)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2IMAGE_INCLUDE_DIR})

add_executable(SdlGame src/main.cpp)
target_link_libraries(SdlGame ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY} -lmingw32 -lSDL2main -lSDL2 -mwindows)