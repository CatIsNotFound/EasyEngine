
#include "Basic.h"
#include "Algorithm.h"

namespace EasyEngine {
    Geometry::Geometry() : x(0), y(0), width(0), height(0) {}

    Geometry::Geometry(int x, int y, int width, int height)
            : x(x), y(y), width(width), height(height) {}

    void Geometry::setGeometry(int x, int y, int width, int height) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    void Geometry::setPosition(int x, int y) {
        this->x = x;
        this->y = y;
    }

    void Geometry::resize(int width, int height) {
        this->width = width;
        this->height = height;
    }

    Vector2::Vector2(float x, float y) : x(x), y(y) {}

    void Vector2::reset(float x, float y) {
        this->x = x;
        this->y = y;
    }

    void Vector2::reset(const EasyEngine::Vector2 &vector2) { reset(vector2.x, vector2.y); }

    Size::Size(float width, float height) : width(width), height(height) {}

    void Size::reset(float width, float height) {
        this->width = width;
        this->height = height;
    }

    void Size::reset(const EasyEngine::Size &size) { reset(size.width, size.height); }

    Graphics::Point::Point(Vector2 pos, const std::string &hex_color) :
        pos(pos), color(Algorithm::hexToRGBA(hex_color)){}

    Graphics::Point::Point(float x, float y, const std::string &hex_color) :
        pos(x, y), color(Algorithm::hexToRGBA(hex_color)) {}

    Graphics::Line::Line(float x1, float y1, float x2, float y2, uint8_t width, const std::string &hex_color) :
            start(x1, y1), end(x2, y2), width(width), color(Algorithm::hexToRGBA(hex_color)) {}

    Graphics::Line::Line(Vector2 start, Vector2 end, uint8_t width, const std::string &hex_color) :
            start(start), end(end), width(width), color(Algorithm::hexToRGBA(hex_color)) {}

    Graphics::Rectangle::Rectangle(Vector2 pos, Size size, const std::string &foreground, bool bordered_mode,
                                   bool filled_mode, const std::string &background) :
                                   pos(pos), size(size), fore_color(Algorithm::hexToRGBA(foreground)),
                                   bordered_mode(bordered_mode), filled_mode(filled_mode),
                                   back_color(Algorithm::hexToRGBA(background)) {}

    Graphics::Rectangle::Rectangle(float x, float y, float width, float height, const std::string &foreground,
                                   bool bordered_mode, bool filled_mode, const std::string& background) :
                                    pos(x, y), size(width, height), fore_color(Algorithm::hexToRGBA(foreground)),
                                    bordered_mode(bordered_mode), filled_mode(filled_mode),
                                    back_color(Algorithm::hexToRGBA(background)) {}


    template<typename T>
    uint32_t Matrix2<T>::count(Pos2 &start, Pos2 &end) {
        correct(start, end);
        uint32_t sum_rows = data.size(), sum_cols = data[0].size(), sum = sum_rows * sum_cols;
        uint32_t start_id = start.row * sum_rows + start.col,
                 end_id = end.row * sum_rows + end.col;
        if (end_id >= sum) end_id = sum;
        return end_id - start_id + 1;
    }

    template<typename T>
    void Matrix2<T>::forRange(Pos2 &start, Pos2 &end,
                              const std::function<void(const T &)> &function) {
        correct(start, end);
        uint32_t sum_cols = data[0].size();
        bool f = false;
        for (uint32_t r = start.row; r <= end.row; ++r) {
            for (uint32_t c = 0; c <= sum_cols; ++c) {
                if (!f && start.col >= c) { f = true; }
                else {
                    if (function) function(data[r][c]);
                }
            }
        }
    }

    template<typename T>
    void Matrix2<T>::forRange(Pos2 &start, Pos2 &end,
                              const std::function<void(const Pos2 &)> &function) {
        correct(start, end);
        uint32_t sum_cols = data[0].size();
        bool f = false;
        for (uint32_t r = start.row; r <= end.row; ++r) {
            for (uint32_t c = 0; c <= sum_cols; ++c) {
                if (!f && start.col >= c) { f = true; }
                else {
                    if (function) function({r, c});
                }
            }
        }
    }

    template<typename T>
    void Matrix2<T>::correct(Pos2 &start, Pos2 &end) {
        if (start > end) {
            Matrix2 temp = start;
            start = end;
            end = temp;
        }
        uint32_t sum_rows = data.size(), sum_cols = data[0].size();
        Pos2 max_end = {sum_rows - 1, sum_cols - 1};
        if (max_end > end) {
            end = max_end;
        }
    }

    template<typename T>
    std::vector<T> Matrix2<T>::getN(Pos2 &start, Pos2 &end) {
        std::vector<T> ret(data.size() * data[0].size());
        forRange(start, end, [&ret](const T& value) { ret.emplace_back(value); });
        return ret;
    }

    template<typename T>
    void Matrix2<T>::setN(Pos2 &start, Pos2 &end, const T& value) {
        forRange(start, end, [this, &value](const Pos2& pos) { data[pos.row][pos.col] = value; });
    }

    template<typename T>
    void Matrix2<T>::foreach(const std::function<void(T&)>& function) {
        if (!function) return;
        for (auto& row : data) {
            for (auto& col : row) {
                function(col);
            }
        }
    }
}