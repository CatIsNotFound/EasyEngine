#define EASYENGINE_NO_ABOUT_INFO
#include "src/Core.h"
using namespace EasyEngine;
using namespace Components;

int main() {
    Engine engine("", 1024, 800);
    engine.setResizable(true);
    engine.setFPS(60);
    engine.show();
    auto res_sys = ResourceSystem::global();
    res_sys->setRootPath("./assets");
    res_sys->load("icon", "./demo.png", ResourceSystem::Resource::Image);
    res_sys->load("bgm", "./bgm.mp3", ResourceSystem::Resource::Audio);
    res_sys->load("load1", "./Load1.png", ResourceSystem::Resource::Image);
    res_sys->load("load3", "./Load3.png", ResourceSystem::Resource::Image);
    res_sys->load("text", "./text.txt", ResourceSystem::Resource::Text);
//    auto res = res_sys->metaData("text");
//    fmt::println("Index: {}", std::get<1>(res));
//    fmt::println("{}", std::get<res.index()>(res));
    engine.painter()->installPaintEvent([](Painter& painter) {
        painter.fillBackColor(StdColor::White);

    });

    return engine.exec();
}