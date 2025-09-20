#define NO_ABOUT_MESSAGE
#include "src/Core.h"
using namespace EasyEngine;
using namespace Components;

int main() {
    Engine engine("", 1024, 800);
    engine.setResizable(true);
    engine.setFPS(60);
    engine.show();
    engine.setFullScreen(true, true);
    uint32_t show_font = 0;
    Components::Font font("./assets/SourceHanSansCN-Medium.otf", 96.0f);
    font.setFontColor({255, 255, 255, 127});
    font.setOutline(1);
    font.setOutlineColor({0, 0, 0, 127});
    font.setFontHinting(Font::Hinting::Light);
    font.setFontKerning(true);
    auto sprite = font.textToSprite("Success!", *engine.painter());
    font.setFontColor(StdColor::Transparent);
    auto border_sprite = font.textToSprite("Success!", *engine.painter());
    font.setFontColor(StdColor::White);
    font.setOutline();
    auto filled_sprite = font.textToSprite("Success!", *engine.painter());
    engine.painter()->installPaintEvent([&](Painter& painter) {
        painter.fillBackColor(StdColor::BurlyWood);
        if (show_font == 0)
            painter.drawSprite(sprite, {(float)(engine.screenGeometry().width / 2) - sprite.size().width / 2,
                                        (float)(engine.screenGeometry().height / 2) - sprite.size().height / 2});
        else if (show_font == 1)
            painter.drawSprite(border_sprite, {(float)(engine.screenGeometry().width / 2) - border_sprite.size().width / 2,
                                               (float)(engine.screenGeometry().height / 2) - border_sprite.size().height / 2});
        else if (show_font == 2)
            painter.drawSprite(filled_sprite, {(float)(engine.screenGeometry().width / 2) - filled_sprite.size().width / 2,
                                               (float)(engine.screenGeometry().height / 2) - filled_sprite.size().height / 2});
    });
    engine.installEventHandler([&show_font](SEvent ev) {
        if (ev.key.down && ev.key.key == SDLK_ESCAPE) {
            return false;
        }
        if (ev.key.down && ev.key.key == SDLK_1) {
            show_font = 1;
        } else if (ev.key.down && ev.key.key == SDLK_2) {
            show_font = 2;
        } else if (ev.key.down && ev.key.key == SDLK_0) {
            show_font = 0;
        }
        return true;
    });
    engine.installCleanUpEvent([]{
        fmt::println("Ended!");
    });

    return engine.exec();
}