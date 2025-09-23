
#include "Scene.h"

using namespace EasyEngine;

Components::Layer::Layer(const std::string &name) : _name(name) {}

Components::Layer::~Layer() {
}

void Components::Layer::setName(const std::string &name) { _name = name; }

const std::string &Components::Layer::name() const { return _name; }

bool Components::Layer::append(uint32_t z_order, Components::Sprite *sprite) {
    if (!checkAppendError(z_order)) return false;
    if (!sprite) {
        SDL_Log("[ERROR] The specified sprite is not valid!");
        return false;
    }
    auto idx = indexOf(sprite->name());
    if (idx > 0) {
        SDL_Log("[ERROR] The name of the specified sprite is already exist!\n"
                "Exception: Name '%s' is located at index %u.", sprite->name().c_str(), idx);
        return false;
    }
    _elements.emplace(z_order, std::shared_ptr<Sprite>(sprite));
    return true;
}

bool Components::Layer::append(uint32_t z_order, Components::SpriteGroup *group) {
    if (!checkAppendError(z_order)) return false;
    if (!group) {
        SDL_Log("[ERROR] The specified sprite group is not valid!");
        return false;
    }
    auto idx = indexOf(group->name());
    if (idx > 0) {
        SDL_Log("[ERROR] The name of the specified sprite is already exist!\n"
                "Exception: Name '%s' is located at index %u.", group->name().c_str(), idx);
        return false;
    }
    _elements.emplace(z_order, std::shared_ptr<SpriteGroup>(group));
    return true;
}

bool Components::Layer::append(uint32_t z_order, Components::Animation *animation) {
    if (!checkAppendError(z_order)) return false;
    if (!animation) {
        SDL_Log("[ERROR] The specified animation is not valid!");
        return false;
    }
    auto idx = indexOf(animation->name());
    if (idx > 0) {
        SDL_Log("[ERROR] The name of the specified sprite is already exist!\n"
                "Exception: Name '%s' is located at index %u.", animation->name().c_str(), idx);
        return false;
    }
    _elements.emplace(z_order, std::shared_ptr<Animation>(animation));
    return true;
}

bool Components::Layer::append(uint32_t z_order, Components::Entity *entity) {
    if (!checkAppendError(z_order)) return false;
    if (!entity) {
        SDL_Log("[ERROR] The specified entity is not valid!");
        return false;
    }
    auto idx = indexOf(entity->name());
    if (idx > 0) {
        SDL_Log("[ERROR] The name of the specified sprite is already exist!\n"
                "Exception: Name '%s' is located at index %u.", entity->name().c_str(), idx);
        return false;
    }
    _elements.emplace(z_order, std::shared_ptr<Entity>(entity));
    return true;
}

bool Components::Layer::append(uint32_t z_order, Components::Control *control) {
    if (!checkAppendError(z_order)) return false;
    if (!control) {
        SDL_Log("[ERROR] The specified sprite is not valid!");
        return false;
    }
    auto idx = indexOf(control->name());
    if (idx > 0) {
        SDL_Log("[ERROR] The name of the specified sprite is already exist!\n"
                "Exception: Name '%s' is located at index %u.", control->name().c_str(), idx);
        return false;
    }
    _elements.emplace(z_order, std::shared_ptr<Control>(control));
    return true;
}

void Components::Layer::remove(uint32_t z_order) {
    if (_elements.contains(z_order)) {
        _elements.erase(z_order);
    } else {
        SDL_Log("[ERROR] The specified z_order is not found!");
    }
}

Components::Sprite *Components::Layer::sprite(uint32_t z_order) const {
    if (!_elements.contains(z_order)) {
        SDL_Log("[ERROR] The specified z_order %u is not found!", z_order);
        return nullptr;
    }
    try {
        auto ret = std::get<std::shared_ptr<Sprite>>(_elements.at(z_order));
        return ret.get();
    } catch (const std::exception &e) {
        SDL_Log("[ERROR] The z_order %u in layer '%s' is not the sprite!", z_order, _name.c_str());
        throw std::runtime_error(fmt::format("[FATAL] The z_order {} in layer '{}' is not the sprite!",
                                             z_order, _name));
    }
}

Components::SpriteGroup *Components::Layer::spriteGroup(uint32_t z_order) const {
    if (!_elements.contains(z_order)) {
        SDL_Log("[ERROR] The specified z_order is not found!");
        return nullptr;
    }
    try {
        auto ret = std::get<std::shared_ptr<SpriteGroup>>(_elements.at(z_order));
        return ret.get();
    } catch (const std::exception &e) {
        SDL_Log("[ERROR] The z_order %u in layer '%s' is not the sprite group!", z_order, _name.c_str());
        throw std::runtime_error(fmt::format("[FATAL] The z_order {} in layer '{}' is not the sprite group!",
                                             z_order, _name));
    }
}

Components::Animation *Components::Layer::animation(uint32_t z_order) const {
    if (!_elements.contains(z_order)) {
        SDL_Log("[ERROR] The specified z_order is not found!");
        return nullptr;
    }
    try {
        auto ret = std::get<std::shared_ptr<Animation>>(_elements.at(z_order));
        return ret.get();
    } catch (const std::exception &e) {
        SDL_Log("[ERROR] The z_order %u in layer '%s' is not the animation!", z_order, _name.c_str());
        throw std::runtime_error(fmt::format("[FATAL] The z_order {} in layer '{}' is not the animation!",
                                             z_order, _name));
    }
}

Components::Entity *Components::Layer::entity(uint32_t z_order) const {
    if (!_elements.contains(z_order)) {
        SDL_Log("[ERROR] The specified z_order is not found!");
        return nullptr;
    }
    try {
        auto ret = std::get<std::shared_ptr<Entity>>(_elements.at(z_order));
        return ret.get();
    } catch (const std::exception &e) {
        SDL_Log("[ERROR] The z_order %u in layer '%s' is not the entity!", z_order, _name.c_str());
        throw std::runtime_error(fmt::format("[FATAL] The z_order {} in layer '{}' is not the entity!",
                                             z_order, _name));
    }
}

Components::Control *Components::Layer::control(uint32_t z_order) const {
    if (!_elements.contains(z_order)) {
        SDL_Log("[ERROR] The specified z_order is not found!");
        return nullptr;
    }
    try {
        auto ret = std::get<std::shared_ptr<Control>>(_elements.at(z_order));
        return ret.get();
    } catch (const std::exception &e) {
        SDL_Log("[ERROR] The z_order %u in layer '%s' is not the control!", z_order, _name.c_str());
        throw std::runtime_error(fmt::format("[FATAL] The z_order {} in layer '{}' is not the control!",
                                             z_order, _name));
    }
}

uint32_t Components::Layer::indexOf(const std::string &name, uint32_t start_of, uint32_t end_of) const {
    for (auto& _ele : _elements) {
        if (_ele.first < start_of) continue;
        if (_ele.first > end_of) break;
        bool name_matched = std::visit(
            [&name](const auto& element) -> bool {
                if (element && element->name() == name) {
                    return true;
                }
                return false;
            },_ele.second);
        if (name_matched) {
            return _ele.first;
        }
    }
    return 0;
}

void Components::Layer::swap(uint32_t z_order1, uint32_t z_order2) {
    auto temp = _elements.at(z_order1);
    _elements.at(z_order1) = _elements.at(z_order2);
    _elements.at(z_order2) = temp;
}

bool Components::Layer::setZOrder(uint32_t old_z_order, uint32_t new_z_order) {
    if (!_elements.contains(old_z_order)) {
        SDL_Log("[ERROR] The specified old_z_order %u is not found!", old_z_order);
        return false;
    }
    if (_elements.contains(new_z_order)) {
        SDL_Log("[ERROR] The specified new_z_order %u is already exist!", old_z_order);
        return false;
    }
    _elements.emplace(new_z_order, _elements.at(old_z_order));
    _elements.erase(old_z_order);
    return true;
}

void Components::Layer::setVisible(bool visible) {
    _visible = visible;
}

bool Components::Layer::visible() const {
    return _visible;
}

bool Components::Layer::checkAppendError(uint32_t z_order) {
    if (_elements.contains(z_order)) {
        SDL_Log("[ERROR] The specified z_order is already appended!");
        return false;
    }
    if (z_order == 0) {
        SDL_Log("[ERROR] The specified z_order can not equal 0!");
        return false;
    }
    return true;
}

void Components::Layer::update() {
    if (!_visible) return;
    for (auto& _ele : _elements) {
        auto _idx = _ele.second.index();
        if (_idx == 0) {
            Sprite* t = std::get<0>(_ele.second).get();
            t->draw();
        } else if (_idx == 1) {
            SpriteGroup* t = std::get<1>(_ele.second).get();
            t->draw();
        } else if (_idx == 2) {
            Animation* t = std::get<2>(_ele.second).get();
            t->draw();
        } else if (_idx == 3) {
            Entity* t = std::get<3>(_ele.second).get();
            t->update();
        } else if (_idx == 4) {
            Control* t = std::get<4>(_ele.second).get();
            t->update();
        } else {
            SDL_Log("Unknown!");
        }
    }
}

void Components::Layer::setViewport(const Geometry &geometry) {
    _viewport_geometry.setGeometry(geometry.x, geometry.y, geometry.width, geometry.height);
}

void Components::Layer::setViewport(int x, int y, int w, int h) {
    _viewport_geometry.setGeometry(x, y, w, h);
}

void Components::Layer::setViewportScaled(float width, float height) {
    _scaled.reset(width, height);
}

void Components::Layer::setViewportScaled(const Size &scaled) {
    _scaled.reset(scaled.width, scaled.height);
}

void Components::Layer::setClipViewport(const Geometry &geometry) {
    _clipViewport_geometry.setGeometry(geometry.x, geometry.y, geometry.width, geometry.height);
}

void Components::Layer::setClipViewport(int x, int y, int w, int h) {
    _clipViewport_geometry.setGeometry(x, y, w, h);
}

void Components::Layer::setViewportPainter(Painter *painter) {
    _painter = painter;
}

void Components::Layer::draw(bool viewport_mode, bool clip_mode) {
    if (_painter) {
        if (viewport_mode) {
            _painter->setViewport(_viewport_geometry, _scaled);
        }
        if (clip_mode) {
            _painter->setClipView(_clipViewport_geometry);
        }
    }
    update();
}

const Geometry &Components::Layer::viewport() const {
    return _viewport_geometry;
}

const Size &Components::Layer::viewportScaled() const {
    return _scaled;
}

const Geometry &Components::Layer::clipViewport() const {
    return _clipViewport_geometry;
}

const Painter *Components::Layer::viewportPainter() const {
    return _painter;
}

