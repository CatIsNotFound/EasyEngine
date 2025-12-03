#include "../src/Core.h"

using namespace EasyEngine;
int main() {
    Engine engine("测试窗口");
    engine.setResizable(true);
    engine.show();

    auto res = ResourceSystem::global();
    res->append("block", "assets/gear.png", Resource::Image);
    res->append("load1", "assets/Load1.png", Resource::Image);
    res->append("load2", "assets/Load2.png", Resource::Image);
    res->append("load3", "assets/Load3.png", Resource::Image);
    res->append("load4", "assets/Load4.png", Resource::Image);
    res->append("load5", "assets/Load5.png", Resource::Image);

    Components::Sprite block("block", "block", engine.painter());
    Components::FrameAnimation ani("loading", std::vector<Components::Sprite *>{
                                               new Components::Sprite("load1", "load1", engine.painter()),
                                               new Components::Sprite("load2", "load2", engine.painter()),
                                               new Components::Sprite("load3", "load3", engine.painter()),
                                               new Components::Sprite("load4", "load4", engine.painter()),
                                               new Components::Sprite("load5", "load5", engine.painter())});
    Components::Entity entity("block", block);
    entity.setPosition(engine.window()->geometry.width / 2 - block.size().width,
                       engine.window()->geometry.height / 2 - block.size().height);
    entity.setVisible(true);
//    Graphics::Rectangle rect(0, 0, block.size().width, block.size().height, StdColor::Yellow);
    Graphics::Ellipse ellipse(0, 0, block.size().width, block.size().height, StdColor::Yellow);
    entity.setColliderSelf(ellipse);

    Components::Entity load_obj("load", ani);
    load_obj.setPosition({20, 20});
    load_obj.setVisible(true);
    engine.painter()->installPaintEvent([&entity, &load_obj](Painter& painter) {
        painter.fillBackColor(StdColor::DarkSlateGray);
        entity.update();
        load_obj.update();
        painter.drawEllipse(*entity.collider()->shape<Graphics::Ellipse>());
    });
    auto win = engine.window()->geometry;
    engine.installEventHandler([&entity, &win](SEvent& ev) {
        auto size = entity.self<Components::Sprite>()->size();
        auto pos = Cursor::global()->position() - Vector2(size.width / 2, size.height / 2);
        static Vector2 real = {0, 0};
        if (pos.x >= 0 && pos.x <= win.width - size.width && pos.y >= 0 && pos.y <= win.height - size.height) {
            real = pos;
        }
        entity.setPosition(real);
        return true;
    });


    return engine.exec();
}

