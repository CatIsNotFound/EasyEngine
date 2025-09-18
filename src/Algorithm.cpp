
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
    return _new_global_left_top_pos;
}

Vector2 Algorithm::spiritScaledPosition(const Vector2 &position, const float scaled, const Vector2 &scaled_position) {
//    Vector2 _global_center_pos(position.x + scaled_position.x, position.y + scaled_position.y);
    Vector2 _global_center_pos = position + scaled_position;
//    Vector2 _new_global_left_top_pos((position.x - _global_center_pos.x) * scaled + _global_center_pos.x,
//                                     (position.y - _global_center_pos.y) * scaled + _global_center_pos.y);
    Vector2 _new_global_left_top_pos = (position - _global_center_pos) * scaled + _global_center_pos;
    return _new_global_left_top_pos;
}

Size Algorithm::spriteScaledSize(const Components::Sprite &sprite, const Components::Sprite::Properties &properties) {
    return sprite.size() * properties.scaled;
}

Size Algorithm::spriteScaledSize(const Components::Sprite &sprite, const float scaled) {
    return sprite.size() * scaled;
}

int8_t Algorithm::comparePosRect(const Vector2 &pos, const Graphics::Rectangle &rectangle) {
    float minX = rectangle.pos.x,
          minY = rectangle.pos.y,
          maxX = rectangle.size.width + rectangle.pos.x,
          maxY = rectangle.size.height + rectangle.pos.y;
    const float EPSILON = 1e-6f;
    if (pos.x > maxX + EPSILON || pos.x < minX - EPSILON ||
        pos.y > maxY + EPSILON || pos.y < minY - EPSILON) {
        return -1;
    }

    bool onLeftEdge = fabsf(pos.x - minX) <= EPSILON;
    bool onRightEdge = fabsf(pos.x - maxX) <= EPSILON;
    bool onTopEdge = fabsf(pos.y - minY) <= EPSILON;
    bool onBottomEdge = fabsf(pos.y - maxY) <= EPSILON;
    if ((onLeftEdge || onRightEdge || onTopEdge || onBottomEdge) && 
        pos.x >= minX - EPSILON && pos.x <= maxX + EPSILON && 
        pos.y >= minY - EPSILON && pos.y <= maxY + EPSILON) {
        return 0;
    }
    return 1;
}

int8_t Algorithm::comparePosEllipse(const Vector2 &pos, const Graphics::Ellipse &ellipse) {
    Vector2 pt = pos - ellipse.pos;
    float a = ellipse.area.width * 0.5f;
    float b = ellipse.area.height * 0.5f;
    const float EPISON = 1e-6f;
    // 椭圆方程: (x/a)^2 + (y/b)^2 <= 1
    float value = (pt.x * pt.x) / (a * a) +
                  (pt.y * pt.y) / (b * b);
    if (value >= 1.0f - EPISON && value <= 1.0f + EPISON) {
        return 0;
    } else if (value >= 1.0f + EPISON) {
        return -1;
    } else {
        return 1;
    }
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
    // 快速计算中心距离平方，避免开方
    Vector2 centerDiff = ellipse1.pos - ellipse2.pos;
    float distanceSq = centerDiff.x * centerDiff.x + centerDiff.y * centerDiff.y;

    // 获取半轴长度
    float fa1 = ellipse1.area.width * 0.5f;
    float fb1 = ellipse1.area.height * 0.5f;
    float fa2 = ellipse2.area.width * 0.5f;
    float fb2 = ellipse2.area.height * 0.5f;
    // 区分长半轴，短半轴
    float a1 = std::max(fa1, fb1);
    float b1 = std::min(fa1, fb1);
    float a2 = std::max(fa2, fb2);
    float b2 = std::min(fa2, fb2);
    // 使用包围盒，快速粗略判断
    float left1 = ellipse1.pos.x - fa1;
    float right1 = ellipse1.pos.x + fa1;
    float bottom1 = ellipse1.pos.y - fb1;
    float top1 = ellipse1.pos.y + fb1;
    float left2 = ellipse2.pos.x - fa2;
    float right2 = ellipse2.pos.x + fa2;
    float bottom2 = ellipse2.pos.y - fb2;
    float top2 = ellipse2.pos.y + fb2;
    if ((right1 < left2) || (left1 > right2) || (top1 < bottom2) || (bottom1 > top2)) {
        return -1;
    }

    // 快速分离判断：使用最大可能距离
    float maxDistance = (a1 + a2) * (a1 + a2);
    if (distanceSq > maxDistance) {
        return -1;
    }
    // 使用 SAT 定理
    constexpr float EPISON = 1e-6f;
    std::vector<std::vector<bool>> axes = {{true, false}, {false, true}};
    float r1, r2;
    for (auto& axis : axes) {
        r1 = axis[0] ? a1 : b1;
        r2 = axis[0] ? a2 : b2;
        float f = fabs(centerDiff.x * axis[0] + centerDiff.y * axis[1]);
        if (f > r1 + r2) return -1;
        if (fabs(f - (r1 + r2)) < EPISON) {
            return -1;
        }
    }

    // 快速包含判断：检查中心点是否在对方椭圆内
    bool center1InEllipse2 = comparePosEllipse(ellipse1.pos, ellipse2) >= 0;
    bool center2InEllipse1 = comparePosEllipse(ellipse2.pos, ellipse1) >= 0;

    // 如果一个中心在另一个椭圆内，且椭圆大小差异明显，则判断为包含
    if (center1InEllipse2 && center2InEllipse1) {
        // 两个中心互相在对方椭圆内，比较大小
        float area1 = a1 * b1;
        float area2 = a2 * b2;
        return area1 > area2 ? 2 : 1;
    } else if (center1InEllipse2) {
        // 快速检查椭圆1是否完全在椭圆2内
        if (isEllipseInsideEllipse(ellipse1, ellipse2)) {
            return 1;
        }
    } else if (center2InEllipse1) {
        // 快速检查椭圆2是否完全在椭圆1内
        if (isEllipseInsideEllipse(ellipse2, ellipse1)) {
            return 2;
        }
    }
    // 相交或相切
    return 0;
}

bool Algorithm::isEllipseInsideEllipse(const Graphics::Ellipse &inner, const Graphics::Ellipse &outer) {
    // 检查内椭圆的8个端点是否都在外椭圆内（增加对角线方向的检查点以提高准确性）
    float a_in = inner.area.width * 0.5f;
    float b_in = inner.area.height * 0.5f;
    // 计算对角线方向的缩放因子（约为0.7071，即√2/2）
    const float diagonalFactor = 0.7071f;
    Vector2 points[8] = {
            // 基本方向点
            {inner.pos.x + a_in, inner.pos.y},         // 右
            {inner.pos.x - a_in, inner.pos.y},         // 左
            {inner.pos.x, inner.pos.y + b_in},         // 上
            {inner.pos.x, inner.pos.y - b_in},         // 下
            // 对角线方向点
            {inner.pos.x + a_in * diagonalFactor, inner.pos.y + b_in * diagonalFactor},  // 右上
            {inner.pos.x - a_in * diagonalFactor, inner.pos.y + b_in * diagonalFactor},  // 左上
            {inner.pos.x + a_in * diagonalFactor, inner.pos.y - b_in * diagonalFactor},  // 右下
            {inner.pos.x - a_in * diagonalFactor, inner.pos.y - b_in * diagonalFactor}   // 左下
    };
    bool ret = false;
    std::ranges::all_of(std::ranges::begin(points), std::ranges::end(points),
                        [&] (const Vector2& point) {
        if (comparePosEllipse(point, outer) <= 0) {
            return ret;
        }
        ret = true;
        return true;
    });
    return ret;
}

int8_t Algorithm::compareRectEllipse(const Graphics::Rectangle &rect, const Graphics::Ellipse &ellipse) {
    // 快速检查矩形和椭圆是否完全分离
    float rectLeft = rect.pos.x;
    float rectRight = rect.pos.x + rect.size.width;
    float rectTop = rect.pos.y;
    float rectBottom = rect.pos.y + rect.size.height;
    float ellipseLeft = ellipse.pos.x - ellipse.area.width * 0.5f;
    float ellipseRight = ellipse.pos.x + ellipse.area.width * 0.5f;
    float ellipseTop = ellipse.pos.y - ellipse.area.height * 0.5f;
    float ellipseBottom = ellipse.pos.y + ellipse.area.height * 0.5f;
    
    // 如果包围盒不重叠，直接返回分离
    if (rectRight < ellipseLeft || rectLeft > ellipseRight || 
        rectBottom < ellipseTop || rectTop > ellipseBottom) {
        return -1;
    }
    
    // 检查矩形是否完全包含椭圆
    bool ellipseFullyInRect = true;
    float a = ellipse.area.width * 0.5f;
    float b = ellipse.area.height * 0.5f;
    
    // 检查椭圆的关键点是否都在矩形内
    Vector2 ellipsePoints[8] = {
        {ellipse.pos.x + a, ellipse.pos.y},           // 右
        {ellipse.pos.x - a, ellipse.pos.y},           // 左
        {ellipse.pos.x, ellipse.pos.y + b},           // 上
        {ellipse.pos.x, ellipse.pos.y - b},           // 下
        {ellipse.pos.x + a * 0.7071f, ellipse.pos.y + b * 0.7071f},  // 右上
        {ellipse.pos.x - a * 0.7071f, ellipse.pos.y + b * 0.7071f},  // 左上
        {ellipse.pos.x + a * 0.7071f, ellipse.pos.y - b * 0.7071f},  // 右下
        {ellipse.pos.x - a * 0.7071f, ellipse.pos.y - b * 0.7071f}   // 左下
    };
    
    for (const auto& point : ellipsePoints) {
        if (comparePosRect(point, rect) < 0) {
            ellipseFullyInRect = false;
            break;
        }
    }
    
    if (ellipseFullyInRect) {
        return 2;  // 矩形包含椭圆
    }
    
    // 检查椭圆是否完全包含矩形
    bool rectFullyInEllipse = true;
    Vector2 rectPoints[4] = {
        {rectLeft, rectTop},        // 左上
        {rectRight, rectTop},       // 右上
        {rectRight, rectBottom},    // 右下
        {rectLeft, rectBottom}      // 左下
    };
    
    for (const auto& point : rectPoints) {
        if (comparePosEllipse(point, ellipse) < 0) {
            rectFullyInEllipse = false;
            break;
        }
    }
    
    if (rectFullyInEllipse) {
        return 1;  // 椭圆包含矩形
    }
    
    // 检查是否有矩形顶点在椭圆内，或者椭圆关键点在矩形内
    bool hasIntersection = false;
    
    // 检查矩形顶点是否在椭圆内
    for (const auto& point : rectPoints) {
        if (comparePosEllipse(point, ellipse) >= 0) {
            hasIntersection = true;
            break;
        }
    }
    
    // 如果没有找到，检查椭圆关键点是否在矩形内
    if (!hasIntersection) {
        for (const auto& point : ellipsePoints) {
            if (comparePosRect(point, rect) >= 0) {
                hasIntersection = true;
                break;
            }
        }
    }
    
    if (hasIntersection) {
        return 0;  // 相交或相切
    }
    
    // 如果以上都不满足，返回分离
    return -1;
}

