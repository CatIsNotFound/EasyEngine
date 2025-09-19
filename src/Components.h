
#pragma once
#ifndef EASYENGINE_COMPONENTS_H
#define EASYENGINE_COMPONENTS_H

/**
 * @file Components.h
 * @brief 组件库
 *
 * 定义引擎下使用到的所有组件，包含：精灵、动画、对象、碰撞体等
 *
 * @copyright Copyright © 2025 CatIsNotFound
 * @author CatIsNotFound
 */

#include "Basic.h"

namespace EasyEngine {
    class Painter;
    class EventSystem;
    class AudioSystem;
    /**
     * @namespace Components
     * @brief 组件库
     *
     * 包含所有游戏引擎所相关的组件库
     */
    namespace Components {
        class Timer;
        /**
         * @class BGM
         * @brief 背景音乐
         */
        class BGM {
        public:
            explicit BGM();
            ~BGM();
            /**
             * @brief 创建指定路径下的 BGM
             * @param path 指定有效路径
             */
            BGM(const std::string &path);
            /**
             * @brief 设置 BGM 播放源路径
             * @param path 指定路径
             */
            void setPath(const std::string &path);
            /**
             * @brief 获取该 BGM 下的播放源路径
             *
             */
            const std::string& path() const;

            /**
             * @brief 播放 BGM
             * @param loop 是否循环播放（默认循环）
             * @see stop
             * @see paused
             * @see isPlayed
             * @see isLoop
             */
            void play(bool loop = true);
            /**
             * @brief 停止播放 BGM
             * @see play
             * @see paused
             * @see isPlayed
             * @see isLoop
             */
            void stop();
            /**
             * @brief 暂停播放当前 BGM
             * @see play
             * @see stop
             * @see isPlayed
             * @see isLoop
             */
            void pause();
            /**
             * @brief 当前是否正在播放 BGM
             * @see play
             * @see stop
             * @see paused
             * @see isLoop
             */
            bool isPlayed() const;
            /**
             * @brief 当前是否正在循环播放 BGM
             * @see stop
             * @see paused
             * @see isPlayed
             * @see isLoop
             */
            bool isLoop() const;
            /**
             * @brief 获取播放进度
             *
             * @return 返回一个以毫秒单位的播放时长
             */
            int64_t position() const;
        protected:
            void reload();
        private:
            std::string _path;
            uint8_t _channel;
            bool _is_loop{false};
            bool _is_load{false};
            uint64_t _pos{};
            friend class AudioSystem;
        };

        /**
         * @class SFX
         * @brief 音效
         */
        class SFX {
        public:
            explicit SFX();
            ~SFX();
            /**
             * @brief 创建指定路径的 SFX
             * @param path 指定音频路径
             */
            SFX(const std::string& path);
            /**
             * @brief 设置指定源路径
             * @param path 指定音频源路径
             */
            void setPath(const std::string& path);
            /**
             * @brief 获取当前音频源路径
             */
            const std::string& path() const;
            /**
             * @brief 播放 SFX
             */
            void play();
            /**
             * @brief 播放 SFX（循环播放）
             * @param delay 设定间隔（设定毫秒数）
             */
            void play(uint32_t delay);
            /**
             * @brief 立即停止播放 SFX
             */
            void stop();
            /**
             * @brief 获取当前音频是否正在循环播放
             */
            bool isLoop() const;


        protected:
            void reload();

        private:
            std::string _path;
            uint8_t _channel;
            bool _is_loop{false};
            bool _is_load{false};
            Timer* _timer{nullptr};
            friend class AudioSystem;
        };

        /**
         * @class Timer
         * @brief 定时器
         *
         * 用于定时触发事件的组件
         * @note 此组件只能以指针的形式使用
         */
        class Timer {
            friend class EventSystem;
        public:
            /**
             * @brief 创建定时器
             * @note 这里只能使用指针的形式创建！
             * @code
             * Timer* timer = new Timer();
             * timer->setDelay(1000);   // 1000ms = 1sec
             * timer->setEvent(your_function);
             * timer->start();
             * // delete timer; // 调用 start() 函数后，不能释放此指针！
             * @endcode
             * @see setDelay
             * @see setEvent
             * @see start
             */
            explicit Timer();
            ~Timer();
            /**
             * @brief 创建定时器
             * @param delay     设定触发延迟（毫秒）
             * @param function  触发事件
             * @note 这里只能使用指针的形式创建！
             * @see start
             */
            Timer(uint64_t delay, const std::function<void()>& function);
            /**
             * @brief 设定定时器触发延迟
             * @param delay 指定延迟（毫秒）
             * @see setEvent
             * @see start
             */
            void setDelay(uint64_t delay);
            /**
             * @brief 设置或替换定时器触发事件
             * @param function 指定触发事件
             * @see setDelay
             * @see start
             */
            void setEvent(const std::function<void()>& function);
            /**
             * @brief 启用定时器
             * @param loop 是否循环启用定时器（之前的事件触发后将再次启用）
             * @see stop
             * @see enabled
             * @see loop
             * @see count
             */
            void start(bool loop = false);
            /**
             * @brief 禁用定时器
             * @see start
             * @see enabled
             */
            void stop();
            /**
             * @brief 返回是否启用计时器
             */
            bool enabled() const;
            /**
             * @brief 返回是否正在循环使用计时器
             */
            bool loop() const;

            /**
             * @brief 获取触发延迟时长
             * @see setDelay
             */
            uint64_t delay() const;
            /**
             * @brief 更新定时器状态（无需手动调用）
             */
            void update();
            /**
             * @brief 获取触发定时器的总次数
             * @see clearCount
             */
            size_t count() const;
            /**
             * @brief 清零定时器触发总次数
             * @see count
             */
            void clearCount();

        private:
            uint64_t _start_time{0};
            uint64_t _delay{0};
            uint64_t _id{0};
            bool _enabled{false};
            bool _loop{false};
            std::function<void()> _timer_function;
            size_t _triggered_count{0};
            static uint64_t _currentTimeMs();
        };

        /**
         * @class Trigger
         * @brief 触发器
         *
         * 用于触发待执行的事件
         * @note 此组件只能以指针的形式使用
         */
        class Trigger {
            friend class EventSystem;
        public:
            explicit Trigger();
            ~Trigger();
            /**
             * @brief 设置触发条件
             * @param condition 触发条件函数
             */
            void setCondition(const std::function<bool()>& condition);
            /**
             * @brief 设置触发后执行的事件
             * @param function 执行事件函数
             */
            void setEvent(const std::function<void()>& function);
            /**
             * @brief 设置启用/禁用触发器
             * @param enabled 是否启用触发器
             * @see enabled
             */
            void setEnabled(bool enabled);
            /**
             * @brief 是否已启用触发器
             * @return 返回触发器是否已安装于事件系统中
             * @see setEnabled
             */
            bool enabled() const;
            /**
             * @brief 更新触发器状态（无需手动执行）
             */
            void __update();
            /**
             * @brief 手动触发触发器
             */
            void trigger();
            /**
             * @brief 获取当前的触发条件函数
             */
            std::function<bool()>& condition();
            /**
             * @brief 获取当前的触发后执行函数
             */
            std::function<void()>& event();
        private:
            std::function<bool()> _condition;
            std::function<void()> _event;
            bool _enabled{false};
            uint64_t _id{0};
        };

        /**
         * @class Sprite
         * @brief 精灵
         *
         * 用于存储精灵（即纹理）以及如何绘制精灵
         */
        class Sprite {
        public:
            /**
             * @brief 创建一个精灵，无精灵纹理
             * @param name 精灵别名
             * @param renderer 使用的渲染器
             */
            explicit Sprite(const std::string& name, SRenderer* renderer);
            /**
             * @brief 创建一个精灵
             * @param name      精灵别名
             * @param surface   指定的图片
             * @param renderer  使用的渲染器
             */
            Sprite(const std::string& name, SSurface* surface, SRenderer* renderer);
            /**
             * @brief 克隆已有的精灵
             * @param name   精灵别名
             * @param spirit 指定精灵
             */
            Sprite(const std::string& name, const Sprite& spirit);
            /**
             * @brief 克隆并裁剪已有的精灵
             * @param name  精灵别名
             * @param spirit 指定精灵
             * @param clip_pos 裁剪位置（相对坐标）
             * @param clip_size 裁剪大小
             */
            Sprite(const std::string& name, const Sprite& spirit,
                   const Vector2& clip_pos, const Size& clip_size);
            /**
             * @brief 创建一个精灵
             * @param name 精灵别名
             * @param path 精灵纹理路径
             * @param renderer 使用的渲染器
             */
            Sprite(const std::string& name, const std::string& path, SRenderer* renderer);
            /**
             * @brief 创建一个精灵并裁剪
             * @param name 精灵别名
             * @param path 精灵纹理路径
             * @param clip_pos 裁剪位置（相对坐标）
             * @param clip_size 裁剪大小
             * @param renderer 使用的渲染器
             */
            Sprite(const std::string& name, const std::string& path,
                   const Vector2& clip_pos, const Size& clip_size, SRenderer* renderer);
            ~Sprite();
            /**
             * @brief 设置精灵别名
             * @param new_name 新的精灵别名
             */
            void setName(const std::string& new_name);
            /**
             * @brief 获取精灵别名
             */
            std::string name() const;
            /**
             * @brief 设置精灵纹理路径
             * @param new_path 指定图片路径，支持绝对路径或相对路径
             */
            bool setPath(const std::string& new_path);
            /**
             * @brief 获取精灵纹理路径
             */
            std::string path() const;
            /**
             * @brief 检查精灵纹理路径是否有效
             * @param path 指定路径
             * @return 返回 true 表示路径有效
             */
            bool isValid(const std::string& path) const;
            /**
             * @brief 设置渲染器
             * @param renderer  指定新的渲染器
             */
            void setRenderer(SRenderer* renderer);
            /**
             * @brief 获取当前使用的渲染器
             */
            const SRenderer* renderer() const;
            /**
             * @brief 重新调整精灵纹理的尺寸
             * @param width 新的宽度
             * @param height 新的高度
             */
            void resize(float width, float height);
            /**
             * @brief 获取当前精灵纹理的尺寸
             */
            Size size() const;
            /**
             * @brief 获取精灵纹理
             */
            STexture* spirit() const;
            /**
             * @brief 在指定位置上绘制精灵
             * @param pos 指定绘制坐标
             * @see painter
             */
            void draw(const Vector2 &pos, Painter *painter) const;
            /**
             * @brief 在指定位置下绘制临时指定大小的精灵
             * @param pos     指定绘制坐标
             * @param scaled  缩放范围
             * @param painter 指定绘图器
             * @param center  缩放中心点（默认为左上角）
             */
            void draw(const Vector2 &pos, float scaled, Painter *painter,
                      const Vector2 &center = Vector2(0, 0)) const;
            /**
             * @brief 在指定位置下临时裁剪并绘制精灵
             * @param pos       指定绘制坐标
             * @param clip_pos  裁剪坐标（相对坐标）
             * @param clip_size 裁剪大小（相对大小）
             * @param painter   指定绘图器
             */
            void draw(const Vector2 &pos, const Vector2 &clip_pos, const Size &clip_size, Painter *painter) const;

            /**
             * @enum FlipMode
             * @brief 翻转模式
             *
             * 实现简单的水平、垂直镜像翻转，主要用于 Spirit 的绘制
             * @see draw
             * @see Properties
             */
            enum class FlipMode {
                /// 无
                None,
                /// 水平翻转
                HFlip,
                /// 垂直翻转
                VFlip
            };
            /**
             * @brief 在指定位置下临时旋转、翻转并绘制精灵
             * @param pos           指定绘制坐标
             * @param rotate        旋转角度
             * @param painter       指定绘图器
             * @param flipMode      翻转方式（默认为无翻转）
             * @param rotate_center 旋转中心点（默认为左上坐标）
             */
            void draw(const Vector2 &pos, double rotate, Painter *painter, const FlipMode &flipMode = FlipMode::None,
                      const Vector2 &rotate_center = {0, 0}) const;
            /**
             * @brief 在指定位置下，使用颜色通道绘制精灵
             * @param pos           指定位置
             * @param color_alpha   颜色通道（或透明通道）
             * @param painter       指定绘图器
             * @see StdColor
             * @see hexToRGBA
             * @see Painter
             */
            void draw(const Vector2 &pos, const SColor &color_alpha, Painter* painter) const;

            /**
             * @struct Properties
             * @brief 绘制属性（仅用于 Spirit 绘制）
             *
             * 针对多种绘制属性，一次性绘制具有多种属性的精灵
             *
             * @see draw
             */
            struct Properties {
                /// 精灵所属位置
                Vector2 position{0, 0};
                /// 缩放中心点
                Vector2 scaled_center{0, 0};
                /// 缩放倍数
                float scaled{1.0f};
                /**
                 * @brief 渲染颜色通道（可调整 Alpha 透明通道）
                 * @see StdColor
                 * @see hexToRGBA
                 */
                SColor color_alpha{255, 255, 255, 255};
                /// 旋转角度
                double rotate{0.0};
                /// 旋转中心点
                Vector2 rotate_center{0, 0};
                /// 翻转模式
                FlipMode flip_mode{FlipMode::None};
                /// 裁剪模式（是否允许裁剪）
                bool clip_mode{false};
                /// 裁剪位置
                Vector2 clip_pos{0, 0};
                /// 裁剪尺寸、裁剪大小
                Size clip_size{0, 0};
            };
            /**
             * @brief 获取当前精灵的绘制属性，可通过 draw(Painter*) 函数使用当前属性进行绘制
             * @see draw
             */
            Properties* properties() const;
            /**
             * @brief 通过指定的精灵绘制属性进行详细绘制
             *
             * 适合用于需要同时满足多种效果的情况下使用。
             * @param properties 绘制精灵属性
             * @param painter    指定绘图器
             * @see Properties
             * @see Painter
             */
            void draw(const EasyEngine::Components::Sprite::Properties &properties, Painter *painter) const;
            /**
             * @brief 使用精灵自己的绘制属性进行绘制
             * @param painter 指定绘图器
             * @see properties
             */
            void draw(Painter* painter) const;

        private:
            SSurface* _surface;
            STexture* _texture;
            SRenderer* _renderer;
            std::unique_ptr<Properties> _properties;
            std::string _name;
            std::string _path;
            Size _size;
        };

        /**
         * @class SpriteGroup
         * @brief 精灵组合
         *
         * 由多个精灵组合而成的组合，用于绘制比较复杂的多精灵组合体
         */
        class SpriteGroup {
        public:
            explicit SpriteGroup() = default;
            SpriteGroup(const SpriteGroup& group) = default;
            SpriteGroup(const SpriteGroup&& group);
            /**
             * @brief 重新调整整体精灵组合大小
             * @param width  新的宽度
             * @param height 新的高度
             * @see size
             */
            void resize(float width, float height);
            /**
             * @brief 重新调整整体精灵组合大小
             * @param size  给定新的大小
             * @see size
             */
            void resize(const Size& size);
            /**
             * @brief 获取当前精灵组合的整体大小
             * @see resize
             */
            EasyEngine::Size size() const;
            /**
             * @brief 添加精灵
             * @param sprite 指定精灵
             */
            void append(const Sprite &sprite);
            /**
             * @brief 在指定位置上插入精灵
             * @param index 指定位置
             * @param sprite 指定精灵
             */
            void insert(uint32_t index, const Sprite& sprite);
            /**
             * @brief 移除指定名称的精灵
             * @param name 指定的精灵名称
             * @see indexOf
             * @see lastIndexOf
             * @see nameOf
             */
            void remove(const std::string &name);
            /**
             * @brief 移除指定位置的精灵
             * @param index 指定位置
             * @see indexAt
             * @see indexOf
             * @see lastIndexOf
             */
            void remove(uint32_t index = 0);
            /**
             * @brief 替换指定索引的精灵
             * @param index 指定索引位置
             * @param sprite 指定精灵
             * @see indexAt
             * @see indexOf
             * @see lastIndexOf
             */
            void replace(uint32_t index, const Sprite& sprite);
            /**
             * @brief 交换精灵位置
             * @param index1 第一个精灵所在索引
             * @param index2 第二个精灵所在索引
             */
            void swap(uint32_t index1, uint32_t index2);
            /**
             * @brief 交换精灵位置
             * @param sprite1 第一个精灵名称
             * @param sprite2 第二个精灵名称
             */
            void swap(const std::string& sprite1, const std::string& sprite2);
            /**
             * @brief 交换精灵位置
             * @param index 第一个精灵所在索引
             * @param name 第二个精灵的指定名称
             */
            void swap(uint32_t index, const std::string& name);
            /**
             * @brief 获取精灵所在索引
             * @param name 精灵名称
             * @param start 从哪个位置开始
             * @see lastIndexAt
             * @see sprite
             */
            uint32_t indexAt(const std::string& name, uint32_t start = 0);
            /**
             * @brief 获取精灵所在索引
             * @param name 精灵名称
             * @param start 从最后第几个位置开始
             * @see indexAt
             * @see sprite
             */
            uint32_t lastIndexAt(const std::string& name, uint32_t start = 0);
            /**
             * @brief 获取指定索引的精灵
             * @param index 指定索引
             * @see indexAt
             * @see lastIndexOf
             * @see nameOf
             * @see propertiesOf
             */
            Sprite * indexOf(uint32_t index);
            /**
             * @brief 获取指定名称的精灵
             * @param name 指定名称
             * @see indexAt
             * @see indexOf
             * @see propertiesOf
             */
            Sprite * nameOf(const std::string& name);
            /**
             * @brief 根据指定索引获取精灵对应的属性
             * @param index 指定索引
             * @see indexAt
             * @see indexOf
             */
            Sprite::Properties* propertiesOf(uint32_t index);
            /**
             * @brief 根据精灵名称获取精灵对应的属性
             * @param name 指定精灵名称
             * @see indexAt
             * @see indexOf
             */
            Sprite::Properties* propertiesOf(const std::string& name);
            /**
             * @brief 指定绘图器并绘制精灵组
             * @param pos 指定绘制的位置
             * @param painter 指定绘图器
             */
            void draw(const Vector2 &pos, Painter *painter);
            /**
             * @brief 获取当前精灵组合的精灵总个数
             *
             */
            uint32_t count() const;

        private:
            std::vector<std::shared_ptr<Sprite>> _sprites;
        };

        /**
         * @class Animation
         * @brief 动画
         *
         * 由帧组成的精灵动画
         */
        class Animation {
            struct Frame;
        public:
            /**
             * @brief 创建动画
             * @param name 动画别名
             */
            explicit Animation(const std::string& name);
            /**
             * @brief 创建动画
             * @param name 动画别名
             * @param sprite_list 精灵列表
             * @param duration_per_frame 每帧的持续时间（单位：毫秒），默认 50 毫秒
             */
            Animation(const std::string& name, const std::vector<Sprite *> &sprite_list,
                      uint64_t duration_per_frame = 50);
            ~Animation();
            /**
             * @brief 将精灵图像加入到帧
             * @param sprite 指定精灵
             * @param duration 帧持续时间（单位：毫秒），默认 50 毫秒
             * @see insertFrame
             * @see replaceFrame
             */
            void addFrame(EasyEngine::Components::Sprite *sprite, uint64_t duration = 50);
            /**
             * @brief 将精灵图像插入到指定帧
             * @param sprite 指定精灵
             * @param duration 帧持续时间（毫秒），默认 50 毫秒
             * @param frame 指定帧数，默认为第 0 帧
             * @see addFrame
             * @see replaceFrame
             */
            void insertFrame(EasyEngine::Components::Sprite *sprite, uint64_t duration = 50, const size_t frame = 0);
            /**
             * @brief 在指定帧下替换精灵图像
             * @param sprite 新的精灵图像
             * @param frame 指定帧数
             * @param duration 设定持续时间（毫秒）
             */
            void replaceFrame(EasyEngine::Components::Sprite *sprite, const size_t frame,
                              const uint64_t duration);
            /**
             * @brief 移除指定帧对应的图像
             * @param frame 指定帧数，默认为第 0 帧
             */
            void removeFrame(const size_t frame = 0);
            /**
             * @brief 清空所有帧下的图像
             */
            void clearFrames();
            /**
             * @brief 获取该动画下的总帧数
             */
            size_t framesCount() const;
            /**
             * @brief 获取当前帧的持续时间（单位：毫秒）
             * @param frame 指定帧数
             */
            uint64_t durationInFrame(const size_t frame = 0);
            /**
             * @brief 获取指定帧数下的精灵
             * @see addFrame
             * @see insertFrame
             * @see removeFrame
             * @see replaceFrame
             */
            Sprite* sprite(const size_t frame = 0) const;
            /**
             * @brief 绘制动画
             * @param position 指定绘制的位置
             * @param painter 指定绘制的绘制器
             * @note 必须执行，否则将无法在窗口上显示动画
             * @see play
             * @see stop
             */
            void draw(const Vector2& position, Painter* painter);
            /**
             * @brief 播放动画
             * @param loop          是否循环播放动画（默认循环播放）
             * @param start_frame   从哪一帧开始播放（默认从头开始）
             * @note 欲显示绘制动画，需在绘图事件中调用 `draw()` 函数！
             * @see draw
             * @see playLoop
             * @see stop
             * @see currentFrame
             * @see isPlayedAnimation
             */
            void play(bool loop = true, size_t start_frame = 0);
            /**
             * @brief 停止动画
             * @see play
             * @see frame
             * @see isPlayedAnimation
             */
            void stop();

            /**
             * @brief 获取当前动画是否在播放
             * @see play
             * @see stop
             * @see frame
             */
            bool isPlayedAnimation() const;
            /**
             * @brief 获取当前正在绘制的帧数
             *
             */
            size_t currentFrame() const;
            /**
             * @brief 获取当前完整播放动画的总次数
             *
             * 当动画完整播放一次后计数
             * @see clearPlayedCount
             */
            size_t playedCount() const;
            /**
             * @brief 清零完整播放动画次数
             * @see playedCount
             */
            void clearPlayedCount();
            /**
             * @brief 获取当前使用的定时器
             */
            Timer * timer();
        private:
            /**
             * @struct Frame
             * @brief 动画帧
             * 
             * 每帧动画包含的精灵、帧持续时间
             */
            struct Frame {
                /// 精灵
                Sprite* sprite;
                /// 帧持续时间
                uint64_t duration;
            };
            std::vector<Frame> _animations;
            std::string _name;
            size_t _cur_frame{0};
            bool _is_loop{false};
            size_t _played{0};
            Timer* _frame_changer{nullptr};
        };

        /**
         * @class Element
         * @brief 存储元素
         *
         * 用于存储 Sprite, Animation, SpriteGroup, ClipSprite 等
         */
        class Element {
            friend class Entity;
            friend class Control;
        private:
            /// @brief 用于区分存储的类型
            ///
            /// 按照 Self 的顺序表示（从 1 开始）
            uint8_t type_id{0};
            /// @union Self
            /// @brief 在元素中存储的指针或数据
            union Self {
                explicit Self() {}
                ~Self() {}
                /// @brief 精灵 1
                std::shared_ptr<Sprite> sprite;
                /// @brief 精灵组合 2
                std::shared_ptr<SpriteGroup> sprite_group;
                /// @brief 动画 3
                std::shared_ptr<Animation> animation;
                /// @brief 裁剪精灵 4
                GeometryF clip_sprite;
            };
            Self self{};
        };

        /**
         * @class Collider
         * @brief 碰撞器
         *
         */
        class Collider {
            /**
             * @union Container
             * @brief 碰撞器容器
             *
             * 用于选择使用哪种形状的碰撞器
             */
            struct Container {
                uint8_t mode{0};
                union Shape {
                    /// 矩形 1
                    Graphics::Rectangle rectangle;
                    /// 椭圆 2
                    Graphics::Ellipse ellipse;
                    /// 圆点 3
                    Graphics::Point point;
                };
                Shape shape{};
            };
        public:
            /**
             * @brief 创建碰撞器
             */
            explicit Collider();
            /**
             * @brief 创建碰撞器（相对位置下）
             * @param rect 矩形碰撞器
             */
            explicit Collider(const Graphics::Rectangle& rect);
            /**
             * @brief 创建碰撞器
             * @param ellipse 椭圆碰撞器
             */
            explicit Collider(const Graphics::Ellipse& ellipse);
            /**
             * @brief 创建碰撞器
             * @param point 圆点碰撞器
             */
            explicit Collider(const Graphics::Point& point);
            /**
             * @brief 创建碰撞器（相对位置下）
             * @param x       所在位置 x 轴
             * @param y       所在位置 y 轴
             * @param width   碰撞器的宽度
             * @param height  碰撞器的高度
             * @param shape_mode 碰撞器形状（1 = 矩形，2 = 椭圆，3 = 圆点）
             */
            Collider(float x, float y, float width, float height, uint8_t shape_mode);
            /**
             * @brief 检查此碰撞器是否与其它碰撞器存在碰撞？
             * @param collider  指定碰撞器（除自己以外）
             * @return `< 0` 表示未碰撞，`>= 0` 表示接触或已碰撞
             * @see compareRect
             * @see compareEllipse
             * @see bounds
             */
            int8_t check(const EasyEngine::Components::Collider &collider) const;
            /**
             * @brief 获取碰撞器的位置、大小
             * @see setBoundsGeometry
             */
            const GeometryF & bounds() const;
            /**
             * @brief 设置碰撞器的位置、大小
             * @param geometry 设定新的位置、大小
             * @see bounds
             */
            void setBoundsGeometry(const GeometryF& geometry);
            /**
             * @brief 设置碰撞器的位置、大小
             * @param position 设定新的位置
             * @param size     设定新的大小
             * @see bounds
             */
            void setBoundsGeometry(const Vector2& position, const Size& size);
            /**
             * @brief 设置碰撞器的大小、位置
             * @param x         设定新的位置横坐标
             * @param y         设定新的位置纵坐标
             * @param width     设定新的宽度
             * @param height    设定新的高度
             */
            void setBoundsGeometry(float x, float y, float width, float height);
            /**
             * @brief 移动碰撞器的位置
             * @param position 指定新的位置
             * @see resizeBounds
             * @see setBoundsGeometry
             */
            void moveBounds(const Vector2& position);
            /**
             * @brief 移动碰撞器的位置
             * @param x 指定位置横坐标
             * @param y 指定位置纵坐标
             * @see resizeBounds
             * @see setBoundsGeometry
             */
            void moveBounds(float x, float y);
            /**
             * @brief 重新调整碰撞器的大小
             * @param size 指定新的大小
             * @see moveBounds
             * @see setBoundsGeometry
             */
            void resizeBounds(const Size& size);
            /**
             * @brief 重新调整碰撞器的大小
             * @param width   指定新的宽度
             * @param height  指定新的高度
             * @see moveBounds
             * @see setBoundsGeometry
             */
            void resizeBounds(float width, float height);
            /**
             * @brief 设置是否允许启用碰撞器
             * @param v 碰撞器开关
             */
            void setEnabled(bool v);
            /**
             * @brief 获取碰撞器是否正在启用
             * @see setEnabled
             */
            bool enabled() const;
            /**
             * @brief 设置碰撞器本体并自动设置位置和大小
             * @param rect 指定矩形
             */
            void setSelf(const Graphics::Rectangle& rect);
            /**
             * @brief 设置碰撞器本体并自动设置位置和大小
             * @param ellipse 指定椭圆
             */
            void setSelf(const Graphics::Ellipse& ellipse);
            /**
             * @brief 设置碰撞体本体并自动设置位置和大小
             * @param point 指定点
             */
            void setSelf(const Graphics::Point& point);
            /**
             * @brief 查看是否可用当前碰撞器？
             * 
             */
            bool isValid() const;
            /**
             * @brief 获取碰撞器的形状类型
             * @return const std::type_info& 碰撞器的形状类型
             * @note 对于未定义的形状，返回 `std::type_info(typeid(void))`
             */
            const std::type_info& shapeType() const;

            /**
             * @brief 获取碰撞器的形状
             * @tparam Shape 指定的形状
             * @note 可支持的形状：Rectangle、Ellipse、Point
             * @warning 若指定的形状与现有的形状不匹配，将强制报错并退出！
             */
            template<class Shape>
            const Shape* shape() const {
                static_assert(std::is_same_v<Shape, Graphics::Rectangle> ||
                              std::is_same_v<Shape, Graphics::Ellipse> ||
                              std::is_same_v<Shape, Graphics::Point>, "[ERROR] The specified shape is not supported!");
                if constexpr (std::is_same_v<Shape, Graphics::Rectangle>) {
                    if (_con.mode == 1) return &_con.shape.rectangle;
                } else if constexpr (std::is_same_v<Shape, Graphics::Ellipse>) {
                    if (_con.mode == 2) return &_con.shape.ellipse;
                } else if constexpr (std::is_same_v<Shape, Graphics::Point>) {
                    if (_con.mode == 3) return &_con.shape.point;
                }
                throw std::runtime_error("[ERROR] The specified shape is not match the current shape!");
            }
        private:
            Container _con;
            bool _enabled;
            GeometryF _geometry;
        };

        /**
         * @class Entity
         * @brief 游戏实体
         *
         * 用于作为游戏中的实体
         */
        class Entity {
            friend class Element;
        public:
            /**
             * @brief 创建一个游戏实体
             * @param name 指定实体名称
             */
            explicit Entity(const std::string& name);
            Entity(const std::string& name, const Sprite& sprite);
            Entity(const std::string& name, const SpriteGroup& group);
            Entity(const std::string& name, const Animation& animation);
            Entity(const std::string& name, const Sprite& sprite, const GeometryF& clip);
            /**
             * @brief 设置实体名称
             * @param name 新的实体名称
             */
            void setName(const std::string& name);
            /**
             * @brief 获取实体名称
             */
            std::string name() const;
            /**
             * @brief 设置碰撞器本身形状
             * @param rect 指定矩形
             * @note 使用此函数后将自动按照实体调整大小及位置并自动启用碰撞器
             */
            void setColliderSelf(const Graphics::Rectangle& rect);
            /**
             * @brief 设置碰撞器本身形状
             * @param ellipse 指定椭圆
             * @note 使用此函数后将自动按照实体调整大小及位置并自动启用碰撞器
             */
            void setColliderSelf(const Graphics::Ellipse& ellipse);
            /**
             * @brief 设置碰撞器本身形状
             * @param point 指定圆点
             * @note 使用此函数后将自动按照实体调整大小及位置并自动启用碰撞器
             */
            void setColliderSelf(const Graphics::Point& point);
            /**
             * @brief 设置实体在窗口中所在的位置
             * @param pos
             */
            void setPosition(Vector2 pos);
            /**
             * @brief 设置实体在窗口中所在的位置
             * @param x
             * @param y
             */
            void setPosition(float x, float y);
            /**
             * @brief 设置实体的中心点
             * @param pos
             */
            void setCenterPosition(Vector2 pos);
            /**
             * @brief 设置实体的中心点
             * @param x
             * @param y
             */
            void setCenterPosition(float x, float y);
            /**
             * @brief 获取对象位于窗口的坐标
             */
            Vector2 position() const;
            /**
             * @brief 获取对象的中心点
             */
            Vector2 centerPosition() const;
            /**
             * @brief 获取实体碰撞器
             */
            Collider* collider() const;

            /**
             * @brief 获取实体本身
             * @tparam Type 指定实体类型
             * @return 返回对应类型的本体
             * @note 目前支持的类：Sprite、SpriteGroup、Animation、GeometryF
             * @note 可通过使用 typeInfo() 判断当前本体的类型
             * @see typeInfo
             */
            template<class Type>
            Type* self() const;

            /**
             * @brief 获取当前实体的类型
             * @return 返回一个类型信息，可用于 self() 上获取本体
             * @note 对于未存放本体的游戏实体，将返回 void 类型
             * @see self
             */
            const std::type_info & typeInfo() const;
            void update(Painter* painter) const;

        private:
            Vector2 _pos, _center_pos;
            std::unique_ptr<Collider> _collider;
            std::string _obj_name;
            std::shared_ptr<Element> _container;
            std::unique_ptr<Sprite> _def_sprite;
        };

        /**
         * @brief 获取游戏实体的本体
         * @tparam Type 指定类型
         * @return 返回指定类型的本体
         * @note 可支持的类：Sprite、SpriteGroup、Animation、GeometryF
         * @warning 若指定的类型与现有本体的类型不一致，将强制报错并退出！
         */
        template<class Type>
        Type *EasyEngine::Components::Entity::self() const {
            if constexpr (std::is_same_v<Type, Sprite>) {
                if (_container->type_id == 1) return _container->self.sprite.get();
            } else if constexpr (std::is_same_v<Type, SpriteGroup>) {
                if (_container->type_id == 2) return _container->self.sprite_group.get();
            } else if constexpr (std::is_same_v<Type, Animation>) {
                if (_container->type_id == 3) return _container->self.animation.get();
            } else if constexpr (std::is_same_v<Type, GeometryF>) {
                if (_container->type_id == 4) return &_container->self.clip_sprite;
            } else {
                static_assert(std::is_same_v<Type, Sprite> ||
                        std::is_same_v<Type, SpriteGroup> ||
                        std::is_same_v<Type, Animation> ||
                        std::is_same_v<Type, GeometryF>, "[ERROR] Can't support the specified type!");
            }
            throw std::runtime_error("[ERROR] The specified type is not match the current entity!\n");
        }

        /**
         * @class Control
         * @brief 控件
         *
         * 适用于 UI 界面的操作控件
         */
        class Control {
            friend class Element;
        public:
            /**
             * @enum Status
             * @brief 控件状态
             */
            enum class Status {
                /// 默认
                Default,
                /// 活动
                Active,
                /// 鼠标经过
                Hovered,
                /// 鼠标按下
                Pressed,
                /// 禁用
                Disabled
            };
            /**
             * @enum Event
             * @brief 控件触发事件
             *
             * 罗列了可能的触发事件
             */
            enum class Event {
                /// 空，用于占位，不作任何触发
                None,
                /// 获取焦点事件
                GetFocus = 0x10,
                /// 失去焦点事件
                LostFocus,
                /// 控件重新调整大小事件
                Resized,
                /// 控件移动事件
                Moved,
                /// 控件移动、重新调整大小同时改变事件
                MovedResized,
                /// 控件可用改变事件
                EnabledChange,
                /// 鼠标单击事件
                Clicked,
                /// 鼠标双击事件
                DblClicked,
                /// 鼠标按下事件
                MouseDown,
                /// 鼠标松开事件
                MouseUp,
                /// 鼠标经过控件事件
                MouseHover,
                /// 鼠标离开控件事件
                MouseLeave,
                /// 按键盘事件
                KeyPressed,
                /// 键盘按下事件
                KeyDown,
                /// 键盘松开事件
                KeyUp
            };
            /**
             * @brief 创建控件
             * @param name 创建时需给定名称
             */
            explicit Control(const std::string& name);
            /**
             * @brief 指定一个精灵并创建控件
             * @param name  创建时需给定名称
             * @param sprite 指定的精灵（作为裁剪精灵）
             */
            Control(const std::string& name, const Sprite&& sprite);
            /**
             * @brief 完全克隆已有的控件
             * @param name  创建时需给定名称
             * @param control 原有的控件
             */
            Control(const std::string& name, const Control& control);
            ~Control();

            /**
             * @brief 设定一个控件名称
             * @param name 控件名称
             */
            void setName(const std::string& name);
            /**
             * @brief 获取该控件的名称
             */
            const std::string& name() const;
            /**
             * @brief 设定控件指定状态
             * @param status 选择任一状态
             * @param sprite 在当前状态下绘制成什么精灵
             * @see status
             * @see removeStatus
             */
            void setStatus(const EasyEngine::Components::Control::Status &status, Sprite *sprite);
            /**
             * @brief 设定控件指定状态
             * @param status 选择任一状态
             * @param sprite_group 在当前状态下绘制什么精灵组
             * @see status
             * @see removeStatus
             */
            void setStatus(const EasyEngine::Components::Control::Status &status, SpriteGroup *sprite_group);
            /**
             * @brief 设定控件指定状态
             * @param status 选择任一状态
             * @param sprite 在当前状态下绘制成什么精灵动画
             * @see status
             * @see removeStatus
             */
            void setStatus(const EasyEngine::Components::Control::Status &status, Animation *animation);
            /**
             * @brief 设定控件指定状态
             * @param status 选择任一状态
             * @param clip_sprite 在当前状态下定义裁剪精灵
             * @see status
             * @see removeStatus
             */
            void setStatus(const Status &status, const GeometryF& clip_sprite);
            /**
             * @brief 移除指定的控件状态
             * @param status 选择任一状态以移除
             * @see status
             */
            void removeStatus(const enum Status& status);

            /**
             * @brief 获取当前指定状态下的精灵、组、动画
             * @tparam Type 指定类型
             * @param status 选择任一状态
             * @return 返回当前状态下的精灵、组、动画
             * @note 目前支持的类：Sprite、SpriteGroup、Animation、GeometryF
             * @note 如果无法确定当前状态下使用的类，请使用 getTypename() 以获取该状态下使用的类。
             * @warning 若指定的类型与对应状态下的现有类型不一致，将强制报错并异常退出！
             * @see setStatus
             * @see removeStatus
             * @see typeInfo
             * @see getTypename
             */
            template<class Type>
            Type* status(const enum Status& status) const {
                if (!_container_list.contains(status)) {
                    SDL_Log("[ERROR] Status %d not found in Control '%s'", static_cast<int>(status), _name.c_str());
                    return nullptr;
                }
                auto _con = _container_list.at(status).get();
                if constexpr (std::is_same_v<Type, Sprite>) {
                    if (_con->type_id == 1) return _con->self.sprite.get();
                } else if constexpr (std::is_same_v<Type, SpriteGroup>) {
                    if (_con->type_id == 2) return _con->self.sprite_group.get();
                } else if constexpr (std::is_same_v<Type, Animation>) {
                    if (_con->type_id == 3) return _con->self.animation.get();
                } else if constexpr (std::is_same_v<Type, GeometryF>) {
                    if (_con->type_id == 4) return &_con->self.clip_sprite;
                } else {
                    static_assert(std::is_same_v<Type, Sprite> ||
                                  std::is_same_v<Type, SpriteGroup> ||
                                  std::is_same_v<Type, Animation> ||
                                  std::is_same_v<Type, GeometryF>,
                                  "[ERROR] Unsupported type for Control::status()");
                }
                throw std::runtime_error("[ERROR] The specified type is not match the current type!");
            }
            /**
             * @brief 获取当前指定状态下的类型
             * @param status 指定控件的状态
             * @return 返回类型名称，若没有当前状态，将返回空字符串
             * @retval Sprite
             * @retval SpriteGroup
             * @retval Animation
             * @retval Unknown
             * @see status
             */
            const char* getTypename(const enum Status &status) const;
            /**
             * @brief 获取其指定状态下的类型
             * @note 对于指定状态下不存在的本体，将会返回 `void` 类型。
             *
             * 可搭配 status() 函数一起使用！
             * @see status
             */
            const std::type_info& typeInfo(const enum Status &status) const;

            /**
             * @brief 指定某一事件设定触发函数（即：触发器）
             * @param event     指定事件
             * @param function  触发函数
             * @see event
             * @see removeEvent
             * @see Trigger
             */
            void setEvent(const enum Event& event, const std::function<void()>& function);
            /**
             * @brief 指定某一事件设定触发函数（即：触发器）
             * @param event     指定事件
             * @param condition 触发条件（额外条件）
             * @param function  触发函数
             * @see event
             * @see removeEvent
             * @see Trigger
             */
            void setEvent(const enum Event& event, const std::function<bool()>& condition,
                          const std::function<void()>& function);
            /**
             * @brief 移除指定事件下的触发器
             * @param event 指定事件
             * @see event
             * @see setEvent
             */
            void removeEvent(const enum Event& event);

            /**
             * @brief 设定该控件是否可用
             * @param enabled 设定控件是否能被使用
             */
            void setEnabled(bool enabled);
            /**
             * @brief 获取当前控件是否可用
             *
             */
            bool enabled() const;
            /**
             * @brief 设置当前控件为活动状态
             * @see setInactive
             */
            void setActive();
            /**
             * @brief 设置当前控件为非活动状态
             * @see setActive
             */
            void setInactive();
            /**
             * @brief 获取当前控件是否属于活动状态
             */
            bool active() const;

            /**
             * @brief 移动控件的位置
             * @param pos 指定位置
             * @see position
             */
            void move(const Vector2& pos);
            /**
             * @brief 移动控件的位置
             * @param x 指定横坐标
             * @param y 指定纵坐标
             * @see position
             */
            void move(float x, float y);
            /**
             * @brief 获取当前控件所在的位置
             * @see move
             * @see setGeometry
             */
            Vector2 position() const;
            /**
             * @brief 重新调整控件的大小
             * @param size 新的控件大小
             * @see size
             */
            void resize(const Size& size);
            /**
             * @brief 重新调整控件的大小
             * @param width 新的控件宽度
             * @param height 新的控件高度
             * @see size
             */
            void resize(float width, float height);
            /**
             * @brief 获取当前控件的大小
             * @see resize
             * @see setGeometry
             */
            Size size() const;
            /**
             * @brief 设定控件的位置、大小
             * @param pos   指定控件所处的位置
             * @param size  指定控件的大小
             * @see position
             * @see size
             */
            void setGeometry(const Vector2& pos, const Size& size);
            /**
             * @brief 设定控件的位置、大小
             * @param x         指定控件所处位置的横坐标
             * @param y         指定控件所处位置的纵坐标
             * @param width     指定控件的宽度
             * @param height    指定控件的高度
             * @see position
             * @see size
             */
            void setGeometry(float x, float y, float width, float height);
            /**
             * @brief 设定热区的位置、大小
             * @param pos   指定热区位置（相对坐标）
             * @param size  指定热区大小
             * @see hotArea
             */
            void setGeometryForHotArea(const Vector2& pos, const Size& size);
            /**
             * @brief 设定热区的位置、大小
             * @param x         指定热区位置（相对坐标）横坐标
             * @param y         指定热区位置（相对坐标）纵坐标
             * @param width     指定热区的宽度
             * @param height    指定热区的高度
             * @see hotArea
             */
            void setGeometryForHotArea(float x, float y, float width, float height);
            /**
             * @brief 获取热区
             * @see setGeometryForHotArea
             */
            const Graphics::Rectangle& hotArea() const;
            /**
             * @brief 更新控件状态（无需手动调用）
             */
            void update(Painter *painter);
            void __updateStatus(const Status& status);
            void __updateEvent(const Event& event);
            Status __currentStatus() const;
            Event __currentEvent() const;
        private:
            std::string _name;
            std::shared_ptr<Sprite> _def_sprite;
            std::map<Status, std::shared_ptr<Element>> _container_list;
            std::map<Event, std::shared_ptr<Trigger>> _trigger_list;
            Status _status{Status::Default};
            Event _event{Event::None};
            bool _active{false};
            Vector2 _position, _hot_position;
            Size _size;
            Graphics::Rectangle _hot_area;
        };

        /**
         * @class Font
         * @brief 字体
         *
         * 包含字体相关的信息
         */
        class Font {
        public:
            /**
             * @enum Style
             * @brief 字体样式
             */
            enum Style {
                /// 常规样式
                Regular = 0x0,
                /// 粗体样式
                Bold = 0x1,
                /// 斜体样式
                Italic = 0x2,
                /// 下划线样式
                Underline = 0x4,
                /// 删除线样式
                Strikethrough = 0x8
            };
            /**
             * @enum Direction
             * @brief 字体方向
             */
            enum Direction {
                /// 从左到右
                LeftToRight = 4,
                /// 从右到左
                RightToLeft,
                /// 从上到下
                TopToBottom,
                /// 从下到上
                BottomToTop
            };
            /**
             * @enum 字体微调
             */
            enum Hinting {
                Normal,
                Light,
                Mono,
                None,
                SubPixel
            };
        public:
            /**
             * @brief 创建并打开字体
             * @param path      指定路径
             * @param font_size 字体大小（按点为单位）
             * @see load
             */
            explicit Font(const std::string& path, float font_size);
            ~Font();
            /**
             * @brief 加载字体
             * @param path      指定路径
             * @param font_size 字体大小（按点为单位）
             * @return 返回是否成功加载字体
             */
            bool load(const std::string& path, float font_size);
            /**
             * @brief 卸载字体
             * @note 必须执行，否则将无法完全卸载字体！
             */
            void unload();
            /**
             * @brief 设置字体大小
             * @param size 字体大小
             */
            void setFontSize(float size);
            /**
             * @brief 获取当前字体大小
             */
            float fontSize() const;
            /**
             * @brief 设置字体样式
             * @param style 指定样式
             */
            void setStyle(uint32_t style);
            /**
             * @brief 设置字体描边
             * @param value 设定值（0 = 无描边）
             */
            void setOutline(uint32_t value = 0);
            /**
             * @brief 获取字体描边值
             * @return 返回描边值，0 表示无描边
             */
            uint32_t outline() const;
            /**
             * @brief 设置字体描边颜色
             * @param color 指定描边颜色
             */
            void setOutlineColor(const SColor& color);
            /**
             * @brief 获取字体描边颜色
             */
            const SColor& outlineColor() const;
            /**
             * @brief 设置字体方向
             * @param direction 指定字体方向
             */
            void setFontDirection(const Direction& direction);
            /**
             * @brief 获取字体方向
             */
            const Direction& fontDirection() const;
            /**
             * @brief 设置字体颜色
             * @param color 指定颜色
             */
            void setFontColor(const SColor& color);
            /**
             * @brief 获取字体颜色
             */
            const SColor& fontColor() const;
            /**
             * @brief 设置字体微调
             * @param hinting 指定的字体微调
             */
            void setFontHinting(uint32_t hinting);
            /**
             * @brief 设置是否允许字体间距
             * @param enabled 允许/禁用字体间距
             */
            void setFontKerning(bool enabled);
            /**
             * @brief 获取当前字体下是否允许字体间距
             */
            bool fontKerning() const;
            /**
             * @brief 设置行间距
             * @param spacing   间距值
             */
            void setLineSpacing(uint32_t spacing);
            /**
             * @brief  获取行间距
             */
            uint32_t lineSpacing() const;
            /**
             * @brief 文本转换精灵
             * @param text 指定内容
             */
            Sprite textToSprite(const std::string &text, EasyEngine::Painter &painter);

        private:
            /// 字体指针
            TTF_Font* _font;
            float _font_size;
            SColor _font_color;
            uint32_t _font_style_flags;
            uint32_t _font_outline;
            SColor _outline_color;
            Direction _font_direction;
            uint32_t _font_hinting;
            bool _font_kerning;
            uint32_t _line_spacing;
        };
    }
}

#include "Core.h"

#endif //EASYENGINE_COMPONENTS_H
