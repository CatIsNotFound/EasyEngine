
#include "src/Core.h"

using namespace EasyEngine;
using namespace Components;

bool is_rotated = false;
double rotate_degree = 0.0;
int main() {
    Engine engine("测试 Spirit FPS: 0");
    engine.setGeometry(200, 200, 1024, 800);
    engine.setResizable(true);
    engine.setFPS(60);
    engine.show();
    AudioSystem::instance()->init();
    AudioSystem::instance()->setAudioSpec(StdAudioSpec::Mono);
    AudioSystem::instance()->setBGMVolume(1.0f);
    Spirit spirit("Demo", "assets/demo.png", engine.window()->renderer);
    Graphics::Rectangle rect;
    engine.painter()->installPaintEvent([&spirit, &engine, &rect](Painter& painter) {
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
        properties.scaled = scaled;
        properties.scaled_center = Vector2(spirit.size().width / 2, spirit.size().height / 2);
        properties.flip_mode = Spirit::FlipMode::HFlip;
        spirit.draw(properties, &painter);
        rect.pos = Algorithm::spiritScaledPosition(properties.position, properties.scaled, properties.scaled_center);
        rect.size = Algorithm::spiritScaledSize(spirit, properties.scaled);
        rect.fore_color = Algorithm::hexToRGBA("#9080EF");
        painter.drawRectangle(rect);
        char title[128] = "";
        sprintf(title, "测试 Spirit FPS: %d", engine.fps());
        engine.setWindowTitle(title);

    });
    engine.installEventHandler([&engine](SEvent event) -> bool {
        if (event.window.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
            engine.removeWindow(engine.windowIDList().front());
        }
        if (event.button.down && event.button.button == 1) {
            is_rotated = !is_rotated;
        }
        if (event.button.down && event.button.button == 2) {
            rotate_degree = 0.0;
        }
        if (event.button.down && event.button.button == 3) {
            engine.setFPS(engine.fps() >= 54 && engine.fps() <= 60 ? 144 : 60);
        }
        return true;
    });

    return engine.exec();
}