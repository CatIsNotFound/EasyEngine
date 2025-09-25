
#ifndef EASYENGINE_TRANSITION_H
#define EASYENGINE_TRANSITION_H
#include "Components.h"

namespace EasyEngine {
    /**
     * @class Transition
     * @brief 过渡效果
     *
     * 用于实现场景的基本过场效果
     */
    class Transition {
    public:
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
         * @brief 创建过渡效果
         * @param duration          持续时间
         * @param deletion_policy   删除策略
         * @param painter           指定绘图器
         */
        explicit Transition(uint64_t duration, DeletionPolicy deletion_policy, Painter *painter);
        virtual ~Transition() = default;
        /**
         * @brief 设置删除策略
         * @param deletion_policy
         *
         * 当过渡效果过渡之后将触发删除策略
         * @see DeletionPolicy
         */
        void setDeletionPolicy(const DeletionPolicy& deletion_policy);
        /**
         * @brief 设置过渡持续时间
         * @param duration 持续时间（按毫秒计算）
         */
        void setDuration(uint64_t duration);
        /**
         * @brief 设置过渡方式
         * @param direction 选择 `Forward` 正向过渡， `Backward` 反向过渡
         */
        void setDirection(const Direction& direction);
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
        const Direction& direction() const;
        /**
         * @brief 获取当前过渡的状态
         * @see State
         */
        const State& state() const;
        /**
         * @brief 获取当前过渡效果的删除策略
         * @see DeletionPolicy
         */
        const DeletionPolicy& deletionPolicy() const;
        /**
         * @brief 仅用于被其它系统处理，禁止手动调用
         */
        void ______();
        /**
         * @brief 检查是否将 A 效果切换到 B 效果
         */
        bool __is_changed_signal();
    protected:
        uint64_t _duration;
        Direction _direction{Forward};
        State _state{Stopped};
        DeletionPolicy _deletion_policy{KeepWhenStopped};
        Painter* _painter{nullptr};
        bool _change_signal{false};
    private:
        virtual void update() = 0;
        uint64_t _start_time{0};
        uint64_t _current_time{0};
        uint64_t _loop_count{0};

    };

    /**
     * @class FadeTransition
     * @brief 淡入淡出效果
     *
     * 实现最简单的淡入淡出效果
     */
    class FadeTransition : public Transition {
    public:
        FadeTransition(uint64_t _duration, DeletionPolicy deletion_policy, Painter *painter);
        ~FadeTransition() override;

        void setFadeColor(const SColor& color);

    private:
        void update() override;
        Graphics::Rectangle _mask;
    };

} // EasyEngine

#endif //EASYENGINE_TRANSITION_H
