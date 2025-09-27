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
- **事件系统** - 完整的键盘、鼠标、窗口事件处理
- **帧率控制** - 精确的帧率控制和性能统计
- **多窗口支持** - 基础的多窗口管理功能

### 🎨 图形渲染
- **基础图形** - 矩形、椭圆等基础图形绘制
- **颜色系统** - 内置81种预定义标准颜色（StdColor命名空间）
- **颜色转换** - RGBA/十六进制颜色转换工具函数
- **绘制属性** - 支持描边、填充、颜色设置等

### 🔧 开发工具
- **光标控制** - 多种标准光标样式支持
- **坐标系** - 完整的2D向量和几何计算支持
- **音频系统** - 背景音乐（BGM）和音效（SFX）播放控制
- **定时器** - 基础定时器功能
- **跨平台** - 基于SDL3的跨平台支持

### 📐 算法支持
- **几何算法** - 点与图形位置关系判断
- **精灵变换** - 缩放、位置计算等功能
- **颜色转换** - 十六进制与RGBA颜色互转
- **图形计算** - 中心圆算法等图形计算工具

## 📦 快速开始

### 环境要求
- **编译器**: 支持 C++23 的现代编译器 (MSVC 2022+, GCC 11+, Clang 14+)
- **依赖库**: SDL3 及其扩展库 (`SDL3_image`, `SDL_ttf`, `SDL_mixer`, `SDL_gfx`)
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

### 基础示例代码

```cpp
#include "src/Core.h"
using namespace EasyEngine;

int main() {
    // 创建引擎实例
    Engine engine("我的游戏", 800, 600);
    engine.show();
    
    // 设置事件处理器
    engine.installEventHandler([&engine](SDL_Event event) {
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

## 🏗️ 项目结构

```
EasyEngine/
├── src/                    # 引擎源码
│   ├── Core.h/.cpp        # 引擎核心管理（Engine、Cursor、Painter、窗口管理等）
│   ├── Components.h/.cpp  # 游戏组件系统（BGM、SFX、Timer、Trigger、Sprite等）
│   ├── Basic.h/.cpp       # 基础数据结构（Vector2、Size、Geometry、StdColor等）
│   ├── Algorithm.h/.cpp   # 算法和工具函数（颜色转换、图形算法等）
│   ├── Resources.h/.cpp   # 资源管理（FileSystem、ResourceSystem）
│   ├── Scene.h/.cpp       # 场景管理（Layer、Scene、SceneManager - 待完善）
│   └── preinclude.h       # 预包含头文件
├── docs/                  # 文档和教程
├── libs/                  # 第三方库（SDL3_gfx等扩展库）
├── assets/                # 游戏资源文件（图片、音频等）
├── examples/              # 示例代码
├── main.cpp              # 主示例程序
└── CMakeLists.txt        # 构建配置
```

## 🛠️ 核心模块完成状态

### ✅ 已完成模块
- **Core 模块 (85%)** - 引擎核心功能，包括窗口管理、事件处理、渲染控制
- **Basic 模块 (90%)** - 基础数据结构，包括向量、几何图形、颜色系统
- **Components 模块 (80%)** - 音频组件、定时器、触发器、基础精灵功能
- **Algorithm 模块 (85%)** - 颜色转换、图形算法、位置关系判断
- **Resources 模块 (70%)** - 文件系统和基础资源管理

### 🚧 开发中模块
- **Scene 模块 (30%)** - 场景管理系统，目前仅有基础类定义
- **动画系统** - 精灵动画和过渡效果
- **高级碰撞检测** - 基于SAT定理的碰撞检测
- **资源异步加载** - 异步资源加载和缓存管理

## 🎯 使用场景

- **2D 游戏原型** - 快速开发2D游戏概念验证
- **图形应用** - 简单的图形界面应用开发
- **教育项目** - 游戏开发教学和图形编程学习
- **交互演示** - 创建交互式图形演示程序

## 📝 开发注意事项

1. **当前状态**: Beta 版本，核心功能已实现但部分高级功能仍在开发中
2. **Scene 模块**: 场景管理系统尚未完全实现，建议暂时使用基础渲染功能
3. **资源管理**: 支持基础文件操作，但异步加载和热重载功能待完善
4. **编译器要求**: 必须使用支持 C++23 的现代编译器
5. **依赖库**: 需要正确配置 SDL3 及其扩展库开发环境

## 🔮 开发路线图

### 短期目标
- 完成 Scene 模块的场景管理功能
- 完善资源异步加载系统
- 添加基础动画支持
- 优化渲染性能和内存管理

### 长期目标
- 实现完整的碰撞检测系统
- 添加粒子系统和特效
- 支持更多图形格式和音频格式
- 增强跨平台兼容性
- 提供完整的文档和教程

## 🤝 贡献指南

我们欢迎社区贡献！项目采用开源开发模式，欢迎提交问题报告和功能请求。

### 开发设置

```bash
# 开发构建
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# 编译项目
cmake --build build

# 运行测试
./build/EasyEngine

# 生成文档
doxygen docs/Doxyfile
```

## 📄 许可证

本项目采用开源许可证，详见 [LICENSE](LICENSE) 文件。

## 🙋‍♂️ 获取帮助

- 📖 **文档**: 查看[项目文档](./docs/html/index.html)了解详细API使用
- 💬 **讨论**: 通过GitHub Issues提交问题和建议
- 🔍 **示例**: 参考 main.cpp 和 examples/ 目录中的示例代码

---

**Easy Engine** - 简化游戏开发，专注创意实现！



