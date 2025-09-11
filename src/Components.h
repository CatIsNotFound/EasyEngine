
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

using SSurface = SDL_Surface;
using STexture = SDL_Texture;
using SRenderer = SDL_Renderer;
using SFRect = SDL_FRect;

namespace EasyEngine {
    class Painter;
    class EventSystem;
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

        class Video {
        public:

        private:

        };

        /**
         * @class Timer
         * @brief 定时器
         *
         * 用于设置延迟或定时的工具
         * @note 此组件只能使用指针的形式创建！
         */
        class Timer {
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
            void update();
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
            void addFrame(const Sprite& sprite, uint64_t duration = 50);
            /**
             * @brief 将精灵图像插入到指定帧
             * @param sprite 指定精灵
             * @param duration 帧持续时间（毫秒），默认 50 毫秒
             * @param frame 指定帧数，默认为第 0 帧
             * @see addFrame
             * @see replaceFrame
             */
            void insertFrame(const Sprite& sprite, uint64_t duration = 50, const size_t frame = 0);
            /**
             * @brief 在指定帧下替换精灵图像
             * @param sprite 新的精灵图像
             * @param frame 指定帧数
             * @param duration 设定持续时间（毫秒）
             */
            void replaceFrame(const EasyEngine::Components::Sprite &sprite, const size_t frame,
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
                std::unique_ptr<Sprite> sprite;
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

        class Collider {
        public:
            /**
             * @brief 创建碰撞器
             */
            explicit Collider();
            /**
             * @brief 创建碰撞器（相对位置下）
             * @param rect 矩形碰撞器
             */
            Collider(SFRect rect);
            /**
             * @brief 创建碰撞器（相对位置下）
             * @param x       所在位置 x 轴
             * @param y       所在位置 y 轴
             * @param width   碰撞器的宽度
             * @param height  碰撞器的高度
             */
            Collider(float x, float y, float width, float height);
            /**
             * @brief 检查此碰撞器是否与其它碰撞器碰撞
             * @param collider  指定碰撞器（除自己以外）
             * @return 返回 true 表示已与其它碰撞器碰撞，其反之
             * @see bounds
             */
            bool check(const Collider& collider);
            /**
             * @brief 获取碰撞器的位置、大小
             */
            SFRect bounds() const;
        private:
            SFRect* rect;
        };

        class Entity {
        public:
            /**
             * @brief 创建一个游戏实体
             * @param name 指定实体名称
             */
            explicit Entity(const std::string& name);
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
             * @brief 设置碰撞器开关
             * @param enabled 是否启动碰撞器
             * @see isColliderEnabled
             */
            void setColliderEnabled(const bool enabled);
            /**
             * @brief 碰撞器是否开启
             * @return 返回 `true` 表示已开启碰撞器
             * @see setColliderEnabled
             */
            bool isColliderEnabled() const;
            /**
             * @brief 检查实体是否与其它实体碰撞
             * @param entity 指定实体
             * @return 返回是否碰撞
             * @note 使用前，须先开启碰撞器功能才能使用，否则无论如何都会返回为 false
             * @see isColliderEnabled
             * @see setColliderEnabled
             */
            bool isCollision(const Entity& entity);
        private:
            Vector2 _pos, _center;
            std::map<std::string, Animation> _animators;
            Collider _collider;
            std::string _obj_name;
            bool _is_collider{false};
        };
    }
}

#include "Core.h"

#endif //EASYENGINE_COMPONENTS_H
