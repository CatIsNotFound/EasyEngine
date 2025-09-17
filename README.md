# Easy Engine

基于 SDL3 图形引擎库二次封装的开源游戏引擎

[![C++23](https://img.shields.io/badge/C++-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![SDL3](https://img.shields.io/badge/SDL-3-green.svg)](https://github.com/libsdl-org/SDL)
[![License](https://img.shields.io/badge/License-Open%20Source-blue.svg)](LICENSE)

## 🚀 项目简介

Easy Engine 是一个现代化的 C++23 游戏引擎，基于 SDL3 构建，专为简化 2D 游戏开发而设计。它提供了直观易用的 API，将复杂的底层图形编程抽象成简单的函数调用，让开发者能够专注于游戏创意而非技术细节。当前版本：0.1.1 Beta

## ✨ 核心特性

### 🎮 游戏开发
- **窗口系统** - 支持创建、管理和操作游戏窗口
- **精灵系统** - 完整的 2D 精灵渲染和管理
- **碰撞检测** - 内置高效的碰撞检测算法（Collider类）
- **事件系统** - 完整的键盘、鼠标、窗口事件处理

### 🎨 图形渲染
- **基础图形** - 矩形、椭圆等基础图形绘制
- **颜色系统** - 内置81种预定义标准颜色（StdColor命名空间）
- **颜色转换** - RGBA/十六进制颜色转换工具函数
- **绘制属性** - 支持描边、填充、颜色设置等

### 🔧 开发工具
- **光标控制** - 多种标准光标样式支持
- **坐标系** - 完整的2D向量和几何计算支持
- **音频系统** - 背景音乐（BGM）播放控制
- **跨平台** - 基于SDL3的跨平台支持

### 📐 算法支持
- **几何算法** - 点与图形位置关系判断
- **碰撞检测** - 基于SAT定理的碰撞检测实现
- **精灵变换** - 缩放、位置计算等功能

## 📦 快速开始

### 环境要求
- **编译器**: 支持 C++23 的现代编译器 (MSVC 2022+, GCC 11+, Clang 14+)
- **依赖库**: SDL3 及其扩展库 (SDL3_image, SDL3_ttf, SDL3_mixer)
- **构建工具**: CMake 3.31+

### 构建项目

```bash
# 克隆项目
git clone https://github.com/CatIsNotFound/EasyEngine.git
cd EasyEngine

# 配置构建
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 编译项目
cmake --build build

# 运行示例
./build/EasyEngine
```

### 完整示例代码

```cpp
#include "src/Core.h"

using namespace EasyEngine;
using namespace Components;

int main() {
    // 创建游戏引擎实例
    Engine engine("", 1024, 800);
    engine.setResizable(true);
    engine.setFPS(60);
    
    // 窗口居中
    auto geometry = engine.screenGeometry();
    engine.move(geometry.width / 2 - 1024 / 2, geometry.height / 2 - 800 / 2 - 100);
    engine.show();
    
    // 创建图形对象
    Graphics::Rectangle rect(100, 100, 280, 280, StdColor::Black, false, 
                             true, StdColor::LightBlue);
    Graphics::Ellipse ellipse(650, 500, 380, 380, StdColor::Black, false, 
                              true, StdColor::LightPink);
    
    // 创建鼠标跟随图形
    Graphics::Rectangle mouse_rect(0, 0, 100, 100, StdColor::Black, false, 
                                  true, {243, 233, 64, 64});
    Graphics::Ellipse mouse_ellipse(0, 0, 100, 100, StdColor::Black, false, 
                                   true, {243, 233, 64, 164});
    
    // 碰撞检测触发器
    static bool trigger = false;
    
    // 创建碰撞体
    Collider rect_collider(rect), ellipse_collider(ellipse),
             mouse_rect_collider(mouse_rect), mouse_ellipse_collider(mouse_ellipse);
    rect_collider.setEnabled(true);
    ellipse_collider.setEnabled(true);
    mouse_rect_collider.setEnabled(true);
    mouse_ellipse_collider.setEnabled(true);
    
    // 安装事件处理器
    engine.installEventHandler([&](SEvent ev) {
        // 鼠标位置和光标样式控制
        auto cur_pos = Cursor::global()->position();
        auto old_cur = Cursor::global()->cursor();
        if (Algorithm::comparePosRect(cur_pos, rect) >= 0) {
            if (old_cur != EasyEngine::Cursor::Hand) {
                old_cur = EasyEngine::Cursor::Hand;
                Cursor::global()->setCursor(Cursor::Hand);
            }
        } else if (Algorithm::comparePosEllipse(cur_pos, ellipse) >= 0) {
            if (old_cur != EasyEngine::Cursor::Hand) {
                old_cur = EasyEngine::Cursor::Hand;
                Cursor::global()->setCursor(Cursor::Hand);
            }
        } else {
            if (old_cur != EasyEngine::Cursor::Default) {
                old_cur = EasyEngine::Cursor::Default;
                Cursor::global()->setCursor(Cursor::Default);
            }
        }
        
        // 鼠标点击切换模式
        if (ev.button.type == SDL_EVENT_MOUSE_BUTTON_DOWN && ev.button.button == SDL_BUTTON_LEFT) {
            trigger = !trigger;
        }
        
        // 碰撞检测和反馈
        if (trigger) {
            mouse_ellipse_collider.moveBounds(cur_pos);
            auto ch = mouse_ellipse_collider.check(ellipse_collider);
            if (ch > 0) {
                ellipse.back_color = StdColor::DarkRed;
            } else if (ch == 0) {
                ellipse.back_color = StdColor::Tomato;
            } else {
                ellipse.back_color = StdColor::LightPink;
            }
        } else {
            mouse_rect_collider.moveBounds(cur_pos);
            if (mouse_rect_collider.check(rect_collider) >= 0) {
                rect.back_color = StdColor::DarkBlue;
            } else {
                rect.back_color = StdColor::LightBlue;
            }
        }
        
        // 更新窗口标题显示鼠标位置
        engine.setWindowTitle(fmt::format("Cursor({},{})", cur_pos.x, cur_pos.y));
        return true;
    });
    
    // 安装渲染事件
    engine.painter()->installPaintEvent([&](Painter& painter) {
        painter.fillBackColor(StdColor::White);
        painter.drawRectangle(rect);
        painter.drawEllipse(ellipse);
        if (trigger) {
            mouse_ellipse.pos = mouse_ellipse_collider.bounds().pos;
            painter.drawEllipse(mouse_ellipse);
        } else {
            mouse_rect.pos = mouse_rect_collider.bounds().pos;
            painter.drawRectangle(mouse_rect);
        }
    });

    // 运行游戏主循环
    return engine.exec();
}
```

## 📚 文档和示例

- **[完整文档](docs/mainpage.md)** - 详细的 API 文档和使用指南
- **[示例程序](main.cpp)** - 代码示例程序
- **[Doxygen 文档](docs/html/index.html)** - 自动生成的 API 参考

## 🏗️ 项目结构

```
EasyEngine/
├── src/                    # 引擎源码
│   ├── Core.h/.cpp        # 引擎核心管理（Window、Engine、Painter、EventSystem等）
│   ├── Components.h/.cpp  # 游戏组件系统（Sprite、BGM、Collider等）
│   ├── Basic.h/.cpp       # 基础数据结构（Vector2、Size、Geometry、StdColor等）
│   ├── Algorithm.h/.cpp   # 算法和工具函数（碰撞检测、颜色转换等）
│   ├── Resources.h/.cpp   # 资源管理（待完善）
│   └── preinclude.h       # 预包含头文件
├── docs/                  # 文档和教程（包含Doxygen生成的API文档）
├── libs/                  # 第三方库（SDL3_gfx等扩展库）
├── assets/                # 游戏资源文件（图片、音频等）
├── main.cpp              # 示例程序
└── CMakeLists.txt        # 构建配置
```

## 🎯 使用场景

- **2D 游戏开发** - 平台游戏、射击游戏、休闲小游戏
- **交互式应用** - 简单的图形界面应用开发
- **教育项目** - 游戏开发教学、图形编程学习
- **原型开发** - 快速游戏概念原型验证

## 🛠️ 核心类和组件

### 引擎核心（Core）
- **Engine** - 游戏引擎主类，管理整个游戏生命周期
- **Window** - 窗口管理类，负责窗口创建和控制
- **Painter** - 绘图类，提供各种渲染功能
- **EventSystem** - 事件系统，处理用户输入和窗口事件
- **Cursor** - 鼠标光标管理类

### 基础数据结构（Basic）
- **Vector2** - 二维向量，用于表示位置、大小等
- **Size** - 尺寸类，存储宽高信息
- **Geometry** - 几何信息类
- **StdColor** - 预定义标准颜色命名空间

### 算法库（Algorithm）
- **hexToRGBA** / **rgbaToHex** - 颜色转换函数
- **comparePosRect** / **comparePosEllipse** - 点与图形位置关系判断
- **碰撞检测算法** - 基于SAT定理的实现

### 组件系统（Components）
- **Sprite** - 精灵类，游戏中可移动的图像元素
- **BGM** - 背景音乐控制类
- **Collider** - 碰撞体类，用于检测碰撞
- **Timer** - 定时器类（待完善）

### 图形（Graphics）
- **Rectangle** - 矩形图形类
- **Ellipse** - 椭圆图形类

## 📝 开发注意事项

1. 当前版本为 Beta 版本，部分功能仍在开发中
2. Resources 类和动画系统尚未完全实现
3. 建议使用 C++23 兼容的编译器进行开发
4. 项目依赖 SDL3 及其扩展库，需要正确配置开发环境

## 🔮 未来规划

- 完善资源管理系统
- 实现完整的动画系统
- 添加粒子系统
- 增强碰撞检测能力
- 支持更多图形特效
- 优化性能和稳定性

## 🤝 贡献指南

我们欢迎社区贡献！请查看 [CONTRIBUTING.md](CONTRIBUTING.md) 了解如何参与项目开发。

### 开发设置

```bash
# 开发构建
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# 运行测试
./build/EasyEngine

# 生成文档
doxygen docs/Doxyfile
```

## 📄 许可证

本项目采用开源许可证，详见 [LICENSE](LICENSE) 文件。

## 🙋‍♂️ 获取帮助

- 📖 **文档**: [查看完整文档](docs/mainpage.md)
- 💬 **讨论**: [GitHub Issues](https://github.com/your-repo/EasyEngine/issues)
- 📧 **联系**: 项目维护者

---

**Easy Engine** - 让游戏开发变得简单而有趣！



