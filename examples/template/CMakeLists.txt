cmake_minimum_required(VERSION 3.31)
project(MyGame)

set(CMAKE_CXX_STANDARD 23)
set(SDL_DIR        "D:/CPPLibs/SDL")
set(SDL_IMAGE_DIR  "D:/CPPLibs/SDL3_image")
set(SDL_TTF_DIR    "D:/CPPLibs/SDL3_ttf")
set(SDL_MIXER_DIR  "D:/CPPLibs/SDL3_mixer")
set(SDL_GFX_DIR    "D:/CPPLibs/SDL3_gfx")
set(FMT_DIR        "D:/CPPLibs/FMT")
set(EASYENGINE_DIR "D:/EasyEngine")

list(APPEND CMAKE_PREFIX_PATH ${SDL_DIR})
list(APPEND CMAKE_PREFIX_PATH ${SDL_IMAGE_DIR})
list(APPEND CMAKE_PREFIX_PATH ${SDL_TTF_DIR})
list(APPEND CMAKE_PREFIX_PATH ${SDL_MIXER_DIR})
list(APPEND CMAKE_PREFIX_PATH ${SDL_GFX_DIR})
list(APPEND CMAKE_PREFIX_PATH ${FMT_DIR})
list(APPEND CMAKE_PREFIX_PATH ${EASYENGINE_DIR})

find_package(EasyEngine REQUIRED)

add_executable(MyGame main.cpp)

target_link_libraries(MyGame PRIVATE
        EasyEngine::EasyEngine
)

if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL_DIR}/bin ${CMAKE_BINARY_DIR}
        if (MINGW)
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL_IMAGE_DIR}/bin ${CMAKE_BINARY_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL_TTF_DIR}/bin ${CMAKE_BINARY_DIR}
        else()
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL_IMAGE_DIR}/bin/x86_64-w64-mingw32 ${CMAKE_BINARY_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL_TTF_DIR}/bin/x86_64-w64-mingw32 ${CMAKE_BINARY_DIR}
        endif()
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL_MIXER_DIR}/bin ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL_GFX_DIR}/bin   ${CMAKE_BINARY_DIR}
    )
endif()