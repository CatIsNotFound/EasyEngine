# Easy Engine 游戏引擎文档

## 项目概述

Easy Engine 是一个基于 SDL3 图形引擎库二次封装的开源游戏引擎，专为简化 2D 游戏开发流程而设计。它提供了现代化的 C++23 接口，将复杂的 SDL3 操作封装成直观易用的 API，让开发者能够专注于游戏逻辑而非底层图形编程。

引擎采用模块化设计，包含核心引擎管理、图形渲染、组件系统、算法库、过渡动画等多个模块，为 2D 游戏开发提供了完整的解决方案。无论是开发简单的 2D 游戏，还是构建复杂的图形应用，Easy Engine 都能提供强大的支持。

## 主要特性

### 🎮 核心引擎功能
- **窗口管理**：支持创建和管理游戏窗口，提供完整的窗口生命周期管理
- **事件系统**：完整的事件处理架构，支持键盘、鼠标、窗口事件处理
- **帧率控制**：精确的帧率管理和性能统计，支持动态帧率调整
- **资源管理**：自动化的纹理、音频、字体资源加载和管理系统

### 🎨 图形渲染系统
- **基础图形绘制**：
  - 点、线、矩形、椭圆等基础图形绘制
  - 支持填充和描边模式，透明度和颜色混合
  - 颜色系统内置81种预定义标准颜色
- **精灵系统**：
  - 完整的精灵纹理加载、管理和绘制功能
  - 支持精灵裁剪、缩放、旋转、翻转（水平/垂直）
  - 颜色通道调整和透明度控制
  - 多种绘制模式（位置、缩放、旋转、翻转、颜色）
- **精灵组管理**：
  - 精灵批量管理和组合操作
  - 支持精灵组的统一变换和控制

### 🧩 组件化架构
- **音频组件**：
  - BGM（背景音乐）组件：支持播放、暂停、停止、循环控制
  - SFX（音效）组件：支持音效播放、停止、带延迟循环播放
- **定时器组件**：功能完整的定时器，支持延迟触发、循环启用、事件绑定
- **触发器组件**：条件触发事件系统，支持自定义条件和事件处理
- **精灵组件**：完整的精灵管理组件，支持多种创建方式和属性控制

### 📐 数学和算法库
- **几何算法**：
  - 中心圆算法生成和计算
  - 向量数学运算和2D几何计算
  - 点与图形位置关系判断（点/矩形/椭圆包含关系）
  - 碰撞检测算法支持
- **颜色处理**：
  - RGBA/十六进制颜色转换工具函数
  - 颜色插值和渐变计算
- **精灵变换**：缩放、位置计算、尺寸调整等图形变换算法

### 🎬 过渡动画系统
- **缓动曲线**：
  - 线性缓动（LinearCurve）
  - 二次方缓动（InQuadCurve、OutQuadCurve）
  - 三次方缓动（InCubicCurve、OutCubicCurve）
  - 正弦缓动（InSineCurve）
- **动画控制**：支持正向和反向播放，持续时间控制，启用状态管理
- **抽象框架**：提供AbstractEasingCurve基类，便于扩展自定义缓动效果

### 🔧 开发工具
- **光标控制**：多种标准光标样式支持（箭头、手型、文本、等待等）和自定义光标
- **调试支持**：
  - 实时性能监控和帧率统计
  - 详细的日志系统和错误处理
- **文件系统**：完整的文件和目录操作，支持文本和二进制文件读写
- **跨平台支持**：基于 SDL3 的跨平台兼容性（Windows/Linux/macOS）

## 技术架构

### 模块结构
```
EasyEngine/
├── src/                    # 引擎源码
│   ├── Core.h/.cpp        # 引擎核心管理（Engine、Cursor、Painter、窗口管理等）
│   ├── Components.h/.cpp  # 游戏组件系统（BGM、SFX、Timer、Trigger、Sprite等）
│   ├── Basic.h/.cpp       # 基础数据结构（Vector2、Size、Geometry、StdColor等）
│   ├── Algorithm.h/.cpp   # 算法和工具函数（颜色转换、图形算法等）
│   ├── Resources.h/.cpp   # 资源管理（FileSystem、ResourceSystem）
│   ├── Scene.h/.cpp       # 场景管理（Layer、Scene、SceneManager - 开发中）
│   ├── Transition.h/.cpp  # 过渡动画系统（缓动曲线、过渡效果）
│   └── preinclude.h       # 预包含头文件
├── docs/                  # 文档和教程
├── assets/                # 游戏资源文件（图片、音频等）
├── examples/              # 示例代码
├── main.cpp              # 主示例程序
└── CMakeLists.txt        # 构建配置
```

### 核心类层次
- **Engine**：主引擎类，管理整个游戏生命周期和核心系统
- **Window**：窗口管理类，支持窗口创建、属性设置和事件处理
- **Cursor**：鼠标光标管理，支持系统光标和自定义光标创建
- **Painter**：图形绘制器，提供所有绘图接口和渲染功能
- **EventSystem**：事件系统，统一处理键盘、鼠标、窗口等各种事件
- **AudioSystem**：音频系统，支持背景音乐和音效的播放与管理
- **FontSystem**：字体系统，支持文本渲染和字体管理

### 组件系统
- **BGM**：背景音乐组件，提供完整的音频播放控制
- **SFX**：音效组件，支持即时音效和循环音效播放
- **Timer**：定时器组件，支持延迟执行和循环触发功能
- **Trigger**：触发器组件，基于条件的事件触发机制
- **Sprite**：精灵组件，完整的精灵纹理管理和绘制功能
- **SpriteGroup**：精灵组组件，支持批量精灵管理

### 资源管理
- **ResourceSystem**：资源系统，提供统一的资源加载、缓存和管理
- **FileSystem**：文件系统，支持文件和目录的各种操作
- **Resource**：资源结构体，定义资源类型和元数据管理

## 快速开始

### 最小示例
```cpp
#include <EasyEngine/Core.h>
using namespace EasyEngine;

int main() {
    // 创建引擎实例
    Engine engine("Hello EasyEngine!", 800, 600);
    engine.show();
    
    // 设置事件处理器
    engine.installEventHandler([&engine](SDL_Event& e) {
        if (e.key.down && e.key.key == SDLK_ESCAPE) {
            return false;
        }
        return true;
    });
    
    // 设置渲染回调
    engine.painter()->installPaintEvent([&](Painter& painter) {
        painter.fillBackColor(StdColor::White);
        
        // 绘制一个红色矩形
        Graphics::Rectangle rect(100, 100, 200, 150, StdColor::Red, true, true, StdColor::Yellow);
        painter.drawRectangle(rect);
    });
    return engine.exec();
}
```

## 核心功能详解

### 引擎核心 (Core Module)
- **完成度：85%**
- 提供引擎初始化、窗口管理、事件处理等核心功能
- 支持多窗口创建和管理
- 集成Painter绘制系统、Cursor光标系统
- 完整的应用生命周期管理

### 基础模块 (Basic Module)
- **完成度：90%**
- 提供Vector2向量、Size尺寸、Geometry几何图形等基础数据结构
- 内置StdColor命名空间，包含81种预定义标准颜色
- 支持基本的几何计算和颜色操作

### 组件系统 (Components Module)
- **完成度：75%**
- BGM类：背景音乐管理，支持播放、暂停、停止等操作
- SFX类：音效管理，支持播放、停止及循环播放
- Timer类：定时器组件，支持设置延迟、触发事件及循环功能
- Trigger类：条件触发事件系统
- Sprite类：完整的精灵系统，支持多种创建方式和绘制模式
- SpriteGroup类：精灵组管理，支持批量操作

### 算法库 (Algorithm Module)
- **完成度：70%**
- 提供UTF-8字符串分割、十六进制/RGBA颜色转换
- 中心圆算法、精灵缩放位置与尺寸计算
- 点/矩形/椭圆间的碰撞检测和位置关系判断
- 窗口内容捕获功能

### 资源管理 (Resources Module)
- **完成度：80%**
- FileSystem：完整的文件和目录操作支持
- ResourceSystem：统一的资源加载、预加载、异步加载管理
- 支持资源缓存、替换、卸载等完整生命周期管理

### 过渡动画 (Transition Module)
- **完成度：60%**
- 提供AbstractEasingCurve抽象基类和多种具体缓动实现
- 支持LinearCurve、InQuadCurve、OutQuadCurve、InCubicCurve等
- 完整的动画进度控制和方向管理

### 场景管理 (Scene Module)
- **完成度：30%**
- 提供Layer图层管理、Scene场景管理、SceneManager场景管理器基础架构
- 支持图层顺序管理、精灵和控件的组织
- 场景切换和事件管理功能待完善

## 构建、部署和安装

项目使用 CMake 3.28+ 构建系统，支持 C++23 标准。具体构建过程参见[构建向导](./build.md)。

### 依赖要求
- **编译器**：支持 C++23 的现代编译器 (MSVC 2022+, GCC 11+, Clang 14+)
- **依赖库**：SDL3 及其扩展库 (SDL3_image, SDL3_ttf, SDL3_mixer, SDL3_gfx)
- **构建工具**：CMake 3.28+
- **其他**：fmt 库用于格式化输出

## 版本信息

- **当前版本**：v1.1.0-alpha
- **API 状态**：稳定开发中，核心API已稳定
- **兼容性**：Windows/Linux/macOS (通过 SDL3)
- **许可证**：MIT 开源许可证
- **整体完成度**：约70%

## 贡献和支持

Easy Engine 是一个持续开发中的开源项目，欢迎开发者参与贡献。项目采用模块化设计，便于扩展和维护。

### 如何贡献
1. Fork 项目仓库
2. 创建功能分支进行开发
3. 提交 Pull Request
4. 参与代码审查和讨论

如需技术支持、功能建议或报告问题，请通过 GitHub Issues 或项目文档中的联系方式获取帮助。

## 相关文档

- [API 参考文档](./html/index.html) - 完整的API文档
- [构建向导](./build.md) - 详细的构建和安装指南


