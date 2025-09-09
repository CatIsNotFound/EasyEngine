
#include "src/Core.h"

using namespace EasyEngine;
using namespace Components;
#define MFPS 40
bool is_rotated = false;
bool is_high_fps = false;
bool is_focused = false;
double rotate_degree = 0.0;
int main() {
    Engine engine("测试 Spirit FPS: 0");
    engine.setGeometry(0, 0, 800, 600);
    engine.setResizable(true);
    engine.setFPS(MFPS);
    engine.setBackgroundRenderingEnabled(false);
    engine.show();
    BGM bgm("assets/bgm.mp3");
    SFX sfx("assets/sound.oga");
    Sprite spirit("Demo", "assets/demo.png", engine.window()->renderer);
    spirit.properties()->rotate_center = Vector2(spirit.size().width / 2, spirit.size().height / 2);
    spirit.properties()->scaled_center = Vector2(spirit.size().width / 2, spirit.size().height / 2);

    Sprite gear1("gear1", "assets/gear.png", engine.window()->renderer),
           gear2("gear2", "assets/gear.png", engine.window()->renderer);
    SpriteGroup gear_group;
    gear_group.append(gear1);
    gear_group.append(gear2);
    gear_group.append(gear1);
    gear_group.append(gear2);
    gear_group.indexOf(1)->setPath("assets/EasyEngine_Icon.png");
    gear_group.indexOf(1)->setName("Icon");
    gear_group.propertiesOf(2)->scaled = 0.15f;
    gear_group.propertiesOf(3)->scaled = 0.05f;
    gear_group.propertiesOf(0)->scaled = 0.70f;
    gear_group.propertiesOf(1)->scaled = 0.5f;
    gear_group.propertiesOf(0)->position = {0, 0};
    gear_group.propertiesOf(1)->position = {95, 95};
    gear_group.propertiesOf(0)->rotate_center = {gear_group.indexOf(0)->size().width / 2,
                                                 gear_group.indexOf(0)->size().height / 2};
    gear_group.propertiesOf(1)->rotate_center = {gear_group.indexOf(1)->size().width / 2,
                                                 gear_group.indexOf(1)->size().height / 2};
    gear_group.propertiesOf(0)->color_alpha = StdColor::Gray;
    gear_group.propertiesOf(gear_group.indexAt("gear1"))->color_alpha = StdColor::DeepPink;
    SpriteGroup copied(std::move(gear_group));
    copied.remove(3);
    copied.remove(2);
    engine.painter()->installPaintEvent([&spirit, &gear_group, &copied, &engine, &bgm](Painter& painter) {
        painter.fillBackColor(is_focused ? StdColor::DarkBlue : StdColor::DarkGray);
        static double rotate = 0.0;
        if (is_rotated) rotate_degree = (rotate_degree >= 360.0 ? 0.0 : rotate_degree + 1.0);
        Vector2 position = {static_cast<float>(engine.window()->geometry.width / 2 - spirit.size().width / 2),
                            static_cast<float>(engine.window()->geometry.height / 2 - spirit.size().height / 2)};
        spirit.properties()->rotate = rotate_degree;
        spirit.properties()->rotate_center = Vector2(spirit.size().width / 2, spirit.size().height / 2);
        spirit.draw(position, &painter);
        gear_group.propertiesOf(0)->rotate = 360 - rotate_degree;
        gear_group.propertiesOf(1)->rotate = rotate_degree + 18;
        gear_group.draw({100, 100}, &painter);
        copied.propertiesOf(0)->rotate = rotate_degree;
        copied.propertiesOf(1)->rotate = 360 - rotate_degree - 18;
        copied.draw({100, 400}, &painter);
        auto vol = static_cast<int>(AudioSystem::instance()->bgmVolume() * 100);
        auto du = AudioSystem::instance()->bgmChannel(0).Stream.duration;
        engine.setWindowTitle(fmt::format("测试 FPS: {}, {}/{}, Vol:{}%", engine.fps(), bgm.position(), du, vol));
    });
    engine.installEventHandler([&engine, &bgm, &sfx](SEvent event) -> bool {
        if (event.window.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
            engine.removeWindow(event.window.windowID);
        }
        if (event.button.down && event.button.button == 1) {
            is_rotated = !is_rotated;
            if (is_rotated) {
                bgm.play();
            } else {
                bgm.pause();
            }
        }
        if (event.button.down && event.button.button == 2) {
            rotate_degree = 0.0;
            bgm.stop();
            sfx.stop();
            is_rotated = false;
        }
        if (event.button.down && event.button.button == 3) {
            is_high_fps = !is_high_fps;
            engine.setFPS(is_high_fps ? 144 : MFPS);
            sfx.play();
        }
        if (event.key.down && event.key.key == SDLK_R) {
            AudioSystem::instance()->setBGMVolume(0.0f);
        }
        if (event.key.down && event.key.mod == SDL_KMOD_LCTRL && event.key.key == SDLK_DOWN) {
            AudioSystem::instance()->setBGMVolume(AudioSystem::instance()->bgmVolume() - 0.1f);
        } else if (event.key.down && event.key.key == SDLK_DOWN) {
            AudioSystem::instance()->setBGMVolume(AudioSystem::instance()->bgmVolume() - 0.01f);
        }
        if (event.key.down && event.key.mod == SDL_KMOD_LCTRL && event.key.key == SDLK_UP) {
            AudioSystem::instance()->setBGMVolume(AudioSystem::instance()->bgmVolume() + 0.1f);
        } else if (event.key.down && event.key.key == SDLK_UP) {
            AudioSystem::instance()->setBGMVolume(AudioSystem::instance()->bgmVolume() + 0.01f);
        }
        if (event.window.type == SDL_EVENT_WINDOW_FOCUS_GAINED) {
            is_focused = true;
            if (engine.backgroundRenderingEnabled() && is_rotated) bgm.play();
        } else if (event.window.type == SDL_EVENT_WINDOW_FOCUS_LOST) {
            is_focused = false;
            if (engine.backgroundRenderingEnabled()) bgm.pause();
        }
        return true;
    });

    return engine.exec();
}
