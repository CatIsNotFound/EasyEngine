cmake_minimum_required(VERSION 3.31)
project(EasyEngine VERSION 0.1.1)

set(CMAKE_CXX_STANDARD 23)

# TODO：请记得在此处修改目录
set(SDL_DIR       "D:/CPPLibs/SDL")
set(SDL_IMAGE_DIR "D:/CPPLibs/SDL3_image")
set(SDL_TTF_DIR   "D:/CPPLibs/SDL3_ttf")
set(SDL_MIXER_DIR "D:/CPPLibs/SDL3_mixer")
set(SDL_GFX_DIR   "D:/CPPLibs/SDL3_gfx")
set(FMT_DIR       "D:/CPPLibs/FMT")


list(APPEND CMAKE_PREFIX_PATH ${SDL_DIR})
list(APPEND CMAKE_PREFIX_PATH ${SDL_IMAGE_DIR})
list(APPEND CMAKE_PREFIX_PATH ${SDL_TTF_DIR})
list(APPEND CMAKE_PREFIX_PATH ${SDL_MIXER_DIR})
list(APPEND CMAKE_PREFIX_PATH ${SDL_GFX_DIR})
list(APPEND CMAKE_PREFIX_PATH ${FMT_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(SDL3_gfx REQUIRED)
find_package(fmt REQUIRED)
#find_package(pugixml REQUIRED)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}" CACHE PATH "" FORCE)
endif()
message(STATUS "CURRENT INSTALL PATH: ${CMAKE_INSTALL_PREFIX}")


add_executable(EasyEngine
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
        src/Scene.cpp
        src/Scene.h
        src/Transition.cpp
        src/Transition.h
        main.cpp
)

#include_directories(${FFMPEG_DIR}/include)
#link_directories(${FFMPEG_DIR}/lib)

target_link_libraries(EasyEngine PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        SDL3_mixer::SDL3_mixer
        SDL3_gfx::SDL3_gfx
        fmt::fmt
)

set(POST_BUILD_COMMANDS
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL_DIR}/bin ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL_MIXER_DIR}/bin ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL_GFX_DIR}/bin ${CMAKE_BINARY_DIR}
)

if (WIN32)
    if (MINGW)
        list(APPEND POST_BUILD_COMMANDS
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL_IMAGE_DIR}/x86_64-w64-mingw32/bin ${CMAKE_BINARY_DIR}
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL_TTF_DIR}/x86_64-w64-mingw32/bin ${CMAKE_BINARY_DIR}
        )
    else ()
        list(APPEND POST_BUILD_COMMANDS
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL_IMAGE_DIR}/bin ${CMAKE_BINARY_DIR}
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL_TTF_DIR}/bin ${CMAKE_BINARY_DIR}
        )
    endif()

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            ${POST_BUILD_COMMANDS}
    )
endif()