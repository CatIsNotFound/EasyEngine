cmake_minimum_required(VERSION 3.28)
project(MyGame)

set(CMAKE_CXX_STANDARD 23)
set(SDL_DIR        "/path/to/SDL")
set(SDL_IMAGE_DIR  "/path/to/SDL3_image")
set(SDL_TTF_DIR    "/path/to/SDL3_ttf")
set(SDL_MIXER_DIR  "/path/to/SDL3_mixer")
set(SDL_GFX_DIR    "/path/to/SDL3_gfx")
set(FMT_DIR        "/path/to/FMT")
set(EASYENGINE_DIR "/path/to/EasyEngine")

list(APPEND CMAKE_PREFIX_PATH ${SDL_DIR})
list(APPEND CMAKE_PREFIX_PATH ${SDL_IMAGE_DIR})
list(APPEND CMAKE_PREFIX_PATH ${SDL_TTF_DIR})
list(APPEND CMAKE_PREFIX_PATH ${SDL_MIXER_DIR})
list(APPEND CMAKE_PREFIX_PATH ${SDL_GFX_DIR})
list(APPEND CMAKE_PREFIX_PATH ${FMT_DIR})
list(APPEND CMAKE_PREFIX_PATH ${EASYENGINE_DIR})

find_package(EasyEngine REQUIRED)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        EasyEngine::EasyEngine
)


set(POST_BUILD_COMMANDS
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