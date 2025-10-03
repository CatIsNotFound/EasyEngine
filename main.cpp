#include "src/Core.h"

using namespace EasyEngine;
using namespace Components;

bool _roll_caption = false;
int main() {
    Engine engine("Hello EasyEngine!", 1280, 720);
    engine.setBackgroundRenderingEnabled(true);
    engine.setResizable(true);
    engine.setFPS(144);
    engine.show();

    ResourceSystem::global()->setRootPath("./assets");
    ResourceSystem::global()->append("font", "Jersey_15/Jersey15-Regular.ttf", Resource::Font);
    ResourceSystem::global()->append("font_sub", "SourceHanSansCN-Medium.otf", Resource::Font);
    ResourceSystem::global()->append("icon", "demo.png", Resource::Image);
    ResourceSystem::global()->append("welcome", "Peace.wav", Resource::Audio);
    ResourceSystem::global()->append("bgm", "bgm.mp3", Resource::Audio);

    auto sceneManager = new SceneManager();
    engine.painter()->setSceneManager(sceneManager);
    auto scene_start = new Scene("start");
    auto scene_main = new Scene("main");
    scene_start->appendLayer(1, "image");
    scene_start->layer("image")->append(1, new Sprite("icon", "icon", engine.painter()));
    auto icon = scene_start->layer("image")->sprite(1);
    icon->properties()->position = {
            1280.f / 2 - icon->size().width / 2,
            720.f / 2 - icon->size().height / 2
    };
    scene_start->setSceneEvent([&engine]{
        engine.painter()->fillBackColor(StdColor::White);
    });
    sceneManager->append(new Scene("black"), 1);
    sceneManager->append(scene_start, 2);
    SFX sfx("welcome");
    auto dark = new Transition::DarkTransition(1000, EasyEngine::Transition::KeepWhenStopped, engine.painter());
    auto tmp_timer = new Timer(5000, [&] {
        sceneManager->changeScene(3);
    });
    BGM bgm("bgm");
    sceneManager->setEnterSceneEvent(2, [&]{
        dark->setDirection(Transition::Forward);
        dark->start();
        sfx.play();
        tmp_timer->start();
    });
    sceneManager->setLeaveSceneEvent(2, 1000, [&dark] {
        dark->setDirection(Transition::Backward);
        dark->start();
    });
    auto timer = new Timer(1000, [&] {
        sceneManager->changeScene(2);
        fmt::println("Start");
    });

    sceneManager->append(scene_main, 3);
    scene_main->setSceneEvent([&engine]{
        engine.painter()->fillBackColor(StdColor::LightBlue);
    });
    sceneManager->setEnterSceneEvent(3, [&dark, &bgm] {
        dark->setDirection(Transition::Forward);
        dark->start();
        bgm.play(false);
    });
    timer->start();
    return engine.exec();
}