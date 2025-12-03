cmake_minimum_required(VERSION 3.28)
project(EasyEngine VERSION 1.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# TODO：请记得在此处修改目录
set(SDL_DIR       "D:/CPPLibs/GIT/MSVC/SDL")
set(SDL_IMAGE_DIR "D:/CPPLibs/GIT/MSVC/SDL3_image")
set(SDL_TTF_DIR   "D:/CPPLibs/GIT/MSVC/SDL3_ttf")
set(SDL_MIXER_DIR "D:/CPPLibs/GIT/MSVC/SDL3_mixer")
set(SDL_GFX_DIR   "D:/CPPLibs/GIT/MSVC/SDL3_gfx")
set(FMT_DIR       "D:/CPPLibs/GIT/MSVC/FMT")

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

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}" CACHE PATH "" FORCE)
endif()
message(STATUS "CURRENT INSTALL PATH: ${CMAKE_INSTALL_PREFIX}")

add_library(EasyEngine STATIC
    src/Core.cpp
    src/Basic.cpp
    src/Algorithm.cpp
    src/Components.cpp
    src/Resources.cpp
    src/Scene.cpp
    src/Transition.cpp
)

add_subdirectory(test)

target_include_directories(EasyEngine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(EasyEngine PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    SDL3_mixer::SDL3_mixer
    SDL3_gfx::SDL3_gfx
    fmt::fmt
)

install(TARGETS EasyEngine
    EXPORT EasyEngineTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY src/
    DESTINATION include/EasyEngine
    FILES_MATCHING PATTERN "*.h"
)

install(DIRECTORY docs/
    DESTINATION docs
)

install(DIRECTORY examples/
        DESTINATION examples
)

install(FILES LICENSE
    DESTINATION License
)

install(EXPORT EasyEngineTargets
    FILE EasyEngineTargets.cmake
    NAMESPACE EasyEngine::
    DESTINATION lib/cmake/EasyEngine
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/EasyEngineConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/EasyEngineConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/EasyEngineConfig.cmake"
    INSTALL_DESTINATION lib/cmake/EasyEngine
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/EasyEngineConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/EasyEngineConfigVersion.cmake"
    DESTINATION lib/cmake/EasyEngine
)
