#include "src/Core.h"

using namespace EasyEngine;
using namespace Components;

int main() {
    Engine engine("", 1024, 800);
    engine.setResizable(true);
    engine.setFPS(60);
    auto geometry = engine.screenGeometry();
    engine.move(geometry.width / 2 - 1024 / 2, geometry.height / 2 - 800 / 2 - 100);
    engine.show();
    Graphics::Rectangle rect(100, 100, 280, 280, StdColor::Black, false,
                             true, StdColor::LightBlue);
    Graphics::Ellipse ellipse(650, 500, 380, 380, StdColor::Black, false,
                              true, StdColor::LightPink);
    Graphics::Rectangle mouse_rect(0, 0, 100, 100, StdColor::Black, false,
                                   true, {243, 233, 64, 64});
    Graphics::Ellipse mouse_ellipse(0, 0, 100, 100, StdColor::Black, false,
                                    true, {243, 233, 64, 164});
    static bool trigger = false;
    Collider rect_collider(rect), ellipse_collider(ellipse),
             mouse_rect_collider(mouse_rect), mouse_ellipse_collider(mouse_ellipse);
    rect_collider.setEnabled(true);
    ellipse_collider.setEnabled(true);
    mouse_rect_collider.setEnabled(true);
    mouse_ellipse_collider.setEnabled(true);
    engine.installEventHandler([&](SEvent ev) {
        auto cur_pos = Cursor::global()->position();
        auto old_cur = Cursor::global()->cursor();
        if (Algorithm::comparePosRect(cur_pos, rect) >= 0) {
            if (old_cur != EasyEngine::Cursor::Hand) {
                old_cur = EasyEngine::Cursor::Hand;
                Cursor::global()->setCursor(Cursor::Hand);
            }
        } else if (Algorithm::comparePosEllipse(cur_pos, ellipse) >= 0) {
            if (old_cur != EasyEngine::Cursor::Hand) {
                old_cur = EasyEngine::Cursor::Hand;
                Cursor::global()->setCursor(Cursor::Hand);
            }
        } else {
            if (old_cur != EasyEngine::Cursor::Default) {
                old_cur = EasyEngine::Cursor::Default;
                Cursor::global()->setCursor(Cursor::Default);
            }
        }
        if (ev.button.type == SDL_EVENT_MOUSE_BUTTON_DOWN && ev.button.button == SDL_BUTTON_LEFT) {
            trigger = !trigger;
        }
        if (trigger) {
            mouse_ellipse_collider.moveBounds(cur_pos);
            auto ch = mouse_ellipse_collider.check(ellipse_collider);
            if (ch > 0) {
                ellipse.back_color = StdColor::DarkRed;
            } else if (ch == 0) {
                ellipse.back_color = StdColor::Tomato;
            } else {
                ellipse.back_color = StdColor::LightPink;
            }
        } else {
            mouse_rect_collider.moveBounds(cur_pos);
            if (mouse_rect_collider.check(rect_collider) >= 0) {
                rect.back_color = StdColor::DarkBlue;
            } else {
                rect.back_color = StdColor::LightBlue;
            }
        }
        engine.setWindowTitle(fmt::format("Cursor({},{})", cur_pos.x, cur_pos.y));
        return true;
    });
    engine.painter()->installPaintEvent([&](Painter& painter) {
        painter.fillBackColor(StdColor::White);
        painter.drawRectangle(rect);
        painter.drawEllipse(ellipse);
        if (trigger) {
            mouse_ellipse.pos = mouse_ellipse_collider.bounds().pos;
            painter.drawEllipse(mouse_ellipse);
        } else {
            mouse_rect.pos = mouse_rect_collider.bounds().pos;
            painter.drawRectangle(mouse_rect);
        }
    });

    return engine.exec();
}