
#include "Transition.h"

namespace EasyEngine {
    namespace EasingCurve {
        AbstractEasingCurve::AbstractEasingCurve(uint32_t duration, Painter* painter)
            : _duration(duration), _painter(painter) {}

        void AbstractEasingCurve::setDuration(uint32_t duration) {
            if (_enabled) {
                auto cur_time = static_cast<float>(SDL_GetTicks());
                auto sum_time = static_cast<float>(_duration + _start_time);
                float _percent = cur_time / sum_time;
                _percent = std::clamp(_percent, 0.f, 1.f);
                // 保持当前进度不变，重新计算起始时间
                _start_time = static_cast<uint32_t>(cur_time - (_percent * duration));
            }
            _duration = duration;
        }

        void AbstractEasingCurve::setDirection(const Direction &direction) {
            _direction = direction;
        }

        void AbstractEasingCurve::setEnabled(bool enabled, bool loop, bool change_direction) {
            _enabled = enabled;
            if (_enabled) {
                _start_time = SDL_GetTicks();
                _loop = loop;
                _cur_direction = _direction;
                _change_direction = change_direction;
                getReady();
                if (!_added_list && _painter) {
                    _painter->_startEasingCurve(std::shared_ptr<AbstractEasingCurve>(this));
                    _added_list = true;
                }
            }
        }

        float AbstractEasingCurve::current() const {
            return _position;
        }

        uint32_t AbstractEasingCurve::elapsedTime() const {
            auto _cur_time = SDL_GetTicks();
            return _cur_time - _start_time;
        }

        uint32_t AbstractEasingCurve::duration() const {
            return _duration;
        }

        const enum Direction &AbstractEasingCurve::direction() const {
            return _cur_direction;
        }

        bool AbstractEasingCurve::enabled() const {
            return _enabled;
        }

        void AbstractEasingCurve::______() {
            if (!_enabled) return;
            update();
            if (elapsedTime() >= duration()) {
                if (_loop) {
                    _start_time = SDL_GetTicks();
                    if (_change_direction) _cur_direction = (_cur_direction == Forward ? Backward : Forward);
                } else {
                    _enabled = false;
                }
            }
        }

        LinearCurve::LinearCurve(uint32_t duration, Painter *painter) : AbstractEasingCurve(duration, painter) {}

        void LinearCurve::update() {
            auto cur_time = static_cast<float>(elapsedTime());
            auto sum_time = static_cast<float>(duration());
            _position = (direction() == Forward) ? cur_time / sum_time : 1.f - (cur_time / sum_time);
            _position = std::clamp(_position, 0.f, 1.f);
        }

        void LinearCurve::getReady() {};

        InQuadCurve::InQuadCurve(uint32_t duration, Painter *painter) : AbstractEasingCurve(duration, painter) {}

        void InQuadCurve::update() {
            auto cur_time = static_cast<float>(elapsedTime());
            auto sum_time = static_cast<float>(duration());
            auto percent = cur_time / sum_time;
            if (direction() == Forward) {
                _position = percent * percent;
            } else {
                _position = (1.f - percent) * (1.f - percent);
            }
            _position = std::clamp(_position, 0.f, 1.f);
        }

        void InQuadCurve::getReady() {}

        InCubicCurve::InCubicCurve(uint32_t duration, Painter *painter) : AbstractEasingCurve(duration, painter) {}

        void InCubicCurve::update() {
            auto cur_time = static_cast<float>(elapsedTime());
            auto sum_time = static_cast<float>(duration());
            auto percent = cur_time / sum_time;
            if (direction() == Forward) {
                _position = percent * percent * percent;
            } else {
                _position = (1.f - percent) * (1.f - percent) * (1.f - percent);
            }
            _position = std::clamp(_position, 0.f, 1.f);
        }

        void InCubicCurve::getReady() {}

        InSineCurve::InSineCurve(uint32_t duration, Painter *painter) : AbstractEasingCurve(duration, painter) {}

        void InSineCurve::update() {
            auto cur_time = static_cast<float>(elapsedTime());
            auto sum_time = static_cast<float>(duration());
            auto percent = cur_time / sum_time;
            if (direction() == Forward) {
                float _res = cos(percent * M_PI * 0.5f);
                _position = 1.f - _res;
            } else {
                float _res = sin(percent * M_PI * 0.5f);
                _position = 1.f - _res;
            }
            _position = std::clamp(_position, 0.f, 1.f);
        }

        void InSineCurve::getReady() {}


        OutQuadCurve::OutQuadCurve(uint32_t duration, Painter *painter) : AbstractEasingCurve(duration, painter) {}

        void OutQuadCurve::update() {
            auto cur_time = static_cast<float>(elapsedTime());
            auto sum_time = static_cast<float>(duration());
            auto percent = cur_time / sum_time;
            if (direction() == Forward) {
                _position = 1.f - (1.f - percent) * (1.f - percent);
            } else {
                _position = 1.f - percent * percent;
            }
            _position = std::clamp(_position, 0.f, 1.f);
        }

        void OutQuadCurve::getReady() {}

        OutCubicCurve::OutCubicCurve(uint32_t duration, Painter *painter) : AbstractEasingCurve(duration, painter) {}

        void OutCubicCurve::update() {
            auto cur_time = static_cast<float>(elapsedTime());
            auto sum_time = static_cast<float>(duration());
            auto percent = cur_time / sum_time;
            if (direction() == Forward) {
                _position = 1.f - (1.f - percent) * (1.f - percent) * (1.f - percent);
            } else {
                _position = 1.f - percent * percent * percent;
            }
            _position = std::clamp(_position, 0.f, 1.f);
        }

        void OutCubicCurve::getReady() {}

        OutSineCurve::OutSineCurve(uint32_t duration, Painter *painter) : AbstractEasingCurve(duration, painter) {}

        void OutSineCurve::update() {
            auto cur_time = static_cast<float>(elapsedTime());
            auto sum_time = static_cast<float>(duration());
            auto percent = cur_time / sum_time;
            if (direction() == Forward) {
                float _res = sin(percent * M_PI * 0.5f);
                _position = _res;
            } else {
                float _res = cos(percent * M_PI * 0.5f);
                _position = _res;
            }
            _position = std::clamp(_position, 0.f, 1.f);
        }

        void OutSineCurve::getReady() {}

        InOutQuadCurve::InOutQuadCurve(uint32_t duration, Painter *painter) : AbstractEasingCurve(duration, painter) {}

        void InOutQuadCurve::update() {
            auto cur_time = static_cast<float>(elapsedTime());
            auto sum_time = static_cast<float>(duration());
            auto percent = cur_time / sum_time;
            if (direction() == Forward) {
                if (percent < 0.5f) {
                    percent = 2.f * percent;
                    _position = 0.5f * percent * percent;
                } else {
                    percent = 2.f * percent - 1.f;
                    _position = 1.f - 0.5f * (1.f - percent) * (1.f - percent);
                }
            } else {
                if (percent < 0.5f) {
                    percent = 2.f * percent;
                    _position = 1.f - 0.5f * percent * percent;
                } else {
                    percent = 2.f * percent - 1.f;
                    _position = 0.5f * (1.f - percent) * (1.f - percent);
                }
            }
            _position = std::clamp(_position, 0.f, 1.f);
        }

        void InOutQuadCurve::getReady() {}

        InOutCubicCurve::InOutCubicCurve(uint32_t duration, Painter *painter)
            : AbstractEasingCurve(duration, painter) {}

        void InOutCubicCurve::update() {
            auto cur_time = static_cast<float>(elapsedTime());
            auto sum_time = static_cast<float>(duration());
            auto percent = cur_time / sum_time;
            if (direction() == Forward) {
                if (percent < 0.5f) {
                    percent = 2.f * percent;
                    _position = 0.5f * percent * percent * percent;
                } else {
                    percent = 2.f * percent - 1.f;
                    _position = 1.f - 0.5f * (1.f - percent) * (1.f - percent) * (1.f - percent);
                }
            } else {
                if (percent < 0.5f) {
                    percent = 2.f * percent;
                    _position = 1.f - 0.5f * (percent * percent * percent);
                } else {
                    percent = 2.f * percent - 1.f;
                    _position = 0.5f * (1.f - percent) * (1.f - percent) * (1.f - percent);
                }
            }
            _position = std::clamp(_position, 0.f, 1.f);
        }

        void InOutCubicCurve::getReady() {}

        InOutSineCurve::InOutSineCurve(uint32_t duration, Painter *painter) : AbstractEasingCurve(duration, painter) {}

        void InOutSineCurve::update() {
            auto cur_time = static_cast<float>(elapsedTime());
            auto sum_time = static_cast<float>(duration());
            auto percent = cur_time / sum_time;
            if (direction() == Forward) {
                if (percent < 0.5f) {
                    percent = 2.f * percent;
                    float _res = cos(percent * M_PI * 0.5f);
                    _position = 0.5f * (1.f - _res);
                } else {
                    percent = 2.f * percent - 1.f;
                    float _res = sin(percent * M_PI * 0.5f);
                    _position = 0.5f + 0.5f * _res;
                }
            } else {
                if (percent < 0.5f) {
                    percent = 2.f * percent;
                    float _res = cos(percent * M_PI * 0.5f);
                    _position = 0.5f + 0.5f * _res;
                } else {
                    percent = 2.f * percent - 1.f;
                    float _res = sin(percent * M_PI * 0.5f);
                    _position = 0.5f * (1.f - _res);
                }
            }
            _position = std::clamp(_position, 0.f, 1.f);
        }

        void InOutSineCurve::getReady() {}
    }

    namespace Transition {
        AbstractTransition::AbstractTransition(uint64_t duration, DeletionPolicy deletion_policy, Painter *painter)
                : _duration(duration), _deletion_policy(deletion_policy), _painter(painter) {
            _painter->_addTransition(this);
        }

        void AbstractTransition::setDeletionPolicy(const DeletionPolicy &deletion_policy) {
            _deletion_policy = deletion_policy;
        }

        void AbstractTransition::setDuration(uint64_t duration) {
            _duration = duration;
        }

        void AbstractTransition::setDirection(const Direction &direction) {
            _direction = direction;
        }

        void AbstractTransition::start() {
            // if (_state == Running) return;
            _state = Running;
            _start_time = SDL_GetTicks();
            _change_signal = false;
            getReady();
            EventSystem::global()->setHandlerEnabled(false);
            _painter->_startTransition(this);
        }

        void AbstractTransition::pause() {
            EventSystem::global()->setHandlerEnabled(true);
            _state = Paused;
        }

        void AbstractTransition::stop() {
            // if (_state == Stopped) return;
            _state = Stopped;
            _change_signal = false;
            _painter->_stopTransition(this);
            EventSystem::global()->setHandlerEnabled(true);
        }

        uint64_t AbstractTransition::duration() const {
            return _duration;
        }

        uint64_t AbstractTransition::currentTime() const {
            return _current_time;
        }

        uint64_t AbstractTransition::loopCount() const {
            return _loop_count;
        }

        void AbstractTransition::clearLoopCount() {
            _loop_count = 0;
        }

        const Direction &AbstractTransition::direction() const {
            return _direction;
        }

        const State &AbstractTransition::state() const {
            return _state;
        }

        const DeletionPolicy &AbstractTransition::deletionPolicy() const {
            return _deletion_policy;
        }

        void AbstractTransition::______() {
            if (_state == Running) {
                auto _cur_time = SDL_GetTicks();
                _current_time = _cur_time - _start_time;
                if (_current_time <= _duration) update();
                else {
                    _loop_count += 1;
                    _state = Stopped;
                    EventSystem::global()->setHandlerEnabled(true);
                    _painter->_stopTransition(this);
                    if (_deletion_policy == DeleteWhenStopped) {
                        _painter->_removeTransition(this);
                    }
                }
            }
        }

        bool AbstractTransition::__is_changed() {
            return _change_signal;
        }

        DarkTransition::DarkTransition(uint64_t _duration, DeletionPolicy deletion_policy, Painter *painter)
                : AbstractTransition(
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
                    _mask.back_color.a = (uint8_t) (255.f * percent);
                if (_direction == Forward)
                    _mask.back_color.a = (uint8_t) (255.f * (1.f - percent));
            }
            _painter->drawRectangle(_mask);
        }

        void DarkTransition::getReady() {
            _mask.pos = {0, 0};
            _mask.size = {(float) _painter->window()->geometry.width, (float) _painter->window()->geometry.height};
        }

        const SColor &DarkTransition::backgroundColor() const {
            return _mask.back_color;
        }

        EraseTransition::EraseTransition(uint64_t duration, const enum EraseDirection &direction,
                                         DeletionPolicy deletion_policy, Painter *painter)
                : AbstractTransition(duration, deletion_policy, painter), _direction(direction) {
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
                    if (AbstractTransition::_direction == Forward) {
                        _rect.pos = {(_rect.size.width * percent), 0};
                    } else {
                        _rect.pos = {_rect.size.width * (1.f - percent), 0};
                    }
                } else if (_direction == EraseDirection::RightToLeft) {
                    if (AbstractTransition::_direction == Forward) {
                        _rect.pos = {(-(_rect.size.width * percent)), 0};
                    } else {
                        _rect.pos = {(-(_rect.size.width * (1.f - percent))), 0};
                    }
                } else if (_direction == EraseDirection::TopToBottom) {
                    if (AbstractTransition::_direction == Forward) {
                        _rect.pos = {0, (_rect.size.height * percent)};
                    } else {
                        _rect.pos = {0, _rect.size.height * (1.f - percent)};
                    }
                } else if (_direction == EraseDirection::BottomToTop) {
                    if (AbstractTransition::_direction == Forward) {
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
                                       DeletionPolicy deletion_policy, Painter *painter)
                : AbstractTransition(duration, deletion_policy, painter), _move_direction(direction) {}

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
                _sprite1->properties()->position = (_direction == Forward ?
                                                    Vector2(_sprite1->size().width * percent, 0) :
                                                    Vector2(_sprite1->size().width * (1.f - percent), 0));
                if (_draw_second_pic) {
                    _sprite2->properties()->position = (_direction == Forward ?
                                                        Vector2(-(_sprite2->size().width * (1.f - percent)), 0) :
                                                        Vector2(-(_sprite2->size().width * percent), 0));
                }
            } else if (_move_direction == RightToLeft) {
                _sprite1->properties()->position = (_direction == Forward ?
                                                    Vector2(-(_sprite1->size().width * percent), 0) :
                                                    Vector2(-(_sprite1->size().width * (1.f - percent)), 0));
                if (_draw_second_pic) {
                    _sprite2->properties()->position = (_direction == Forward ?
                                                        Vector2(_sprite2->size().width * percent, 0) :
                                                        Vector2(_sprite2->size().width * (1.f - percent), 0));
                }
            } else if (_move_direction == TopToBottom) {
                _sprite1->properties()->position = (_direction == Forward ?
                                                    Vector2(0, _sprite1->size().height * percent) :
                                                    Vector2(0, _sprite1->size().height * (1.f - percent)));
                if (_draw_second_pic) {
                    _sprite2->properties()->position = (_direction == Forward ?
                                                        Vector2(-(_sprite2->size().height * (1.f - percent)), 0) :
                                                        Vector2(-(_sprite2->size().height * percent), 0));
                }
            } else if (_move_direction == BottomToTop) {
                _sprite1->properties()->position = (_direction == Forward ?
                                                    Vector2(0, -(_sprite1->size().height * percent)) :
                                                    Vector2(0, -(_sprite1->size().height * (1.f - percent))));
                if (_draw_second_pic) {
                    _sprite2->properties()->position = (_direction == Forward ?
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
    }
} // EasyEngine