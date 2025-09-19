#define NO_ABOUT_MESSAGE
#include "src/Core.h"
using namespace EasyEngine;
using namespace Components;

int main() {
    Engine engine("", 1024, 800);
    engine.setResizable(true);
    engine.setFPS(60);
    engine.show();
    engine.setFullScreen(true, false);
    Components::Font font("./assets/SourceHanSansCN-Heavy.otf", 96.0f);
    font.setFontColor(StdColor::Black);
    font.setOutline(1);
    font.setOutlineColor(StdColor::White);
    font.setFontHinting(Font::Hinting::Light);
    font.setFontKerning(true);
//    font.setLineSpacing(10);
    auto sprite = font.textToSprite("Success!", *engine.painter());
    fmt::println("SP: SIZE({}, {})", sprite.size().width, sprite.size().height);
    engine.painter()->installPaintEvent([&](Painter& painter) {
        painter.fillBackColor(StdColor::BurlyWood);
        painter.drawSprite(sprite, {(float)(engine.screenGeometry().width / 2) - sprite.size().width / 2,
                                    (float)(engine.screenGeometry().height / 2) - sprite.size().height / 2});
    });
    engine.installEventHandler([](SEvent ev) {
        if (ev.key.down && ev.key.key == SDLK_ESCAPE) {
            return false;
        }
        return true;
    });
    engine.installCleanUpEvent([&]{
        font.unload();
    });

    return engine.exec();
}