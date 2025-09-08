
#include "Components.h"

EasyEngine::Components::BGM::BGM() {}

EasyEngine::Components::BGM::~BGM() {}

EasyEngine::Components::BGM::BGM(const std::string &path) : _path(path) {
    auto ret = AudioSystem::instance()->loadBGM(*this);
    _is_load = (ret != -1);
    _channel = static_cast<uint8_t>(ret);
    SDL_Log("%s: CH %d", (_is_load ? "Yes" : "No"), _channel);
}

void EasyEngine::Components::BGM::setPath(const std::string &path) {
    _path = path;
    auto ret = AudioSystem::instance()->loadBGM(*this);
    _is_load = (ret != -1);
    _channel = static_cast<uint8_t>(ret);
    SDL_Log("%s: CH %d", (_is_load ? "Yes" : "No"), _channel);
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
        // TODO: 循环播放 SFX（通过事件系统不断调用）

    }
}

void EasyEngine::Components::SFX::stop() {
    if (_is_load)
        AudioSystem::instance()->stopSFX(_channel);
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
    Vector2 new_pos = pos + _properties->position;
    painter->drawSprite(*this, new_pos);
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


