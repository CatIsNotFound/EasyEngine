
#pragma once
#ifndef EASYENGINE_SCENE_H
#define EASYENGINE_SCENE_H
/**
 * @file Scene.h
 * @brief 场景管理
 *
 * 定义游戏场景管理
 *
 * @copyright Copyright © 2025 CatIsNotFound
 * @author CatIsNotFound
 */
#include "Components.h"

namespace EasyEngine {
    using namespace Components;
    /**
     * @class Layer
     * @brief 图层
     *
     * 定义单个图层，在该图层下绘制元素
     */
    class Layer {
    public:
        explicit Layer(const std::string& name);


    private:
        std::string name;
        std::map<uint64_t, std::shared_ptr<Element>> _elements;
    };

    /**
     * @class Scene
     * @brief 场景
     *
     * 定义单个场景，包含多个图层，可用于管理图层
     */
    class Scene {

    };

    /**
     * @class SceneManager
     * @brief 场景管理
     *
     * 定义场景管理器，用于管理多个游戏场景
     */
    class SceneManager {

    };
}

#endif //EASYENGINE_SCENE_H
