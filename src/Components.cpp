
#include "Components.h"


EasyEngine::Components::Spirit::Spirit(const std::string &name, SRenderer *renderer)
    : _name(name), _renderer(renderer), _size(0, 0) {
    if (!_renderer) {
        SDL_Log("[ERROR] Current renderer is not valid!");
        return;
    }
    _surface = SDL_CreateSurface(0, 0, SDL_PIXELFORMAT_RGBA64);
    _texture = SDL_CreateTextureFromSurface(renderer, _surface);
}

EasyEngine::Components::Spirit::Spirit(const std::string &name, const EasyEngine::Components::Spirit &spirit)
    : _name(name), _renderer(spirit._renderer), _size(spirit._size) {
    if (!_renderer) {
        SDL_Log("[ERROR] Current renderer is not valid!");
        return;
    }
    _surface = SDL_DuplicateSurface(spirit._surface);
    _texture = SDL_CreateTextureFromSurface(_renderer, _surface);
}

EasyEngine::Components::Spirit::Spirit(const std::string &name, const EasyEngine::Components::Spirit &spirit,
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
}

EasyEngine::Components::Spirit::Spirit(const std::string &name, const std::string &path, SRenderer *renderer)
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
    _size.reset(_surface->w, _surface->h);
    _texture = SDL_CreateTextureFromSurface(_renderer, _surface);
}

EasyEngine::Components::Spirit::Spirit(const std::string &name, const std::string &path,
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
}

EasyEngine::Components::Spirit::~Spirit() {
    if (_texture) SDL_DestroyTexture(_texture);
    if (_surface) SDL_DestroySurface(_surface);
}

void EasyEngine::Components::Spirit::setName(const std::string &new_name) {
    _name = new_name;
}

std::string EasyEngine::Components::Spirit::name() const {
    return _name;
}

bool EasyEngine::Components::Spirit::setPath(const std::string &new_path) {
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

std::string EasyEngine::Components::Spirit::path() const {
    return _path;
}

bool EasyEngine::Components::Spirit::isValid(const std::string &path) const {
    if (!_renderer) return false;
    if (!_surface) return false;
    if (!_texture) return false;
    return true;
}

void EasyEngine::Components::Spirit::setRenderer(SRenderer *renderer) {
    if (!_renderer) {
        SDL_Log("[ERROR] The specified renderer is not valid!");
        return;
    }
    _renderer = renderer;
}

const SRenderer *EasyEngine::Components::Spirit::renderer() const {
    return _renderer;
}

void EasyEngine::Components::Spirit::resize(float width, float height) {
    _size.reset(width, height);
}

EasyEngine::Size EasyEngine::Components::Spirit::size() const {
    return _size;
}

STexture *EasyEngine::Components::Spirit::spirit() const {
    return _texture;
}

void EasyEngine::Components::Spirit::draw(const Vector2 &pos, Painter *painter) const {
    painter->drawSpirit(*this, pos);
}

void
EasyEngine::Components::Spirit::draw(const Vector2 &pos, float scaled, Painter *painter, const Vector2 &center) const {
    Properties properties;
    properties.position = pos;
    properties.scaled = scaled;
    properties.scaled_center = center;
    painter->drawSpirit(*this, properties);
}

void EasyEngine::Components::Spirit::draw(const Vector2 &pos, const Vector2 &clip_pos, const Size &clip_size,
                                          Painter *painter) const {
    Properties properties;
    properties.position = pos;
    properties.clip_pos = clip_pos;
    properties.clip_size = clip_size;
    painter->drawSpirit(*this, properties);
}

void EasyEngine::Components::Spirit::draw(const Vector2 &pos, double rotate, Painter *painter, const FlipMode &flipMode,
                                          const Vector2 &rotate_center) const {
    Properties properties;
    properties.position = pos;
    properties.rotate = rotate;
    properties.flip_mode = flipMode;
    properties.rotate_center = rotate_center;
    painter->drawSpirit(*this, properties);
}

void EasyEngine::Components::Spirit::draw(const EasyEngine::Vector2 &pos, const SColor &color_alpha,
                                          Painter *painter) const {
    Properties properties;
    properties.position = pos;
    properties.color_alpha = color_alpha;
    painter->drawSpirit(*this, properties);
}

void
EasyEngine::Components::Spirit::draw(const EasyEngine::Components::Spirit::Properties &properties,
                                     Painter *painter) const {
    painter->drawSpirit(*this, properties);
}


