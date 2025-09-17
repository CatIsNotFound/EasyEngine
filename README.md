# Easy Engine

基于 SDL3 图形引擎库二次封装的开源游戏引擎

[![C++23](https://img.shields.io/badge/C++-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![SDL3](https://img.shields.io/badge/SDL-3-green.svg)](https://github.com/libsdl-org/SDL)
[![License](https://img.shields.io/badge/License-Open%20Source-blue.svg)](LICENSE)

## 🚀 项目简介

Easy Engine 是一个现代化的 C++23 游戏引擎，基于 SDL3 构建，专为简化 2D 游戏开发而设计。它提供了直观易用的 API，将复杂的底层图形编程抽象成简单的函数调用，让开发者能够专注于游戏创意而非技术细节。

## ✨ 核心特性

### 🎮 游戏开发
- **多窗口管理** - 支持创建和管理多个独立窗口
- **精灵系统** - 完整的 2D 精灵渲染和管理
- **碰撞检测** - 内置高效的碰撞检测算法
- **动画系统** - 支持帧动画和过渡效果

### 🎨 图形渲染
- **基础图形** - 点、线、矩形、椭圆等基础图形
- **纹理渲染** - 支持 PNG、JPG、BMP 等格式
- **颜色处理** - RGBA/十六进制颜色转换
- **特效支持** - 旋转、缩放、翻转、透明度

### 🔧 开发工具
- **实时调试** - 内置性能监控和调试面板
- **热重载** - 运行时参数调整
- **跨平台** - Windows、Linux、macOS 支持
- **事件系统** - 完整的键盘、鼠标、窗口事件处理

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

### 最小示例代码

```cpp
#include "src/Core.h"
using namespace EasyEngine;

int main() {
    // 创建游戏引擎
    Engine game("我的第一个游戏", 800, 600);
    
    // 设置事件处理
    game.installEventHandler([&](SDL_Event e) {
        return e.type != SDL_QUIT;
    });
    
    // 游戏渲染循环
    game.painter()->installPaintEvent([&](Painter& p) {
        p.fillBackColor(SColor(30, 30, 60, 255));
        
        // 绘制一个移动的矩形
        static float x = 100;
        x += 0.5f;
        if (x > 700) x = 100;
        
        Graphics::Rectangle rect(x, 300, 50, 50, SColor(255, 100, 100, 255));
        rect.filled_mode = true;
        p.drawRectangle(rect);
    });
    
    game.show();
    return game.exec();
}
```

## 📚 文档和示例

- **[完整文档](docs/mainpage.md)** - 详细的 API 文档和使用指南
- **[示例程序](main.cpp.old)** - 包含帧丢失测试和性能监控的完整示例
- **[Doxygen 文档](docs/html/index.html)** - 自动生成的 API 参考

## 🏗️ 项目结构

```
EasyEngine/
├── src/                    # 引擎源码
│   ├── Core.h/.cpp        # 引擎核心管理
│   ├── Components.h/.cpp  # 游戏组件系统
│   ├── Basic.h/.cpp       # 基础数据结构
│   ├── Algorithm.h/.cpp   # 算法和工具函数
│   └── Resources.h/.cpp   # 资源管理
├── docs/                  # 文档和教程
├── libs/                  # 第三方库
├── main.cpp              # 示例程序
└── CMakeLists.txt        # 构建配置
```

## 🎯 使用场景

- **2D 游戏开发** - 平台游戏、射击游戏、益智游戏
- **图形应用** - 数据可视化、交互式应用
- **教育项目** - 游戏开发教学、算法演示
- **原型开发** - 快速游戏原型验证

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



