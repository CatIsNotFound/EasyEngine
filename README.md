# Easy Engine

**Notes: 此项目已迁移至 [S3GF](https://github.com/CatIsNotFound/S3GF) 项目中，后续可能会删除此项目！**

基于 SDL3 图形引擎库二次封装的开源游戏引擎

[![C++23](https://img.shields.io/badge/C++-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![SDL3](https://img.shields.io/badge/SDL-3-blue.svg)](https://github.com/libsdl-org/SDL)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
![Version](https://img.shields.io/badge/Version-1.1.0--Alpha-red.svg)

## 🚀 项目简介

Easy Engine 是一个现代化的 C++23 游戏引擎，基于 SDL3 构建，专为简化 2D 游戏开发而设计。它提供了直观易用的 API，将复杂的底层图形编程抽象成简单的函数调用，让开发者能够专注于游戏创意而非技术细节。

## ✨ 核心特性

### 🎮 游戏开发
- **窗口系统** - 支持创建、管理和操作游戏窗口
- **事件系统** - 完整的键盘、鼠标、窗口事件处理
- **帧率控制** - 精确的帧率控制和性能统计
- **多窗口支持** - 基础的多窗口管理功能

### 🎨 图形渲染
- **精灵系统** - 完整的精灵管理、绘制和变换功能
- **精灵组** - 支持精灵批量管理和组合操作
- **基础图形** - 矩形、椭圆等基础图形绘制
- **颜色系统** - 内置81种预定义标准颜色（StdColor命名空间）
- **颜色转换** - RGBA/十六进制颜色转换工具函数
- **绘制属性** - 支持描边、填充、颜色设置、透明度等

### 🔧 开发工具
- **光标控制** - 多种标准光标样式和自定义光标支持
- **坐标系** - 完整的2D向量和几何计算支持
- **音频系统** - 背景音乐（BGM）和音效（SFX）播放控制
- **定时器** - 功能完整的定时器组件，支持循环和事件触发
- **触发器** - 条件触发事件系统
- **跨平台** - 基于SDL3的跨平台支持

### 📐 算法支持
- **几何算法** - 点与图形位置关系判断、碰撞检测
- **精灵变换** - 缩放、位置计算、中心圆算法
- **颜色转换** - 十六进制与RGBA颜色互转
- **图形计算** - 中心圆算法等图形计算工具

### 🎬 过渡动画
- **缓动曲线** - 线性和多种缓动函数（二次方、三次方、正弦等）
- **过渡效果** - 支持正向和反向播放控制
- **动画系统** - 基础过渡动画框架

## 📦 快速开始

### 环境要求
- **编译器**: 支持 C++23 的现代编译器 (MSVC 2022+, GCC 11+, Clang 14+)
- **依赖库**: SDL3 及其扩展库 (`SDL3_image`, `SDL3_ttf`, `SDL3_mixer`, `SDL3_gfx`)
- **构建工具**: CMake 3.28+

### 如何构建项目

Step 1. [点击前往此页面](https://github.com/CatIsNotFound/EasyEngine/releases/latest)以下载最新版本；

Step 2. 新建一个你的项目目录，例如：`MyGame`。

Step 3. 解压所有包，并将解压后的多个目录复制到你的项目目录的 `libs` 目录。

Step 4. 从项目 `examples` 目录下复制 `CMakeLists.txt` 文件和 `main.cpp` 文件。

记得修改 `CMakeLists.txt` 文件，具体如下内容：

```cmake
cmake_minimum_required(VERSION 3.28)
project(MyGame)     # 在此处修改你的项目名称

# 这里的目录需要自行修改实际的目录
set(SDL_DIR        "/path/to/SDL")
set(SDL_IMAGE_DIR  "/path/to/SDL3_image")
set(SDL_TTF_DIR    "/path/to/SDL3_ttf")
set(SDL_MIXER_DIR  "/path/to/SDL3_mixer")
set(SDL_GFX_DIR    "/path/to/SDL3_gfx")
set(FMT_DIR        "/path/to/FMT")
set(EASYENGINE_DIR "/path/to/EasyEngine")
```

Step 5: 编译当前项目，或在终端执行以下命令：

```bash
mkdir build
cd build
cmake ..
cmake --build .
./MyGame          # 你的实际项目，这里以 MyGame 为例
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
│   ├── Scene.h/.cpp       # 场景管理（Layer、Scene、SceneManager - 开发中）
│   ├── Transition.h/.cpp  # 过渡动画系统（缓动曲线、过渡效果）
│   └── preinclude.h       # 预包含头文件
├── docs/                  # 文档和教程
├── assets/                # 游戏资源文件（图片、音频等）
├── examples/              # 示例代码
├── main.cpp              # 主示例程序
└── CMakeLists.txt        # 构建配置
```

## 🛠️ 核心模块完成状态

### ✅ 已完成模块
- **Core 模块 (85%)** - 引擎核心功能，包括窗口管理、事件处理、渲染控制
- **Basic 模块 (90%)** - 基础数据结构，包括向量、几何图形、颜色系统
- **Components 模块 (75%)** - 音频组件、定时器、触发器、精灵系统
- **Algorithm 模块 (70%)** - 颜色转换、图形算法、位置关系判断
- **Resources 模块 (80%)** - 文件系统和资源管理
- **Transition 模块 (60%)** - 过渡动画和缓动曲线

### 🚧 开发中模块
- **Scene 模块 (30%)** - 场景管理系统，目前仅有基础类定义
- **动画系统** - 精灵动画序列播放
- **高级碰撞检测** - 基于SAT定理的碰撞检测
- **资源异步加载** - 异步资源加载和缓存管理优化

## 🎯 使用场景

- **2D 游戏开发** - 快速开发2D游戏原型和完整游戏
- **图形应用** - 简单的图形界面应用开发
- **教育项目** - 游戏开发教学和图形编程学习
- **交互演示** - 创建交互式图形演示程序
- **多媒体应用** - 音频播放和简单动画展示

## 📝 开发注意事项

1. **当前状态**: Alpha 版本，核心功能已实现但部分高级功能仍在开发中
2. **Scene 模块**: 场景管理系统尚未完全实现，建议暂时使用基础渲染功能
3. **资源管理**: 支持基础文件操作和资源加载，但异步加载功能待完善
4. **编译器要求**: 必须使用支持 C++23 的现代编译器
5. **依赖库**: 需要正确配置 SDL3 及其扩展库开发环境
6. **跨平台**: 基于SDL3开发，理论支持多平台，主要在Windows平台测试

## 🔮 开发路线图

### 短期目标 (v1.2.0)
- 完成 Scene 模块的场景管理功能
- 完善资源异步加载系统
- 添加基础精灵动画支持
- 优化渲染性能和内存管理
- 完善碰撞检测系统

### 中期目标 (v1.5.0)
- 实现完整的UI控件系统
- 添加粒子系统和特效
- 支持更多图形格式和音频格式
- 增强跨平台兼容性
- 完善文档和教程

### 长期目标 (v2.0.0)
- 集成Box2D物理引擎
- 支持多线程渲染
- 添加网络功能支持
- 提供可视化编辑器
- 构建完整的游戏开发生态

## 🤝 贡献指南

我们欢迎社区贡献！项目采用开源开发模式，欢迎提交问题报告和功能请求。

### 如何贡献
1. Fork 项目仓库
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 打开 Pull Request

## 📄 许可证

本项目采用 MIT 开源许可证，详见 [LICENSE](LICENSE) 文件。

## 🙋‍♂️ 获取帮助

- 📖 **文档**: 查看[项目文档](./docs/html/index.html)了解详细API使用
- 💬 **讨论**: 通过 GitHub Issues 提交问题和建议
- 🔍 **示例**: 参考 `main.cpp` 和 `examples/` 目录中的示例代码
- 📧 **邮件**: 发送邮件至项目维护者获取技术支持

## 🏆 致谢

感谢 SDL3 开发团队提供的优秀图形库支持，以及所有为项目贡献代码和提出建议的开发者们。

---

**Easy Engine** - 简化游戏开发，专注创意实现！



