cmake_minimum_required(VERSION 3.16)
project(SdlGame)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED sdl2)
find_package(SDL2_image REQUIRED SDL2_image >= 2.0.0)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

set(SOURCE src/main.cpp src/LTexture.cpp Debugging/Debug.cpp src/LButton.cpp)
add_executable(SdlGame ${SOURCE})

if (WIN32)
target_link_libraries(SdlGame ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} -lmingw32 -lSDL2main -lSDL2 -mwindows)
endif(WIN32)

if (UNIX)
target_link_libraries(SdlGame ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} )
endif (UNIX)