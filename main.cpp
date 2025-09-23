#include "src/Core.h"

using namespace EasyEngine;
using namespace Components;

int main() {
    Engine engine("Graphics Test", 1280, 680);
    engine.setResizable(true);
    engine.setBackgroundRenderingEnabled(false);
    engine.setFPS(60);
    engine.show();

    auto res = ResourceSystem::global();
    res->setRootPath("./assets");
    res->append("red1", "red.png", Resource::Image);
    res->append("green1", "green.png", Resource::Image);
    res->append("red2", "red.png", Resource::Image);
    res->append("green2", "green.png", Resource::Image);
    res->append("red3", "red.png", Resource::Image);
    res->append("green3", "green.png", Resource::Image);
    res->append("red4", "red.png", Resource::Image);
    res->append("mask", "white_mask.png", Resource::Image);
    res->append("font", "Jersey_15/Jersey15-Regular.ttf", Resource::Font);

    Font font("font", 128.0f);
    font.setFontColor(StdColor::Yellow);
    font.setOutline(1);
    font.setOutlineColor(StdColor::LimeGreen);

    bool is_mouse_down = false;
    Layer layer("main");
    layer.append(1, new Sprite("red1", "red1", engine.painter()));
    layer.append(2, new Sprite("green1", "green1", engine.painter()));
    layer.append(3, new Sprite("red2", "red2", engine.painter()));
    layer.append(4, new Sprite("green2", "green2", engine.painter()));
    layer.append(5, new Sprite("red3", "red3", engine.painter()));
    layer.append(6, new Sprite("green3", "green3", engine.painter()));
    layer.append(7, new Sprite("red4", "red4", engine.painter()));
    layer.append(8, font.textToSprite("Hello world!", *engine.painter()));
    layer.append(124, new Sprite("red124", "red4", engine.painter()));
    layer.sprite(1)->properties()->position = {0, 0};
    layer.sprite(1)->properties()->color_alpha = StdColor::Green;
    layer.sprite(1)->properties()->scaled = 5.f;
    layer.sprite(2)->properties()->position = {100, 0};
    layer.sprite(3)->properties()->position = {200, 10};
    layer.sprite(4)->properties()->position = {200, 140};
    layer.sprite(5)->properties()->position = {300, 120};
    layer.sprite(6)->properties()->position = {400, 100};
    layer.sprite(7)->properties()->position = {400, 400};
    layer.remove(7);
    layer.swap(1, 3);
    layer.setZOrder(4, 14);
    layer.sprite(layer.indexOf("Hello world!"))->properties()->position = {20, 200};
    fmt::println("Index of {}\n", layer.indexOf("green2"));
    fmt::println("Index of {}\n", layer.indexOf("red124", 1));
    layer.setVisible(true);
    layer.setViewportPainter(engine.painter());
    layer.setViewport(0, 0, 600, 600);
    Sprite mask("mask", "mask", engine.painter());
    mask.resize(108, 108);
    mask.properties()->color_alpha = StdColor::Black;
    engine.painter()->installPaintEvent([&](Painter& p) {
        p.fillBackColor(StdColor::Black);
        Vector2 pos = Cursor::global()->position();
        int s = (is_mouse_down ? 0 : 100);
        layer.setClipViewport((int)pos.x - 50, (int)pos.y - 50, s, s);
        layer.draw(false, true);
        if (!is_mouse_down)
        mask.draw({pos.x - mask.size().width / 2, pos.y - mask.size().height / 2});
        p.setClipView({0, 0, 0, 0});
        p.drawPixelText(fmt::format("Move mouse! FPS: {}", engine.fps()), {10, 10}, {1.f, 1.f}, StdColor::White);
        if (!is_mouse_down)
        mask.draw({static_cast<float>(engine.window()->geometry.width - 120),
                   static_cast<float>(engine.window()->geometry.height - 120)}, StdColor::White);
    });

    engine.installEventHandler([&](SEvent& e) {
        if (e.button.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            is_mouse_down = true;
        } else if (e.button.type == SDL_EVENT_MOUSE_BUTTON_UP) {
            is_mouse_down = false;
        }
        return true;
    });

    return engine.exec();
}