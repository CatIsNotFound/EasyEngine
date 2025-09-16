#include "src/Core.h"

using namespace EasyEngine;
using namespace Components;
using Clip = GeometryF;

int main() {
    Engine engine("...", 1024, 800);
    engine.setBackgroundRenderingEnabled(false);
    engine.setFPS(60);
    engine.setResizable(true);
    engine.show();

    int i = 1;
    Sprite load1(fmt::format("Load{}", i), fmt::format("assets/load{}.png", i++), engine.window()->renderer);
    Sprite load2(fmt::format("Load{}", i), fmt::format("assets/load{}.png", i++), engine.window()->renderer);
    Sprite load3(fmt::format("Load{}", i), fmt::format("assets/load{}.png", i++), engine.window()->renderer);
    Sprite load4(fmt::format("Load{}", i), fmt::format("assets/load{}.png", i++), engine.window()->renderer);
    Sprite load5(fmt::format("Load{}", i), fmt::format("assets/load{}.png", i++), engine.window()->renderer);
    Animation ani("Loading");
    ani.addFrame(&load1, 500);
    ani.addFrame(&load2, 500);
    ani.addFrame(&load3, 500);
    ani.addFrame(&load4, 500);
    ani.addFrame(&load5, 500);
    ani.play(true);
    Sprite number("number", "assets/number.png", engine.window()->renderer);

    std::array<Clip, 10> numbers;
    numbers[1] = {.pos = {15, 18}, .size = {42, 60}};
    numbers[2] = {.pos = {80, 18}, .size = {42, 60}};
    numbers[3] = {.pos = {145, 18}, .size = {42, 60}};
    numbers[4] = {.pos = {210, 18}, .size = {42, 60}};
    numbers[5] = {.pos = {275, 18}, .size = {42, 60}};
    numbers[6] = {.pos = {15, 120}, .size = {42, 60}};
    numbers[7] = {.pos = {80, 120}, .size = {42, 60}};
    numbers[8] = {.pos = {145, 120}, .size = {42, 60}};
    numbers[9] = {.pos = {210, 120}, .size = {42, 60}};
    numbers[0] = {.pos = {275, 120}, .size = {42, 60}};
    number.properties()->clip_mode = true;
    number.properties()->position = {100, 100};
    number.resize(42, 60);
    number.properties()->color_alpha = StdColor::Gold;

    Sprite button_sprite("buttons", "assets/button.png", engine.window()->renderer);
    std::array<Clip, 5> button_clip;
    button_clip[0] = {.pos = {11, 7}, .size = {144, 65}};
    button_clip[1] = {.pos = {165, 7}, .size = {144, 65}};
    button_clip[2] = {.pos = {319, 7}, .size = {144, 65}};
    button_clip[3] = {.pos = {473, 7}, .size = {144, 65}};
    button_clip[4] = {.pos = {627, 7}, .size = {144, 65}};
    button_sprite.resize(144, 65);
    button_sprite.properties()->clip_mode = true;
    Control my_button("my_button", std::move(button_sprite));
    my_button.setStatus(Control::Status::Default, button_clip[0]);
    my_button.setStatus(Control::Status::Active, button_clip[4]);
    my_button.setStatus(Control::Status::Disabled, button_clip[3]);
    my_button.setStatus(Control::Status::Hovered, button_clip[1]);
    my_button.setStatus(Control::Status::Pressed, button_clip[2]);
    my_button.setGeometryForHotArea({0, 0}, button_sprite.size());
    my_button.move(200, 220);
    my_button.setEvent(Control::Event::MouseHover, [] { fmt::println("Hoverd"); Cursor::global()->setCursor(Cursor::Hand); });
    my_button.setEvent(Control::Event::MouseLeave, [] { Cursor::global()->setCursor(Cursor::Default); fmt::println("Leave");});
    my_button.setEvent(Control::Event::MouseDown,[]{ fmt::println("Mouse Down!"); });
    my_button.setEvent(Control::Event::MouseUp, [] { fmt::println("Mouse Up!"); });
    my_button.setEvent(Control::Event::KeyPressed, [] { fmt::println("Pressed");});
    my_button.setEvent(Control::Event::Clicked, []{fmt::println("Clicked on button 1!");});
    my_button.setEvent(Control::Event::DblClicked, []{fmt::println("DblClicked");});
    my_button.setEvent(Control::Event::KeyDown, []{fmt::println("KeyDown");});
    my_button.setEvent(Control::Event::KeyUp, []{fmt::println("KeyUp");});
    my_button.setEvent(Control::Event::EnabledChange, [&my_button] {
        fmt::println("Control can {}use.", my_button.enabled() ? "not " : "");
    });
    my_button.setActive();
    my_button.resize(button_clip[0].size * 0.75f);
    Control copied_button("Copied_button", my_button);
    copied_button.removeEvent(Control::Event::Clicked);
    copied_button.setEvent(Control::Event::Clicked, [&my_button]{
        my_button.setEnabled(!my_button.enabled());
    });
    copied_button.move(500, 200);
    copied_button.resize(button_clip[0].size * 1.25f);
    auto timer = new Timer(500, [&]{
        int n = ani.currentFrame();
        number.properties()->clip_pos = numbers[n].pos;
        number.properties()->clip_size = numbers[n].size;
    });
    timer->start(true);
    engine.painter()->installPaintEvent([&] (Painter& painter) {
        painter.fillBackColor(StdColor::DarkGray);
        ani.draw({200, 200}, &painter);
        number.draw(&painter);
        my_button.update(&painter);
        copied_button.update(&painter);
        engine.setWindowTitle(fmt::format("FPS: {}", engine.fps()));
    });
    fmt::println("结果：{}", Algorithm::compareRect(my_button.hotArea(), copied_button.hotArea()));
    Graphics::Ellipse ellipse1, ellipse2;
    ellipse1.pos = {0, 0};
    ellipse1.area = {100, 100};
    ellipse2.pos = {50, 100};
    ellipse2.area = {100, 100};
    fmt::println("结果：{}", Algorithm::compareEllipse(ellipse1, ellipse2));
    return engine.exec();
}