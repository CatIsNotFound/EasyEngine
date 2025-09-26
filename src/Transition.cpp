
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
        // if (_state == Running) return;
        _state = Running;
        _start_time = SDL_GetTicks();
        _change_signal = false;
        getReady();
        EventSystem::global()->setHandlerEnabled(false);
        _painter->_startTransition(this);
    }

    void Transition::pause() {
        EventSystem::global()->setHandlerEnabled(true);
        _state = Paused;
    }

    void Transition::stop() {
        // if (_state == Stopped) return;
        _state = Stopped;
        _change_signal = false;
        _painter->_stopTransition(this);
        EventSystem::global()->setHandlerEnabled(true);
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
                EventSystem::global()->setHandlerEnabled(true);
                _painter->_stopTransition(this);
                if (_deletion_policy == DeleteWhenStopped) {
                    auto _r = _painter->_removeTransition(this);
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
        // _mask.size = {(float)_painter->window()->geometry.width, (float)_painter->window()->geometry.height};
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
    }

    void DarkTransition::getReady() {
        _mask.pos = {0, 0};
        _mask.size = {(float)_painter->window()->geometry.width, (float)_painter->window()->geometry.height};
    }

    const SColor &DarkTransition::backgroundColor() const {
        return _mask.back_color;
    }

    EraseTransition::EraseTransition(uint64_t duration, const enum EraseDirection& direction,
                                     Transition::DeletionPolicy deletion_policy, Painter *painter)
            : Transition(duration, deletion_policy, painter), _direction(direction) {
        _rect.bordered_mode = false;
        _rect.filled_mode = true;
        _rect.back_color = StdColor::Black;
    }

    EraseTransition::~EraseTransition() {}

    void EraseTransition::setEraseDirection(const enum EraseDirection &direction) {
        _direction = direction;
    }

    const EraseTransition::EraseDirection &EraseTransition::eraseDirection() const {
        return _direction;
    }

    void EraseTransition::setBackColor(const SColor &color) { _rect.back_color = color; }

    const SColor &EraseTransition::backColor() const { return _rect.back_color; }

    void EraseTransition::update() {
        auto current = currentTime();
        auto percent = ((current * 1.f) / _duration);
        if (percent <= 1.f) {
            if (_direction == EraseDirection::LeftToRight) {
                if (Transition::_direction == Forward) {
                    _rect.pos = {(_rect.size.width * percent), 0};
                } else {
                    _rect.pos = {_rect.size.width * (1.f - percent), 0};
                }
            } else if (_direction == EraseDirection::RightToLeft) {
                if (Transition::_direction == Forward) {
                    _rect.pos = {(-(_rect.size.width * percent)), 0 };
                } else {
                    _rect.pos = {(-(_rect.size.width * (1.f - percent))), 0};
                }
            } else if (_direction == EraseDirection::TopToBottom) {
                if (Transition::_direction == Forward) {
                    _rect.pos = {0, (_rect.size.height * percent)};
                } else {
                    _rect.pos = {0, _rect.size.height * (1.f - percent)};
                }
            } else if (_direction == EraseDirection::BottomToTop) {
                if (Transition::_direction == Forward) {
                    _rect.pos = {0, (-(_rect.size.height * percent))};
                } else {
                    _rect.pos = {0, (-(_rect.size.height * (1.f - percent)))};
                }
            }
        }
        _painter->drawRectangle(_rect);
    }

    void EraseTransition::getReady() {
        _rect.size = {static_cast<float>(_painter->window()->geometry.width),
                      static_cast<float>(_painter->window()->geometry.height)};
    }

    MoveTransition::MoveTransition(uint32_t duration, const MoveTransition::MoveDirection &direction,
                                   Transition::DeletionPolicy deletion_policy, Painter *painter)
           : Transition(duration, deletion_policy, painter), _move_direction(direction) {}

    void MoveTransition::setFirstPicture(SSurface *surface) {
        if (!surface) return;
        if (_sprite1) {
            _sprite1->setSurface(surface);
        } else {
            _sprite1 = std::make_shared<Components::Sprite>("first_pic", surface, _painter);
        }
    }

    void MoveTransition::setFirstPicture(Components::Sprite *sprite) {
        if (!sprite) return;
        if (_sprite1) {
            _sprite1->copySprite(sprite);
        } else {
            _sprite1 = std::make_shared<Components::Sprite>("first_pic", *sprite);
        }
    }

    void MoveTransition::clearFirstPicture() {
        if (_sprite1) {
            _sprite1.reset();
        }
    }

    Components::Sprite *MoveTransition::firstPicture() const {
        return _sprite1.get();
    }

    void MoveTransition::setSecondPicture(SSurface *surface) {
        if (!surface) return;
        if (_sprite2) {
            _sprite2->setSurface(surface);
        } else {
            _sprite2 = std::make_shared<Components::Sprite>("second_pic", surface, _painter);
        }
    }

    void MoveTransition::setSecondPicture(Components::Sprite *sprite) {
        if (!sprite) return;
        if (_sprite2) {
            _sprite2->copySprite(sprite);
        } else {
            _sprite2 = std::make_shared<Components::Sprite>("second_pic", *sprite);
        }
    }

    void MoveTransition::clearSecondPicture() {
        if (_sprite2) {
            _sprite2.reset();
        }
    }

    Components::Sprite *MoveTransition::secondPicture() const {
        return _sprite2.get();
    }

    void MoveTransition::setMoveDirection(const MoveTransition::MoveDirection &direction) {
        _move_direction = direction;
    }

    const MoveTransition::MoveDirection &MoveTransition::moveDirection() const {
        return _move_direction;
    }

    void MoveTransition::update() {
        auto current = currentTime();
        auto percent = ((current * 1.f) / _duration);
        if (percent > 1.f) return;
        if (!_sprite1) return;
        if (_move_direction == LeftToRight) {
            _sprite1->properties()->position = ( _direction == Forward ?
                    Vector2(_sprite1->size().width * percent, 0) :
                    Vector2(_sprite1->size().width * (1.f - percent), 0));
            if (_draw_second_pic) {
                _sprite2->properties()->position = ( _direction == Forward ?
                        Vector2(-(_sprite2->size().width * (1.f - percent)), 0) :
                        Vector2(-(_sprite2->size().width * percent), 0));
            }
        } else if (_move_direction == RightToLeft) {
            _sprite1->properties()->position = ( _direction == Forward ?
                    Vector2(-(_sprite1->size().width * percent), 0) :
                    Vector2(-(_sprite1->size().width * (1.f - percent)), 0));
            if (_draw_second_pic) {
                _sprite2->properties()->position = ( _direction == Forward ?
                        Vector2(_sprite2->size().width * percent, 0) :
                        Vector2(_sprite2->size().width * (1.f - percent), 0));
            }
        } else if (_move_direction == TopToBottom) {
            _sprite1->properties()->position = ( _direction == Forward ?
                    Vector2(0, _sprite1->size().height * percent) :
                    Vector2(0, _sprite1->size().height * (1.f - percent)));
            if (_draw_second_pic) {
                _sprite2->properties()->position = ( _direction == Forward ?
                        Vector2(-(_sprite2->size().height * (1.f - percent)), 0) :
                        Vector2(-(_sprite2->size().height * percent), 0));
            }
        } else if (_move_direction == BottomToTop) {
            _sprite1->properties()->position = ( _direction == Forward ?
                    Vector2(0, -(_sprite1->size().height * percent)) :
                    Vector2(0, -(_sprite1->size().height * (1.f - percent))));
            if (_draw_second_pic) {
                _sprite2->properties()->position = ( _direction == Forward ?
                        Vector2(0, _sprite2->size().height * percent) :
                        Vector2(0, _sprite2->size().height * (1.f - percent)));
            }
        }
        _sprite1->draw();
        if (_draw_second_pic) _sprite2->draw();
    }

    void MoveTransition::getReady() {
        _draw_second_pic = secondPicture();
    }
} // EasyEngine