
#include "Components.h"

#include <memory>

EasyEngine::Components::BGM::BGM() {}

EasyEngine::Components::BGM::~BGM() {}

EasyEngine::Components::BGM::BGM(const std::string &path) : _path(path) {
    auto ret = AudioSystem::instance()->loadBGM(*this);
    _is_load = (ret != -1);
    _channel = static_cast<uint8_t>(ret);
}

void EasyEngine::Components::BGM::setPath(const std::string &path) {
    _path = path;
    auto ret = AudioSystem::instance()->loadBGM(*this);
    _is_load = (ret != -1);
    _channel = static_cast<uint8_t>(ret);
}

const std::string &EasyEngine::Components::BGM::path() const {
    return _path;
}

void EasyEngine::Components::BGM::play(bool loop) {
    if (_is_load) {
        _is_loop = loop;
        reload();
        AudioSystem::instance()->playBGM(_channel, loop);
    }
}

void EasyEngine::Components::BGM::stop() {
    if (_is_load)
        AudioSystem::instance()->stopBGM(_channel, false, 100);
}

void EasyEngine::Components::BGM::pause() {
    if (_is_load)
        AudioSystem::instance()->stopBGM(_channel, true);
}

bool EasyEngine::Components::BGM::isPlayed() const {
    return AudioSystem::instance()->bgmChannel(_channel).status == AudioSystem::Audio::Playing;
}

bool EasyEngine::Components::BGM::isLoop() const {
    return _is_loop;
}

int64_t EasyEngine::Components::BGM::position() const {
    auto _ms = MIX_AudioFramesToMS(AudioSystem::instance()->bgmChannel(_channel).audio,
                                   MIX_GetTrackPlaybackPosition(AudioSystem::instance()->bgmChannel(_channel).Stream.track));
    if (AudioSystem::instance()->bgmChannel(_channel).status == AudioSystem::Audio::Loaded) return 0;
    return _ms;
}

void EasyEngine::Components::BGM::reload() {
    if (AudioSystem::instance()->bgmChannel(_channel).url != _path) {
        auto ret = AudioSystem::instance()->loadBGM(*this);
        _is_load = (ret != -1);
        _channel = static_cast<uint8_t>(ret);
    }
}

EasyEngine::Components::SFX::SFX() {}

EasyEngine::Components::SFX::~SFX() {}

EasyEngine::Components::SFX::SFX(const std::string &path) {
    _path = path;
    auto ret = AudioSystem::instance()->loadSFX(*this);
    _is_load = (ret != -1);
    _channel = static_cast<uint8_t>(ret);
}

void EasyEngine::Components::SFX::setPath(const std::string &path) {
    _path = path;
    auto ret = AudioSystem::instance()->loadSFX(*this);
    _is_load = (ret != -1);
    _channel = static_cast<uint8_t>(ret);
}

const std::string &EasyEngine::Components::SFX::path() const {
    return _path;
}

void EasyEngine::Components::SFX::play() {
    if (_is_load) {
        reload();
        AudioSystem::instance()->playSFX(_channel);
    }
}

void EasyEngine::Components::SFX::play(uint32_t delay) {
    if (_is_load) {
        reload();
        play();
        if (!_timer) {
            _timer = new Timer(delay, [this](){
                play();
            });
        }
        _timer->start(true);
    }
}

void EasyEngine::Components::SFX::stop() {
    if (_is_load) {
        if (_timer) _timer->stop();
        AudioSystem::instance()->stopSFX(_channel);
    }
}

bool EasyEngine::Components::SFX::isLoop() const {
    return _is_loop;
}

void EasyEngine::Components::SFX::reload() {
    if (AudioSystem::instance()->sfxChannel(_channel).url != _path) {
        auto ret = AudioSystem::instance()->loadSFX(*this);
        _is_load = (ret != -1);
        _channel = static_cast<uint8_t>(ret);
    }
}

EasyEngine::Components::Timer::Timer() : _delay(0) {}

EasyEngine::Components::Timer::~Timer() {}

EasyEngine::Components::Timer::Timer(uint64_t delay, const std::function<void()> &function) 
    : _delay(delay), _timer_function(function) {
}

void EasyEngine::Components::Timer::setDelay(uint64_t delay) {
    _delay = delay;
}

void EasyEngine::Components::Timer::installTimerEvent(const std::function<void()> &function) {
    _timer_function = function;
}

void EasyEngine::Components::Timer::stop() {
    _enabled = false;
}

void EasyEngine::Components::Timer::start(bool loop) {
    if (!_enabled) {
        _enabled = true;
        _loop = loop;
        _start_time = _currentTimeMs();
        _id = EventSystem::global()->addTimer(this);
    }
}

bool EasyEngine::Components::Timer::enabled() const {
    return _enabled;
}

bool EasyEngine::Components::Timer::loop() const {
    return _loop;
}

uint64_t EasyEngine::Components::Timer::delay() const {
    return _delay;
}

uint64_t EasyEngine::Components::Timer::_currentTimeMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

void EasyEngine::Components::Timer::update() {
    if (!_enabled || !_timer_function) return;

    uint64_t current_time = _currentTimeMs();
    uint64_t elapsed = current_time - _start_time;
    if (elapsed >= _delay) {
        try {
            _timer_function();
        } catch (const std::exception &e) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                         "[ERROR] Failed to called timer function! Code: %s", e.what());
        }
        if (_loop) _start_time = current_time;
        else {
            _enabled = false;
        }
    }
}

EasyEngine::Components::Sprite::Sprite(const std::string &name, SRenderer *renderer)
    : _name(name), _renderer(renderer), _size(0, 0) {
    if (!_renderer) {
        SDL_Log("[ERROR] Current renderer is not valid!");
        return;
    }
    _surface = SDL_CreateSurface(0, 0, SDL_PIXELFORMAT_RGBA64);
    _texture = SDL_CreateTextureFromSurface(renderer, _surface);
    _properties = std::make_unique<Properties>();
}

EasyEngine::Components::Sprite::Sprite(const std::string &name, const EasyEngine::Components::Sprite &spirit)
    : _name(name), _renderer(spirit._renderer), _size(spirit._size) {
    if (!_renderer) {
        SDL_Log("[ERROR] Current renderer is not valid!");
        return;
    }
    _surface = SDL_DuplicateSurface(spirit._surface);
    _texture = SDL_CreateTextureFromSurface(_renderer, _surface);
    _properties = std::make_unique<Properties>();
}

EasyEngine::Components::Sprite::Sprite(const std::string &name, const EasyEngine::Components::Sprite &spirit,
                                       const EasyEngine::Vector2 &clip_pos, const EasyEngine::Size &clip_size)
    : _name(name), _renderer(spirit._renderer), _size(spirit._size) {
    if (!_renderer) {
        SDL_Log("[ERROR] Current renderer is not valid!");
        return;
    }
    _surface = SDL_DuplicateSurface(spirit._surface);
    auto _rect = SDL_Rect(
            static_cast<int>(clip_pos.x),
            static_cast<int>(clip_pos.y),
            static_cast<int>(clip_size.width),
            static_cast<int>(clip_size.height)
    );
    SDL_SetSurfaceClipRect(_surface, &_rect);
    _texture = SDL_CreateTextureFromSurface(_renderer, _surface);
    _properties = std::make_unique<Properties>();
}

EasyEngine::Components::Sprite::Sprite(const std::string &name, const std::string &path, SRenderer *renderer)
    : _name(name), _path(path), _renderer(renderer), _size(0, 0) {
    if (!_renderer) {
        SDL_Log("[ERROR] Current renderer is not valid!");
        return;
    }
    _surface = IMG_Load(path.data());
    if (!_surface) {
        SDL_Log("[ERROR] Can't load image file: %s\nps: Try to use `Spirit::setPath()`.", path.data());
        return;
    }
    _properties = std::make_unique<Properties>();
    _size.reset(_surface->w, _surface->h);
    _texture = SDL_CreateTextureFromSurface(_renderer, _surface);
}

EasyEngine::Components::Sprite::Sprite(const std::string &name, const std::string &path,
                                       const EasyEngine::Vector2 &clip_pos, const EasyEngine::Size &clip_size,
                                       SRenderer *renderer)
    : _name(name), _path(path), _renderer(renderer), _size(0, 0) {
    if (!_renderer) {
        SDL_Log("[ERROR] Current renderer is not valid!");
        return;
    }
    _surface = IMG_Load(path.data());
    if (!_surface) {
        SDL_Log("[ERROR] Can't load image file: %s\nps: Try to use `Spirit::setPath()`.", path.data());
        return;
    }
    auto _rect = SDL_Rect(
            static_cast<int>(clip_pos.x),
            static_cast<int>(clip_pos.y),
            static_cast<int>(clip_size.width),
            static_cast<int>(clip_size.height)
    );
    SDL_SetSurfaceClipRect(_surface, &_rect);
    _size.reset(_surface->w, _surface->h);
    _texture = SDL_CreateTextureFromSurface(_renderer, _surface);
    _properties = std::make_unique<Properties>();
}

EasyEngine::Components::Sprite::~Sprite() {
    if (_texture) SDL_DestroyTexture(_texture);
    if (_surface) SDL_DestroySurface(_surface);
}

void EasyEngine::Components::Sprite::setName(const std::string &new_name) {
    _name = new_name;
}

std::string EasyEngine::Components::Sprite::name() const {
    return _name;
}

bool EasyEngine::Components::Sprite::setPath(const std::string &new_path) {
    _path = new_path;
    if (_texture) SDL_DestroyTexture(_texture);
    if (_surface) SDL_DestroySurface(_surface);
    _surface = IMG_Load(new_path.data());
    if (!_surface) {
        SDL_Log("[ERROR] Can't load image file: %s", new_path.data());
        return false;
    }
    _texture = SDL_CreateTextureFromSurface(_renderer, _surface);
    return true;
}

std::string EasyEngine::Components::Sprite::path() const {
    return _path;
}

bool EasyEngine::Components::Sprite::isValid(const std::string &path) const {
    if (!_renderer) return false;
    if (!_surface) return false;
    if (!_texture) return false;
    return true;
}

void EasyEngine::Components::Sprite::setRenderer(SRenderer *renderer) {
    if (!_renderer) {
        SDL_Log("[ERROR] The specified renderer is not valid!");
        return;
    }
    _renderer = renderer;
}

const SRenderer *EasyEngine::Components::Sprite::renderer() const {
    return _renderer;
}

void EasyEngine::Components::Sprite::resize(float width, float height) {
    _size.reset(width, height);
}

EasyEngine::Size EasyEngine::Components::Sprite::size() const {
    return _size;
}

STexture *EasyEngine::Components::Sprite::spirit() const {
    return _texture;
}

void EasyEngine::Components::Sprite::draw(const Vector2 &pos, Painter *painter) const {
    painter->drawSprite(*this, pos);
}

void
EasyEngine::Components::Sprite::draw(const Vector2 &pos, float scaled, Painter *painter, const Vector2 &center) const {
    Properties properties;
    properties.position = pos;
    properties.scaled = scaled;
    properties.scaled_center = center;
    painter->drawSprite(*this, properties);
}

void EasyEngine::Components::Sprite::draw(const Vector2 &pos, const Vector2 &clip_pos, const Size &clip_size,
                                          Painter *painter) const {
    Properties properties;
    properties.position = pos;
    properties.clip_pos = clip_pos;
    properties.clip_size = clip_size;
    painter->drawSprite(*this, properties);
}

void EasyEngine::Components::Sprite::draw(const Vector2 &pos, double rotate, Painter *painter, const FlipMode &flipMode,
                                          const Vector2 &rotate_center) const {
    Properties properties;
    properties.position = pos;
    properties.rotate = rotate;
    properties.flip_mode = flipMode;
    properties.rotate_center = rotate_center;
    painter->drawSprite(*this, properties);
}

void EasyEngine::Components::Sprite::draw(const EasyEngine::Vector2 &pos, const SColor &color_alpha,
                                          Painter *painter) const {
    Properties properties;
    properties.position = pos;
    properties.color_alpha = color_alpha;
    painter->drawSprite(*this, properties);
}

void
EasyEngine::Components::Sprite::draw(const EasyEngine::Components::Sprite::Properties &properties,
                                     Painter *painter) const {
    painter->drawSprite(*this, properties);
}

EasyEngine::Components::Sprite::Properties *EasyEngine::Components::Sprite::properties() const {
    return _properties.get();
}

void EasyEngine::Components::Sprite::draw(EasyEngine::Painter *painter) const {
    painter->drawSprite(*this, _properties.get());
}


EasyEngine::Components::SpriteGroup::SpriteGroup(const EasyEngine::Components::SpriteGroup &&group) {
    uint32_t _idx = 0;
    for (auto& sprite : group._sprites) {
        _sprites.push_back(std::make_shared<Sprite>(sprite->name(), *sprite.get()));
        auto dst_properties = _sprites.back()->properties();
        auto src_properties = group._sprites.at(_idx++)->properties();
        dst_properties->position = src_properties->position;
        dst_properties->color_alpha = src_properties->color_alpha;
        dst_properties->scaled = src_properties->scaled;
        dst_properties->scaled_center = src_properties->scaled_center;
        dst_properties->rotate = src_properties->rotate;
        dst_properties->rotate_center = src_properties->rotate_center;
        dst_properties->clip_mode = src_properties->clip_mode;
        dst_properties->clip_pos = src_properties->clip_pos;
        dst_properties->clip_size = src_properties->clip_size;
    }
}

void EasyEngine::Components::SpriteGroup::append(const Sprite &sprite) {
    _sprites.push_back(std::make_shared<Sprite>(sprite.name(), sprite));
}

void EasyEngine::Components::SpriteGroup::insert(uint32_t index, const EasyEngine::Components::Sprite &sprite) {
    _sprites.insert(_sprites.begin() + index,
                    std::make_shared<Sprite>(sprite.name(), sprite));
}

void EasyEngine::Components::SpriteGroup::remove(const std::string &name) {
    _sprites.erase(_sprites.begin() + indexAt(name));
}

void EasyEngine::Components::SpriteGroup::remove(uint32_t index) {
    _sprites.erase(_sprites.begin() + index);
}

void EasyEngine::Components::SpriteGroup::replace(uint32_t index, const EasyEngine::Components::Sprite &sprite) {
    _sprites.at(index).reset(new Sprite(sprite.name(), sprite));
}

void EasyEngine::Components::SpriteGroup::swap(uint32_t index1, uint32_t index2) {
    std::swap(_sprites.at(index1), _sprites.at(index2));
}

void EasyEngine::Components::SpriteGroup::swap(const std::string &sprite1, const std::string &sprite2) {
    std::swap(_sprites.at(indexAt(sprite1)), _sprites.at(indexAt(sprite2)));
}

void EasyEngine::Components::SpriteGroup::swap(uint32_t index, const std::string &name) {
    std::swap(_sprites.at(index), _sprites.at(indexAt(name)));
}

uint32_t EasyEngine::Components::SpriteGroup::indexAt(const std::string &name, uint32_t start) {
    for (uint32_t _idx = 0; _idx < _sprites.size(); ++_idx) {
        if (_sprites[_idx]->name() == name) return _idx;
    }
    return UINT32_MAX;
}

uint32_t EasyEngine::Components::SpriteGroup::lastIndexAt(const std::string &name, uint32_t start) {
    for (uint32_t _idx = 0; _idx < _sprites.size(); ++_idx) {
        if (_sprites[_sprites.size() - _idx - 1]->name() == name) return _idx;
    }
    return UINT32_MAX;
}

EasyEngine::Components::Sprite *EasyEngine::Components::SpriteGroup::indexOf(uint32_t index) {
    return _sprites.at(index).get();
}

EasyEngine::Components::Sprite *EasyEngine::Components::SpriteGroup::nameOf(const std::string &name) {
    return _sprites.at(indexAt(name)).get();
}

EasyEngine::Components::Sprite::Properties *EasyEngine::Components::SpriteGroup::propertiesOf(uint32_t index) {
    return _sprites.at(index)->properties();
}

EasyEngine::Components::Sprite::Properties *EasyEngine::Components::SpriteGroup::propertiesOf(const std::string &name) {
    return _sprites.at(indexAt(name))->properties();
}

void EasyEngine::Components::SpriteGroup::draw(const Vector2 &pos, Painter *painter) {
    for (auto& sprite : _sprites) {
        sprite->draw(pos, painter);
    }
}

uint32_t EasyEngine::Components::SpriteGroup::count() const {
    return _sprites.size();
}

EasyEngine::Components::Animation::Animation(const std::string &name) : _name(name) {}

EasyEngine::Components::Animation::Animation(const std::string &name, const std::vector<Sprite> &sprite_list,
                                             uint64_t duration_per_frame) : _name(name) {
    for (auto& sprite : sprite_list) {
        _animations.push_back({std::make_unique<Sprite>(sprite.name(), sprite), duration_per_frame});
    }
}

void EasyEngine::Components::Animation::addFrame(const EasyEngine::Components::Sprite &sprite, uint64_t duration) {
    _animations.push_back({std::make_unique<Sprite>(sprite.name(), sprite)});
    _animations.back().duration = duration;
}

void EasyEngine::Components::Animation::insertFrame(const EasyEngine::Components::Sprite &sprite, uint64_t duration,
                                                    const size_t frame) {
    _animations.insert(_animations.begin() + frame,
                       {std::make_unique<Sprite>(sprite.name(), sprite)});
    _animations[frame].duration = duration;
}

void EasyEngine::Components::Animation::replaceFrame(const EasyEngine::Components::Sprite &sprite, const size_t frame,
                                                     const uint64_t duration) {
    _animations.at(frame).sprite.reset();
    _animations.at(frame).sprite = std::make_unique<Sprite>(sprite.name(), sprite);
    SDL_Log("D1 %llu D2 %llu", _animations.at(frame).duration, duration);
    _animations.at(frame).duration = duration;
}

void EasyEngine::Components::Animation::removeFrame(const size_t frame) {
    _animations.erase(_animations.begin() + frame);
}

void EasyEngine::Components::Animation::clearFrames() {
    _animations.clear();
}

size_t EasyEngine::Components::Animation::framesCount() const {
    return _animations.size();
}

uint64_t EasyEngine::Components::Animation::durationInFrame(const size_t frame) {
    return _animations.at(frame).duration;
}

EasyEngine::Components::Sprite *EasyEngine::Components::Animation::sprite(const size_t frame) const {
    return _animations.at(frame).sprite.get();
}

void EasyEngine::Components::Animation::draw(const EasyEngine::Vector2 &position, EasyEngine::Painter* painter) {
    _animations[_cur_frame].sprite->draw(position, painter);
}

void EasyEngine::Components::Animation::play(bool loop) {
    if (!_frame_changer) {
        _frame_changer = new Timer();
        _frame_changer->setDelay(_animations.front().duration);
        _frame_changer->installTimerEvent([this]{
            _frame_changer->setDelay(_animations[_cur_frame].duration);
            if (_cur_frame >= framesCount() - 1) {
                _cur_frame = 0;
                if (!_is_loop) stop();
            } else _cur_frame += 1;
        });
    }
    if (!_frame_changer->enabled()) {
        _cur_frame = 0;
        _frame_changer->start(true);
        _is_loop = loop;
    }
}

void EasyEngine::Components::Animation::stop() {
    if (_frame_changer) _frame_changer->stop();
}

bool EasyEngine::Components::Animation::isPlayedAnimation() const {
    return (!_frame_changer ? false : _frame_changer->enabled());
}

EasyEngine::Components::Sprite * EasyEngine::Components::Animation::frame(size_t frame) const {
    return _animations[frame].sprite.get();
}

