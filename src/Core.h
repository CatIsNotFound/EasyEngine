
#pragma once
#ifndef EASYENGINE_CORE_H
#define EASYENGINE_CORE_H

/**
 * @file Core.h
 * @brief 引擎内核
 *
 * 定义引擎核心控制类，负责窗口管理、渲染控制、事件处理等核心流程。
 *
 * @copyright Copyright © 2025 CatIsNotFound
 * @author CatIsNotFound
 */

#include "Basic.h"
#include "Algorithm.h"
#include "Components.h"
#include "Resources.h"
#include "Scene.h"

/// 主版本号（重大功能更新）
#define EASYENGINE_MAJOR_VERSION 1
/// 副版本号（功能新增、优化）
#define EASYENGINE_MINOR_VERSION 1
/// 修订版本号（bug 修复及部分优化）
#define EASYENGINE_MACRO_VERSION 0
/// 版本类型（指定 Alpha/Beta/Release）
#define EASYENGINE_VERSION "Alpha"
/// 项目名称
#define EASYENGINE_NAME "EasyEngine"

/**
 * @namespace EasyEngine
 * @brief 引擎
 *
 * 几乎包含所有引擎相关的类和命名空间。
 * 通过使用 `using namespace EasyEngine;` 可直接使用引擎里的一切东西
 */
namespace EasyEngine {
    /**
     * @class Window
     * @brief 窗口
     *
     * 窗口封装类，关联 SDL 窗口实例、渲染器实例及窗口的几何属性（位置、尺寸）
     */
    class Window {
    public:
        Window(SDL_Window* window, SRenderer* renderer)
            : window(window), renderer(renderer) {};
        Window() : window(nullptr), renderer(nullptr) {}
        ~Window() = default;
        /// 窗口
        SWindow* window;
        /// 渲染器
        SRenderer* renderer;
        /// 窗口大小、尺寸
        Geometry geometry;
    };

    /**
     * @class Cursor
     * @brief 鼠标光标
     *
     */
    class Cursor {
        explicit Cursor();
        static std::unique_ptr<Cursor> _instance;
    public:
        /**
         * @enum StdCursor
         * @brief 标准鼠标光标样式
         *
         * 获取系统下当前使用的鼠标光标
         */
        enum StdCursor {
            Default             = 0x00,
            Normal              = 0x00,
            Edit                = 0x01,
            Wait                = 0x02,
            Crosshair           = 0x03,
            Busy                = 0x04,
            Resize_NWSE         = 0x05,
            Resize_NESW         = 0x06,
            Resize_Horizontal   = 0x07,
            Resize_EW           = 0x07,
            Resize_Vertical     = 0x08,
            Resize_NS           = 0x08,
            Move                = 0x09,
            Not_Allowed         = 0x0a,
            Forbbiden           = 0x0a,
            Hand                = 0x0b,
            Pointer             = 0x0b,
            Resize_NW           = 0x0c,
            Resize_N            = 0x0d,
            Resize_NE           = 0x0e,
            Resize_E            = 0x0f,
            Resize_SE           = 0x10,
            Resize_S            = 0x11,
            Resize_SW           = 0x12,
            Resize_W            = 0x13,
            Count               = 0x14,
            Custom              = 0x15
        };

        /**
         * @struct UserCustom
         * @brief 用户自定义鼠标光标数据
         *
         * 包含热点坐标（点击生效点）和光标表面资源
         */
        struct UserCustom {

            /**
             * @struct HotPoint
             * @brief 用于定义鼠标光标的热点坐标
             */
            struct HotPoint { int x, y; };
            HotPoint hot_point;
            std::shared_ptr<SSurface> cursor;
            UserCustom(const HotPoint& hot_point, SSurface* cursor) {
                this->hot_point.x = hot_point.x;
                this->hot_point.y = hot_point.y;
                this->cursor = std::shared_ptr<SSurface>(cursor);
            }
            UserCustom(const UserCustom& copy) {
                hot_point.x = copy.hot_point.x;
                hot_point.y = copy.hot_point.y;
                cursor = copy.cursor;
            }
        };

    public:
        ~Cursor();
        /**
         * @brief 获取鼠标光标全局
         */
        static Cursor* global();
        /**
         * @brief 获取鼠标光标在显示器中所在的位置
         * @return 返回全局坐标
         * @see position
         * @see move
         */
        Vector2 globalPosition() const;
        /**
         * @brief 获取鼠标光标在获得焦点的窗口下的位置
         * @return 返回鼠标光标相对获得焦点窗口下的位置
         * @see globalPosition
         * @see move
         */
        EasyEngine::Vector2 position() const;
        /**
         * @brief 获取鼠标当前聚焦于哪个窗口
         * @return 返回鼠标所在的窗口 ID
         */
        uint64_t focusOn() const;
        /**
         * @brief 移动鼠标光标至指定位置
         * @param pos 指定位置
         * @param window 指定窗口
         * @note 若不指定 window 参数，将默认以全局显示器屏幕为主。
         * @see position
         * @see globalPosition
         */
        void move(const Vector2& pos, const Window* window = nullptr);
        /**
         * @brief 移动光标至指定位置
         * @param x 指定横坐标
         * @param y 指定纵坐标
         * @param window 指定窗口
         * @note 若不指定 window 参数，将默认以全局显示器屏幕为主。
         * @see position
         * @see globalPosition
         */
        void move(float x, float y, const Window* window = nullptr);
        /**
         * @brief 将鼠标光标移动至画面中心点
         * @param window 指定窗口
         * @note 若不设置 window 参数，将默认以全局显示器屏幕为主
         */
        void moveToCenter(const Window* window = nullptr);
        /**
         * @brief 设置鼠标光标
         * @param cursor 指定光标类型
         * @code
         * Cursor::global()->setCursor(Cursor::Hand);
         * @endcode
         * @note 对于已经开启自定义鼠标光标的情况，将会改变对应类型的自定义光标；
         * 若指定对应的光标类型没有定义鼠标光标，则会使用系统对应的鼠标光标！
         * @see cursor
         * @see userCustomEnabled
         * @see StdCursor
         */
        void setCursor(const StdCursor& cursor);
        /**
         * @brief 设置自定义鼠标光标
         * @param path 指定路径下加载鼠标光标
         * @param hot_x 中心点横坐标
         * @param hot_y 中心点纵坐标
         * @note 若指定的 `path` 参数无效，则无法设置！
         *
         * 从指定路径加载图像作为自定义鼠标光标，并设置热点坐标。
         */
        void setCursor(const std::string &path, int hot_x, int hot_y);
        /**
         * @brief 是否允许使用自定义鼠标光标
         * @param enabled 启用/禁用
         */
        void setUserCustomEnabled(bool enabled);
        /**
         * @brief 当前是否使用自定义鼠标光标
         *
         */
        bool userCustomEnabled() const;
        /**
         * @brief 设置自定义鼠标光标
         * @param stdCursor 指定标准鼠标类型
         * @param path      指定本地路径
         * @param hot_point 指定热点
         * @see setUserCustomEnabled
         * @see userCustomEnabled
         */
        void setCustomCursor(const StdCursor& stdCursor, const std::string& path, const UserCustom::HotPoint& hot_point);
        /**
         * @brief 获取当前鼠标光标样式
         *
         */
        StdCursor cursor() const;
        /**
         * @brief 显示/隐藏鼠标光标
         * @param visible 决定是否显示鼠标光标
         * @see visible
         */
        void setVisible(bool visible);
        /**
         * @brief 获取当前鼠标光标是否显示
         * @see setVisible
         */
        bool visible() const;
        void unload();

    private:
        StdCursor _std_cursor{Custom};
        SCursor* _cursor{nullptr};
        SSurface* _surface{nullptr};
        bool _custom_cursor{false};
        std::map<StdCursor, UserCustom> _user_custom;
        bool _visible{true};
    };

    class Painter;
    class SceneManager;
    namespace EasingCurve {
        class AbstractEasingCurve;
    }
    namespace Transition {
        class AbstractTransition;
    }

    /**
     * @class Engine
     * @brief 引擎
     *
     * 引擎核心控制类，负责窗口创建与管理、渲染流程调度、事件循环驱动及资源生命周期管理
     */
    class Engine {
    public:
        /**
         * @brief 定义窗口并初始化引擎资源
         * @param title 窗口标题
         * @param width 窗口宽度（像素）
         * @param height 窗口高度（像素）
         * @return 是否已初始化引擎
         */
        explicit Engine(const std::string& title, uint32_t width = 800, uint32_t height = 600);
        ~Engine();
        /**
         * @brief 启动主事件循环
         * @return 返回程序退出码，一般作用于 `main()` 函数下的返回值
         * @note 必需调用此函数才能使整个引擎持续运行
         *
         * 启动引擎主事件循环，处理输入、渲染等核心流程。
         */
        int exec();
        /**
         * @brief 显示指定窗口
         * @param window_id 窗口 ID （默认指定主窗口）
         * @return 返回 true 表示成功，若找不到窗口 ID 等则返回 false
         * @see showAll
         * @see hide
         * @see windowIDList
         */
        bool show(SWindowID window_id = _main_window_id);
        /**
         * @brief 显示所有窗口
         * @return 若成功则返回 0，失败将返回失败的窗口个数
         * @see show
         * @see hide
         */
        int showAll();
        /**
         * @brief 隐藏指定窗口
         * @param window_id 窗口 ID （默认指定主窗口）
         * @return 返回 true 表示成功，若找不到窗口 ID 等则返回 false
         * @see show
         * @see showAll
         * @see windowIDList
         */
        bool hide(SWindowID window_id = _main_window_id);
        /**
         * @brief 重新调整窗口大小
         * @param width     窗口宽度（按像素）
         * @param height    窗口高度（按像素）
         * @param window_id 窗口 ID（默认指定主窗口）
         * @return          返回 true 表示已调整窗口大小，否则为 false
         * @see windowIDList
         */
        bool resize(int width, int height, SWindowID window_id = _main_window_id);
        /**
         * @brief 移动指定窗口位置
         * @param x         窗口在显示器的横坐标
         * @param y         窗口在显示器的纵坐标
         * @param window_id 窗口 ID（默认指定主窗口）
         * @return          返回 true 表示已移动指定窗口
         * @see windowIDList
         */
        bool move(int x, int y, SWindowID window_id = _main_window_id);
        /**
         * @brief 设置指定窗口的位置、大小
         * @param x         指定窗口位置（横坐标）
         * @param y         指定窗口位置（纵坐标）
         * @param width     指定窗口宽度
         * @param height    指定窗口高度
         * @param window_id 窗口 ID（默认指定主窗口）
         * @return 返回 true 表示是否生效
         */
        bool setGeometry(int x, int y, int width, int height, SWindowID window_id = _main_window_id);
        /**
         * @brief 是否可调节窗口大小
         * @param resizeable 设置指定窗口是否可以缩放
         * @param window_id 窗口 ID （默认为主窗口）
         * @return 返回 true 表示成功，若找不到窗口 ID 等则返回 false
         * @see windowIDList
         */
        bool setResizable(bool resizeable, SWindowID window_id = _main_window_id);
        /**
         * @brief 设置窗口为无边框窗口
         * @param enabled 是否允许
         * @param window_id 指定的窗口 ID（默认为主窗口）
         * @return 返回 true 表示成功，若找不到窗口 ID 等则返回 false
         * @see windowIDList
         */
        bool setBorderlessWindow(bool enabled, SWindowID window_id = _main_window_id);
        /**
         * @brief 设置指定窗口是否为全屏
         * @param enabled               允许窗口是否全屏
         * @param move_cursor_to_center 是否将鼠标光标移动到中心
         * @param window_id             指定窗口 ID（默认主窗口）
         * @return 返回 true 表示成功，若找不到窗口 ID 等返回 false
         * @see windowIDList
         */
        bool setFullScreen(bool enabled, bool move_cursor_to_center = false, SDL_WindowID window_id = _main_window_id);
        /**
         * @brief 修改窗口标题
         * @param title  新的窗口标题名称
         * @param window_id 指定窗口 ID（默认主窗口）
         * @return 返回 true 表示成功，若找不到窗口 ID 等返回 false
         * @see windowTitle
         * @see windowIDList
         */
        bool setWindowTitle(const std::string& title, SWindowID window_id = _main_window_id);
        /**
         * @brief 获取指定窗口的标题
         * @param window_id 指定窗口 ID（默认主窗口）
         * @return 返回窗口标题
         * @see setWindowTitle
         * @see windowIDList
         */
        std::string windowTitle(SWindowID window_id);
        /**
         * @brief 新建一个子窗口
         * @param title   窗口标题
         * @param width   窗口宽度
         * @param height  窗口高度
         * @return   返回新打开的窗口 ID；若无法新建，则返回 0。
         * @see window
         * @see windowIDList
         * @see removeWindow
         */
        uint32_t newWindow(const std::string& title, uint32_t width = 800, uint32_t height = 600);
        /**
         * @brief 移除一个窗口
         * @param window_id  窗口 ID
         * @return 返回 true 表示成功，若找不到窗口 ID 等则返回 false
         * @see window
         * @see windowIDList
         */
        bool removeWindow(SWindowID window_id);
        /**
         * @brief 获得指定 ID 的窗口
         * @param window_id 窗口 ID（默认为主窗口）
         * @return 返回得到的窗口，若找不到窗口 ID 则返回空定义
         * @see windowIDList
         * @see windowCount
         */
        const Window *window(SWindowID window_id = _main_window_id);
        /**
         * @brief 获取所有可用的窗口 ID
         * @return 返回一个存储窗口 ID 的列表，通常第一个为主窗口
         * @see window
         * @see windowCount
         */
        std::vector<uint32_t> windowIDList();
        /**
         * @brief 获取窗口总个数
         * @return 返回窗口个数
         * @see windowIDList
         */
        uint32_t windowCount() const;
        /**
         * @brief 安装自定义事件处理器
         * @param _event_handler 用于处理的事件处理器
         *
         * 可使用函数指针、`std::function`、语法糖等
         * @note 需要特别注意：
         * @note 1. 指定的事件处理器必须以 `bool` 返回。当返回 true 表示持续运行，false 表示结束运行并释放引擎；
         * @note 2. 原有指定函数指针、 `std::function`、语法糖等的事件处理器将被替换。
         */
        void installEventHandler(std::function<bool(SEvent &)> event_handler);
        /**
         * @brief 获取指定窗口的渲染器
         * @param window_id 指定窗口（默认为主窗口）
         * @return 返回 true 表示成功，若找不到窗口 ID 等则返回 false
         * @see windowIDList
         */
        Painter* painter(SWindowID window_id = _main_window_id);
        /**
         * @brief 设置最大限定帧数
         * @param fps 固定帧率，即每秒持续刷新多少画面
         * @see fps
         * @see frameDropTolerance
         * @see frameDropThreshold
         * @see setFrameDropTolerance
         * @see setFrameDropThreshold
         */
        void setFPS(uint32_t fps);
        /**
         * @brief 实时获取当前刷新的帧数
         *
         * 即每秒渲染了多少画面
         * @return 返回当前秒刷新的帧数
         * @see setFPS
         */
        uint32_t fps() const;
        
        /**
         * @brief 设置丢帧检测的宽容度比例
         * @param ratio 丢帧触发比例，默认1.5表示超过目标帧时长1.5倍时开始检测
         * @see setFrameDropThreshold
         * @see frameDropTolerance
         * @see frameDropThreshold
         */
        void setFrameDropTolerance(float ratio = 1.5f);
        
        /**
         * @brief 获取当前丢帧检测的宽容度比例
         * @return 当前宽容度比例
         * @see setFrameDropTolerance
         * @see setFrameDropThreshold
         * @see frameDropThreshold
         */
        float frameDropTolerance() const;
        
        /**
         * @brief 设置连续慢帧阈值
         * @param count 连续多少帧慢帧后触发丢帧，默认3帧
         * @see setFrameDropTolerance
         * @see frameDropTolerance
         * @see frameDropThreshold
         */
        void setFrameDropThreshold(uint32_t count = 3);
        
        /**
         * @brief 获取当前连续慢帧阈值
         * @return 当前阈值
         * @see setFrameDropThreshold
         * @see setFrameDropTolerance
         * @see frameDropTolerance
         */
        uint32_t frameDropThreshold() const;

        /**
         * @brief 设置是否允许在后台渲染画面
         * @param enabled 启用/禁用
         *
         * 当窗口处于非活动状态下，根据 enabled 决定是否仍然渲染画面。
         * 这对于低性能设备而言，启用后能节省性能。
         * @see backgroundRenderingEnabled
         */
        void setBackgroundRenderingEnabled(bool enabled);
        /**
         * @brief 获取当前是否允许后台渲染画面
         * @see setBackgroundRenderingEnabled
         */
        bool backgroundRenderingEnabled() const;
        /**
         * @brief 设置清理引擎资源事件
         * @param function 自定清理函数
         *
         * 仅在引擎释放期间执行！
         */
        void installCleanUpEvent(const std::function<void()>& function);
        /**
         * @brief 获取当前显示器屏幕
         */
        Geometry screenGeometry();


    private:
        Engine() = delete;
        Engine(const Engine&) = delete;
        Engine &operator=(const Engine &) = delete;

        bool init(const char *title, uint32_t width, uint32_t height, uint32_t *wID);
        bool unload(SWindowID window_id);
        int run();
        void cleanUp();
        std::map<SWindowID, std::shared_ptr<Window>> _sdl_window_list;
        static SWindowID _main_window_id;
        std::map<SWindowID, std::unique_ptr<Painter>> _renderer_list;
        std::mutex _mutex;
        std::function<void()> _clean_up_function;
        bool _is_running{false};
        static bool _is_stopped;
        bool _is_allowed_stop_render{false};
        uint32_t _window_count{0};
        uint32_t _fps{0};
        uint32_t _real_fps{0};
        uint64_t max_frame_duration{0};
        uint64_t max_consecutive_slow_frames{3};
        uint64_t target_frame_duration{0};
        Vector2 _cursor_old_pos{};
        friend class Painter;
        friend class EventSystem;
    };

    /**
     * @class Painter
     * @brief 图形绘制器
     *
     * 绑定单个窗口的渲染器，通过存储并执行绘制命令（点、线、精灵等）实现画面渲染
     */
    class Painter {
    public:
        /**
         * @brief 指定窗口以获取渲染器
         * @param window 指定的窗口
         */
        explicit Painter(EasyEngine::Window *window);
        ~Painter();
        /**
         * @brief 获取当前绘制的窗口
         * @return 返回绘制窗口
         */
        const Window *window() const;
        /**
         * @brief 设置绘制时的粗细程度
         * @param value 新的粗细值，值越小越细
         *
         * @note 决定了绘制点、线段、边框时的粗细程度
         * @see thickness
         */
        void setThickness(uint16_t value);
        /**
         * @brief 获取绘制时的粗细程度
         * @return 返回当前粗细值
         */
        uint16_t thickness() const;
        /**
         * @brief 安装自定义绘图事件
         * @param function 自定义绘图函数
         */
        void installPaintEvent(std::function<void(Painter&)> function);
        /**
         * @brief 设置场景管理器以做到实时渲染
         * @param sceneManager 指定场景管理器
         */
        void setSceneManager(SceneManager* sceneManager);
        /**
         * @brief 执行所有绘制命令并刷新窗口画面
         * @warning 请勿手动使用此函数，会出现异常退出！
         */
        void ______();
        /**
         * @brief 填充背景颜色
         * @param color 指定颜色
         * @see StdColor
         * @see hexToRGBA
         */
        void fillBackColor(const SColor& color);
        /**
         * @brief 绘制点
         * @param point 点
         */
        void drawPoint(const Graphics::Point& point);
        /**
         * @brief 绘制线段
         * @param line 线段
         */
        void drawLine(const Graphics::Line& line);
        /**
         * @brief 绘制矩形
         * @param rect 矩形
         */
        void drawRectangle(const Graphics::Rectangle& rect);
        /**
         * @brief 绘制椭圆
         * @param ellipse 椭圆
         */
        void drawEllipse(const Graphics::Ellipse& ellipse);
        /**
         * @brief 绘制精灵
         * @param sprite 精灵
         * @param pos    绘制位置
         * @see Spirit
         */
        void drawSprite(const Components::Sprite &sprite, const Vector2 &pos);
        /**
         * @brief 根据精灵属性绘制精灵
         * @param sprite 指定精灵
         * @param properties 精灵属性
         * @see Spirit
         * @see Properties
         */
        void drawSprite(const Components::Sprite &sprite,
                        const Components::Sprite::Properties &properties);

        void drawSprite(const Components::Sprite &sprite,
                        const Components::Sprite::Properties *properties);
        /**
         * @brief 绘制像素文本
         * @param text  指定文本内容（仅支持 ASCII 字符）
         * @param pos   指定绘制的位置
         * @param size  指定绘制的尺寸比例（默认 1:1）
         * @param color 指定绘制的颜色（默认白色）
         */
        void drawPixelText(const std::string& text, const Vector2& pos,
                           const Size& size = {1.0f, 1.0f}, const SColor& color = StdColor::White);
        /**
         * @brief 清空所有绘制命令
         */
        void clear();
        /**
         * @brief 设置自定义视图
         * @param geometry 用于调整视图的位置、大小
         * @param size     用于调整视图的缩放比例（默认使用 1:1）
         *
         * 执行后，整个位置及大小都将发生改变！
         * @note 当指定的宽度或高度小于等于 0，则取消使用自定义视图
         */
        void setViewport(const Geometry &geometry, const Size &size = {1.0f, 1.0f});
        /**
         * @brief 裁剪可见视图
         * @param geometry 用于调整裁剪可见视图的位置、大小
         * @note 当指定的宽度或高度小于等于 0，则取消使用裁剪！
         */
        void setClipView(const Geometry &geometry);
        /**
         * @brief 绘制文本
         * @param text      指定文本指针
         * @param position  指定绘制的位置
         */
        void drawText(TTF_Text* text, const Vector2& position);
        bool _addTransition(Transition::AbstractTransition* transition);
        bool _removeTransition(Transition::AbstractTransition* transition);
        bool _startTransition(Transition::AbstractTransition* transition);
        bool _stopTransition(Transition::AbstractTransition* transition);

        bool _startEasingCurve(const std::shared_ptr<EasingCurve::AbstractEasingCurve> &easing_curve);
        bool _stopEasingCurve(const std::shared_ptr<EasingCurve::AbstractEasingCurve> &easing_curve);
    private:
        /**
         * @brief 绘图事件
         *
         * 告诉图形绘制器如何绘图，即存储绘图步骤
         */
        void paintEvent();
        Window* _window;
        struct Command {
            explicit Command() {};
            virtual ~Command() = default;
            virtual void exec(SRenderer *renderer, uint32_t thickness) = 0;
        };
        struct PointCMD : Command {
            Graphics::Point pt;
            explicit PointCMD(const Graphics::Point& point) : pt(point) {}
            void exec(SRenderer *renderer, uint32_t thickness) override;
        };
        struct LineCMD : Command {
            Graphics::Line line;
            explicit LineCMD(const Graphics::Line& line) : line(line) {}
            void exec(SRenderer *renderer, uint32_t thickness) override;
        };
        struct RectCMD : Command {
            Graphics::Rectangle rect;
            explicit RectCMD(const Graphics::Rectangle& rect) : rect(rect) {}
            void exec(SRenderer *renderer, uint32_t thickness) override;
        };
        struct EllipseCMD : Command {
            Graphics::Ellipse ellipse;
            explicit EllipseCMD(const Graphics::Ellipse& ellipse) : ellipse(ellipse)
                {}
            void exec(SRenderer *renderer, uint32_t thickness) override;
        };
        struct FillCMD : Command {
            SColor color;
            explicit FillCMD(const SColor& color) : color(color)
                {}
            void exec(SRenderer *renderer, uint32_t thickness) override;
        };
        struct SpriteCMD : Command {
            STexture* _sprite;
            Vector2 _pos, _clip_pos;
            Size _size, _clip_size;
            bool _is_clip{false};
            Vector2 _rotate_center;
            double _rotate{0.0};
            Vector2 _scaled_center;
            float _scaled{1.0f};
            SColor _color_alpha{255, 255, 255, 255};
            SDL_FlipMode _flip_mode{SDL_FLIP_NONE};
            explicit SpriteCMD(const Components::Sprite& sprite, const Vector2& pos = Vector2(0, 0))
                : _sprite(sprite.sprite()), _size(sprite.size()), _pos(pos),
                  _rotate_center(0, 0), _scaled_center(0, 0),
                  _clip_pos(0, 0), _clip_size(0, 0)
                {}
            SpriteCMD(const Components::Sprite& sprite, const Vector2& pos,
                      const Components::Sprite::Properties& properties)
                      : _sprite(sprite.sprite()), _size(sprite.size().width, sprite.size().height),
                        _pos(pos + properties.position), _rotate(properties.rotate), _rotate_center(properties.rotate_center),
                        _scaled(properties.scaled), _scaled_center(properties.scaled_center),
                        _clip_size(properties.clip_size), _is_clip(properties.clip_mode) {
                if (_is_clip)
                    _flip_mode = (properties.flip_mode == Components::Sprite::FlipMode::HFlip) ? SDL_FLIP_HORIZONTAL :
                        ((properties.flip_mode == Components::Sprite::FlipMode::VFlip) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);
            }
            void exec(SRenderer *renderer, uint32_t) override;
        };
        struct PixelTextCMD : Command {
            Vector2 pos;
            Size size;
            SColor color;
            std::string text;
            explicit PixelTextCMD() : pos(0, 0), size(0, 0), color(StdColor::White), text() {}
            PixelTextCMD(const GeometryF& geometry, const SColor& color, const std::string& text)
                : pos(geometry.pos), size(geometry.size), color(color), text(text) {}
            void exec(SRenderer *renderer, uint32_t) override;
        };
        struct ViewportCMD : Command {
            Geometry geometry;
            bool is_clipped_mode;
            Size scaled;
            explicit ViewportCMD() : geometry(0, 0, 0, 0), is_clipped_mode(false), scaled(1.0f, 1.0f) {}
            ViewportCMD(const Geometry& geometry, bool is_clipped_mode, const Size& scaled = {1.f, 1.f})
                : geometry(geometry.x, geometry.y, geometry.width, geometry.height),
                is_clipped_mode(is_clipped_mode), scaled(scaled.width, scaled.height) {}
            void exec(SRenderer *renderer, uint32_t) override;
        };
        struct TextCMD : Command {
            Vector2 position;
            TTF_Text* text;
            explicit TextCMD() : position(0, 0), text(nullptr) {}
            TextCMD(const Vector2& position, TTF_Text* text) : position(position.x, position.y), text(text) {}
            void exec(SRenderer *renderer, uint32_t thickness) override;
        };
        std::vector<std::unique_ptr<Command>> command_list;
        std::function<void(Painter&)> paint_function;
        uint32_t _thickness;
        SceneManager* _scene_manager{nullptr};
        std::vector<std::shared_ptr<Transition::AbstractTransition>> _running_transition_list;
        std::vector<std::shared_ptr<Transition::AbstractTransition>> _transition_list;
        std::vector<std::shared_ptr<EasingCurve::AbstractEasingCurve>> _curve_list;
        std::vector<std::shared_ptr<EasingCurve::AbstractEasingCurve>> _running_curve_list;
        friend class Components::Sprite;
    };

    /**
     * @class EventSystem
     * @brief 事件系统
     *
     */
    class EventSystem {
        friend class Engine;
    public:
        /**
         * @brief 获取全局事件系统
         */
        static EventSystem* global();
        explicit EventSystem() = default;
        EventSystem(const EventSystem&) = delete;
        EventSystem operator=(const EventSystem&) = delete;
        ~EventSystem();
        /**
         * @brief 设置是否启用全局事件监听
         * @param enabled 启用/禁用全局事件监听
         *
         * 当 `enabled` 为 `false` 时，除了基本的事件处理以及计时器、触发器以外，
         * 其它事件（如：自定义事件处理、控件处理）都将被忽略。一般情况下，当场景正在切换时会禁用全局事件监听。
         * @see handlerEnabled
         */
        void setHandlerEnabled(bool enabled);
        /**
         * @brief 获取当前是否启用全局事件监听
         * @retval true 表示事件系统正在监听所有事件,
         * @retval false 表示事件系统只监听基础事件、计时器、触发器事件
         * @see setHandlerEnabled
         */
        bool handlerEnabled() const;
        /**
         * @brief 事件处理器
         *
         * 处理所有注册的事件（定时器、触发器、控件事件等）。
         *
         * @return 返回 `true` 将持续处理事件，`false` 将结束处理事件
         */
        bool handler();
        /**
         * @brief 清理所有事件
         */
        void cleanUp();
        /**
         * @brief 添加定时器事件
         * @param timer 指定定时器
         * @see replaceTimer
         * @see removeTimer
         * @return 返回定时器 ID
         */
        uint64_t addTimer(EasyEngine::Components::Timer *timer);
        /**
         * @brief 替换定时器事件
         * @param id 指定定时器 ID
         * @param timer 新的定时器
         * @note 原先被替换的定时器将被释放
         */
        void replaceTimer(uint64_t id, EasyEngine::Components::Timer *timer);
        /**
         * @brief 移除定时器事件
         * @see clearTimer
         */
        void removeTimer(uint64_t id);
        /**
         * @brief 移除定时器事件
         * @param timer 指定定时器
         */
        void removeTimer(EasyEngine::Components::Timer *timer);
        /**
         * @brief 清空所有定时器事件
         */
        void clearTimer();
        /**
         * @brief 添加触发器
         * @param trigger 指定触发器
         * @return 返回触发器 ID
         * @see replaceTrigger
         * @see removeTrigger
         */
        uint64_t addTrigger(EasyEngine::Components::Trigger* trigger);
        /**
         * @brief 替换触发器
         * @param id        触发器 ID
         * @param trigger   指定触发器
         * @note 原先被替换的触发器将被释放！
         * @see addTrigger
         */
        void replaceTrigger(uint64_t id, EasyEngine::Components::Trigger* trigger);
        /**
         * @brief 移除触发器
         * @param id 指定触发器 ID
         * @note 原有的触发器将被释放
         * @see addTrigger
         * @see replaceTrigger
         */
        void removeTrigger(uint64_t id);
        /**
         * @brief 清空触发器
         */
        void clearTrigger();
        /**
         * @brief 添加控件
         * @param control 指定控件
         * @return 返回控件 ID
         * @see replaceControl
         * @see removeControl
         */
        uint64_t addControl(Components::Control* control);
        /**
         * @brief 替换控件
         * @param id        指定控件 ID
         * @param control   指定新的控件
         * @see addControl
         */
        void replaceControl(uint64_t id, Components::Control* control);
        /**
         * @brief 移除指定控件
         * @param id    指定控件 ID
         * @see addControl
         * @see replaceControl
         * @see clearControls
         */
        void removeControl(uint64_t id);
        /**
         * @brief 清空所有控件
         */
        void clearControls();
        /**
         * @brief 添加场景管理器
         * @param scene_manager 指定场景管理器
         * @return 添加成功后，将返回 ID
         */
        uint64_t addSceneManager(EasyEngine::SceneManager *scene_manager);
        /**
         * @brief 移除场景管理器
         * @param id 指定 ID
         */
        void removeSceneManager(uint64_t id);
        /**
         * @brief 清空场景管理器
         */
        void clearSceneManger();

    private:
        static std::function<bool(SEvent&)> _my_event_handler;
        static std::unique_ptr<EventSystem> _instance;
        static bool _handler_trigger;
        std::map<uint64_t, std::unique_ptr<Components::Timer>> _timer_list;
        std::map<uint64_t, std::unique_ptr<Components::Trigger>> _trigger_list;
        std::map<uint64_t, Components::Control*> _control_list;
        std::map<uint64_t, std::unique_ptr<SceneManager>> _scene_mgr_list;
        uint64_t _timer_id{0};
        uint64_t _trigger_id{0};
        uint64_t _control_id{0};
        uint64_t _scene_id{0};
    };

    /**
     * @class AudioSystem
     * @brief 音频系统
     */
    class AudioSystem {
    protected:
        explicit AudioSystem();
    public:
        ~AudioSystem();
        AudioSystem(AudioSystem&) = delete;
        AudioSystem& operator=(AudioSystem&) = delete;
        /**
         * @brief 获取全局音频系统
         */
        static AudioSystem* global();
        /**
         * @brief 初始化音频系统
         *
         * @note 必须执行，否则无法正常使用音频
         */
        bool init();
        /**
         * @brief 卸载音频系统
         *
         * @note 必须执行，否则将无法正常完整卸载
         */
        void unload();

        /**
         * @struct Audio
         * @brief 音频属性
         *
         */
        struct Audio {
            /**
             * @enum PlayStatus
             * @brief 播放状态
             *
             * 标识了该音频的播放状态
             */
            enum PlayStatus {
                /// 未加载，没有载入任何音频
                Unload,
                /// 正在载入音频
                Loading,
                /// 准备就绪
                Loaded = 0x2,
                /// 正在播放
                Playing = 0x4,
                /// 已暂停
                Paused = 0x8,
                /// 失效，常用于表示无法加载音频时
                Failed = 0x10
            };
            /// 音频源地址
            std::string url;
            /// 播放状态
            PlayStatus status{Unload};
            /// 音频
            MIX_Audio* audio{nullptr};
            /// 是否为流音频
            bool is_stream{false};
            /// 针对流音频的属性
            struct {
                MIX_Track* track{nullptr};
                /// 当前播放位置
                uint64_t position{0};
                /// 该音频下的总时长
                uint64_t duration{0};
            } Stream;
        };

        /**
         * @brief 设置音频格式属性
         * @param spec 指定的音频格式属性
         * @note 这取决于当前的音频设备是否支持此音频格式，否则即使设置，也是无效
         * @see audioSpec
         * @see StdAudioSpec
         */
        void setAudioSpec(const SAudioSpec& spec);
        /**
         * @brief 获取音频格式
         * @return 返回完整的音频格式
         */
        const SAudioSpec& audioSpec() const;
        /**
         * @brief 设置背景音乐音量
         * @param volume 指定音量（按 0.0 ~ 1.0 表示音量百分比）
         * @code
         * // 75% volume of BGM
         * AudioSystem::global()->setBGMVolume(0.75f);
         * @endcode
         * @see bgmVolume
         */
        void setBGMVolume(float volume);
        /**
         * @brief 获取当前背景音乐音量
         * @see setBGMVolume
         */
        float bgmVolume() const;
        /**
         * @brief 设置音效音量
         * @param volume 指定音量（按 0.0 ~ 1.0 表示音量百分比）
         * * @code
         * // 75% volume of SFX
         * AudioSystem::global()->setSFXVolume(0.75f);
         * @endcode
         * @see sfxVolume
         */
        void setSFXVolume(float volume);
        /**
         * @brief 获取当前音效音量
         * @see setSFXVolume
         */
        float sfxVolume() const;
        /**
         * @brief 载入背景音乐
         * @param bgm   指定 BGM
         * @return 返回播放的通道，加载失败时返回 -1
         *
         * 将会自动从现有的空闲通道中播放，若无空闲通道，则强占最先加入的通道。
         * @see playBGM
         * @see stopBGM
         * @see stopAllBGM
         * @see unloadBGM
         */
        int16_t loadBGM(const Components::BGM &bgm);
        /**
         * @brief 载入音效
         * @param sfx  指定 SFX
         * @return 返回播放的通道，加载失败时返回 -1
         *
         * 将会自动从现有的空闲通道中播放，若无空闲通道，则强占最先加入的通道。
         * @see playSFX
         * @see stopSFX
         * @see stopAllSFX
         * @see unloadSFX
         */
        int16_t loadSFX(const Components::SFX &sfx);
        /**
         * @brief 播放背景音
         * @param channel 指定通道
         * @param loop 是否循环播放（默认不循环）
         * @return 是否成功播放背景音
         * @see loadBGM
         * @see stopBGM
         * @see stopAllBGM
         * @see unloadBGM
         */
        bool playBGM(uint8_t channel, bool loop = false);
        /**
         * @brief 播放音效
         * @param channel 指定通道
         * @return 是否成功播放音效
         * @see loadSFX
         * @see stopSFX
         * @see stopAllSFX
         * @see unloadSFX
         */
        bool playSFX(uint8_t channel);
        /**
         * @brief 立刻暂停/停止播放指定通道的 BGM
         * @param channel 指定通道（范围：0 ~ 15）
         * @param pause  是否选择暂停（否则将停止）
         * @param fade_out_duration 设定淡出音量持续时长，按帧
         * @see loadBGM
         * @see playBGM
         * @see stopAllBGM
         * @see unloadBGM
         */
        void stopBGM(uint8_t channel, bool pause, int64_t fade_out_duration = 0);
        /**
         * @brief 立刻停止播放指定通道的 SFX
         * @param channel 指定通道（范围：0 ~ 255）
         * @see loadSFX
         * @see playSFX
         * @see stopAllSFX
         * @see unloadSFX
         */
        void stopSFX(uint8_t channel);
        /**
         * @brief 立刻停止播放所有通道的背景音
         * @see loadBGM
         * @see playBGM
         * @see stopBGM
         * @see unloadBGM
         */
        void stopAllBGM();
        /**
         * @brief 立刻停止播放所有通道的音效
         * @see loadSFX
         * @see playSFX
         * @see stopSFX
         * @see unloadSFX
         */
        void stopAllSFX();
        /**
         * @brief 卸载背景音，移除播放源路径
         * @param channel 指定通道（范围：0 ~ 15）
         * @see loadBGM
         * @see playBGM
         * @see stopBGM
         * @see stopAllBGM
         */
        void unloadBGM(uint8_t channel);
        /**
         * @brief 卸载音效，移除播放源路径
         * @param channel 指定通道
         * @see loadSFX
         * @see playSFX
         * @see stopSFX
         * @see stopAllSFX
         * @see unloadSFX
         */
        void unloadSFX(uint8_t channel);
        /**
         * @brief 卸载所有的音频
         *
         * 对于卸载音频系统非常有用
         */
        void unloadAllChannel();
        /**
         * @brief 从背景音通道中获取指定通道的音频
         * @param channel 指定通道 (范围：0 ~ 15)
         * @return 返回该通道下的音频属性
         * @see Audio
         */
        const Audio& bgmChannel(uint8_t channel);
        /**
         * @brief 从音效通道中获取指定通道的音频
         * @param channel 指定通道 (范围：0 ~ 255)
         * @return 返回该通道下的音频属性
         * @see Audio
         */
        const Audio& sfxChannel(uint8_t channel);
        MIX_Mixer* mixer() const;
    private:
        static std::unique_ptr<AudioSystem> _instance;
        SAudioSpec _audio_spec;
        MIX_Mixer* _bgm_mixer, *_sfx_mixer;
        bool _is_loaded{false}, _is_init{false};
        std::array<Audio, 16> _bgm_channels;
        std::deque<uint8_t> _bgm_played_list;
        std::array<Audio, 256> _sfx_channels;
        std::deque<uint8_t> _sfx_played_list;
        float _bgm_volume{1.0f};
        float _sfx_volume{1.0f};
    };

    /**
     * @namespace StdAudioSpec
     * @brief 标准音频格式
     *
     * 包含了不同级别的配置，可满足不同需求。
     * @note 不是所有的配置都能适用于你的音频设备中，
     * @note 请根据实际的音频设备，设置适合的音频格式！
     * @see Core.h
     */
    namespace StdAudioSpec {
        /// 音频格式（磁带录像机级别）
        constexpr SAudioSpec Low = {SDL_AUDIO_S16, 1, 16000};
        /// 音频格式（单声道）
        constexpr SAudioSpec Mono = {SDL_AUDIO_S16, 1, 44100};
        /// 音频格式 (CD 音质)
        constexpr SAudioSpec Stereo = {SDL_AUDIO_S16, 2, 44100};
        /// 音频格式（高级别）
        constexpr SAudioSpec High = {SDL_AUDIO_S16, 2, 48000};
        /// 音频格式（Hi-fi 高音质）
        constexpr SAudioSpec HIFI = {SDL_AUDIO_S16, 2, 96000};
    }

    /**
     * @class TextSystem
     * @brief 文本系统
     *
     * 用于加载、渲染字体、支持文本排版功能，实现更为复杂的功能
     * @note 自 v1.1.0-alpha 版本起，原 `FontSystem` 重命名为 `TextSystem`，用于文本排版等功能。
     * @note 并且文本系统将新增文本排版相关功能。
     * @since v1.1.0-alpha
     */
    class TextSystem {
    public:
        /**
         * @brief 获取全局文本系统
         */
        static TextSystem* global();
        /**
         * @brief 初始化文本系统
         */
        void init();
        /**
         * @brief 卸载文本系统
         */
        void unload();
        /**
         * @brief 加载文本系统
         * @param painter 指定绘图器
         * @return 返回 `bool` 以表示是否成功加载文本系统
         */
        bool load(const Painter* painter);

        /**
         * @brief 加载字体到文本系统
         * @param name      命名字体名称（便于后续管理）
         * @param font      指定字体
         * @note 对于指定已加载的字体，将重新加载新的字体！
         * @see font
         * @see unloadFont
         */
        void loadFont(const std::string& name, Components::Font* font);
        /**
         * @brief 从资源系统中加载字体到文本系统
         * @param name             命名新的字体名称（便于后续管理）
         * @param resource_name    指定资源名称
         * @param font_size        指定字体大小
         * @see font
         * @see unloadFont
         */
        void loadFont(const std::string& name, const std::string& resource_name, float font_size);
        /**
         * @brief 从文本系统中卸载指定字体
         * @param name 指定字体名称
         * @see loadFont
         * @see font
         */
        void unloadFont(const std::string& name);
        /**
         * @brief 从文本系统中获取字体信息
         * @param name 指定字体名称
         *
         */
        Components::Font* font(const std::string& name);
        /**
         * @brief 添加新文本
         * @param text_name 新的文本名称
         * @param text 新的文本内容
         * @param font_name 指定字体名称
         */
        void addText(const std::string &text_name, const std::string &text, const std::string &font_name);
        /**
         * @brief 删除已有的文本
         * @param text_name 文本名称
         */
        void removeText(const std::string& text_name);
        /**
         * @brief 追加文本到末尾
         * @param text_name     指定文本名称
         * @param append_text   加入的文本内容
         *
         * 将追加的文本添加到原有文本的结尾。
         */
        void appendText(const std::string& text_name, const std::string& append_text);
        /**
         * @brief 追加字符到末尾
         * @param text_name 指定文本名称
         * @param ch        加入的字符
         *
         * 将追加的字符添加到原有文本的末尾
         */
        void appendText(const std::string& text_name, const char ch);
        /**
         * @brief 修改已有的文本
         * @param text_name 指定文本名称
         * @param text 新的文本内容
         */
        void modifyText(const std::string& text_name, const std::string& text);
        /**
         * @brief 清空已有的文本
         * @param text_name 指定文本名称
         */
        void clearText(const std::string& text_name);
        /**
         * @brief 获取文本
         * @param text_name 指定文本名称
         * @return 返回实际的文本内容。当指定的文本名称不存在时，返回空内容
         */
        std::string text(const std::string& text_name);
        /**
         * @brief 修改文本的字体
         * @param text_name 指定文本名称
         * @param font_name 指定字体名称
         */
        void setTextFont(const std::string& text_name, const std::string& font_name);
        /**
         * @brief 修改文本的颜色
         * @param text_name     指定文本名称
         * @param text_color    指定文本颜色
         */
        void setTextColor(const std::string& text_name, const SColor& text_color);

        /**
         * @brief 设置文本行的宽度
         * @param text_name 指定文本名称
         * @param width 指定宽度
         *
         * 当某行的文本宽度超过设置的宽度时，超出宽度文本的部分将会换行。
         */
        void setTextWrapWidth(const std::string& text_name, int width);
        /**
         * @brief 设置文本的方向
         * @param text_name 指定文本名称
         * @param direction 指定方向
         */
        void setTextDirection(const std::string& text_name, const Components::Font::Direction& direction);
        /**
         * @brief 实时渲染文本
         * @param text_name     指定文本名称
         * @param position      指定渲染位置
         * @note 最好只在绘图事件下渲染，这样就能实时渲染每一帧文本！
         */
        void renderText(const std::string &text_name, const Vector2 &position);
        /**
         * @brief 获取当前文本区域的大小
         * @param text_name
         * @return 若指定的文本名称找不到时，返回空的大小；否则返回实际的文本区域大小
         */
        EasyEngine::Size textAreaSize(const std::string& text_name) const;

        TextSystem(TextSystem&) = delete;
        TextSystem& operator=(const TextSystem&) = delete;
    private:
        TextSystem() = default;
        static std::unique_ptr<TextSystem> _instance;
        std::map<std::string, std::shared_ptr<Components::Font>> _font_info;
        std::map<std::string, std::shared_ptr<TTF_Text>> _text_list;
        TTF_TextEngine* _text_engine{nullptr};
        Painter* _painter{nullptr};
    };
}


#endif //EASYENGINE_CORE_H
