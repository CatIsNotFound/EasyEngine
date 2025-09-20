#include "src/Core.h"

using namespace EasyEngine;
using namespace Components;

int main() {
    Engine engine("Test Entity");
    engine.setResizable(true);
    engine.setFPS(60);
    engine.show();
    Sprite sp_test("test", "assets/load1.png", engine.window()->renderer), sp_block("red", "assets/red.png", engine.window()->renderer);
    Sprite sp_green("green", "assets/green.png", engine.window()->renderer);

    Entity test("test", std::move(sp_test));
    Entity block("red", sp_block);
    Entity green("green", sp_test);
    test.setPosition(100, 100);
    test.collider()->setEnabled(true);
    block.setPosition(200, 200);
    block.setColliderSelf(Graphics::Rectangle());
    block.collider()->setEnabled(true);
    fmt::println("Test: {} {} {} {}", block.collider()->bounds().pos.x, block.collider()->bounds().pos.y, block.collider()->bounds().size.width, block.collider()->bounds().size.height);
    green.setColliderSelf(Graphics::Ellipse());
    Graphics::Ellipse show_box(0, 0, 0, 0, StdColor::Yellow);
    auto self = block.self<Sprite>();
    show_box.area.reset(self->size());
    show_box.pos.reset(block.position() + block.centerPosition());
    engine.installEventHandler([&](SEvent ev) {
        auto cur_pos = Cursor::global()->position();
        green.setPosition(cur_pos - green.centerPosition());
        if (green.collider()->check(*block.collider()) >= 0) {
            block.setPosition((float)(rand() % 600), (float)(rand() % 400));
        }
        return true;
    });
    engine.painter()->installPaintEvent([&](Painter& painter) {
        painter.fillBackColor(StdColor::Gray);
        // test.update(&painter);
        block.update(&painter);
        green.update(&painter);
    });

    return engine.exec();
}