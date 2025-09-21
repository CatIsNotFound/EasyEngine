#define EASYENGINE_NO_ABOUT_INFO
#include "src/Core.h"
using namespace EasyEngine;
using namespace Components;

void callback(uint64_t err) {
    fmt::println("Resource loaded done! Err: {}", err);
}

int main() {
    Engine engine("", 1024, 800);
    engine.setResizable(true);
    engine.setFPS(60);
    engine.show();
    Cursor::global()->setCursor(Cursor::Wait);
    ResourceSystem::global()->append("bgm", "assets/bgm.mp3", ResourceSystem::Resource::Audio);
    ResourceSystem::global()->append("bgm1", "assets/bgm.mp3", ResourceSystem::Resource::Audio);
    ResourceSystem::global()->append("bgm2", "assets/bgm.mp3", ResourceSystem::Resource::Audio);
    ResourceSystem::global()->append("bgm3", "assets/bgm.mp3", ResourceSystem::Resource::Audio);
    ResourceSystem::global()->append("bgm4", "assets/bgm.mp3", ResourceSystem::Resource::Audio);
    ResourceSystem::global()->append("font1", "assets/SourceHanSansCN-Bold.otf", ResourceSystem::Resource::Font);
    ResourceSystem::global()->append("font2", "assets/SourceHanSansCN-Medium.otf", ResourceSystem::Resource::Font);
    ResourceSystem::global()->append("font3", "assets/SourceHanSansCN-Heavy.otf", ResourceSystem::Resource::Font);
    ResourceSystem::global()->append("font4", "assets/SourceHanSansCN-Heavy.otf", ResourceSystem::Resource::Font);
    ResourceSystem::global()->append("font5", "assets/SourceHanSansCN-Heavy.otf", ResourceSystem::Resource::Font);
    ResourceSystem::global()->append("font6", "assets/SourceHanSansCN-Heavy.otf", ResourceSystem::Resource::Font);
    ResourceSystem::global()->append("font7", "assets/SourceHanSansCN-Heavy.otf", ResourceSystem::Resource::Font);
    auto time = SDL_GetTicks();
//    ResourceSystem::global()->asyncLoad({});
    ResourceSystem::global()->asyncLoad({}, [](uint64_t cnt) { fmt::println("加载完成！失败: {}", cnt); });
    fmt::println("Welcome! Loaded: {}ms", SDL_GetTicks() - time);
    std::vector<Graphics::Rectangle> history;
    engine.painter()->installPaintEvent([&](Painter& painter) {
        auto cur_pos = Cursor::global()->position();
        painter.fillBackColor(StdColor::Olive);
        for (auto& h : history) {
            painter.drawRectangle(h);
        }

        painter.drawPixelText(fmt::format("FPS: {}", engine.fps()), {20, 20}, {1.25f, 1.25f}, StdColor::LightGray);
        painter.drawPixelText(fmt::format("Cursor ({}, {})", cur_pos.x, cur_pos.y), {98, 20}, {1.25f, 1.25f}, StdColor::LightGray);
        painter.drawPixelText(fmt::format("Point(s): {}", history.size()), {20, 30}, {1.25f, 1.25f}, StdColor::LightGray);
    });

    engine.installEventHandler([&] (SEvent ev) {
        auto pos = Cursor::global()->position();
        if (ev.button.button == SDL_BUTTON_LEFT) {
            if (pos >= Vector2{0, 0} && pos < Vector2(1024, 800)) {
                auto pt = Graphics::Rectangle(pos - Vector2{4, 4}, {8, 8}, StdColor::White, false, true, StdColor::White);
                history.push_back(pt);
            }
        }
        if (ev.button.down && ev.button.button == SDL_BUTTON_RIGHT) {
            history.clear();
            history.shrink_to_fit();
        }
        return true;
    });
    Cursor::global()->setCursor(Cursor::Crosshair);
    

    return engine.exec();
}