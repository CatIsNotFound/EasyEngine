# Easy Engine 游戏引擎文档

## 项目概述

Easy Engine 是一个基于 SDL3 图形引擎库二次封装的开源游戏引擎，专为简化游戏开发流程而设计。它提供了现代化的 C++23 接口，将复杂的 SDL3 操作封装成直观易用的 API，让开发者能够专注于游戏逻辑而非底层图形编程。

引擎采用模块化设计，包含核心引擎管理、图形渲染、组件系统、算法库等多个模块，为 2D 游戏开发提供了完整的解决方案。无论是开发简单的 2D 游戏，还是构建复杂的图形应用，Easy Engine 都能提供强大的支持。

## 主要特性

### 🎮 核心引擎功能
- 多窗口管理：支持创建和管理多个独立窗口
- 事件驱动架构：完整的事件处理系统，支持键盘、鼠标、窗口事件
- 帧率控制：精确的帧率管理，支持动态调整帧率限制
- 资源管理：自动化的纹理、音频资源加载和管理

### 🎨 图形渲染系统
- 基本图形绘制：
  - 点、线、矩形、椭圆等基础图形
  - 支持填充和描边模式
  - 透明度和颜色混合
- 精灵系统：
  - 纹理加载和管理
  - 精灵裁剪和缩放
  - 旋转和翻转（水平/垂直）
  - 颜色通道调整
- 高级渲染：
  - 批量渲染优化
  - 动态粒子系统
  - 实时性能监控

### 🧩 组件化架构
- 实体系统：基于实体-组件模式的游戏对象管理
- 精灵组件：支持动画和静态精灵
- 碰撞检测：内置碰撞体组件和碰撞检测算法
- 动画系统：帧动画和过渡动画支持

### 📐 数学和算法库
- 几何算法：
  - 中心圆算法生成
  - 向量数学运算
  - 碰撞检测算法
- 颜色处理：
  - RGBA/十六进制颜色转换
  - 颜色插值和渐变
- 性能优化：高效的数学计算和内存管理

### 🔧 开发工具
- 调试支持：
  - 实时性能监控面板
  - 帧率统计和丢帧检测
  - 详细的日志系统
- 热重载：运行时参数调整（帧率、窗口大小等）
- 跨平台：基于 SDL3 的跨平台兼容性

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
│   ├── Scene.h/.cpp       # 场景管理（Layer、Scene、SceneManager - 待完善）
│   └── preinclude.h       # 预包含头文件
├── docs/                  # 文档和教程
├── assets/                # 游戏资源文件（图片、音频等）
├── examples/              # 示例代码
├── main.cpp              # 主示例程序
└── CMakeLists.txt        # 构建配置
```

### 核心类层次
- Engine：主引擎类，管理整个游戏生命周期
- Window：窗口管理，支持多窗口操作
- Cursor：鼠标光标管理，支持自定义热点
- Painter：图形绘制器，提供所有绘图接口
- EventSystem：事件系统，处理键盘、鼠标、窗口等事件
- AudioSystem：音频系统，支持音频播放与管理
- FontSystem：字体系统，支持字体渲染
- Entity：游戏实体基类
- Spirit：精灵类，管理纹理和渲染属性
- Collider：碰撞体组件，处理碰撞检测
- ResourceSystem/FileSystem：资源加载与管理

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

## 构建、部署和安装

这里以最简要的方式来编写构建过程，具体参见[构建向导](./build.md)。

## 版本信息

- 当前版本：v1.1.0-alpha
- API 状态：稳定开发中
- 兼容性：Windows/Linux/macOS (通过 SDL3)
- 许可证：MIT 开源项目

## 贡献和支持

Easy Engine 是一个持续开发中的项目，欢迎开发者参与贡献。项目采用模块化设计，便于扩展和维护。

如需技术支持或功能建议，请通过项目文档中的联系方式获取帮助。


