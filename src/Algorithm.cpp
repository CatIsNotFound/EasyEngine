
#include "Algorithm.h"

using namespace EasyEngine;

SColor Algorithm::hexToRGBA(const std::string &hex) {
    if (hex.empty() || hex.front() != '#' || hex.size() < 7) {
        fmt::println("ERROR: Format Error! Color In hex: {0}", hex);
        return {};
    }
    uint8_t color[4] = {0, 0, 0, 255};
    uint8_t tmp, _x = 0;
    for (int i = 1; i < hex.length(); i += 2) {
        if (hex[i] >= '0' && hex[i] <= '9') tmp = 16 * (hex[i] - '0');
        else if (hex[i] >= 'A' && hex[i] <= 'F') tmp = 16 * (hex[i] - 55);
        else if (hex[i] >= 'a' && hex[i] <= 'f') tmp = 16 * (hex[i] - 87);
        else {
            fmt::println("ERROR: Format Error! Color In hex: {0}", hex);
            return {};
        }
        if (hex[i + 1] >= '0' && hex[i + 1] <= '9') tmp += (hex[i + 1] - '0');
        else if (hex[i + 1] >= 'A' && hex[i + 1] <= 'F') tmp += (hex[i + 1] - 55);
        else if (hex[i + 1] >= 'a' && hex[i + 1] <= 'f') tmp += (hex[i + 1] - 87);
        else {
            fmt::println("ERROR: Format Error! Color In hex: {0}", hex);
            return {};
        }
        color[_x++] = tmp;
    }
    return SColor(color[0], color[1], color[2], color[3]);
}

std::string Algorithm::rgbaToHex(const SColor &color, bool add_alpha) {
    std::string ret = "#";
    uint8_t rgba[4] = {color.r, color.g, color.b, color.a},
            rgb[3] = {color.r, color.g, color.b};
    const uint8_t* real = (add_alpha ? &rgba[0] : &rgb[0]);
    for (uint8_t t = 0; t < (add_alpha ? 4 : 3); ++t) {
        int hex[2] = {(real[t] / 16) % 16, real[t] % 16};
        for (auto& h : hex) {
            if (h < 10) ret += static_cast<char>(48 + h);
            else ret += static_cast<char>(55 + h);
        }
    }
    return ret;
}

std::vector<Vector2>
EasyEngine::Algorithm::midCircleF(const Vector2 &point, uint32_t radius) {
    std::vector<Vector2> ret;
    auto r = (float)radius;
    float x = 0, y = r;
    float d = 1.25f - r;
    while (x <= y) {
        ret.emplace_back(x + point.x, y + point.y);
        ret.emplace_back(-x + point.x, y + point.y);
        ret.emplace_back(x + point.x, -y + point.y);
        ret.emplace_back(-x + point.x, -y + point.y);
        ret.emplace_back(y + point.x, x + point.y);
        ret.emplace_back(-y + point.x, x + point.y);
        ret.emplace_back(y + point.x, -x + point.y);
        ret.emplace_back(-y + point.x, -x + point.y);
        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
    return ret;
}

Vector2 Algorithm::spiritScaledPosition(const Components::Sprite::Properties &properties) {
    Vector2 _pos = properties.position, _scaled_center = properties.scaled_center;
    float _scaled = properties.scaled;
    Vector2 _global_center_pos(_pos.x + _scaled_center.x, _pos.y + _scaled_center.y);
    Vector2 _new_global_left_top_pos((_pos.x - _global_center_pos.x) * _scaled + _global_center_pos.x,
                                     (_pos.y - _global_center_pos.y) * _scaled + _global_center_pos.y);
    return {_new_global_left_top_pos.x, _new_global_left_top_pos.y};
}

Vector2 Algorithm::spiritScaledPosition(const Vector2 &position, const float scaled, const Vector2 &scaled_position) {
    Vector2 _global_center_pos(position.x + scaled_position.x, position.y + scaled_position.y);
    Vector2 _new_global_left_top_pos((position.x - _global_center_pos.x) * scaled + _global_center_pos.x,
                                     (position.y - _global_center_pos.y) * scaled + _global_center_pos.y);
    return {_new_global_left_top_pos.x, _new_global_left_top_pos.y};
}

Size Algorithm::spriteScaledSize(const Components::Sprite &sprite, const Components::Sprite::Properties &properties) {
    return {sprite.size().width * properties.scaled, sprite.size().height * properties.scaled};
}

Size Algorithm::spriteScaledSize(const Components::Sprite &sprite, const float scaled) {
    return {sprite.size().width * scaled, sprite.size().height * scaled};
}

int8_t Algorithm::comparePosRect(const Vector2 &pos, const Graphics::Rectangle &rectangle) {
    float minX = rectangle.pos.x,
          minY = rectangle.pos.y,
          maxX = rectangle.size.width + rectangle.pos.x,
          maxY = rectangle.size.height + rectangle.pos.y;
    if (pos.x > maxX || pos.x < minX || pos.y > maxY || pos.y < minY) return -1;
    if ((pos.x == minX || pos.x == maxX) && (pos.y >= minY && pos.y <= maxY)) {
        return 0;
    }
    if ((pos.y == minY || pos.y == maxY) && (pos.x >= minX && pos.x <= maxX)) {
        return 0;
    }
    return 1;
}

int8_t Algorithm::comparePosEllipse(const Vector2 &pos, const Graphics::Ellipse &ellipse) {
    float rx = ellipse.area.width / 2.0f;
    float ry = ellipse.area.height / 2.0f;
    if (rx <= 0.0f || ry <= 0.0f) {
        return -1;
    }
    float dx = pos.x - ellipse.pos.x;
    float dy = pos.y - ellipse.pos.y;
    float distance = (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry);
    // 使用 epsilon 处理浮点数精度问题
    const float EPSILON = 1e-6f;
    if (distance < 1.0f - EPSILON)
        return 1;
    else if (abs(distance - 1.0f) <= EPSILON)
        return 0;
    else
        return -1;
}

int8_t Algorithm::compareRect(const Graphics::Rectangle &rect1, const Graphics::Rectangle &rect2) {
    float x1 = rect1.pos.x, x2 = rect1.pos.x + rect1.size.width, y1 = rect1.pos.y, y2 = rect1.pos.y + rect1.size.height,
        x3 = rect2.pos.x, x4 = rect2.pos.x + rect2.size.width, y3 = rect2.pos.y, y4 = rect2.pos.y + rect2.size.height;

    if (x2 < x3 || x1 > x4 || y2 < y3 || y1 > y4) return -1;
    if (x3 <= x1 && y3 <= y1 && x4 >= x2 && y4 >= y2) return 1;
    if (x1 <= x3 && y1 <= y3 && x2 >= x4 && y2 >= y4) return 2;
    return 0;
}

int8_t Algorithm::compareEllipse(const Graphics::Ellipse &ellipse1, const Graphics::Ellipse &ellipse2) {
    return 0;
}

