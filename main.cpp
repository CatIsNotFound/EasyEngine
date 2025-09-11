#include "src/Core.h"

#define DURATITON 60
using namespace EasyEngine;
using namespace Components;

uint8_t group = 0;
bool is_fulled = false;
int main() {
    Engine engine("...", 1024, 800);
    engine.setBackgroundRenderingEnabled(true);
    engine.setFPS(30);
    engine.setResizable(true);
    engine.show();
    Cursor::global()->setCursor(Cursor::Forbbiden);
    AudioSystem::global()->setAudioSpec(StdAudioSpec::Low);
    BGM bgm("assets/output/output.mp3");
    uint64_t st = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    std::vector<Sprite*> sprites;
    for (int i = 0; i < 500; ++i) {
        sprites.push_back(new Sprite(fmt::format("F{}", i), fmt::format("assets/output/output_{:0>4}.png", i + 1),
                                     engine.window()->renderer));
        engine.setWindowTitle(fmt::format("Loading: {}/500", i));
    }
    Animation ani("ani", sprites,33);
    ani.play(true, 0);
    bgm.play(false);
    uint64_t ed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    fmt::println("用时：{}ms", ed - st);

    auto trigger = new Trigger();
    trigger->setCondition([&ani]() {
        return ani.timer()->count() > 0;
    });
    trigger->setEvent([&ani, &engine](){
        if (ani.playedCount() > 0) {
            group = (group == 13 ? 0 : group + 1);
            ani.clearPlayedCount();
        }
        ani.timer()->clearCount();
        ani.sprite(ani.currentFrame())->setPath(
                fmt::format("assets/output/output_{:0>4}.png", 500 * group + ani.currentFrame() + 1));
    });
    trigger->setEnabled(true);
//    auto timer = new Timer(1000, []{ fmt::println("Triggered!"); });
//    timer->start(false);

    engine.painter()->installPaintEvent([&ani, &engine](Painter& painter) {
        painter.fillBackColor(StdColor::Black);
        ani.draw({painter.window()->geometry.width / 2 - ani.sprite()->size().width / 2,
                  painter.window()->geometry.height / 2 - ani.sprite()->size().height / 2}, &painter);
        engine.setWindowTitle(fmt::format("测试动画 Frame: {}/{}", ani.currentFrame(), ani.framesCount()));
    });

    engine.installEventHandler([&ani, &bgm, &engine](const SEvent& ev) {
        if (ev.button.type == SDL_EVENT_MOUSE_BUTTON_UP && ev.button.button == 1) {
            if (ani.isPlayedAnimation()) {
                ani.stop();
                bgm.pause();
            }
            else {
                ani.play(true, ani.currentFrame());
                bgm.play(false);
            }
        }
        if (ev.button.clicks == 2 && ev.button.type == SDL_EVENT_MOUSE_BUTTON_UP && ev.button.button == 3) {
            is_fulled = !is_fulled;
            engine.setFullScreen(is_fulled);
        }

        return true;
    });
    engine.installCleanUpEvent([]{
        fmt::println("Clean up.");
    });
    return engine.exec();
}