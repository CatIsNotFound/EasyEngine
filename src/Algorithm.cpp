
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

Vector2 Algorithm::spiritScaledPosition(const Components::Spirit::Properties &properties) {
    Vector2 _pos = properties.position, _scaled_center = properties.scaled_center;
    float _scaled = properties.scaled;
    Vector2 _global_center_pos(_pos.x + _scaled_center.x, _pos.y + _scaled_center.y);
    Vector2 _new_global_left_top_pos((_pos.x - _global_center_pos.x) * _scaled + _global_center_pos.x,
                                     (_pos.y - _global_center_pos.y) * _scaled + _global_center_pos.y);
    return Vector2(_new_global_left_top_pos.x, _new_global_left_top_pos.y);
}

Vector2 Algorithm::spiritScaledPosition(const Vector2 &position, const float scaled, const Vector2 &scaled_position) {
    Vector2 _global_center_pos(position.x + scaled_position.x, position.y + scaled_position.y);
    Vector2 _new_global_left_top_pos((position.x - _global_center_pos.x) * scaled + _global_center_pos.x,
                                     (position.y - _global_center_pos.y) * scaled + _global_center_pos.y);
    return Vector2(_new_global_left_top_pos.x, _new_global_left_top_pos.y);
}

Size Algorithm::spiritScaledSize(const Components::Spirit &spirit, const Components::Spirit::Properties &properties) {
    return Size(spirit.size().width * properties.scaled, spirit.size().height * properties.scaled);
}

Size Algorithm::spiritScaledSize(const Components::Spirit &spirit, const float scaled) {
    return Size(spirit.size().width * scaled, spirit.size().height * scaled);
}

