#include "src/Core.h"

using namespace EasyEngine;
using namespace Components;

int main() {
    Engine engine("Graphics Test", 1024, 800);
    engine.setFPS(60);
    engine.show();
    static bool is_mouse_down = false;
    ResourceSystem::global()->setRootPath("./assets");
    ResourceSystem::global()->append("pixel", "Jersey_15/Jersey15-Regular.ttf", Resource::Font);
    FontSystem::global()->loadFont("pixel", "pixel", 128.f);
    Cursor::global()->setVisible(false);
    ResourceSystem::global()->append("icon", "my_cursor.png", Resource::Image);
    ResourceSystem::global()->append("clicked", "clicked_cursor.png", Resource::Image);
    ResourceSystem::global()->append("btn", "Button.png", Resource::Image);
    ResourceSystem::global()->append("bgm", "Peace.wav", Resource::Audio);
    BGM bgm("bgm");
    bgm.play();
    Sprite icon("icon", "icon", engine.painter());
    Sprite clicked("icon_clicked", "clicked", engine.painter());
    Sprite clip_button("icon", "btn", engine.painter());
    clip_button.setResource("btn");
    Control button("button", clip_button);
    button.setStatus(Control::Status::Default, GeometryF({8, 5}, {142, 65}));
    button.setStatus(Control::Status::Hovered, GeometryF({162, 5}, {142, 65}));
    button.setStatus(Control::Status::Pressed, GeometryF({316, 5}, {142, 65}));
    button.setStatus(Control::Status::Disabled, GeometryF({470, 5}, {142, 65}));
    button.setStatus(Control::Status::Active, GeometryF({624, 5}, {142, 65}));
    button.setEvent(Control::Event::Clicked, [] { fmt::println("Hello world!"); });
    button.setEvent(Control::Event::KeyPressed, [] { fmt::println("Hello world!"); });
    button.move(340, 400);
    button.resize(142, 65);
    button.setGeometryForHotArea(0, 0, 142, 65);
    button.setActive();
    Control button2("nb", button);
    button2.move(340, 550);
    button2.setInactive();
    auto pixel_font = FontSystem::global()->font("pixel");
    pixel_font->setFontColor(StdColor::DarkBlue);
    pixel_font->setOutline(2);
    pixel_font->setOutlineColor({255, 143, 92, 192});
    auto pixel_text = pixel_font->textToSprite("Hello EasyEngine!", *engine.painter());
    auto timer = new Timer(5000, [&icon]{
        fmt::println("Timer is up!");
        icon.properties()->color_alpha = {255, 255, 255, 128};
    });
    engine.painter()->installPaintEvent([&](Painter& painter) {
        painter.fillBackColor(StdColor::White);
        pixel_text.draw(Vector2{100, 100});
        if (is_mouse_down) clicked.draw();
        else icon.draw();
    });
    engine.installEventHandler([&](SEvent& ev) {
        timer->stop();
        icon.properties()->color_alpha = StdColor::White;
        if (ev.button.type == SDL_EVENT_MOUSE_BUTTON_DOWN) is_mouse_down = true;
        if (ev.button.type == SDL_EVENT_MOUSE_BUTTON_UP) is_mouse_down = false;
        if (!is_mouse_down)
            icon.properties()->position = Cursor::global()->position();
        else
            clicked.properties()->position = Cursor::global()->position();

        timer->start(false);
        return true;
    });
    timer->start(false);
    return engine.exec();
}