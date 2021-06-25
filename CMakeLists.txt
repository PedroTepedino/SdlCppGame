cmake_minimum_required(VERSION 3.19)
project(BigMamma)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(BigMamma src/main.cpp)
target_link_libraries(BigMamma ${SDL2_LIBRARY} -lmingw32 -lSDL2main -lSDL2 -mwindows)