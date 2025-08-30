# Easy Engine 游戏引擎文档

## 项目概述

Easy Engine 是一个基于 SDL3 图形引擎库二次封装的开源游戏引擎，专为简化游戏开发流程而设计。它提供了现代化的 C++23 接口，将复杂的 SDL3 操作封装成直观易用的 API，让开发者能够专注于游戏逻辑而非底层图形编程。

引擎采用模块化设计，包含核心引擎管理、图形渲染、组件系统、算法库等多个模块，为 2D 游戏开发提供了完整的解决方案。无论是开发简单的 2D 游戏，还是构建复杂的图形应用，Easy Engine 都能提供强大的支持。

## 主要特性

### 🎮 核心引擎功能
- **多窗口管理**：支持创建和管理多个独立窗口
- **事件驱动架构**：完整的事件处理系统，支持键盘、鼠标、窗口事件
- **帧率控制**：精确的帧率管理，支持动态调整帧率限制
- **资源管理**：自动化的纹理、音频资源加载和管理

### 🎨 图形渲染系统
- **基本图形绘制**：
  - 点、线、矩形、椭圆等基础图形
  - 支持填充和描边模式
  - 透明度和颜色混合
- **精灵系统**：
  - 纹理加载和管理
  - 精灵裁剪和缩放
  - 旋转和翻转（水平/垂直）
  - 颜色通道调整
- **高级渲染**：
  - 批量渲染优化
  - 动态粒子系统
  - 实时性能监控

### 🧩 组件化架构
- **实体系统**：基于实体-组件模式的游戏对象管理
- **精灵组件**：支持动画和静态精灵
- **碰撞检测**：内置碰撞体组件和碰撞检测算法
- **动画系统**：帧动画和过渡动画支持

### 📐 数学和算法库
- **几何算法**：
  - 中心圆算法生成
  - 向量数学运算
  - 碰撞检测算法
- **颜色处理**：
  - RGBA/十六进制颜色转换
  - 颜色插值和渐变
- **性能优化**：高效的数学计算和内存管理

### 🔧 开发工具
- **调试支持**：
  - 实时性能监控面板
  - 帧率统计和丢帧检测
  - 详细的日志系统
- **热重载**：运行时参数调整（帧率、窗口大小等）
- **跨平台**：基于 SDL3 的跨平台兼容性

## 技术架构

### 模块结构
```
EasyEngine/
├── Core/           # 引擎核心（窗口、事件、渲染管理）
├── Components/     # 游戏组件（精灵、实体、碰撞体）
├── Basic/         # 基础数据结构（图形、数学、颜色）
├── Algorithm/     # 算法库（几何、数学、工具）
├── Resources/     # 资源管理（纹理、音频、字体）
└── Utils/         # 工具类和辅助函数
```

### 核心类层次
- **Engine**：主引擎类，管理整个游戏生命周期
- **Window**：窗口管理，支持多窗口操作
- **Painter**：图形绘制器，提供所有绘图接口
- **Entity**：游戏实体基类
- **Spirit**：精灵类，管理纹理和渲染属性
- **Collider**：碰撞体组件，处理碰撞检测

## 快速开始

### 最小示例
```cpp
#include "src/Core.h"
using namespace EasyEngine;

int main() {
    // 创建引擎实例
    Engine engine("我的游戏", 800, 600);
    
    // 设置事件处理器
    engine.installEventHandler([&engine](SDL_Event event) {
        if (event.type == SDL_QUIT) return false;
        return true;
    });
    
    // 设置渲染回调
    engine.painter()->installPaintEvent([&](Painter& painter) {
        painter.fillBackColor(SColor(50, 50, 80, 255));
        
        // 绘制一个红色矩形
        Graphics::Rectangle rect(100, 100, 200, 150, SColor(255, 0, 0, 255));
        rect.filled_mode = true;
        painter.drawRectangle(rect);
    });
    
    engine.show();
    return engine.exec();
}
```

### 高级功能示例

#### 精灵动画
```cpp
// 创建精灵
Spirit player("player", "assets/player.png", renderer);
player.setPosition(Vector2(400, 300));

// 在渲染循环中
engine.painter()->installPaintEvent([&](Painter& painter) {
    // 绘制精灵
    player.draw(Vector2(400, 300), &painter);
    
    // 添加旋转效果
    static float rotation = 0;
    rotation += 1.0f;
    player.draw(Vector2(400, 300), rotation, Vector2(32, 32), 
                FlipMode::None, &painter);
});
```

#### 碰撞检测
```cpp
Entity player("player");
Entity enemy("enemy");

// 在更新循环中检查碰撞
if (player.isCollision(enemy)) {
    // 处理碰撞逻辑
}
```

#### 性能监控
```cpp
// 启用帧率显示
engine.setFPS(60);
engine.setFrameDropTolerance(1.5f);

// 实时显示性能数据
engine.painter()->installPaintEvent([&](Painter& painter) {
    char fpsText[64];
    sprintf(fpsText, "FPS: %u", engine.fps());
    
    // 绘制性能面板
    Graphics::Rectangle panel(10, 10, 200, 80, SColor(0, 0, 0, 128));
    panel.filled_mode = true;
    painter.drawRectangle(panel);
    
    // 这里可以添加文字渲染
});
```

## 构建和部署

### 系统要求
- **编译器**：支持 C++23 的现代编译器
- **依赖库**：
  - SDL3 (图形和窗口管理)
  - SDL3_image (图像加载)
  - SDL3_ttf (字体渲染)
  - SDL3_mixer (音频播放)
  - fmt (格式化库)

### 构建命令
```bash
# 配置项目
cmake -B build

# 构建项目
cmake --build build

# 运行示例
./build/EasyEngine
```

### 开发环境配置
项目已配置好 CLion/Visual Studio 的 CMake 支持，可直接导入构建。

## 版本信息

- **当前版本**：0.1.1-beta
- **API 状态**：稳定开发中
- **兼容性**：Windows/Linux/macOS (通过 SDL3)
- **许可证**：开源项目

## 贡献和支持

Easy Engine 是一个持续开发中的项目，欢迎开发者参与贡献。项目采用模块化设计，便于扩展和维护。

如需技术支持或功能建议，请通过项目文档中的联系方式获取帮助。


