
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
#include "Transition.h"

namespace EasyEngine {
    class Transition;
    namespace Components {
        /// 使用的元素集合
        using Elements = std::variant<std::shared_ptr<Sprite>, std::shared_ptr<SpriteGroup>,
                std::shared_ptr<Animation>, std::shared_ptr<Entity>, std::shared_ptr<Control>>;
        /**
         * @class Layer
         * @brief 图层
         *
         * 定义单个图层，在该图层下绘制元素
         */
        class Layer {
        public:
            /**
             * @brief 创建图层
             * @param name 图层别名
             */
            explicit Layer(const std::string &name);
            ~Layer();

            /**
             * @brief 设置图层别名
             * @param name 新的图层名
             */
            void setName(const std::string& name);
            /**
             * @brief 获取当前图层别名
             */
            const std::string& name() const;
            /**
             * @brief 添加指定精灵到图层
             * @param z_order   绘制顺序（数字越大，显示最先）
             * @param sprite    指定精灵
             * @return 是否成功添加到图层
             * @see Sprite
             * @see sprite
             * @see remove
             */
            bool append(uint32_t z_order, Sprite* sprite);
            /**
             * @brief 添加指定精灵组合到图层
             * @param z_order   绘制顺序（数字越大，显示最先）
             * @param sprite    指定精灵组合
             * @return 是否成功添加到图层
             * @see SpriteGroup
             * @see spriteGroup
             * @see remove
             */
            bool append(uint32_t z_order, SpriteGroup* group);
            /**
             * @brief 添加指定精灵动画到图层
             * @param z_order   绘制顺序（数字越大，显示最先）
             * @param sprite    指定精灵动画
             * @return 是否成功添加到图层
             * @see Animation
             * @see animation
             * @see remove
             */
            bool append(uint32_t z_order, Animation* animation);
            /**
             * @brief 添加指定游戏实体到图层
             * @param z_order   绘制顺序（数字越大，显示最先）
             * @param sprite    指定游戏实体
             * @return 是否成功添加到图层
             * @see Entity
             * @see entity
             * @see remove
             */
            bool append(uint32_t z_order, Entity* entity);
            /**
             * @brief 添加指定控件到图层
             * @param z_order   绘制顺序（数字越大，显示最先）
             * @param sprite    指定控件
             * @return 是否成功添加到图层
             * @see Control
             * @see control
             * @see remove
             */
            bool append(uint32_t z_order, Control* control);
            /**
             * @brief 移除图层
             * @param z_order   指定图层绘制顺序
             * @note 若不确定 z_order， 可使用 `indexOf()` 获取。
             * @see indexOf
             */
            void remove(uint32_t z_order);
            /**
             * @brief 获取指定图层下的精灵
             * @param z_order   指定图层绘制顺序
             * @return 返回对应图层下的指针
             * @note 若不确定 z_order， 可使用 `indexOf()` 获取。
             * @warning 若指定图层下的不是精灵，则报错并异常退出！
             * @see indexOf
             * @see remove
             */
            Sprite* sprite(uint32_t z_order) const;
            /**
             * @brief 获取指定图层下的精灵组合
             * @param z_order   指定图层绘制顺序
             * @return 返回对应图层下的指针
             * @note 若不确定 z_order， 可使用 `indexOf()` 获取。
             * @warning 若指定图层下的不是精灵组合，则报错并异常退出！
             * @see indexOf
             * @see remove
             */
            SpriteGroup* spriteGroup(uint32_t z_order) const;
            /**
             * @brief 获取指定图层下的精灵动画
             * @param z_order   指定图层绘制顺序
             * @return 返回对应图层下的指针
             * @note 若不确定 z_order， 可使用 `indexOf()` 获取。
             * @warning 若指定图层下的不是精灵动画，则报错并异常退出！
             * @see indexOf
             * @see remove
             */
            Animation* animation(uint32_t z_order) const;
            /**
             * @brief 获取指定图层下的游戏实体
             * @param z_order   指定图层绘制顺序
             * @return 返回对应图层下的指针
             * @note 若不确定 z_order， 可使用 `indexOf()` 获取。
             * @warning 若指定图层下的不是游戏实体，则报错并异常退出！
             * @see indexOf
             * @see remove
             */
            Entity* entity(uint32_t z_order) const;
            /**
             * @brief 获取指定图层下的控件
             * @param z_order   指定图层绘制顺序
             * @return 返回对应图层下的指针
             * @note 若不确定 z_order， 可使用 `indexOf()` 获取。
             * @warning 若指定图层下的不是控件，则报错并异常退出！
             * @see indexOf
             * @see remove
             */
            Control* control(uint32_t z_order) const;
            /**
             * @brief 指定别名并获取对应的图层渲染顺序
             * @param name      指定别名（可能是精灵、精灵组合、精灵动画等）
             * @param start_of  指定从第几个图层渲染顺序开始
             * @param end_of    指定从第几个图层渲染顺序结束
             * @return 获取对应的渲染顺序
             * @see sprite
             * @see spriteGroup
             * @see animation
             * @see entity
             * @see control
             * @see remove
             * @see swap
             * @see setZOrder
             */
            uint32_t indexOf(const std::string &name, uint32_t start_of = 0, uint32_t end_of = UINT32_MAX) const;
            /**
             * @brief 指定两个元素并交换图层渲染顺序
             * @param z_order1 指定第一个
             * @param z_order2 指定第二个
             * @see indexOf
             * @see setZOrder
             */
            bool swap(uint32_t z_order1, uint32_t z_order2);
            /**
             * @brief 将当前元素更换新的图层渲染顺序
             * @param old_z_order   指定原先的图层渲染顺序
             * @param new_z_order   新的图层渲染顺序
             * @return 返回是否成功更换图层渲染顺序
             * @see indexOf
             * @see swap
             */
            bool setZOrder(uint32_t old_z_order, uint32_t new_z_order);
            /**
             * @brief 设置当前图层是否可见
             * @param visible 指定是否可见
             */
            void setVisible(bool visible);
            /**
             * @brief 获取当前图层是否可见
             */
            bool visible() const;
            /**
             * @brief 设置视图
             * @param geometry 指定其位置、大小
             *
             * 决定了图层绘制的区域
             * @note 视图影响了所有元素所在的位置
             */
            void setViewport(const Geometry& geometry);
            /**
             * @brief 设置视图
             * @param x 指定位置横坐标
             * @param y 指定位置纵坐标
             * @param w 指定宽度
             * @param h 指定高度
             *
             * 决定了图层绘制的区域
             * @note 视图影响了所有元素所在的位置
             */
            void setViewport(int x, int y, int w, int h);
            /**
             * @brief 设置视图缩放
             * @param width     宽度比例（注：此处并非调整视图的宽度）
             * @param height    高度比例（注：此处并非调整视图的高度）
             */
            void setViewportScaled(float width, float height);
            /**
             * @brief 设置视图缩放比例
             * @param scaled    尺寸比例（按照 1:1 比例调整）
             */
            void setViewportScaled(const Size& scaled);
            /**
             * @brief 设置裁剪视图
             * @param geometry 指定位置、大小
             *
             * @note 仅在原有的基础上进行裁剪，这不会发生任何的改变！
             */
            void setClipViewport(const Geometry& geometry);
            /**
             * @brief 设置裁剪视图
             * @param x 指定位置横坐标
             * @param y 指定位置纵坐标
             * @param w 指定宽度
             * @param h 指定高度
             *
             * @note 仅在原有的基础上进行裁剪，这不会发生任何的改变！
             */
            void setClipViewport(int x, int y, int w, int h);
            /**
             * @brief 设置绘制视图的绘制器（必需）
             * @param painter 指定绘制器
             *
             * @note 必需设置，否则即便设置视图大小位置，也是无用！
             */
            void setViewportPainter(Painter* painter);
            /**
             * @brief 绘制图层
             * @param viewport_mode 是否使用视图（需调用 `setViewport()`）
             * @param clip_mode     是否使用裁剪视图（需调用 `setClipViewport()`）
             * @note 执行此函数前，若 `viewport_mode` 或 `clip_mode` 为 `true`，
             * @note 还需调用 `setViewportPainter()` 才能生效！
             *
             * 执行后，这将会按照图层的渲染顺序依次绘制！`z_order` 越大，渲染越靠后，显示最靠前！
             */
            void draw(bool viewport_mode = false, bool clip_mode = false);
            /**
             * @brief 获取当前视图的位置和大小
             */
            const Geometry& viewport() const;
            /**
             * @brief 获取当前视图的缩放宽高比例
             */
            const Size& viewportScaled() const;
            /**
             * @brief 获取当前裁剪视图的位置和大小
             */
            const Geometry& clipViewport() const;
            /**
             * @brief 获取当前绘制视图的绘图器
             */
            const Painter* viewportPainter() const;
        private:
            bool checkAppendError(uint32_t z_order);
            void update();
            std::string _name;
            std::map<uint32_t, Elements> _elements;
            Geometry _viewport_geometry;
            Geometry _clipViewport_geometry;
            Size _scaled{1.0f, 1.0f};
            Painter* _painter{nullptr};
            bool _visible{true};
        };

        /**
         * @class Scene
         * @brief 场景
         *
         * 定义单个场景，包含多个图层，可用于管理图层
         */
        class Scene {
        public:
            explicit Scene(const std::string& name);
            void setName(const std::string& name);
            const std::string& name() const;
            bool appendLayer(uint32_t z_order, const std::string &name);
            bool appendLayer(uint32_t z_order, Layer* layer);
            bool removeLayer(uint32_t z_order);
            uint32_t indexOf(const std::string& layer_name) const;
            uint32_t indexOf(const Layer* layer) const;
            Layer* layer(const std::string& layer_name) const;
            Layer* layer(uint32_t z_order) const;
            bool swapLayer(uint32_t z_order1, uint32_t z_order2);
            bool swapLayer(const std::string& layer_name1, const std::string& layer_name2);
            bool setZOrder(uint32_t old_z_order, uint32_t new_z_order);
            bool setZOrder(const std::string& layer_name, uint32_t new_z_order);
            void setSceneEvent(const std::function<void()>& event);
            void drawLayers();
        private:
            std::string _name;
            std::map<uint32_t, std::shared_ptr<Layer>> _layers;
            std::function<void()> _event;
        };
    }
    /**
     * @class SceneManager
     * @brief 场景管理
     *
     * 定义场景管理器，用于管理多个游戏场景
     */
    class SceneManager {
    public:
        /**
         * @struct Property
         * @brief 场景属性
         */
        struct Property {
            /// 场景
            std::shared_ptr<Components::Scene> scene{};
            std::function<void()> change_event{};
            std::shared_ptr<Transition> transition{};
        };
        explicit SceneManager();

        bool append(Components::Scene* scene, uint32_t index);
        bool remove(uint32_t index);
        bool setEvent(uint32_t index, const std::function<void()>& event);
        bool removeEvent(uint32_t index);
        bool setTransition(uint32_t index, Transition* transition);
        bool removeTransition(uint32_t index);
        void changeScene(uint32_t index);
        uint32_t indexOf(const Components::Scene* scene) const;
        uint32_t indexOf(const std::string& scene_name) const;
        uint32_t currentSceneIndex() const;
        Components::Scene *scene(uint64_t index) const;
        Components::Scene* currentScene() const;
        void ______();
    private:
        void changeSceneEvent();
        std::map<uint32_t, Property> _scenes;
        uint32_t _changer_index{0};
        uint32_t _new_changer{0};
        uint32_t _old_changer_index{0};
        bool _change_signal{false};
    };
}

#endif //EASYENGINE_SCENE_H
