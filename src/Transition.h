
#ifndef EASYENGINE_TRANSITION_H
#define EASYENGINE_TRANSITION_H
#include "Components.h"

/**
 * @file Transition.h
 * @brief 过渡效果模块
 *
 * 定义相关过渡效果、动画曲线的模块
 *
 * @copyright Copyright © 2025 CatIsNotFound
 * @author CatIsNotFound
 */

namespace EasyEngine {
    /**
     * @namespace EasingCurve
     * @brief 过渡曲线
     *
     * 用于定义过渡动画的过程
     * @since v1.1.0-alpha
     */
    namespace EasingCurve {
        /**
         * @enum Direction
         * @brief 过渡曲线方向
         */
        enum Direction {
            /// 顺流
            Forward,
            /// 倒流
            Backward
        };
        /**
         * @class AbstractEasingCurve
         * @brief 抽象过渡曲线
         *
         * 用于实现基本的过渡曲线
         * @since v1.1.0-alpha
         */
        class AbstractEasingCurve {
        public:
            /**
             * @brief 定义抽象过渡曲线
             * @param duration 设定持续时长（毫秒）
             */
            explicit AbstractEasingCurve(uint32_t duration, Painter* painter = nullptr);
            /**
             * @brief 设置持续时长
             * @param duration 持续时长（毫秒）
             */
            void setDuration(uint32_t duration);
            /**
             * @brief 设置过渡曲线的方向
             * @param direction 指定顺流（Forward）或倒流（Backward）
             */
            void setDirection(const enum Direction& direction);
            /**
             * @brief 是否启用过渡
             * @param enabled 决定是否开始过渡
             * @param loop    是否循环使用过渡
             * @param change_direction 当完成一次过渡曲线后，是否切换过渡方向（类似于：顺流->倒流->顺流...）
             * @note 当 `loop` 参数设为 `true` 时，`change_direction` 参数才会有用
             */
            void setEnabled(bool enabled, bool loop = false, bool change_direction = false);
            /**
             * @brief 获取当前过渡的进程
             * @return 返回一个当前过渡的百分比，如：75% -> 0.75f
             */
            float current() const;
            /**
             * @brief 获取当前的过渡时间
             * @return 返回从开始到现在所持续的总时间（毫秒）
             */
            uint32_t elapsedTime() const;
            /**
             * @brief 获取当前过渡曲线的持续时长
             * @return 返回从开始到结束的持续时长（毫秒）
             */
            uint32_t duration() const;
            /**
             * @brief 获取当前过渡时的方向
             * @return 返回当前是顺流（Forward）还是倒流（Backward）
             */
            const enum Direction& direction() const;
            /**
             * @brief 获取当前是否正在过渡
             */
            bool enabled() const;
            void ______();

        protected:
            /**
             * @brief 编辑过渡曲线
             *
             * 当过渡曲线正在过渡时，执行此函数，可随时变化动画进度
             */
            virtual void update() = 0;
            /**
             * @brief 初始化曲线
             *
             * 当开始执行过渡曲线时，将先执行此函数
             */
            virtual void getReady() = 0;
            float _position{0};
        private:
            Painter* _painter;
            uint32_t _duration;
            uint32_t _start_time{0}, _current_time{0};
            enum Direction _direction{Forward}, _cur_direction{Forward};
            bool _enabled{false}, _loop{false}, _change_direction{false}, _added_list{false};
        };

        /**
         * @class LinearCurve
         * @brief 线性曲线
         *
         * 匀速运动，速度始终不变
         * @since v1.1.0-alpha
         */
        class LinearCurve : public AbstractEasingCurve {
        public:
            explicit LinearCurve(uint32_t duration, Painter *painter);

        protected:
            void update() override;
            void getReady() override;
        };

        /**
         * @class InQuadCurve
         * @brief 二次方淡入曲线
         *
         * 开始慢，后逐渐加速（曲线先平缓后陡峭）
         * @since v1.1.0-alpha
         */
        class InQuadCurve : public AbstractEasingCurve {
        public:
            explicit InQuadCurve(uint32_t duration, Painter* painter);

        protected:
            void update() override;
            void getReady() override;
        };

        /**
         * @class InCubicCurve
         * @brief 三次方淡入曲线
         *
         * 与 `InQuadCurve` 基本类似，但比 `InQuadCurve` 加速更剧烈，开始更慢，后期冲刺感强。
         * 适合用于强调 “爆发力” 的动画（如小球落地后弹起的启动阶段）
         * @since v1.1.0-alpha
         */
        class InCubicCurve : public AbstractEasingCurve {
        public:
            explicit InCubicCurve(uint32_t duration, Painter* painter);

        protected:
            void update() override;
            void getReady() override;
        };

        /**
         * @class InSineCurve
         * @brief 正弦淡入曲线
         *
         * 最柔和的加速，接近自然物体的启动
         * @since v1.1.0-alpha
         */
        class InSineCurve : public AbstractEasingCurve {
        public:
            explicit InSineCurve(uint32_t duration, Painter* painter);

        protected:
            void update() override;
            void getReady() override;
        };

        /**
         * @class OutQuadCurve
         * @brief 二次方淡出
         *
         * 开始快，随后逐渐减速（曲线先陡峭后平缓）
         * @since v1.1.0-alpha
         */
        class OutQuadCurve : public AbstractEasingCurve {
        public:
            explicit OutQuadCurve(uint32_t duration, Painter* painter);

        protected:
            void update() override;
            void getReady() override;
        };

        /**
         * @class OutCubicCurve
         * @brief 三次方淡出
         *
         * 与 `OutQuadCurve` 类似，但比 `OutQuadCurve` 减速更明显，前期冲力大，后期 “刹车感” 强
         * @since v1.1.0-alpha
         */
        class OutCubicCurve : public AbstractEasingCurve {
        public:
            explicit OutCubicCurve(uint32_t duration, Painter* painter);

        protected:
            void update() override;
            void getReady() override;
        };

        /**
         * @class OutSineCurve
         * @brief 正弦淡出曲线
         *
         * 最自然的 S 形曲线，速度变化平滑无突兀。适合模拟物理世界的自然运动（如飞鸟掠过、水流波动）
         * @since v1.1.0-alpha
         */
        class OutSineCurve : public AbstractEasingCurve {
        public:
            explicit OutSineCurve(uint32_t duration, Painter* painter);

        protected:
            void update() override;
            void getReady() override;
        };


        /**
         * @class InOutQuadCurve
         * @brief 二次方淡入淡出曲线
         *
         * 先加速、后减速（S 形曲线，中间最快）；
         * 适合往返运动、页面切换等需要 “起承转合” 的动画。
         * @since v1.1.0-alpha
         */
        class InOutQuadCurve : public AbstractEasingCurve {
        public:
            explicit InOutQuadCurve(uint32_t duration, Painter* painter);

        protected:
            void update() override;
            void getReady() override;
        };

        class InOutCubicCurve : public AbstractEasingCurve {
        public:
            explicit InOutCubicCurve(uint32_t duration, Painter* painter);

        protected:
            void update() override;
            void getReady() override;
        };

        class InOutSineCurve : public AbstractEasingCurve {
        public:
            explicit InOutSineCurve(uint32_t duration, Painter* painter);

        protected:
            void update() override;
            void getReady() override;
        };
    }

    /**
     * @namespace Transition
     * @brief 场景过渡效果
     *
     * 包含了几种基本的用于场景过渡的效果
     * @note 与早期测试版本定义方式不同，这里将命名为 `Transition` 作为命名空间，而非单独的抽象类！
     * @since v1.1.0-alpha
     */
    namespace Transition {
        /**
         * @enum State
         * @brief 用于定义过渡时的状态
         */
        enum State {
            /// 停止
            Stopped,
            /// 暂停
            Paused,
            /// 运行
            Running,
        };
        /**
         * @enum Direction
         * @brief 用于表示过渡效果时的过渡顺序
         */
        enum Direction {
            /// 顺流
            Forward,
            /// 倒流
            Backward
        };
        /**
         * @enum DeletionPolicy
         * @brief 删除策略
         */
        enum DeletionPolicy {
            /// 当停止时不删除过渡
            KeepWhenStopped,
            /// 当停止时删除过渡
            DeleteWhenStopped
        };
        /**
         * @class AbstractTransition
         * @brief 抽象过渡效果
         *
         * 用于实现场景的基本过场效果
         * @note 早期版本下此类原名为 `Transition`，随后版本该名称已被作为命名空间。
         * @note 从 v1.1.0-alpha 版本起，此类以重命名为 `AbstractTransition` 类作为抽象过渡效果。
         * @since v1.0.1-beta
         * @since v1.1.0-alpha
         */
        class AbstractTransition {
        public:
            /**
             * @brief 创建过渡效果
             * @param duration          持续时间
             * @param deletion_policy   删除策略
             * @param painter           指定绘图器
             */
            explicit AbstractTransition(uint64_t duration, DeletionPolicy deletion_policy, Painter *painter);

            virtual ~AbstractTransition() = default;

            /**
             * @brief 设置删除策略
             * @param deletion_policy
             *
             * 当过渡效果过渡之后将触发删除策略
             * @see DeletionPolicy
             */
            void setDeletionPolicy(const DeletionPolicy &deletion_policy);

            /**
             * @brief 设置过渡持续时间
             * @param duration 持续时间（按毫秒计算）
             */
            void setDuration(uint64_t duration);

            /**
             * @brief 设置过渡方式
             * @param direction 选择 `Forward` 正向过渡， `Backward` 反向过渡
             */
            void setDirection(const Direction &direction);

            /**
             * @brief 开始过渡
             * @see pause
             * @see stop
             */
            void start();

            /**
             * @brief 暂停过渡
             * @see start
             * stop
             */
            void pause();

            /**
             * @brief 停止过渡
             * @see start
             * @see pause
             */
            void stop();

            /**
             * @brief 获取当前设置的过渡持续时长
             * @see currentTime
             * @see loopCount
             */
            uint64_t duration() const;

            /**
             * @brief 获取当前过渡的时间
             * @see currentTime
             * @see loopCount
             */
            uint64_t currentTime() const;

            /**
             * @brief 获取过渡效果完成循环总次数
             */
            uint64_t loopCount() const;

            /**
             * @brief 清零过渡效果完成循环总次数
             */
            void clearLoopCount();

            /**
             * @brief 获取当前过渡的方式是正向过渡还是反向过渡
             * @retval `Forward` 表示正向过渡（由 A 过渡到 B）
             * @retval `Backward` 表示反向过渡（由 B 过渡到 A）
             */
            const Direction &direction() const;

            /**
             * @brief 获取当前过渡的状态
             * @see State
             */
            const State &state() const;

            /**
             * @brief 获取当前过渡效果的删除策略
             * @see DeletionPolicy
             */
            const DeletionPolicy &deletionPolicy() const;

            /**
             * @brief 仅用于被其它系统处理，禁止手动调用
             */
            void ______();

            /**
             * @brief 检查是否将 A 效果切换到 B 效果
             */
            bool __is_changed();

        protected:
            uint64_t _duration;
            Direction _direction{Forward};
            State _state{Stopped};
            DeletionPolicy _deletion_policy{KeepWhenStopped};
            Painter *_painter{nullptr};
            bool _change_signal{false};
        private:
            /**
             * @brief 编辑过渡效果的过渡过程
             */
            virtual void update() = 0;

            /**
             * @brief 编辑在开始执行过渡之前的事件
             */
            virtual void getReady() = 0;

            uint64_t _start_time{0};
            uint64_t _current_time{0};
            uint64_t _loop_count{0};

        };

        /**
         * @class DarkTransition
         * @brief 黑幕效果
         *
         * 实现最简单的黑幕效果，实现画面由透明到黑或由黑到透明的过渡效果
         */
        class DarkTransition : public AbstractTransition {
        public:
            DarkTransition(uint64_t _duration, DeletionPolicy deletion_policy, Painter *painter);

            ~DarkTransition() override;

            /**
             * @brief 设置黑幕背景颜色
             * @param color 指定背景颜色
             */
            void setBackgroundColor(const SColor &color);

            /**
             * @brief 获取当前黑幕背景颜色
             */
            const SColor &backgroundColor() const;

        private:
            void update() override;
            void getReady() override;
            Graphics::Rectangle _mask;
        };

        /**
         * @class EraseTransition
         * @brief 擦除效果
         *
         * 实现基本的不同方向的擦除效果
         */
        class EraseTransition : public AbstractTransition {
        public:
            /**
             * @enum EraseDirection
             * @brief 擦除方向
             */
            enum EraseDirection {
                LeftToRight, RightToLeft, TopToBottom, BottomToTop
            };

            EraseTransition(uint64_t duration, const enum EraseDirection &direction,
                            DeletionPolicy deletion_policy, Painter *painter);

            ~EraseTransition();

            void setEraseDirection(const enum EraseDirection &direction);

            const enum EraseDirection &eraseDirection() const;

            void setBackColor(const SColor &color);

            const SColor &backColor() const;

        private:
            void update() override;

            void getReady() override;

            Graphics::Rectangle _rect;
            enum EraseDirection _direction{EraseDirection::LeftToRight};
        };

        /**
         * @class MoveTransition
         * @brief 平移过渡效果
         *
         * 实现不同方向的平移效果，需要单张图片进行平移
         */
        class MoveTransition : public AbstractTransition {
        public:
            /**
             * @enum MoveDirection
             * @brief 移动方向
             */
            enum MoveDirection {
                LeftToRight, RightToLeft, TopToBottom, BottomToTop
            };

            explicit MoveTransition(uint32_t duration, const enum MoveDirection &direction,
                                    DeletionPolicy deletion_policy, Painter *painter);

            void setFirstPicture(SSurface *surface);

            void setFirstPicture(Components::Sprite *sprite);

            void clearFirstPicture();

            Components::Sprite *firstPicture() const;

            void setSecondPicture(SSurface *surface);

            void setSecondPicture(Components::Sprite *sprite);

            void clearSecondPicture();

            Components::Sprite *secondPicture() const;

            void setMoveDirection(const enum MoveDirection &direction);

            const enum MoveDirection &moveDirection() const;

        private:
            void update() override;

            void getReady() override;

            std::shared_ptr<Components::Sprite> _sprite1;
            std::shared_ptr<Components::Sprite> _sprite2;
            bool _draw_second_pic{false};
            enum MoveDirection _move_direction;
        };
    }
} // EasyEngine

#endif //EASYENGINE_TRANSITION_H
