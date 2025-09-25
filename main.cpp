#include "src/Core.h"

using namespace EasyEngine;
using namespace Components;

int main() {
    Engine engine("Graphics Test", 1280, 680);
     engine.setResizable(true);
    engine.setBackgroundRenderingEnabled(false);
    engine.setFPS(30);
    engine.show();

    auto sceneManager = new SceneManager();
    auto res = ResourceSystem::global();
    res->setRootPath("./assets");
    res->append("font", "Jersey_15/Jersey15-Regular.ttf", Resource::Font);
    res->append("red", "red.png", Resource::Image);
    res->append("block", "block.png", Resource::Image);
    res->append("bgm", "bgm.mp3", Resource::Audio);
    res->append("btn1", "buttons/normal.png", Resource::Image);
    res->append("btn2", "buttons/hover.png", Resource::Image);
    res->append("btn3", "buttons/pressed.png", Resource::Image);
    res->append("btn4", "buttons/disabled.png", Resource::Image);
    res->append("btn5", "buttons/active.png", Resource::Image);
    BGM bgm("bgm");
    uint32_t changer = 1;
    auto block_layer = new Layer("blocks");
    block_layer->append(1, new Sprite("red", "red", engine.painter()));
    block_layer->append(2, new Sprite("blue", "block", engine.painter()));
    block_layer->append(3, new Sprite("green", "block", engine.painter()));
    block_layer->append(4, new Control("button"));
    auto btn = block_layer->control(4);
    auto tmp_sp = new Sprite("default", "btn1", engine.painter());
    btn->setStatus(Control::Status::Default, tmp_sp);
    btn->setStatus(Control::Status::Hovered, new Sprite("hoverd", "btn2", engine.painter()));
    btn->setStatus(Control::Status::Pressed, new Sprite("pressed", "btn3", engine.painter()));
    btn->setStatus(Control::Status::Disabled, new Sprite("disabled", "btn4", engine.painter()));
    btn->setStatus(Control::Status::Active, new Sprite("active", "btn5", engine.painter()));
    btn->setGeometryForHotArea(500, 500, tmp_sp->size().width, tmp_sp->size().height);
    btn->move(100, 0);
    btn->setEvent(Control::Event::Clicked, [sceneManager, &bgm]{ sceneManager->changeScene(2); fmt::println("Goto Scene 2"); });
    btn->setEnabled(true);
    btn->setActive();
    block_layer->setViewportPainter(engine.painter());
    block_layer->setViewport(500, 500, 500, 500);

    Font font("font", 96.0);
    font.setFontColor(StdColor::Yellow);
    font.setOutline(1);
    font.setOutlineColor(StdColor::DarkOrange);


    auto text_layer = new Layer("text_layer");
    text_layer->append(1, font.textToSprite("text1", "Hello EasyEngine!", engine.painter()));
    font.setFontSize(32.f);
    text_layer->append(2, font.textToSprite("text2", "Pay the toll to the angels, drawing circles in the clouds", engine.painter()));
    text_layer->sprite(1)->properties()->position = {370, 100};
    text_layer->sprite(2)->properties()->position = {350, 300};

    auto btn2 = new Control("back");
    btn2->setStatus(Control::Status::Default, tmp_sp);
    btn2->setStatus(Control::Status::Hovered, new Sprite("hoverd", "btn2", engine.painter()));
    btn2->setStatus(Control::Status::Pressed, new Sprite("pressed", "btn3", engine.painter()));
    btn2->setStatus(Control::Status::Disabled, new Sprite("disabled", "btn4", engine.painter()));
    btn2->setStatus(Control::Status::Active, new Sprite("active", "btn5", engine.painter()));
    btn2->setGeometryForHotArea(0, 0, tmp_sp->size().width, tmp_sp->size().height);
    btn2->move(400, 500);
    btn2->setEvent(Control::Event::Clicked, [sceneManager, &bgm]{ sceneManager->changeScene(1); });
    btn2->setEnabled(true);
    btn2->setActive();

    auto text2_layer = new Layer("text2_layer");
    font.setFontSize(96.0f);
    text2_layer->append(1, font.textToSprite("text3", "Please listen music!", engine.painter()));
    text2_layer->sprite(1)->properties()->position = {400, 100};
    text2_layer->append(2, btn2);

    auto moving_layer = new Layer("moving_layer");
    moving_layer->append(1, new Sprite("moving", "block", engine.painter()));
    auto sp = moving_layer->sprite(1);
    sp->properties()->rotate_center = {sp->size().width * 3, sp->size().height / 2};
    sp->properties()->rotate = 0;
    sp->properties()->color_alpha = {5, 132, 190, 255};
    sp->properties()->position = {300, 250};

    auto scene = new Scene("main");
    scene->appendLayer(1, block_layer);
    scene->appendLayer(2, text_layer);
//    scene->setSceneEvent([&]() {
//         engine.painter()->fillBackColor(StdColor::LightBlue);
//    });

    Graphics::Ellipse ellipse(0, 0, 100, 100);
    ellipse.bordered_mode = false;
    ellipse.filled_mode = true;
    ellipse.back_color = StdColor::HalfTransparent;
    auto scene2 = new Scene("exit");
    scene2->appendLayer(1, text2_layer);
//    scene2->appendLayer(2, moving_layer);
    scene2->swapLayer(1, 2);
    scene2->setSceneEvent([&] {
        engine.painter()->fillBackColor(StdColor::LightGreen);
        auto cur_pos = Cursor::global()->position();
        ellipse.pos = cur_pos;
        engine.painter()->drawEllipse(ellipse);
        // static float x = 0, y = 0, fx = 1.0f, fy = 1.0f;
        // if (x > 1280 || x < 0) fx = -fx;
        // if (y > 680 || y < 0) fy = -fy;
        // x += fx; y += fy;
        static float angle = 1.f;
        // sp->properties()->position = {x, y};
        sp->properties()->rotate += angle;
    });

    auto esc_layer = new Layer("esc");
    esc_layer->append(1, font.textToSprite("text", "Press ESC to go back!", engine.painter()));
    auto scene3 = new Scene("scene3");
    scene3->appendLayer(1, esc_layer);
    scene3->setSceneEvent([&]{
        engine.painter()->fillBackColor(StdColor::LightPink);
        auto sp = scene3->layer(1)->sprite(1);
        sp->properties()->position = Cursor::global()->position();
    });

    sceneManager->append(scene, 1);
    sceneManager->append(scene2, 2);
    sceneManager->append(scene3, 3);
    auto dark = new DarkTransition(1500, Transition::KeepWhenStopped, engine.painter());
    auto move_img = new MoveTransition(3000, MoveTransition::LeftToRight, Transition::KeepWhenStopped, engine.painter());
    sceneManager->setEnterSceneEvent(1, [&]() {
        fmt::println("Changed: scene 1!");
        bgm.stop();
        dark->setDirection(Transition::Forward);
        dark->start();
    });
    sceneManager->setEnterSceneEvent(2, [&]() {
        fmt::println("Changed: scene 2!");
        bgm.play();
        dark->setDirection(Transition::Forward);
        dark->start();
    });
    sceneManager->setLeaveSceneEvent(2, [&] {
        dark->setDirection(Transition::Backward);
        dark->start();
    });
    sceneManager->setEnterSceneEvent(3, [&] {
        // move_img->setDirection(Transition::Forward);
        move_img->setMoveDirection(MoveTransition::TopToBottom);
        move_img->setDirection(Transition::Backward);
        move_img->start();
    });
    engine.painter()->setSceneManager(sceneManager);
    sceneManager->changeScene(1);
    engine.painter()->installPaintEvent([&](Painter& painter) {
        painter.fillBackColor(StdColor::LightYellow);
        engine.setWindowTitle(fmt::format("EasyEngine Demo - FPS {} Scene {}", engine.fps(), sceneManager->currentSceneIndex()));
    });
    engine.installEventHandler([&] (SEvent& e) {
        if (e.key.down && e.key.key == SDLK_Q) {
            if (sceneManager->currentSceneIndex() != 3) sceneManager->changeScene(3);
        }
        if (e.key.down && e.key.key == SDLK_ESCAPE) {
            if (sceneManager->currentSceneIndex() != 1) sceneManager->changeScene(1);
        }
        return true;
    });
    
    return engine.exec();
}