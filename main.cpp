#include "src/Core.h"

using namespace EasyEngine;
using namespace Components;

int main() {
    Engine engine("Hello EasyEngine!", 1280, 720);
    engine.setBackgroundRenderingEnabled(true);
    engine.setResizable(true);
    engine.setFPS(144);
    engine.show();

    ResourceSystem::global()->setRootPath("./assets");
    ResourceSystem::global()->append("font", "Jersey_15/Jersey15-Regular.ttf", Resource::Font);

    Graphics::Ellipse ellipse(200, 0, 200, 200, StdColor::Transparent, false, true, {192, 45, 243, 128});
    Graphics::Ellipse ellipse2(400, 0, 200, 200, StdColor::Transparent, false, true, {45, 214, 243, 128});
    Graphics::Ellipse ellipse3(600, 0, 200, 200, StdColor::Transparent, false, true, {45, 96, 243, 128});
    auto inCurve = new EasingCurve::InSineCurve(1000, engine.painter());
    inCurve->setEnabled(true, true, true);
    auto outCurve = new EasingCurve::OutSineCurve(1000, engine.painter());
    outCurve->setEnabled(true, true, true);
    auto inOutCurve = new EasingCurve::InOutSineCurve(1000, engine.painter());
    inOutCurve->setEnabled(true, true, true);

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
        ellipse.pos = {ellipse.pos.x, 180 + (300 * inCurve->current())};
        painter.drawEllipse(ellipse);
        ellipse2.pos = {ellipse2.pos.x, 180 + (300 * outCurve->current())};
        painter.drawEllipse(ellipse2);
        ellipse3.pos = {ellipse3.pos.x, 180 + (300 * inOutCurve->current())};
        painter.drawEllipse(ellipse3);
        painter.drawPixelText(fmt::format("Duration: {:.2f}", inOutCurve->current()), {20, 20}, {1.f, 1.f}, StdColor::Black);
    });

    engine.installEventHandler([&](SEvent &e) {
        if (e.key.down && e.key.key == SDLK_ESCAPE) {
            return false;
        }
//        if (e.key.down && e.key.key == SDLK_Q) {
//            inCubicCurve->setDuration(1000);
//        }
//        if (e.key.down && e.key.key == SDLK_W) {
//            inCubicCurve->setDuration(2500);
//        }
//        if (e.key.down && e.key.key == SDLK_E) {
//            inCubicCurve->setDuration(5000);
//        }
//        if (e.key.down && e.key.key == SDLK_SPACE) {
//            inCubicCurve->setEnabled(!inCubicCurve->enabled());
//        }
        return true;
    });
    engine.installCleanUpEvent([&]() {
        delete text;
    });
    return engine.exec();
}