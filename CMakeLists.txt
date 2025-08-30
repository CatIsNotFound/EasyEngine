cmake_minimum_required(VERSION 3.31)
project(EasyEngine)

set(CMAKE_CXX_STANDARD 23)
list(APPEND CMAKE_PREFIX_PATH "D:/CPPLibs/SDL_GIT")
list(APPEND CMAKE_PREFIX_PATH "D:/CPPLibs/SDL3_image")
list(APPEND CMAKE_PREFIX_PATH "D:/CPPLibs/SDL3_ttf")
list(APPEND CMAKE_PREFIX_PATH "D:/CPPLibs/SDL3_mixer")
list(APPEND CMAKE_PREFIX_PATH "D:/CPPLibs/FMT")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(fmt REQUIRED)

set(SDL3_GFX_LIB
        libs/sdl3_gfx/SDL3_framerate.c
        libs/sdl3_gfx/SDL3_framerate.h
        libs/sdl3_gfx/SDL3_gfxPrimitives.c
        libs/sdl3_gfx/SDL3_gfxPrimitives.h
        libs/sdl3_gfx/SDL3_gfxPrimitives_font.h
        libs/sdl3_gfx/SDL3_imageFilter.c
        libs/sdl3_gfx/SDL3_imageFilter.h
        libs/sdl3_gfx/SDL3_rotozoom.c
        libs/sdl3_gfx/SDL3_rotozoom.h
)

add_executable(EasyEngine main.cpp.old
        ${SDL3_GFX_LIB}
        src/Core.cpp
        src/Core.h
        src/preinclude.h
        src/Basic.cpp
        src/Basic.h
        src/Algorithm.h
        src/Algorithm.cpp
        src/Components.h
        src/Components.cpp
        src/Resources.cpp
        src/Resources.h
        main.cpp

)

target_link_libraries(EasyEngine PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        SDL3_mixer::SDL3_mixer
        fmt::fmt
)
