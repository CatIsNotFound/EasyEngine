
#include "Components.h"

#include <memory>

EasyEngine::Components::BGM::BGM() {}

EasyEngine::Components::BGM::~BGM() {}

EasyEngine::Components::BGM::BGM(const std::string &resource_name) {
    if (ResourceSystem::global()->resourceType(resource_name) == Resource::Audio) {
        _path = ResourceSystem::global()->resourcePath(resource_name);
        auto ret = AudioSystem::global()->loadBGM(*this);
        _is_load = (ret != -1);
        _channel = static_cast<uint8_t>(ret);
    } else {
        SDL_Log("[ERROR] Resource '%s' is not the audio file!", resource_name.c_str());
        _is_load = false;
    }
}

void EasyEngine::Components::BGM::setResource(const std::string &resource_name) {
    if (ResourceSystem::global()->resourceType(resource_name) == Resource::Audio) {
        _path = ResourceSystem::global()->resourcePath(resource_name);
        AudioSystem::global()->unloadBGM(_channel);
        auto ret = AudioSystem::global()->loadBGM(*this);
        _is_load = (ret != -1);
        _channel = static_cast<uint8_t>(ret);
    } else {
        SDL_Log("[ERROR] Resource '%s' is not the audio file!", resource_name.c_str());
        _is_load = false;
    }
}

const std::string &EasyEngine::Components::BGM::path() const {
    return _path;
}

void EasyEngine::Components::BGM::play(bool loop) {
    if (_is_load) {
        _is_loop = loop;
        reload();
        AudioSystem::global()->playBGM(_channel, loop);
    }
}

void EasyEngine::Components::BGM::stop(int64_t fade_out_duration) {
    if (_is_load) {
        AudioSystem::global()->stopBGM(_channel, false, fade_out_duration);
    }
}

void EasyEngine::Components::BGM::pause() {
    if (_is_load)
        AudioSystem::global()->stopBGM(_channel, true);
}

bool EasyEngine::Components::BGM::isPlayed() const {
    return AudioSystem::global()->bgmChannel(_channel).status == AudioSystem::Audio::Playing;
}

bool EasyEngine::Components::BGM::isLoop() const {
    return _is_loop;
}

int64_t EasyEngine::Components::BGM::position() const {
    auto _ms = MIX_AudioFramesToMS(AudioSystem::global()->bgmChannel(_channel).audio,
               MIX_GetTrackPlaybackPosition(AudioSystem::global()->bgmChannel(_channel).Stream.track));
    if (AudioSystem::global()->bgmChannel(_channel).status == AudioSystem::Audio::Loaded) return 0;
    return _ms;
}

void EasyEngine::Components::BGM::reload() {
    if (AudioSystem::global()->bgmChannel(_channel).url != _path) {
        auto ret = AudioSystem::global()->loadBGM(*this);
        _is_load = (ret != -1);
        _channel = static_cast<uint8_t>(ret);
    }
}

EasyEngine::Components::SFX::SFX() {}

EasyEngine::Components::SFX::~SFX() {}

EasyEngine::Components::SFX::SFX(const std::string &resource_name) {
    if (ResourceSystem::global()->resourceType(resource_name) == Resource::Audio) {
        _path = ResourceSystem::global()->resourcePath(resource_name);
        auto ret = AudioSystem::global()->loadSFX(*this);
        _is_load = (ret != -1);
        _channel = static_cast<uint8_t>(ret);
    } else {
        SDL_Log("[ERROR] Resource '%s' is not the audio file!", resource_name.c_str());
        _is_load = false;
    }
}

void EasyEngine::Components::SFX::setResource(const std::string &resource_name) {
    if (ResourceSystem::global()->resourceType(resource_name) == Resource::Audio) {
        _path = ResourceSystem::global()->resourcePath(resource_name);
        AudioSystem::global()->unloadSFX(_channel);
        auto ret = AudioSystem::global()->loadSFX(*this);
        _is_load = (ret != -1);
        _channel = static_cast<uint8_t>(ret);
    } else {
        SDL_Log("[ERROR] Resource '%s' is not the audio file!", resource_name.c_str());
        _is_load = false;
    }
}

const std::string &EasyEngine::Components::SFX::path() const {
    return _path;
}

void EasyEngine::Components::SFX::play() {
    if (_is_load) {
        reload();
        AudioSystem::global()->playSFX(_channel);
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
        AudioSystem::global()->stopSFX(_channel);
    }
}

bool EasyEngine::Components::SFX::isLoop() const {
    return _is_loop;
}

void EasyEngine::Components::SFX::reload() {
    if (AudioSystem::global()->sfxChannel(_channel).url != _path) {
        auto ret = AudioSystem::global()->loadSFX(*this);
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

void EasyEngine::Components::Timer::setEvent(const std::function<void()> &function) {
    _timer_function = std::move(function);
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

void EasyEngine::Components::Timer::______() {
    if (!_enabled || !_timer_function) return;

    uint64_t current_time = _currentTimeMs();
    uint64_t elapsed = current_time - _start_time;
    if (elapsed >= _delay) {
        try {
            _timer_function();
            _triggered_count += 1;
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

size_t EasyEngine::Components::Timer::count() const {
    return _triggered_count;
}

void EasyEngine::Components::Timer::clearCount() {
    _triggered_count = 0;
}

EasyEngine::Components::Trigger::Trigger() : _condition(nullptr), _event(nullptr) {}

EasyEngine::Components::Trigger::~Trigger() {}

void EasyEngine::Components::Trigger::setCondition(const std::function<bool()> &condition) {
    _condition = std::move(condition);
}

void EasyEngine::Components::Trigger::setEvent(const std::function<void()> &function) {
    _event = std::move(function);
}

void EasyEngine::Components::Trigger::setEnabled(bool enabled) {
    _enabled = enabled;
    if (enabled) {
        _id = EventSystem::global()->addTrigger(this);
    }
}

bool EasyEngine::Components::Trigger::enabled() const {
    return _enabled;
}

void EasyEngine::Components::Trigger::______() {
    if (_enabled && _condition()) {
        _event();
    }
}

void EasyEngine::Components::Trigger::trigger() {
    if (_condition()) {
        _event();
    }
}

std::function<bool()> &EasyEngine::Components::Trigger::condition() {
    return _condition;
}

std::function<void()> &EasyEngine::Components::Trigger::event() {
    return _event;
}


EasyEngine::Components::Sprite::Sprite(const std::string &name, Painter *painter)
    : _name(name), _painter(painter), _size(0, 0) {
    _surface = SDL_CreateSurface(0, 0, SDL_PIXELFORMAT_RGBA64);
    _texture = SDL_CreateTextureFromSurface(painter->window()->renderer, _surface);
    _properties = std::make_unique<Properties>();
}

EasyEngine::Components::Sprite::Sprite(const std::string &name, SSurface *surface, Painter *painter)
    : _name(name), _painter(painter) {
    _surface = surface;
    _texture = SDL_CreateTextureFromSurface(painter->window()->renderer, _surface);
    SDL_GetTextureSize(_texture, &_size.width, &_size.height);
    _properties = std::make_unique<Properties>();
}

EasyEngine::Components::Sprite::Sprite(const std::string &name, const std::string &resource_name, Painter *painter)
    : _name(name), _painter(painter) {
    _path = ResourceSystem::global()->resourcePath(resource_name);
    if (ResourceSystem::global()->resourceType(resource_name) == Resource::Image) {
        _surface = IMG_Load(_path.c_str());
    } else {
        SDL_Log("[ERROR] Resource '%s' is not the image file!", resource_name.c_str());
        _surface = SDL_CreateSurface(0, 0, SDL_PIXELFORMAT_RGBA64);
    }
    _texture = SDL_CreateTextureFromSurface(painter->window()->renderer, _surface);
    SDL_GetTextureSize(_texture, &_size.width, &_size.height);
    _properties = std::make_unique<Properties>();
}

EasyEngine::Components::Sprite::Sprite(const std::string &name, const EasyEngine::Components::Sprite &sprite)
    : _name(name), _painter(sprite._painter), _size(sprite._size), _path(sprite._path) {
    _surface = SDL_DuplicateSurface(sprite._surface);
    _texture = SDL_CreateTextureFromSurface(_painter->window()->renderer, _surface);
    _properties = std::make_unique<Properties>();
}

EasyEngine::Components::Sprite::Sprite(const std::string &name, const EasyEngine::Components::Sprite &sprite,
                                       const EasyEngine::Vector2 &clip_pos, const EasyEngine::Size &clip_size)
    : _name(name), _painter(sprite._painter), _size(sprite._size) {
    _surface = SDL_DuplicateSurface(sprite._surface);
    auto _rect = SDL_Rect(
            static_cast<int>(clip_pos.x),
            static_cast<int>(clip_pos.y),
            static_cast<int>(clip_size.width),
            static_cast<int>(clip_size.height)
    );
    SDL_SetSurfaceClipRect(_surface, &_rect);
    _texture = SDL_CreateTextureFromSurface(_painter->window()->renderer, _surface);
    _properties = std::make_unique<Properties>();
}

EasyEngine::Components::Sprite::Sprite(const std::string &name, const std::string &resource_name,
                                       const EasyEngine::Vector2 &clip_pos, const EasyEngine::Size &clip_size,
                                       Painter *painter)
    : _name(name), _painter(painter), _size(0, 0) {
    if (ResourceSystem::global()->resourceType(resource_name) == Resource::Image) {
        _surface = IMG_Load(ResourceSystem::global()->resourcePath(resource_name).c_str());
    } else {
        SDL_Log("[ERROR] Resource '%s' is not the image file!", resource_name.c_str());
        _surface = SDL_CreateSurface(0, 0, SDL_PIXELFORMAT_RGBA64);
    }
    if (!_surface) {
        SDL_Log("[ERROR] Can't load image file: %s\nps: Try to use `Spirit::setPath()`.", resource_name.data());
        return;
    }
    auto _rect = SDL_Rect(
            static_cast<int>(clip_pos.x),
            static_cast<int>(clip_pos.y),
            static_cast<int>(clip_size.width),
            static_cast<int>(clip_size.height)
    );
    SDL_SetSurfaceClipRect(_surface, &_rect);
    _size.reset((float)_surface->w, (float)_surface->h);
    _texture = SDL_CreateTextureFromSurface(_painter->window()->renderer, _surface);
    _properties = std::make_unique<Properties>();
}

EasyEngine::Components::Sprite::~Sprite() {
    if (_texture) SDL_DestroyTexture(_texture);
    if (_surface) SDL_DestroySurface(_surface);
}

void EasyEngine::Components::Sprite::copySprite(EasyEngine::Components::Sprite *sprite) {
    if (!sprite) {
        SDL_Log("[ERROR] The specified sprite is not valid!");
        return;
    }
    if (_texture) SDL_DestroyTexture(_texture);
    if (_surface) SDL_DestroySurface(_surface);
    _surface = SDL_DuplicateSurface(sprite->_surface);
    _texture = SDL_CreateTextureFromSurface(_painter->window()->renderer, _surface);
    SDL_GetTextureSize(_texture, &_size.width, &_size.height);
}

void EasyEngine::Components::Sprite::setName(const std::string &new_name) {
    _name = new_name;
}

std::string EasyEngine::Components::Sprite::name() const {
    return _name;
}

bool EasyEngine::Components::Sprite::setResource(const std::string &resource_name) {
    if (ResourceSystem::global()->resourceType(resource_name)) {
        _path = ResourceSystem::global()->resourcePath(resource_name);
    } else {
        SDL_Log("[ERROR] Resource '%s' is not the image path!", resource_name.c_str());
        return false;
    }
    if (_texture) SDL_DestroyTexture(_texture);
    if (_surface) SDL_DestroySurface(_surface);
    _surface = IMG_Load(_path.data());
    if (!_surface) {
        SDL_Log("[ERROR] Can't load image file: %s", _path.data());
        return false;
    }
    _texture = SDL_CreateTextureFromSurface(_painter->window()->renderer, _surface);
    return true;
}

std::string EasyEngine::Components::Sprite::path() const {
    return _path;
}

void EasyEngine::Components::Sprite::setSurface(SSurface *surface) {
    if (!surface) {
        return;
    }
    if (_texture) SDL_DestroyTexture(_texture);
    if (_surface) SDL_DestroySurface(_surface);
    _surface = surface;
    _texture = SDL_CreateTextureFromSurface(_painter->window()->renderer, _surface);
    if (!_texture) {
        return;
    }
    SDL_GetTextureSize(_texture, &_size.width, &_size.height);
}

bool EasyEngine::Components::Sprite::isValid(const std::string &path) const {
    if (!_painter) return false;
    if (!_surface) return false;
    if (!_texture) return false;
    return true;
}

void EasyEngine::Components::Sprite::setPainter(Painter *painter) {
    _painter = painter;
}

const EasyEngine::Painter * EasyEngine::Components::Sprite::painter() const {
    return _painter;
}

void EasyEngine::Components::Sprite::resize(float width, float height) {
    _size.reset(width, height);
}

EasyEngine::Size EasyEngine::Components::Sprite::size() const {
    return _size;
}

STexture *EasyEngine::Components::Sprite::sprite() const {
    return _texture;
}

void EasyEngine::Components::Sprite::draw(const Vector2 &pos) const {
    _painter->drawSprite(*this, pos);
}

void
EasyEngine::Components::Sprite::draw(const Vector2 &pos, float scaled, const Vector2 &center) const {
    Properties properties;
    properties.position = pos;
    properties.scaled = scaled;
    properties.scaled_center = center;
    _painter->drawSprite(*this, properties);
}

void EasyEngine::Components::Sprite::draw(const Vector2 &pos, const Vector2 &clip_pos, const Size &clip_size) const {
    Properties properties;
    properties.position = pos;
    properties.clip_pos = clip_pos;
    properties.clip_size = clip_size;
    _painter->drawSprite(*this, properties);
}

void EasyEngine::Components::Sprite::draw(const Vector2 &pos, double rotate, const FlipMode &flipMode,
                                          const Vector2 &rotate_center) const {
    Properties properties;
    properties.position = pos;
    properties.rotate = rotate;
    properties.flip_mode = flipMode;
    properties.rotate_center = rotate_center;
    _painter->drawSprite(*this, properties);
}

void EasyEngine::Components::Sprite::draw(const EasyEngine::Vector2 &pos, const SColor &color_alpha) const {
    Properties properties;
    properties.position = pos;
    properties.color_alpha = color_alpha;
    _painter->drawSprite(*this, properties);
}

void
EasyEngine::Components::Sprite::draw(const EasyEngine::Components::Sprite::Properties &properties) const {
    _painter->drawSprite(*this, properties);
}

EasyEngine::Components::Sprite::Properties *EasyEngine::Components::Sprite::properties() const {
    return _properties.get();
}

void EasyEngine::Components::Sprite::draw(EasyEngine::Painter *painter) const {
    if (painter)
        painter->drawSprite(*this, _properties.get());
    else
        _painter->drawSprite(*this, _properties.get());
}

EasyEngine::Components::SpriteGroup::SpriteGroup(const std::string &name) : _name(name) {}

EasyEngine::Components::SpriteGroup::SpriteGroup(const EasyEngine::Components::SpriteGroup &group)
    :_name(group._name) {
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

void EasyEngine::Components::SpriteGroup::setName(const std::string &name) { _name = name; }

const std::string& EasyEngine::Components::SpriteGroup::name() const { return _name; }

void EasyEngine::Components::SpriteGroup::resize(float width, float height) {
    if (width <= 0 || height <= 0) return;
    auto origin_size = size();
    float ratioX = width / origin_size.width;
    float ratioY = height / origin_size.height;
    for (auto& sprite : _sprites) {
        auto props = sprite->properties();
        props->position.x *= ratioX;
        props->position.y *= ratioY;
        auto currentSize = sprite->size();
        sprite->resize(currentSize.width * ratioX, currentSize.height * ratioY);
    }
}

void EasyEngine::Components::SpriteGroup::resize(const EasyEngine::Size &size) {
    resize(size.width, size.height);
}

EasyEngine::Size EasyEngine::Components::SpriteGroup::size() const {
    float minX = _sprites.front()->properties()->position.x;
    float maxX = _sprites.front()->properties()->position.x + _sprites.front()->size().width;
    float minY = _sprites.front()->properties()->position.y;
    float maxY = _sprites.front()->properties()->position.y + _sprites.front()->size().height;
    
    for (uint64_t i = 1; i < _sprites.size(); ++i) {
        float spriteMinX = _sprites[i]->properties()->position.x;
        float spriteMaxX = spriteMinX + _sprites[i]->size().width;
        float spriteMinY = _sprites[i]->properties()->position.y;
        float spriteMaxY = spriteMinY + _sprites[i]->size().height;
        minX = std::min(spriteMinX, minX);
        minY = std::min(spriteMinY, minY);
        maxX = std::max(spriteMaxX, maxX);
        maxY = std::max(spriteMaxY, maxY);
    }
    return {maxX - minX, maxY - minY};
}

void EasyEngine::Components::SpriteGroup::append(Sprite *sprite) {
    _sprites.push_back(std::shared_ptr<Sprite>(sprite));
}

void EasyEngine::Components::SpriteGroup::insert(uint32_t index, EasyEngine::Components::Sprite *sprite) {
    _sprites.insert(_sprites.begin() + index,
                    std::shared_ptr<Sprite>(sprite));
}

void EasyEngine::Components::SpriteGroup::remove(const std::string &name) {
    _sprites.erase(_sprites.begin() + indexAt(name));
}

void EasyEngine::Components::SpriteGroup::remove(uint32_t index) {
    _sprites.erase(_sprites.begin() + index);
}

void EasyEngine::Components::SpriteGroup::replace(uint32_t index, Sprite *sprite) {
    _sprites.at(index) = std::shared_ptr<Sprite>(sprite);
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

void EasyEngine::Components::SpriteGroup::draw(const Vector2 &pos) {
    for (auto& sprite : _sprites) {
        sprite->draw(pos);
    }
}

void EasyEngine::Components::SpriteGroup::setPosition(const EasyEngine::Vector2 &pos) {
    _pos.reset(pos.x, pos.y);
}

EasyEngine::Vector2 EasyEngine::Components::SpriteGroup::position() const {
    return _pos;
}

void EasyEngine::Components::SpriteGroup::draw() {
    for (auto& sprite : _sprites) {
        sprite->draw(_pos);
    }
}

uint32_t EasyEngine::Components::SpriteGroup::count() const {
    return _sprites.size();
}

EasyEngine::Components::FrameAnimation::FrameAnimation(const std::string &name) : _name(name) {}

EasyEngine::Components::FrameAnimation::FrameAnimation(const std::string &name,
                                                       const std::vector<Sprite *> &sprite_list,
                                                       uint64_t duration_per_frame) : _name(name) {
    for (auto& sprite : sprite_list) {
        _animations.push_back({sprite, duration_per_frame});
    }
}

EasyEngine::Components::FrameAnimation::~FrameAnimation() {
    if (_frame_changer) {
        EventSystem::global()->removeTimer(_frame_changer);
    }
}

void EasyEngine::Components::FrameAnimation::setName(const std::string &name) { _name = name; }

const std::string& EasyEngine::Components::FrameAnimation::name() const { return _name; }

void EasyEngine::Components::FrameAnimation::setPosition(const EasyEngine::Vector2 &pos) { _pos.reset(pos.x, pos.y); }

const EasyEngine::Vector2& EasyEngine::Components::FrameAnimation::position() const { return _pos; }

void EasyEngine::Components::FrameAnimation::addFrame(EasyEngine::Components::Sprite *sprite, uint64_t duration) {
    _animations.push_back({.sprite = sprite, .duration = duration});
}

void EasyEngine::Components::FrameAnimation::insertFrame(EasyEngine::Components::Sprite *sprite, uint64_t duration,
                                                         const size_t frame) {
    _animations.insert(_animations.begin() + frame, {.sprite = sprite, .duration = duration});
}

void EasyEngine::Components::FrameAnimation::replaceFrame(EasyEngine::Components::Sprite *sprite, const size_t frame,
                                                          const uint64_t duration) {
    _animations.at(frame).sprite = sprite;
    _animations.at(frame).duration = duration;
}

void EasyEngine::Components::FrameAnimation::removeFrame(const size_t frame) {
    _animations.erase(_animations.begin() + frame);
}

void EasyEngine::Components::FrameAnimation::clearFrames() {
    _animations.clear();
}

size_t EasyEngine::Components::FrameAnimation::framesCount() const {
    return _animations.size();
}

uint64_t EasyEngine::Components::FrameAnimation::durationInFrame(const size_t frame) {
    return _animations.at(frame).duration;
}

EasyEngine::Components::Sprite *EasyEngine::Components::FrameAnimation::sprite(const size_t frame) const {
    return _animations.at(frame).sprite;
}

void EasyEngine::Components::FrameAnimation::draw() {
    draw(_pos);
}

void EasyEngine::Components::FrameAnimation::draw(const Vector2& pos){
    try {
        _animations.at(_cur_frame).sprite->draw(pos);
    } catch (const std::exception& e) {
        SDL_Log("[ERROR] Failed to drawn for frame %llu in animation '%s'!", _cur_frame, _name.c_str());
    }
}

void EasyEngine::Components::FrameAnimation::play(bool loop, size_t start_frame) {
    if (!_frame_changer) {
        _frame_changer = new Timer();
        _frame_changer->setDelay(_animations.front().duration);
        _frame_changer->setEvent([this] {
            _frame_changer->setDelay(_animations[_cur_frame].duration);
            if (_cur_frame >= framesCount() - 1) {
                _cur_frame = 0;
                _played += 1;
                if (!_is_loop) stop();
            } else _cur_frame += 1;
        });
    }
    if (!_frame_changer->enabled()) {
        _cur_frame = start_frame;
        _frame_changer->start(true);
        _is_loop = loop;
    }
}

void EasyEngine::Components::FrameAnimation::stop() {
    if (_frame_changer) _frame_changer->stop();
}

bool EasyEngine::Components::FrameAnimation::isPlayedAnimation() const {
    return (!_frame_changer ? false : _frame_changer->enabled());
}

size_t EasyEngine::Components::FrameAnimation::currentFrame() const {
    return _cur_frame;
}

size_t EasyEngine::Components::FrameAnimation::playedCount() const {
    return _played;
}

void EasyEngine::Components::FrameAnimation::clearPlayedCount() {
    _played = 0;
}

EasyEngine::Components::Timer * EasyEngine::Components::FrameAnimation::timer() {
    return (_frame_changer ? _frame_changer : nullptr);
}

EasyEngine::Components::Control::Control(const std::string &name) : _name(name) {
    _defined_sprite = nullptr;
    EventSystem::global()->addControl(this);
}

EasyEngine::Components::Control::Control(const std::string &name, const EasyEngine::Components::Sprite &sprite) {
    _defined_sprite = std::make_shared<Sprite>(fmt::format("{}_group", sprite.name()), sprite);
    _defined_sprite->properties()->clip_mode = true;
    EventSystem::global()->addControl(this);
}

EasyEngine::Components::Control::Control(const std::string &name, const EasyEngine::Components::Control &control) {
    _name = name;
    _position = control._position;
    _size = control._size;
    _hot_area = control._hot_area;
    _status = control._status;
    _defined_sprite = std::make_shared<Sprite>(
            fmt::format("{}_copy", control._defined_sprite->name()), *control._defined_sprite);
    _defined_sprite->properties()->clip_mode = true;
    for (auto& _con : control._container_list) {
        auto container = std::make_shared<Element>();
        container->type_id = _con.second->type_id;
        if (container->type_id == 1) {
            container->self.sprite = _con.second->self.sprite;
        } else if (container->type_id == 2) {
            container->self.sprite_group = _con.second->self.sprite_group;
        } else if (container->type_id == 3) {
            container->self.frame_animation = _con.second->self.frame_animation;
        } else if (container->type_id == 4) {
            container->self.clip_sprite = {_con.second->self.clip_sprite.pos, _con.second->self.clip_sprite.size};
            // SDL_Log("Copied");
        }
        _container_list.emplace(_con.first, container);
    }
    for (auto& _tri : control._trigger_list) {
        auto trigger = new Trigger();
        trigger->setCondition(_tri.second->condition());
        trigger->setEvent(_tri.second->event());
        trigger->setEnabled(false);
        _trigger_list.emplace(_tri.first, std::unique_ptr<Trigger>(trigger));
    }
    EventSystem::global()->addControl(this);
}

EasyEngine::Components::Control::~Control() {}

void EasyEngine::Components::Control::setName(const std::string &name) {
    _name = name;
}

const std::string &EasyEngine::Components::Control::name() const {
    return _name;
}

void EasyEngine::Components::Control::setVisible(bool visible) {
    _visible = visible;
}

bool EasyEngine::Components::Control::visible() const {
    return _visible;
}

void EasyEngine::Components::Control::setStatus(const EasyEngine::Components::Control::Status &status, Sprite *sprite) {
    auto new_con = std::make_shared<Element>();
    new_con->type_id = 1;
    new_con->self.sprite = std::shared_ptr<Sprite>(sprite);
    _container_list[status] = new_con;
}

void EasyEngine::Components::Control::setStatus(const EasyEngine::Components::Control::Status &status, SpriteGroup *sprite_group) {
    auto new_con = std::make_shared<Element>();
    new_con->type_id = 2;
    new_con->self.sprite_group = std::shared_ptr<SpriteGroup>(sprite_group);
    _container_list[status] = new_con;
}

void EasyEngine::Components::Control::setStatus(const EasyEngine::Components::Control::Status &status, FrameAnimation *frame_animation) {
    auto new_con = std::make_shared<Element>();
    new_con->type_id = 3;
    new_con->self.frame_animation = std::shared_ptr<FrameAnimation>(frame_animation);
    _container_list[status] = new_con;
}

void EasyEngine::Components::Control::setStatus(const EasyEngine::Components::Control::Status &status,
                                                const EasyEngine::GeometryF &clip_sprite) {
    if (_defined_sprite) {
        auto new_con = std::make_shared<Element>();
        new_con->type_id = 4;
        new_con->self.clip_sprite = clip_sprite;
        _container_list[status] = new_con;
    } else {
        SDL_Log("[ERROR] The current control has not clippable sprite!");
    }
}

void EasyEngine::Components::Control::removeStatus(const EasyEngine::Components::Control::Status &status) {
    if (_container_list.contains(status)) {
        _container_list.erase(status);
    }
}

const char * EasyEngine::Components::Control::getTypename(
        const enum Status &status) const {
    if (!_container_list.contains(status)) return "Undefined";
    if (_container_list.at(status)->type_id == 1) return "Sprite";
    if (_container_list.at(status)->type_id == 2) return "SpriteGroup";
    if (_container_list.at(status)->type_id == 3) return "FrameAnimation";
    if (_container_list.at(status)->type_id == 4) return "ClipSprite (GeometryF)";
    return "Unknown";
}

const std::type_info& EasyEngine::Components::Control::typeInfo(const enum Status &status) const {
    if (!_container_list.contains(status)) return typeid(void);
    if (_container_list.at(status)->type_id == 1) return typeid(Sprite);
    if (_container_list.at(status)->type_id == 2) return typeid(SpriteGroup);
    if (_container_list.at(status)->type_id == 3) return typeid(FrameAnimation);
    if (_container_list.at(status)->type_id == 4) return typeid(GeometryF);
    return typeid(void);
}

void EasyEngine::Components::Control::setEvent(const EasyEngine::Components::Control::Event &event, const std::function<void()> &function) {
    if (event == Event::None) {
        SDL_Log("[ERROR] Unable to set up `Event::None` event!");
        return;
    }
    auto trigger = new Trigger();
    trigger->setEvent(function);
    trigger->setCondition([]{ return true; });
    trigger->setEnabled(false);
    _trigger_list.emplace(event, std::unique_ptr<Trigger>(trigger));
}

void EasyEngine::Components::Control::setEvent(const EasyEngine::Components::Control::Event &event, const std::function<bool()> &condition,
                       const std::function<void()> &function) {
    if (event == Event::None) {
        SDL_Log("[ERROR] Unable to set up `Event::None` event!");
        return;
    }
    auto trigger = new Trigger();
    trigger->setEvent(function);
    trigger->setCondition(condition);
    trigger->setEnabled(false);
    _trigger_list.emplace(event, std::unique_ptr<Trigger>(trigger));
}

void EasyEngine::Components::Control::removeEvent(const EasyEngine::Components::Control::Event &event) {
    if (_trigger_list.contains(event)) _trigger_list.erase(event);
}

void EasyEngine::Components::Control::setEnabled(bool enabled) {
    _status = (enabled ? Status::Default : Status::Disabled);
    __updateEvent(Event::EnabledChange);
}

bool EasyEngine::Components::Control::enabled() const {
    return _status != Status::Disabled;
}

void EasyEngine::Components::Control::setActive() {
    _status = Status::Active;
    _active = true;
    __updateEvent(Event::GetFocus);
}

void EasyEngine::Components::Control::setInactive() {
    _status = Status::Default;
    _active = false;
    __updateEvent(Event::LostFocus);
}

bool EasyEngine::Components::Control::active() const {
    return _active;
}

void EasyEngine::Components::Control::move(const Vector2 &pos) {
    _position.reset(pos.x, pos.y);
    _hot_area.pos = _position + _hot_position;
    __updateEvent(Event::Moved);
}

void EasyEngine::Components::Control::move(float x, float y) {
    _position.reset(x, y);
    _hot_area.pos = _position + _hot_position;
    __updateEvent(Event::Moved);
}

EasyEngine::Vector2 EasyEngine::Components::Control::position() const {
    return _position;
}

void EasyEngine::Components::Control::resize(const Size &size) {
    resize(size.width, size.height);
}

void EasyEngine::Components::Control::resize(float width, float height) {
    if (width <= 0 || height <= 0) return;
    _size.reset(width, height);
    float ratioX, ratioY;
    if (_defined_sprite) {
        _defined_sprite->resize(width, height);
    }
    for (auto& con : _container_list) {
        if (con.second->type_id == 1) {
            con.second->self.sprite->resize(width, height);
        } else if (con.second->type_id == 2) {
            con.second->self.sprite_group->resize(width, height);
        } else if (con.second->type_id == 3) {
            auto ani = con.second->self.frame_animation;
            for (auto f = 0; f < ani->framesCount(); ++f) {
                ani->sprite(f)->resize(width, height);
            }
        } else if (con.second->type_id == 4) {
        }
    }
    if (_hot_area.size.width <= 0 || _hot_area.size.height <= 0) return;
    auto [rX, rY] = Size(width, height) / _hot_area.size;
    _hot_area.size.width *= rX;
    _hot_area.size.height *= rY;
    __updateEvent(Event::Resized);
}

EasyEngine::Size EasyEngine::Components::Control::size() const {
    return _size;
}

void EasyEngine::Components::Control::setGeometry(const Vector2 &pos, const Size &size) {
    _position = pos; _size = size;
    __updateEvent(Event::MovedResized);
}

void EasyEngine::Components::Control::setGeometry(float x, float y, float width, float height) {
    _position.reset(x, y);
    _size.reset(width, height);
    _hot_area.pos = _hot_position + _position;
    __updateEvent(Event::MovedResized);
}

void EasyEngine::Components::Control::setGeometryForHotArea(const Vector2 &pos, const Size &size) {
    _hot_position = pos;
    _hot_area.pos = _hot_position + _position;
    _hot_area.size = size;
}

void EasyEngine::Components::Control::setGeometryForHotArea(float x, float y, float width, float height) {
    _hot_position.reset(x, y);
    _hot_area.pos = _hot_position + _position;
    _hot_area.size.reset(width, height);
}

const EasyEngine::Graphics::Rectangle & EasyEngine::Components::Control::hotArea() const {
    return _hot_area;
}

void EasyEngine::Components::Control::update() {
    if (!_container_list.contains(_status)) {
        if (_container_list[Status::Default]->type_id == 1) {
            status<Sprite>(Status::Default)->draw(_position);
        } else if (_container_list[Status::Default]->type_id == 2) {
            status<SpriteGroup>(Status::Default)->draw(_position);
        } else if (_container_list[Status::Default]->type_id == 3) {
            status<FrameAnimation>(Status::Default)->draw(_position);
        } else if (_container_list[Status::Default]->type_id == 4) {
            auto clip = status<GeometryF>(Status::Default);
            _defined_sprite->properties()->clip_pos = clip->pos;
            _defined_sprite->properties()->clip_size = clip->size;
            _defined_sprite->draw(_position);
        }
    } else if (_container_list[_status]->type_id == 1) {
        status<Sprite>(_status)->draw(_position);
    } else if (_container_list[_status]->type_id == 2) {
        status<SpriteGroup>(_status)->draw(_position);
    } else if (_container_list[_status]->type_id == 3) {
        status<FrameAnimation>(_status)->draw(_position);
    } else if (_container_list[_status]->type_id == 4) {
        auto clip = status<GeometryF>(_status);
        _defined_sprite->properties()->clip_pos = clip->pos;
        _defined_sprite->properties()->clip_size = clip->size;
        _defined_sprite->draw(_position);
    }
    if (_event != Event::None && _trigger_list.contains(_event)) {
        _trigger_list.at(_event)->trigger();
        _event = Event::None;
    }
}

void EasyEngine::Components::Control::__updateStatus(const EasyEngine::Components::Control::Status &status) {
    _status = (_container_list.contains(status) ? status : Status::Default);
}

void EasyEngine::Components::Control::__updateEvent(const EasyEngine::Components::Control::Event &event) {
    _event = (_trigger_list.contains(event) ? event : _event);
    // SDL_Log("Updated event: %d", _event);
}

EasyEngine::Components::Control::Status EasyEngine::Components::Control::__currentStatus() const {
    return _status;
}

EasyEngine::Components::Control::Event EasyEngine::Components::Control::__currentEvent() const {
    return _event;
}

EasyEngine::Components::Collider::Collider() : _enabled(false), _geometry(GeometryF()) {}

EasyEngine::Components::Collider::Collider(const EasyEngine::Graphics::Rectangle &rect) : _enabled(false) {
    _geometry.reset(rect.pos, rect.size);
    _con.mode = 1;
    _con.shape.rectangle = rect;
}

EasyEngine::Components::Collider::Collider(const EasyEngine::Graphics::Ellipse &ellipse) : _enabled(false) {
    _geometry.reset(ellipse.pos, ellipse.area);
    _con.mode = 2;
    _con.shape.ellipse = ellipse;
}

EasyEngine::Components::Collider::Collider(const EasyEngine::Graphics::Point &point) : _enabled(false) {
    _geometry.reset(point.pos, {1, 1});
    _con.mode = 3;
    _con.shape.point = point;
}

EasyEngine::Components::Collider::Collider(float x, float y, float width, float height, uint8_t shape_mode) {
    _geometry.reset({x, y}, {width, height});
    _con.mode = shape_mode;
    if (shape_mode == 1) {
        _con.shape.rectangle.pos.reset(x, y);
        _con.shape.rectangle.size.reset(width, height);
    } else if (shape_mode == 2) {
        _con.shape.ellipse.pos.reset(x, y);
        _con.shape.ellipse.area.reset(width, height);
    } else if (shape_mode == 3) {
        _con.shape.point.pos.reset(x, y);
    }
}

int8_t EasyEngine::Components::Collider::check(const EasyEngine::Components::Collider &collider) const {
    if (!_enabled) {
        SDL_Log("[WARNING] The current collider is not be enabled!");
        return 0;
    }
    if (!collider._enabled) {
        SDL_Log("[WARNING] The specified collider is not be enabled!");
        return 0;
    }
    if (_con.mode == 1 && collider._con.mode == 1) {
        return Algorithm::compareRect(_con.shape.rectangle, collider._con.shape.rectangle);
    } else if (_con.mode == 2 && collider._con.mode == 2) {
        return Algorithm::compareEllipse(_con.shape.ellipse, collider._con.shape.ellipse);
    } else if (_con.mode == 1 && collider._con.mode == 2) {
        return Algorithm::compareRectEllipse(_con.shape.rectangle, collider._con.shape.ellipse);
    } else if (_con.mode == 2 && collider._con.mode == 1) {
        return Algorithm::compareRectEllipse(collider._con.shape.rectangle, _con.shape.ellipse);
    } else if (_con.mode == 3 && collider._con.mode == 3) {
        return static_cast<int8_t>(_con.shape.point.pos.isEqual(collider._con.shape.point.pos, 1)) - 1;
    } else if (_con.mode == 3 && collider._con.mode == 1) {
        return Algorithm::comparePosRect(_con.shape.point.pos, collider._con.shape.rectangle);
    } else if (_con.mode == 1 && collider._con.mode == 3) {
        return Algorithm::comparePosRect(collider._con.shape.point.pos, _con.shape.rectangle);
    } else if (_con.mode == 3 && collider._con.mode == 2) {
        return Algorithm::comparePosEllipse(_con.shape.point.pos, collider._con.shape.ellipse);
    } else if (_con.mode == 2 && collider._con.mode == 3) {
        return Algorithm::comparePosEllipse(collider._con.shape.point.pos, _con.shape.ellipse);
    } else {
        SDL_Log("[WARNING] The specified collider is not match by the current collider!");
        return 0;
    }
}

const EasyEngine::GeometryF& EasyEngine::Components::Collider::bounds() const {
    return _geometry;
}

void EasyEngine::Components::Collider::setBoundsGeometry(float x, float y, float width, float height) {
    _geometry.reset(x, y, width, height);
    if (_con.mode == 1) {
        _con.shape.rectangle.pos.reset(x, y);
        _con.shape.rectangle.size.reset(width, height);
    } else if (_con.mode == 2) {
        _con.shape.ellipse.pos.reset(x, y);
        _con.shape.ellipse.area.reset(width, height);
    } else if (_con.mode == 3) {
        _con.shape.point.pos.reset(x, y);
    }
}

void EasyEngine::Components::Collider::setBoundsGeometry(const EasyEngine::Vector2 &position,
                                                         const EasyEngine::Size &size) {
    setBoundsGeometry(position.x, position.y, size.width, size.height);
}

void EasyEngine::Components::Collider::setBoundsGeometry(const EasyEngine::GeometryF &geometry) {
    setBoundsGeometry(geometry.pos.x, geometry.pos.y, geometry.size.width, geometry.size.height);
}

void EasyEngine::Components::Collider::moveBounds(const EasyEngine::Vector2 &position) {
    moveBounds(position.x, position.y);
}

void EasyEngine::Components::Collider::moveBounds(float x, float y) {
    _geometry.pos.x = x;
    _geometry.pos.y = y;
    if (_con.mode == 1) {
        _con.shape.rectangle.pos.reset(x, y);
    } else if (_con.mode == 2) {
        _con.shape.ellipse.pos.reset(x, y);
    } else if (_con.mode == 3) {
        _con.shape.point.pos.reset(x, y);
    }
}

void EasyEngine::Components::Collider::resizeBounds(const EasyEngine::Size &size) {
    resizeBounds(size.width, size.height);
}

void EasyEngine::Components::Collider::resizeBounds(float width, float height) {
    _geometry.size.width = width;
    _geometry.size.height = height;
    if (_con.mode == 1) {
        _con.shape.rectangle.size.reset(width, height);
    } else if (_con.mode == 2) {
        _con.shape.ellipse.area.reset(width, height);
    }
}

void EasyEngine::Components::Collider::setEnabled(bool v) {
    _enabled = v;
}

bool EasyEngine::Components::Collider::enabled() const {
    return _enabled;
}

void EasyEngine::Components::Collider::setSelf(const EasyEngine::Graphics::Rectangle &rect) {
    _con.mode = 1;
    _con.shape.rectangle = rect;
    _geometry.reset(rect.pos, rect.size);

}

void EasyEngine::Components::Collider::setSelf(const EasyEngine::Graphics::Ellipse &ellipse) {
    _con.mode = 2;
    _con.shape.ellipse = ellipse;
    Vector2 real_pos = {ellipse.pos.x + ellipse.area.width / 2, 
                     ellipse.pos.y + ellipse.area.height / 2};
    _geometry.reset(real_pos, ellipse.area);
}

void EasyEngine::Components::Collider::setSelf(const EasyEngine::Graphics::Point &point) {
    _con.mode = 3;
    _con.shape.point = point;
    _geometry.reset(point.pos.x, point.pos.y, 1, 1);
}

bool EasyEngine::Components::Collider::isValid() const {
    return _con.mode != 0;
}

const std::type_info& EasyEngine::Components::Collider::shapeType() const {
    if (_con.mode == 1) {
        return typeid(Graphics::Rectangle);
    } else if (_con.mode == 2) {
        return typeid(Graphics::Ellipse);
    } else if (_con.mode == 3) {
        return typeid(Graphics::Point);
    } else {
        return typeid(void);
    }
}

EasyEngine::Components::Entity::Entity(const std::string &name) : _obj_name(name) {
    _collider = std::make_unique<Collider>();
    _container = std::make_shared<Element>();
}

EasyEngine::Components::Entity::Entity(const std::string &name, const EasyEngine::Components::Sprite &sprite)
    : _obj_name(name) {
    _collider = std::make_unique<Collider>();
    _container = std::make_shared<Element>();
    _container->type_id = 1;
    _container->self.sprite = std::make_shared<Sprite>(sprite.name(), sprite);
}

EasyEngine::Components::Entity::Entity(const std::string &name, const EasyEngine::Components::SpriteGroup &group)
    : _obj_name(name) {
    _collider = std::make_unique<Collider>();
    _container = std::make_shared<Element>();
    _container->type_id = 2;
    _container->self.sprite_group = std::make_shared<SpriteGroup>(group);
}

EasyEngine::Components::Entity::Entity(const std::string &name, const EasyEngine::Components::FrameAnimation &animation)
    : _obj_name(name) {
    _collider = std::make_unique<Collider>();
    _container = std::make_shared<Element>();
    _container->type_id = 3;
    _container->self.frame_animation = std::make_shared<FrameAnimation>(animation);
}

EasyEngine::Components::Entity::Entity(const std::string &name, const EasyEngine::Components::Sprite &sprite,
                                       const EasyEngine::GeometryF &clip)
   : _obj_name(name) {
    _collider = std::make_unique<Collider>();
    _container = std::make_shared<Element>();
    _defined_sprite = std::make_unique<Sprite>(sprite.name(), sprite);
    _container->type_id = 4;
    _container->self.clip_sprite = clip;
}

void EasyEngine::Components::Entity::setName(const std::string &name) {
    _obj_name = name;
}

std::string EasyEngine::Components::Entity::name() const {
    return _obj_name;
}

void EasyEngine::Components::Entity::setVisible(bool visible) {
    _visible = visible;
}

bool EasyEngine::Components::Entity::visible() const {
    return _visible;
}

void EasyEngine::Components::Entity::setColliderSelf(const EasyEngine::Graphics::Rectangle& rect) {
    _collider->setSelf(rect);
    collider()->moveBounds(_pos);
    Size real_size;
    if (_container->type_id == 1) {
        real_size = _container->self.sprite->size();
    } else if (_container->type_id == 2) {
        real_size = _container->self.sprite_group->size();
    } else if (_container->type_id == 3) {
        real_size = _container->self.frame_animation->sprite()->size();
    } else if (_container->type_id == 4 && _defined_sprite) {
        real_size = _defined_sprite->size();
    }
    collider()->resizeBounds(real_size);
    _center_pos.reset(real_size.width / 2, real_size.height / 2);
    collider()->setEnabled(true);
}

void EasyEngine::Components::Entity::setColliderSelf(const EasyEngine::Graphics::Ellipse& ellipse) {
    _collider->setSelf(ellipse);
    Size real_size, mid_circle_size;
    float min;
    if (_container->type_id == 1) {
        real_size = _container->self.sprite->size();
    } else if (_container->type_id == 2) {
        real_size = _container->self.sprite_group->size();
    } else if (_container->type_id == 3) {
        real_size = _container->self.frame_animation->sprite()->size();
    } else if (_container->type_id == 4 && _defined_sprite) {
        real_size = _defined_sprite->size();
    }
    min = std::min(real_size.width, real_size.height);
    collider()->resizeBounds({min, min});
    _center_pos.reset(real_size.width / 2, real_size.height / 2);
    collider()->moveBounds(_pos + _center_pos);
    collider()->setEnabled(true);
}

void EasyEngine::Components::Entity::setColliderSelf(const EasyEngine::Graphics::Point& point) {
    _collider->setSelf(point);
    Size real_size;
    if (_container->type_id == 1) {
        real_size = _container->self.sprite->size();
    } else if (_container->type_id == 2) {
        real_size = _container->self.sprite_group->size();
    } else if (_container->type_id == 3) {
        real_size = _container->self.frame_animation->sprite()->size();
    } else if (_container->type_id == 4 && _defined_sprite) {
        real_size = _defined_sprite->size();
    }
    Size ret_size = real_size / 2;
    _center_pos.reset(ret_size.width, ret_size.height);
    collider()->setBoundsGeometry(ret_size.width, ret_size.height, 1, 1);
    collider()->setEnabled(true);
}

void EasyEngine::Components::Entity::setPosition(EasyEngine::Vector2 pos) {
    setPosition(pos.x, pos.y);
}

void EasyEngine::Components::Entity::setPosition(float x, float y) {
    _pos.reset(x, y);
    if (_collider->isValid()) {
        if (_collider->shapeType() != typeid(Graphics::Rectangle)) {
            _collider->moveBounds(x + _center_pos.x, y + _center_pos.y);
        } else {
            _collider->moveBounds(x, y);
        }
    }
}

void EasyEngine::Components::Entity::setCenterPosition(EasyEngine::Vector2 pos) {
    _center_pos.reset(pos.x, pos.y);
}

void EasyEngine::Components::Entity::setCenterPosition(float x, float y) {
    _center_pos.reset(x, y);
}

EasyEngine::Vector2 EasyEngine::Components::Entity::position() const {
    return _pos;
}

EasyEngine::Vector2 EasyEngine::Components::Entity::centerPosition() const {
    return _center_pos;
}

EasyEngine::Components::Collider *EasyEngine::Components::Entity::collider() const {
    return _collider.get();
}



const std::type_info& EasyEngine::Components::Entity::typeInfo() const {
    if (_container->type_id == 1) {
        return typeid(Sprite);
    } else if (_container->type_id == 2) {
        return typeid(SpriteGroup);
    } else if (_container->type_id == 3) {
        return typeid(FrameAnimation);
    } else if (_container->type_id == 4) {
        return typeid(GeometryF);
    } else {
        return typeid(void);
    }
}

void EasyEngine::Components::Entity::update() const {
    if (!_visible) return;
    if (_container->type_id == 1) {
        _container->self.sprite->draw(_pos);
    } else if (_container->type_id == 2) {
        _container->self.sprite_group->draw(_pos);
    } else if (_container->type_id == 3) {
        _container->self.frame_animation->draw(_pos);
    } else if (_container->type_id == 4) {
        if (_defined_sprite) {
            auto clip = _container->self.clip_sprite;
            _defined_sprite->draw(_pos, clip.pos, clip.size);
        }
    }
}

EasyEngine::Components::Font::Font(const std::string &name, float font_size)
    : _font_size(font_size), _font_direction(LeftToRight), _font_outline(0), _font_style_flags(0) {
    _font = TTF_OpenFont(ResourceSystem::global()->resourcePath(name).c_str(), font_size);
    if (!_font) {
        SDL_Log("[ERROR] Can't load the specified font!\nException: %s", SDL_GetError());
        _font_is_loaded = false;
    } else {
        if (ResourceSystem::global()->isLoaded(name)) {
            ResourceSystem::global()->unload(name);
        }
        _font_is_loaded = true;
    }
}

EasyEngine::Components::Font::~Font() {
    unload();
}

bool EasyEngine::Components::Font::isAvailable() const {
    return _font_is_loaded;
}

bool EasyEngine::Components::Font::load(const std::string &path, float font_size) {
    _font = TTF_OpenFont(path.c_str(), font_size);
    if (!_font) {
        SDL_Log("[ERROR] Can't load the specified font!\nException: %s", SDL_GetError());
        _font_is_loaded = false;
        return false;
    }
    _font_is_loaded = true;
    return true;
}

void EasyEngine::Components::Font::unload() {
    if (_font && _font_is_loaded) {
        TTF_CloseFont(_font);
        _font = nullptr;
        _font_is_loaded = false;
    }
}

void EasyEngine::Components::Font::setFontSize(float size) {
    if (_font_is_loaded) {
        auto ret = TTF_SetFontSize(_font, size);
        if (ret) _font_size = size;
    } else {
        SDL_Log("[WARNING] The current font is not loaded!");
    }
}

float EasyEngine::Components::Font::fontSize() const {
    return _font_size;
}

void EasyEngine::Components::Font::setStyle(uint32_t style) {
    if (_font_is_loaded) {
        TTF_SetFontStyle(_font, style);
        _font_style_flags = style;
    } else {
        SDL_Log("[WARNING] The current font is not loaded!");
    }
}

void EasyEngine::Components::Font::setOutline(uint32_t value) {
    _font_outline = value;
}

uint32_t EasyEngine::Components::Font::outline() const {
    return _font_outline;
}

void EasyEngine::Components::Font::setOutlineColor(const SColor &color) {
    _outline_color = color;
}

const SColor &EasyEngine::Components::Font::outlineColor() const {
    return _outline_color;
}

void EasyEngine::Components::Font::setFontDirection(const EasyEngine::Components::Font::Direction &direction) {
    TTF_SetFontDirection(_font, TTF_Direction(direction));
    _font_direction = direction;
}

const EasyEngine::Components::Font::Direction &EasyEngine::Components::Font::fontDirection() const {
    return _font_direction;
}

void EasyEngine::Components::Font::setFontColor(const SColor &color) {
    _font_color = color;
}

const SColor &EasyEngine::Components::Font::fontColor() const {
    return _font_color;
}

void EasyEngine::Components::Font::setFontHinting(uint32_t hinting) {
    TTF_SetFontHinting(_font, static_cast<TTF_HintingFlags>(hinting));
    _font_hinting = hinting;
}

void EasyEngine::Components::Font::setFontKerning(bool enabled) {
    TTF_SetFontKerning(_font, enabled);
    _font_kerning = enabled;
}

bool EasyEngine::Components::Font::fontKerning() const {
    return _font_kerning;
}

void EasyEngine::Components::Font::setLineSpacing(uint32_t spacing) {
    TTF_SetFontLineSkip(_font, spacing);
    _line_spacing = spacing;
}

uint32_t EasyEngine::Components::Font::lineSpacing() const {
    return _line_spacing;
}

EasyEngine::Components::Sprite *
EasyEngine::Components::Font::textToSprite(const std::string &sprite_name, const std::string &text,
                                           EasyEngine::Painter *painter) {
    SSurface* surface;
    if (!_font_is_loaded) {
        throw std::runtime_error("[FATAL] The current font is not loaded! Please use `isValid()` function at first!");
    }
    if (_font_outline) {
        if (_font_color.a > 0) {
            auto filled_surface = TTF_RenderText_Blended(_font, text.c_str(), 0, _font_color);
            TTF_SetFontOutline(_font, _font_outline);
            auto bordered_surface = TTF_RenderText_Blended(_font, text.c_str(), 0, _outline_color);
            TTF_SetFontOutline(_font, 0);
            int real_width = bordered_surface->w, real_height = bordered_surface->h;
            surface = SDL_CreateSurface(real_width, real_height, bordered_surface->format);

            SDL_FillSurfaceRect(surface, nullptr, SDL_MapSurfaceRGBA(surface, 0, 0, 0, 0));
            SDL_BlitSurface(filled_surface, nullptr, surface, nullptr);
            SDL_BlitSurface(bordered_surface, nullptr, surface, nullptr);

            SDL_DestroySurface(filled_surface);
            SDL_DestroySurface(bordered_surface);
        } else {
            TTF_SetFontOutline(_font, _font_outline);
            surface = TTF_RenderText_Blended(_font, text.c_str(), 0, _outline_color);
            TTF_SetFontOutline(_font, 0);
        }
    } else {
        surface = TTF_RenderText_Blended(_font, text.c_str(), 0, _font_color);
        if (!surface) {
            SDL_Log("[ERROR] Can't draw the current text!\nException: %s", SDL_GetError());
        }
    }
    return new Sprite(sprite_name, surface, painter);
}

TTF_Font *EasyEngine::Components::Font::TTF_font() const {
    return _font;
}

