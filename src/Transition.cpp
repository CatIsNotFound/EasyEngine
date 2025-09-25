
#include "Transition.h"

namespace EasyEngine {
    Transition::Transition(uint64_t duration, DeletionPolicy deletion_policy, Painter *painter)
        : _duration(duration), _deletion_policy(deletion_policy), _painter(painter) {
        _painter->_addTransition(this);
    }

    void Transition::setDeletionPolicy(const Transition::DeletionPolicy &deletion_policy) {
        _deletion_policy = deletion_policy;
    }

    void Transition::setDuration(uint64_t duration) {
        _duration = duration;
    }

    void Transition::setDirection(const Transition::Direction &direction) {
        _direction = direction;
    }

    void Transition::start() {
        if (_state == Running) return;
        _state = Running;
        _start_time = SDL_GetTicks();
        _change_signal = false;
        SDL_Log("Start Transition!");
        getReady();
        _painter->_startTransition(this);
    }

    void Transition::pause() {
        _state = Paused;
    }

    void Transition::stop() {
        if (_state == Stopped) return;
        _state = Stopped;
        _change_signal = false;
        _painter->_stopTransition(this);
    }

    uint64_t Transition::duration() const {
        return _duration;
    }

    uint64_t Transition::currentTime() const {
        return _current_time;
    }

    uint64_t Transition::loopCount() const {
        return _loop_count;
    }

    void Transition::clearLoopCount() {
        _loop_count = 0;
    }

    const Transition::Direction &Transition::direction() const {
        return _direction;
    }

    const Transition::State &Transition::state() const {
        return _state;
    }

    const Transition::DeletionPolicy &Transition::deletionPolicy() const {
        return _deletion_policy;
    }

    void Transition::______() {
        if (_state == Running) {
            _current_time = SDL_GetTicks() - _start_time;
            if (_current_time <= _duration) update();
            else {
                SDL_Log("End: %llu", _current_time);
                _loop_count += 1;
                _state = Stopped;
                _painter->_stopTransition(this);
                if (_deletion_policy == DeleteWhenStopped) {
                    auto _r = _painter->_removeTransition(this);
                    fmt::println("Deleted? {}", _r);
                }
            }
        }
    }

    bool Transition::__is_changed() {
        return _change_signal;
    }

    DarkTransition::DarkTransition(uint64_t _duration, DeletionPolicy deletion_policy, Painter *painter) : Transition(
            _duration, deletion_policy, painter) {
        _mask.bordered_mode = false;
        _mask.filled_mode = true;
        _mask.back_color = StdColor::Black;
        _mask.size = {(float)_painter->window()->geometry.width, (float)_painter->window()->geometry.height};
    }

    DarkTransition::~DarkTransition() = default;

    void DarkTransition::setBackgroundColor(const SColor &color) {
        _mask.back_color = color;
    }

    void DarkTransition::update() {
        auto current = currentTime();
        auto percent = ((current * 1.f) / _duration);
        if (percent <= 1.f) {
            if (_direction == Backward)
                _mask.back_color.a = (uint8_t)(255.f * percent);
            if (_direction == Forward)
                _mask.back_color.a = (uint8_t)(255.f * (1.f - percent));
        }
        _painter->drawRectangle(_mask);
        _painter->drawPixelText(fmt::format("Opacity: {}, Process: {} / {}, {:.4}%",
                                     _mask.back_color.a, currentTime(), _duration, percent * 100.f), {20, 20});

    }

    void DarkTransition::getReady() {
        _mask.pos = {0, 0};
        // _mask.size = {(float)_painter->window()->geometry.width, (float)_painter->window()->geometry.height};

    }

    const SColor &DarkTransition::backgroundColor() const {
        return _mask.back_color;
    }

    MoveTransition::MoveTransition(uint64_t duration, const enum MoveDirection& direction,
            Transition::DeletionPolicy deletion_policy, Painter *painter)
            : Transition(duration, deletion_policy, painter), _direction(direction) {}

    MoveTransition::~MoveTransition() {}

    void MoveTransition::setMoveDirection(const MoveTransition::MoveDirection &direction) {
        _direction = direction;
    }

    const MoveTransition::MoveDirection &MoveTransition::moveDirection() const {
        return _direction;
    }

    void MoveTransition::update() {
        auto current = currentTime();
        auto percent = ((current * 1.f) / _duration);
        auto size = _sprite->size();
        if (percent <= 1.f) {
            if (_direction == MoveDirection::LeftToRight) {
                if (Transition::_direction == Forward)
                    _sprite->draw(Vector2((size.width * percent), 0));
                else
                    _sprite->draw(Vector2((size.width * (1.f - percent)), 0));
            } else {
                if (Transition::_direction == Forward)
                    _sprite->draw(Vector2(0, (size.height * percent)));
                else
                    _sprite->draw(Vector2(0, (size.height * (1.f - percent))));
            }
        }
    }

    void MoveTransition::getReady() {
        if (_sprite) _sprite.reset();
        _surface = Algorithm::captureWindow(_painter);

        _sprite = std::make_unique<Components::Sprite>("move_transition", _surface, _painter);
    }
} // EasyEngine