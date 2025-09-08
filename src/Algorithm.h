
#pragma once
#ifndef EASYENGINE_ALGORITHM_H
#define EASYENGINE_ALGORITHM_H

/**
 * @file Algorithm.h
 * @brief 算法库
 *
 * 提供图形绘制相关的算法实现，为渲染提供计算支持。
 *
 * @copyright Copyright © 2025 CatIsNotFound
 * @author CatIsNotFound
 */

#include "Basic.h"
#include "Components.h"

namespace EasyEngine {
    /**
     * @class Algorithm
     * @brief 算法库
     *
     * 存储图形相关的算法实现，为渲染提供计算支持
     */
    class Algorithm {
    public:
        /**
         * @brief 十六进制转换 RGBA
         * @param hex 十六进制颜色
         * @return 得到转换后的 RGBA 值
         * @code
         * Color color1 = Algorithm::hexToRGBA("#C0C0C0C0"); // RGBA(192, 192, 192, 192)
         * Color color2 = Algorithm::hexToRGBA("#FF00FF");   // RGBA(255, 0, 255, 255)
         * @endcode
         */
        static SColor hexToRGBA(const std::string& hex);
        /**
         * @brief RGBA 转换十六进制
         * @param color RGBA 颜色
         * @param add_alpha 同时转换 Alpha 通道（默认不转换）
         * @code
         * Color color(192, 192, 192, 255);
         * rgbaToHex(color);        // #C0C0C0
         * rgbaToHex(color, true);  // #C0C0C0FF
         * @endcode
         * @return 得到转换后的十六进制值
         */
        static std::string rgbaToHex(const SColor& color, bool add_alpha = false);
        /**
         * @brief 中心圆算法
         * @param point 圆点中心
         * @param radius 圆的半径
         * @return 返回一个用于存储所有可绘制点的坐标数组
         *
         * @brief 通过判断圆的中点与理想圆的位置关系以确定像素点，得到一个近似的圆形。
         * @note 若用于绘图事件，性能会有所影响！
         */
        static std::vector<Vector2> midCircleF(const Vector2 &point, uint32_t radius);

        /**
         * @brief 获取某个精灵经过缩放后所在的位置
         * @param properties    精灵绘制属性
         * @return 返回缩放后的精灵所在位置（左上角坐标）
         * @see Properties
         */
        static Vector2 spiritScaledPosition(const Components::Sprite::Properties &properties);

        /**
         * @brief 获取某个精灵经过缩放后所在的位置
         * @param position        原精灵所在位置
         * @param scaled          缩放倍数
         * @param scaled_position 缩放中心点
         * @return 返回缩放后的精灵所在位置（左上角坐标）
         * @see Properties
         */
        static Vector2 spiritScaledPosition(const Vector2 &position, const float scaled,
                                            const Vector2 &scaled_position);

        /**
         * @brief 获取缩放后的精灵大小/尺寸
         * @param spirit        指定精灵
         * @param properties    精灵绘制属性
         * @return 返回缩放后的大小
         */
        static Size spiritScaledSize(const Components::Sprite &spirit,
                                     const Components::Sprite::Properties &properties);
        /**
         * @brief 获取缩放后的精灵大小/尺寸
         * @param spirit    指定精灵
         * @param scaled    缩放倍数
         * @return 返回缩放后的精灵大小
         */
        static Size spiritScaledSize(const Components::Sprite &spirit,
                                     const float scaled);
    };

}

#endif //EASYENGINE_ALGORITHM_H
