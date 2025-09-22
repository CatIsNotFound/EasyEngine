#include "src/Core.h"

using namespace EasyEngine;
using namespace Components;

int main() {
    Engine engine("Demo", 1024, 800);
    engine.setFPS(60);
    engine.setBackgroundRenderingEnabled(false);
    engine.show();
    
    Graphics::Rectangle rect({362, 250}, {300, 300}, StdColor::Black, true, true, StdColor::LightGray);
    engine.painter()->installPaintEvent([&rect](Painter& painter) { 
        painter.fillBackColor(StdColor::White);
        painter.setThickness(20);
        painter.drawRectangle(rect);
        painter.drawPixelText("Hello, EasyEngine!", {500.0f, 360.0f}, {1.5f, 2.0f}, StdColor::Black);
    });

    engine.installEventHandler([&rect](SEvent event) {
        auto cur_pos = Cursor::global()->position();
        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT &&
           Algorithm::comparePosRect(cur_pos, rect) >= 0) {
            rect.back_color = {rand() % 255, rand() % 255, rand() % 255, 255};
            rect.fore_color = {rand() % 255, rand() % 255, rand() % 255, 255};
        }
        if (event.key.key == SDLK_ESCAPE && event.key.down) {
            return false;   // Exit EasyEngine!
        }
        return true;        // Keep running!
    });

    return engine.exec();
}