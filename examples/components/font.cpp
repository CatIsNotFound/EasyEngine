#include <EasyEngine/Core.h>

using namespace EasyEngine;
using namespace Components;

int main() {
    Engine engine("Hello EasyEngine!", 1280, 720);
    engine.setBackgroundRenderingEnabled(true);
    engine.setResizable(true);
    engine.setFPS(30);
    engine.show();

    ResourceSystem::global()->setRootPath("./assets");
    ResourceSystem::global()->append("font", "Jersey_15/Jersey15-Regular.ttf", Resource::Font);

    Font font("font", 96.f);
    font.setFontColor(StdColor::Yellow);
    font.setOutlineColor(StdColor::DarkOrange);
    font.setOutline(1);
    Sprite* text = font.textToSprite("text", "Hello EasyEngine!", engine.painter());
    engine.painter()->installPaintEvent([&](Painter &painter) {
        painter.fillBackColor(StdColor::LightYellow);
        auto screen = engine.window()->geometry;
        text->properties()->position = {screen.width / 2 - text->size().width / 2, 
                                    screen.height / 2 - text->size().height / 2};
        text->draw();
    });

    engine.installEventHandler([&](SEvent &e) {
        if (e.key.down && e.key.key == SDLK_ESCAPE) {
            return false;
        }
        return true;
    });
    engine.installCleanUpEvent([&]() {
        delete text;
    });
    return engine.exec();
}