
#include "Transition.h"

namespace EasyEngine {
    Transition::Transition(uint64_t duration, DeletionPolicy deletion_policy, Painter *painter)
        : _duration(duration), _deletion_policy(deletion_policy), _painter(painter) {}

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
        _state = Running;
        _start_time = SDL_GetTicks();
        _change_signal = false;
    }

    void Transition::pause() {
        _state = Paused;
    }

    void Transition::stop() {
        _state = Stopped;
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
            // SDL_Log("Transition: %llu / %llu", _current_time, _duration);
            if (_current_time <= _duration) update();
            else {
                _loop_count += 1;
                _state = Stopped;
                SDL_Log("Finished!");
            }
        }
    }

    bool Transition::__is_changed_signal() {
        return _change_signal;
    }

    FadeTransition::FadeTransition(uint64_t _duration, DeletionPolicy deletion_policy, Painter *painter) : Transition(
            _duration, deletion_policy, painter) {
        _mask.bordered_mode = false;
        _mask.filled_mode = true;
        _mask.back_color = StdColor::Black;
        _mask.pos = {0, 0};
        _mask.size = {(float)painter->window()->geometry.width, (float)painter->window()->geometry.height};
    }

    FadeTransition::~FadeTransition() = default;

    void FadeTransition::setFadeColor(const SColor &color) {
        _mask.back_color = color;
    }

    void FadeTransition::update() {
        auto current = currentTime();
        auto percent = ((current * 1.f) / _duration) / 0.5f;
        if (percent <= 1.f)
            _mask.back_color.a = (uint8_t)(255 * percent);
        else {
            _change_signal = true;
            _mask.back_color.a = (uint8_t) (255 * (1.f - percent));
        }
        _painter->drawRectangle(_mask);
    }
} // EasyEngine