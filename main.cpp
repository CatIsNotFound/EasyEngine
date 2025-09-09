#include "src/Core.h"

#define DURATITON 60
using namespace EasyEngine;
using namespace Components;

int main() {
    Engine engine("测试动画", 1024, 800);
    engine.setBackgroundRenderingEnabled(false);
    engine.setFPS(12);
    engine.setResizable(true);
    engine.show();
    Sprite load1("load1", "assets/load1.png", engine.painter()->window()->renderer),
            load2("load2", "assets/load2.png", engine.painter()->window()->renderer),
            load3("load3", "assets/load3.png", engine.painter()->window()->renderer),
            load4("load4", "assets/load4.png", engine.painter()->window()->renderer),
            load5("load5", "assets/load5.png", engine.painter()->window()->renderer);
    Animation load_ani("loading");
    load_ani.addFrame(load1, DURATITON);
    load_ani.addFrame(load2, DURATITON);
    load_ani.addFrame(load4, DURATITON);
    load_ani.addFrame(load4, DURATITON);
    load_ani.addFrame(load5, DURATITON);
    load_ani.addFrame(load5, DURATITON);
    load_ani.addFrame(load5, DURATITON);
    load_ani.addFrame(load4, DURATITON);
    load_ani.addFrame(load3, DURATITON);
    load_ani.addFrame(load1, DURATITON);
    load_ani.addFrame(load1, DURATITON);
    load_ani.removeFrame(5);
    load_ani.removeFrame(4);
    load_ani.removeFrame(10);
    load_ani.insertFrame(load3, DURATITON, 2);
    load_ani.removeFrame(3);
    load_ani.replaceFrame(load2, 7, DURATITON);
    for (uint64_t i = 0; i < load_ani.framesCount(); ++i) {
        fmt::println("Frame {}: {} {}ms", i, load_ani.sprite(i)->name(), load_ani.durationInFrame(i));
    }

    engine.painter()->installPaintEvent([&load1, &load_ani](Painter& painter) {
        painter.fillBackColor({64, 64, 64, 255});
        load_ani.draw({100, 100}, &painter);
    });

    engine.installEventHandler([&load_ani](const SEvent& ev) {
        if (ev.button.down && ev.button.button == 1) {
            if (load_ani.isPlayedAnimation()) {
                load_ani.stop();
            } else {
                load_ani.play(false);
            }
        } else if (ev.button.down && ev.button.button == 3) {
            if (load_ani.isPlayedAnimation()) {
                load_ani.stop();
            } else {
                load_ani.play();
            }
        }

        return true;
    });
    return engine.exec();
}