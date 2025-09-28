#include <EasyEngine/Core.h>
using namespace EasyEngine;
using namespace Components;

int main() {
    // 创建引擎实例
    Engine engine("Hello EasyEngine!", 800, 600);
    engine.show();

    // 设置事件处理器
    engine.installEventHandler([](SDL_Event& e) {
        if (e.key.down && e.key.key == SDLK_ESCAPE) {
            return false;
        }
        return true;
    });

    // 设置渲染回调
    engine.painter()->installPaintEvent([&](Painter& painter) {
        painter.fillBackColor(StdColor::White);

        // 绘制一个红色矩形
        painter.setThickness(20);
        Graphics::Rectangle rect(100, 100, 200, 150, StdColor::Red, true, true, StdColor::Yellow);
        painter.drawRectangle(rect);
        // 添加像素文字
        painter.drawPixelText("Hello EasyEngine!", {20, 20}, {1.f, 1.f}, StdColor::Black);
    });
    return engine.exec();
}