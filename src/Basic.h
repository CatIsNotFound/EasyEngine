
#pragma once
#ifndef EASYENGINE_BASIC_H
#define EASYENGINE_BASIC_H

/**
 * @file Basic.h
 * @brief 基本模块
 *
 * 定义引擎基础数据结构和图形实体，是整个引擎的基础数据层。
 *
 * @copyright Copyright © 2025 CatIsNotFound
 * @author CatIsNotFound
 */

#include "preinclude.h"
using SColor = SDL_Color;

/**
 * @namespace EasyEngine
 * @brief 引擎
 *
 * 几乎包含所有引擎相关的类和命名空间。
 * 通过使用 `using namespace EasyEngine;` 可直接使用引擎里的一切东西
 */
namespace EasyEngine {
    /**
     * @namespace StdColor
     * @brief 预定义标准颜色（81 色）
     * @see Basic.h
     */
    namespace StdColor {
        constexpr SColor Black = {0, 0, 0, 255};
        constexpr SColor White = {255, 255, 255, 255};
        constexpr SColor Red = {255, 0, 0, 255};
        constexpr SColor Green = {0, 255, 0, 255};
        constexpr SColor Blue = {0, 0, 255, 255};
        constexpr SColor Cyan = {0, 255, 255, 255};
        constexpr SColor Magenta = {255, 0, 255, 255};
        constexpr SColor Yellow = {255, 255, 0, 255};

        constexpr SColor LightGray = {200, 200, 200, 255};
        constexpr SColor DarkGray = {64, 64, 64, 255};
        constexpr SColor Gray = {128, 128, 128, 255};
        constexpr SColor Silver = {192, 192, 192, 255};
        constexpr SColor Gainsboro = {220, 220, 220, 255};
        constexpr SColor WhiteSmoke = {245, 245, 245, 255};
        constexpr SColor DimGray = {105, 105, 105, 255};
        constexpr SColor SlateGray = {112, 128, 144, 255};
        constexpr SColor DarkSlateGray = {47, 79, 79, 255};

        constexpr SColor Crimson = {220, 20, 60, 255};
        constexpr SColor Maroon = {128, 0, 0, 255};
        constexpr SColor DarkRed = {139, 0, 0, 255};
        constexpr SColor Tomato = {255, 99, 71, 255};
        constexpr SColor Coral = {255, 127, 80, 255};
        constexpr SColor Salmon = {250, 128, 114, 255};
        constexpr SColor LightCoral = {240, 128, 128, 255};
        constexpr SColor IndianRed = {205, 92, 92, 255};
        constexpr SColor FireBrick = {178, 34, 34, 255};

        constexpr SColor Orange = {255, 165, 0, 255};
        constexpr SColor DarkOrange = {255, 140, 0, 255};
        constexpr SColor OrangeRed = {255, 69, 0, 255};
        constexpr SColor PeachPuff = {255, 218, 185, 255};
        constexpr SColor NavajoWhite = {255, 222, 173, 255};

        constexpr SColor Gold = {255, 215, 0, 255};
        constexpr SColor LightYellow = {255, 255, 224, 255};
        constexpr SColor LemonChiffon = {255, 250, 205, 255};
        constexpr SColor PaleGoldenrod = {238, 232, 170, 255};
        constexpr SColor Khaki = {240, 230, 140, 255};
        constexpr SColor DarkKhaki = {189, 183, 107, 255};

        constexpr SColor Lime = {0, 255, 0, 255};
        constexpr SColor ForestGreen = {34, 139, 34, 255};
        constexpr SColor SeaGreen = {46, 139, 87, 255};
        constexpr SColor Olive = {128, 128, 0, 255};
        constexpr SColor DarkOliveGreen = {85, 107, 47, 255};
        constexpr SColor LimeGreen = {50, 205, 50, 255};
        constexpr SColor SpringGreen = {0, 255, 127, 255};
        constexpr SColor LightGreen = {144, 238, 144, 255};
        constexpr SColor PaleGreen = {152, 251, 152, 255};
        constexpr SColor MediumSpringGreen = {0, 250, 154, 255};

        constexpr SColor Navy = {0, 0, 128, 255};
        constexpr SColor DarkBlue = {0, 0, 139, 255};
        constexpr SColor MediumBlue = {0, 0, 205, 255};
        constexpr SColor RoyalBlue = {65, 105, 225, 255};
        constexpr SColor SteelBlue = {70, 130, 180, 255};
        constexpr SColor LightBlue = {173, 216, 230, 255};
        constexpr SColor SkyBlue = {135, 206, 235, 255};
        constexpr SColor PowderBlue = {176, 224, 230, 255};
        constexpr SColor LightCyan = {224, 255, 255, 255};
        constexpr SColor DeepSkyBlue = {0, 191, 255, 255};
        constexpr SColor DodgerBlue = {30, 144, 255, 255};

        constexpr SColor Purple = {128, 0, 128, 255};
        constexpr SColor DarkPurple = {102, 51, 153, 255};
        constexpr SColor Fuchsia = {255, 0, 255, 255};
        constexpr SColor Violet = {238, 130, 238, 255};
        constexpr SColor Plum = {221, 160, 221, 255};
        constexpr SColor Orchid = {218, 112, 214, 255};
        constexpr SColor MediumOrchid = {186, 85, 211, 255};
        constexpr SColor MediumPurple = {147, 112, 219, 255};
        constexpr SColor Indigo = {75, 0, 130, 255};

        constexpr SColor Brown = {165, 42, 42, 255};
        constexpr SColor SaddleBrown = {139, 69, 19, 255};
        constexpr SColor Sienna = {160, 82, 45, 255};
        constexpr SColor Chocolate = {210, 105, 30, 255};
        constexpr SColor Peru = {205, 133, 63, 255};
        constexpr SColor SandyBrown = {244, 164, 96, 255};
        constexpr SColor BurlyWood = {222, 184, 135, 255};
        constexpr SColor Tan = {210, 180, 140, 255};

        constexpr SColor Pink = {255, 192, 203, 255};
        constexpr SColor LightPink = {255, 182, 193, 255};
        constexpr SColor HotPink = {255, 105, 180, 255};
        constexpr SColor DeepPink = {255, 20, 147, 255};
        constexpr SColor PaleVioletRed = {219, 112, 147, 255};
        constexpr SColor MediumVioletRed = {199, 21, 133, 255};
    }

    /**
     * @struct Geometry
     * @brief 位置、大小
     */
    struct Geometry {
        int x, y, width, height;
        Geometry();
        Geometry(int x, int y, int width, int height);
        void setGeometry(int x, int y, int width, int height);
        void setPosition(int x, int y);
        void resize(int width, int height);
    };

    /**
     * @struct Vector2
     * @brief 向量坐标
     *
     * 用于指定某对象的位置
     */
    struct Vector2 {
        float x, y;
        explicit Vector2() : x(0.0f), y(0.0f) {}
        Vector2(float x, float y);
        /**
         * @brief 重新设置新的向量
         * @param x
         * @param y
         */
        void reset(float x, float y);
        Vector2 operator+(const Vector2& v) const { return {this->x + v.x, this->y + v.y}; }
        Vector2 operator-(const Vector2& v) const { return {this->x - v.x, this->y - v.y}; }
        Vector2 operator*(const Vector2& v) const { return {this->x * v.x, this->y * v.y}; }
        Vector2 operator/(const Vector2& v) const { return {this->x / v.x, this->y / v.y}; }
        void operator+=(const Vector2& v) { this->x += v.x; this->y += v.y; }
        void operator-=(const Vector2& v) { this->x -= v.x; this->y -= v.y; }
        void operator*=(const Vector2& v) { this->x *= v.x; this->y *= v.y; }
        void operator/=(const Vector2& v) { this->x /= v.x; this->y /= v.y; }
        bool operator==(const Vector2& v) const {
            return ((this->x != v.x) ? false : ((this->y != v.y) ? false : true));
        }
        bool operator!=(const Vector2& v) const {
            return ((this->x != v.x) ? true : ((this->y != v.y) ? true : false));
        }
    };

    /**
     * @struct Size
     * @brief 尺寸、大小
     *
     * 用于调整对象的尺寸、大小
     */
    struct Size {
        float width, height;
        explicit Size() : width(0.0f), height(0.0f) {}
        Size(float width, float height);
        /**
         * @brief 重新设置新的尺寸
         * @param width
         * @param height
         */
        void reset(float width, float height);
        Size operator+(const Size& s) const { return {this->width + s.width, this->height + s.height}; }
        Size operator-(const Size& s) const { return {this->width - s.width, this->height - s.height}; }
        Size operator*(const Size& s) const { return {this->width * s.width, this->height * s.height}; }
        Size operator/(const Size& s) const { return {this->width / s.width, this->height / s.height}; }
        void operator+=(const Size& s) { this->width += s.width; this->height += s.height; }
        void operator-=(const Size& s) { this->width -= s.width; this->height -= s.height; }
        void operator*=(const Size& s) { this->width *= s.width; this->height *= s.height; }
        void operator/=(const Size& s) { this->width /= s.width; this->height /= s.height; }
        bool operator==(const Size& s) const {
            return (this->width != s.width) ? false : ((this->height != this->height) ? false : true);
        }
        bool operator!=(const Size& s) const {
            return (this->width != s.width) ? true : ((this->height != this->height) ? true : false);
        }
    };
    /**
     * @namespace Graphics
     * @brief 基本图形
     *
     * 包含所有基本图形，如：点、线段、矩形、椭圆等基本图形。
     */
    namespace Graphics {
        /**
         * @struct Point
         * @brief 圆点
         */
        struct Point {
            Vector2 pos;
            SColor color;
            Point() : pos(0, 0), color(StdColor::Black) {}
            explicit Point(Vector2 pos, SColor color = StdColor::Black) : pos(pos), color(color) {}
            Point(float x, float y, SColor color = StdColor::Black) : pos(x, y), color(color) {}
            Point(float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
                : pos(x, y), color(r, g, b, a) {}
            Point(Vector2 pos, const std::string& hex_color = "#000000");
            Point(float x, float y, const std::string& hex_color = "#000000");
        };
        /**
         * @struct Line
         * @brief 线段
         */
        struct Line {
            Vector2 start;
            Vector2 end;
            uint8_t width;
            SColor color;
            Line() : start(0, 0), end(0, 0), width(1), color(StdColor::Black) {}
            Line(float x1, float y1, float x2, float y2, uint8_t width = 1, SColor color = StdColor::Black)
                : start(x1, y1), end(x2, y2), width(width), color(color) {}
            Line(Vector2 start, Vector2 end, uint8_t width = 1, SColor color = StdColor::Black)
                : start(start), end(end), width(width), color(color) {}
            Line(float x1, float y1, float x2, float y2, uint8_t width = 1, uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255)
                : start(x1, y1), end(x2, y2), width(width), color(r, g, b, a) {}
            Line(float x1, float y1, float x2, float y2, uint8_t width, const std::string &hex_color = "#000000");
            Line(Vector2 start, Vector2 end, uint8_t width, const std::string &hex_color = "#000000");
        };
        /**
         * @struct Rectangle
         * @brief 矩形
         */
        struct Rectangle {
            Vector2 pos;
            Size size;
            bool bordered_mode;
            bool filled_mode;
            SColor fore_color;
            SColor back_color;
            Rectangle() : pos(0, 0), size(0, 0), filled_mode(false), bordered_mode(true),
                fore_color(StdColor::Black), back_color(StdColor::Black) {}
            Rectangle(Vector2 pos, Size size, SColor foreground = StdColor::Black, bool bordered_mode = true,
                      bool filled_mode = false, SColor background = StdColor::Black)
                      : pos(pos), size(size), fore_color(foreground), bordered_mode(bordered_mode),
                        filled_mode(filled_mode), back_color(background) {}
            explicit Rectangle(float x, float y, float width, float height, SColor foreground = StdColor::Black,
                      bool bordered_mode = true, bool filled_mode = false, SColor background = StdColor::Black)
                      : pos(x, y), size(width, height), fore_color(foreground), bordered_mode(bordered_mode),
                         filled_mode(filled_mode), back_color(background) {}
            Rectangle(float x, float y, uint32_t width, uint32_t height,
                      uint8_t fore_r, uint8_t fore_g, uint8_t fore_b, uint8_t fore_a = 255,
                      bool bordered_mode = true, bool filled_mode = false,
                      uint8_t back_r = 0, uint8_t back_g = 0, uint8_t back_b = 0, uint8_t back_a = 255)
                      : pos(x, y), size(width, height), fore_color(fore_r, fore_g, fore_b, fore_a),
                        bordered_mode(bordered_mode), filled_mode(filled_mode),
                        back_color(back_r, back_g, back_b, back_a) {}
            explicit Rectangle(Vector2 pos, Size size, const std::string& foreground = "#000000", bool bordered_mode = true,
                      bool filled_mode = false, const std::string& background = "#000000");
            Rectangle(float x, float y, float width, float height, const std::string& foreground = "#000000",
                      bool bordered_mode = true, bool filled_mode = false, const std::string& background = "#000000");

        };
        /**
         * @struct Ellipse
         * @brief 椭圆
         */
        struct Ellipse {
            Vector2 pos;
            Size area;
            bool bordered_mode;
            bool filled_mode;
            SColor fore_color;
            SColor back_color;
            Ellipse() : pos(0, 0), area(0, 0), bordered_mode(true), filled_mode(false),
                        fore_color(StdColor::Black), back_color(StdColor::Black) {}
            explicit Ellipse(Vector2 pos, Size area, SColor foreground = StdColor::Black, bool bordered_mode = true,
                    bool filled_mode = false, SColor background = StdColor::Black)
                    : pos(pos), area(area), fore_color(foreground), bordered_mode(bordered_mode),
                      filled_mode(filled_mode), back_color(background) {}
            Ellipse(float x, float y, float width, float height, SColor foreground = StdColor::Black,
                    bool bordered_mode = true, bool filled_mode = false, SColor background = StdColor::Black)
                    : pos(x, y), area(width, height), fore_color(foreground), bordered_mode(bordered_mode),
                      filled_mode(filled_mode), back_color(background) {}
        };
    }
}

#endif //EASYENGINE_BASIC_H