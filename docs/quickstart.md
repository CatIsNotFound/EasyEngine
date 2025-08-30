# Easy Engine 快速入门指南

本指南将帮助您快速上手 Easy Engine 游戏引擎，从基础概念到实际项目开发。

## 1. 环境准备

### 1.1 系统要求
- **操作系统**: Windows 10/11, Linux (Ubuntu 20.04+), macOS 10.15+
- **编译器**: 
  - Windows: Visual Studio 2022 或 MinGW-w64
  - Linux: GCC 11+ 或 Clang 14+
  - macOS: Xcode 14+ 或 Clang
- **CMake**: 3.31 或更高版本

### 1.2 安装依赖

#### Windows (使用 vcpkg)
```bash
vcpkg install sdl3 sdl3-image sdl3-ttf sdl3-mixer fmt
```

#### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install libsdl3-dev libsdl3-image-dev libsdl3-ttf-dev libsdl3-mixer-dev libfmt-dev
```

#### macOS (使用 Homebrew)
```bash
brew install sdl3 sdl3_image sdl3_ttf sdl3_mixer fmt
```

## 2. 第一个项目

### 2.1 创建项目结构
```
MyGame/
├── CMakeLists.txt
├── main.cpp
├── assets/
│   ├── images/
│   └── sounds/
└── build/
```

### 2.2 CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.31)
project(MyGame)

set(CMAKE_CXX_STANDARD 23)

# 查找依赖
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(fmt REQUIRED)

# 添加可执行文件
add_executable(MyGame main.cpp)

# 链接库
target_link_libraries(MyGame PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    SDL3_mixer::SDL3_mixer
    fmt::fmt
)

# 包含 Easy Engine 源码
include_directories(path/to/EasyEngine/src)
```

### 2.3 基础游戏循环

```cpp
#include "../EasyEngine/src/Core.h"
using namespace EasyEngine;

class MyGame {
private:
    Engine engine;
    std::unique_ptr<Spirit> player;
    
public:
    MyGame() : engine("我的游戏", 800, 600) {
        initialize();
    }
    
    void initialize() {
        // 设置窗口属性
        engine.setResizable(true);
        engine.setFPS(60);
        
        // 加载资源
        loadAssets();
        
        // 设置事件处理
        setupEventHandlers();
        
        // 设置渲染循环
        setupRenderLoop();
    }
    
    void loadAssets() {
        // 这里可以加载精灵、音频等资源
        // player = std::make_unique<Spirit>("player", "assets/player.png", 
        //                                   engine.painter()->renderer());
    }
    
    void setupEventHandlers() {
        engine.installEventHandler([this](SDL_Event event) {
            switch(event.type) {
                case SDL_EVENT_KEY_DOWN:
                    handleKeyPress(event.key.key);
                    break;
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    return false;
            }
            return true;
        });
    }
    
    void handleKeyPress(SDL_KeyCode key) {
        switch(key) {
            case SDLK_ESCAPE:
                // 退出游戏
                break;
            case SDLK_SPACE:
                // 跳跃或其他动作
                break;
        }
    }
    
    void setupRenderLoop() {
        engine.painter()->installPaintEvent([this](Painter& painter) {
            // 清空屏幕
            painter.fillBackColor(SColor(135, 206, 235, 255)); // 天蓝色背景
            
            // 绘制游戏世界
            renderWorld(painter);
            
            // 绘制UI
            renderUI(painter);
        });
    }
    
    void renderWorld(Painter& painter) {
        // 绘制地面
        Graphics::Rectangle ground(0, 550, 800, 50, SColor(34, 139, 34, 255));
        ground.filled_mode = true;
        painter.drawRectangle(ground);
        
        // 绘制玩家（示例）
        Graphics::Rectangle player(375, 450, 50, 100, SColor(255, 0, 0, 255));
        player.filled_mode = true;
        painter.drawRectangle(player);
    }
    
    void renderUI(Painter& painter) {
        // 绘制简单的UI背景
        Graphics::Rectangle uiPanel(10, 10, 200, 60, SColor(0, 0, 0, 128));
        uiPanel.filled_mode = true;
        painter.drawRectangle(uiPanel);
        
        // 可以在这里添加分数、生命条等
    }
    
    int run() {
        engine.show();
        return engine.exec();
    }
};

int main() {
    MyGame game;
    return game.run();
}
```

## 3. 核心概念

### 3.1 引擎管理
- **Engine**: 主引擎类，管理游戏生命周期
- **Window**: 窗口管理，支持多窗口
- **Painter**: 渲染器，负责所有绘图操作

### 3.2 图形系统
- **基本图形**: Point, Line, Rectangle, Ellipse
- **精灵系统**: Spirit 类管理纹理和渲染
- **颜色系统**: RGBA 和十六进制颜色支持

### 3.3 事件处理
```cpp
// 键盘事件
case SDL_EVENT_KEY_DOWN:
    if (event.key.key == SDLK_LEFT) {
        // 左移
    }
    break;

// 鼠标事件
case SDL_EVENT_MOUSE_MOTION:
    mouseX = event.motion.x;
    mouseY = event.motion.y;
    break;
```

## 4. 进阶功能

### 4.1 精灵动画
```cpp
// 创建精灵
Spirit player("player", "assets/player.png", renderer);

// 动画帧
std::vector<SDL_Rect> frames = {
    {0, 0, 32, 32},    // 第1帧
    {32, 0, 32, 32},   // 第2帧
    {64, 0, 32, 32}    // 第3帧
};

// 在渲染循环中
static int currentFrame = 0;
static Uint32 lastFrameTime = 0;
Uint32 currentTime = SDL_GetTicks();

if (currentTime - lastFrameTime > 100) { // 100ms 每帧
    currentFrame = (currentFrame + 1) % frames.size();
    lastFrameTime = currentTime;
}

// 绘制当前帧
player.draw(Vector2(x, y), frames[currentFrame], &painter);
```

### 4.2 碰撞检测
```cpp
Entity player("player");
Entity enemy("enemy");

// 设置位置
player.setPosition(Vector2(100, 100));
enemy.setPosition(Vector2(150, 150));

// 检测碰撞
if (player.isCollision(enemy)) {
    // 碰撞处理
}
```

### 4.3 性能监控
```cpp
// 启用性能监控
engine.setFPS(60);
engine.setFrameDropTolerance(1.5f);

// 在渲染循环中显示FPS
char fpsText[32];
sprintf(fpsText, "FPS: %u", engine.fps());
// 这里可以添加文字渲染代码
```

## 5. 最佳实践

### 5.1 资源管理
- 预加载所有资源到内存
- 使用精灵表减少纹理切换
- 合理管理内存，避免泄漏

### 5.2 性能优化
- 使用对象池减少内存分配
- 批量渲染相似对象
- 合理设置帧率限制

### 5.3 代码结构
```cpp
// 推荐的项目结构
class Game {
private:
    Engine engine;
    ResourceManager resources;
    GameState state;
    
public:
    void initialize();
    void update(float deltaTime);
    void render();
    void handleInput();
};
```

## 6. 常见问题

### Q: 如何处理窗口大小变化？
```cpp
engine.installEventHandler([&](SDL_Event event) {
    if (event.type == SDL_EVENT_WINDOW_RESIZED) {
        int newWidth = event.window.data1;
        int newHeight = event.window.data2;
        // 调整游戏逻辑
    }
    return true;
});
```

### Q: 如何添加音效？
```cpp
// Easy Engine 支持 SDL3_mixer
// 使用 Resources 类加载音频资源
// resources.loadSound("jump", "assets/jump.wav");
```

### Q: 如何处理不同分辨率？
```cpp
// 使用相对坐标
float relativeX = mouseX / (float)windowWidth;
float relativeY = mouseY / (float)windowHeight;
```

## 7. 下一步

完成本指南后，建议：

1. 查看 [完整文档](mainpage.md) 了解所有 API
2. 研究示例程序 [main.cpp](../main.cpp.old) 学习高级用法
3. 尝试创建自己的游戏项目
4. 加入社区获取支持和分享经验

## 8. 获取帮助

- 📖 **文档**: 查看完整 [API 文档](mainpage.md)
- 🐛 **问题**: 在 GitHub 提交 [Issue](https://github.com/your-repo/EasyEngine/issues)
- 💬 **讨论**: 参与社区讨论

---

**祝您开发愉快！** Easy Engine 让游戏开发变得简单而有趣。


