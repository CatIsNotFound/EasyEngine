#pragma once
#ifndef EASYENGINE_PREINCLUDE_H
#define EASYENGINE_PREINCLUDE_H

/**
 * @file preinclude.h
 * @brief 预定义文件
 *
 * 预编译头文件，集中包含第三方库和标准库依赖，简化其他文件的引用。
 *
 * @note 依赖 SDL3、SDL3_image、SDL3_mixer、SDL3_ttf 等第三方库
 * @copyright Copyright © 2025 CatIsNotFound
 *
 * @author CatIsNotFound
 * @version 0.1.1-beta
 */

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <fmt/core.h>

#include "libs/sdl3_gfx/SDL3_framerate.h"
#include "libs/sdl3_gfx/SDL3_gfxPrimitives.h"
#include "libs/sdl3_gfx/SDL3_gfxPrimitives_font.h"
#include "libs/sdl3_gfx/SDL3_imageFilter.h"
#include "libs/sdl3_gfx/SDL3_rotozoom.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <array>
#include <deque>
#include <map>
#include <tuple>
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <thread>
#include <mutex>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <chrono>

#endif //EASYENGINE_PREINCLUDE_H
