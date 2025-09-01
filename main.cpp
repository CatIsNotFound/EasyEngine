
#include "src/Core.h"

using namespace EasyEngine;
using namespace Components;
#define MFPS 30
bool is_rotated = false;
bool is_high_fps = false;
double rotate_degree = 0.0;
int main() {
    Engine engine("测试 Spirit FPS: 0");
    engine.setGeometry(200, 200, 1024, 800);
    engine.setResizable(true);
    engine.setFPS(MFPS);
    engine.show();
    BGM bgm("assets/sound.wav");

    Spirit spirit("Demo", "assets/demo.png", engine.window()->renderer);
    Graphics::Rectangle rect;
    engine.painter()->installPaintEvent([&spirit, &engine, &rect, &bgm](Painter& painter) {
        painter.fillBackColor(StdColor::DarkBlue);
        static double rotate = 0.0;
        if (is_rotated) rotate_degree = (rotate_degree >= 360.0 ? 0.0 : rotate_degree + 1.0);
        static float scaled = 0.0f, d = 0.1f;
        d = (scaled >= 4.0f ? -0.1f : (scaled <= 0.05f ? 0.05f : d));
        scaled += d;
        Spirit::Properties properties;
        properties.position = {static_cast<float>(painter.window()->geometry.width / 2 - spirit.size().width / 2),
                               static_cast<float>(painter.window()->geometry.height / 2 - spirit.size().height / 2)};
        properties.color_alpha = {255, 255, 255, 255};
        properties.rotate = rotate_degree;
        properties.rotate_center = Vector2(spirit.size().width / 2, spirit.size().height / 2);
//        properties.scaled = scaled;
//        properties.scaled_center = Vector2(spirit.size().width / 2, spirit.size().height / 2);
//        properties.flip_mode = Spirit::FlipMode::HFlip;
        spirit.draw(properties, &painter);
//        rect.pos = Algorithm::spiritScaledPosition(properties.position, properties.scaled, properties.scaled_center);
//        rect.size = Algorithm::spiritScaledSize(spirit, properties.scaled);
//        rect.fore_color = Algorithm::hexToRGBA("#9080EF");
//        painter.drawRectangle(rect);
        auto pos = MIX_GetTrackPlaybackPosition(AudioSystem::instance()->bgmChannel(0).Stream.track);
        auto r_pos = MIX_AudioFramesToMS(AudioSystem::instance()->bgmChannel(0).audio, pos);
        auto vol = static_cast<int>(AudioSystem::instance()->bgmVolume() * 100);
        auto du = AudioSystem::instance()->bgmChannel(0).Stream.duration;
        char title[128] = "";
        sprintf(title, "测试 Spirit FPS: %d, Pos: %lld/%lld, Vol: %d%%", engine.fps(), r_pos, du, vol);
        engine.setWindowTitle(title);
    });
    engine.installEventHandler([&engine, &bgm](SEvent event) -> bool {
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
            is_rotated = false;
        }
        if (event.button.down && event.button.button == 3) {
            is_high_fps = !is_high_fps;
            engine.setFPS(is_high_fps ? 144 : MFPS);
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
        return true;
    });

    return engine.exec();
}