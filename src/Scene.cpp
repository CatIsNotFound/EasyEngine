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

bool Components::Layer::append(uint32_t z_order, Components::FrameAnimation *animation) {
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
    _elements.emplace(z_order, std::shared_ptr<FrameAnimation>(animation));
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
        throw std::runtime_error(fmt::format(
            "[FATAL] The z_order {} in layer '{}' does not contain a Sprite object!\n"
            "Possible reasons: the element at this z_order is of a different type, "
            "or the stored pointer is invalid. Please check the type of the element stored at this z_order.",
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
        throw std::runtime_error(fmt::format(
            "[FATAL] The z_order {} in layer '{}' does not contain a SpriteGroup object!\n"
            "Possible reasons: the element at this z_order is of a different type, "
            "or the stored pointer is invalid. Please check the type of the element stored at this z_order.",
            z_order, _name));
    }
}

Components::FrameAnimation *Components::Layer::animation(uint32_t z_order) const {
    if (!_elements.contains(z_order)) {
        SDL_Log("[ERROR] The specified z_order is not found!");
        return nullptr;
    }
    try {
        auto ret = std::get<std::shared_ptr<FrameAnimation>>(_elements.at(z_order));
        return ret.get();
    } catch (const std::exception &e) {
        SDL_Log("[ERROR] The z_order %u in layer '%s' is not the animation!", z_order, _name.c_str());
        throw std::runtime_error(fmt::format(
            "[FATAL] The z_order {} in layer '{}' does not contain an Animation object!\n"
            "Possible reasons: the element at this z_order is of a different type, "
            "or the stored pointer is invalid. Please check the type of the element stored at this z_order.",
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
        throw std::runtime_error(fmt::format(
            "[FATAL] The z_order {} in layer '{}' does not contain an Entity object!\n"
            "Possible reasons: the element at this z_order is of a different type, "
            "or the stored pointer is invalid. Please check the type of the element stored at this z_order.",
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
        throw std::runtime_error(fmt::format(
            "[FATAL] The z_order {} in layer '{}' does not contain a Control object!\n"
            "Possible reasons: the element at this z_order is of a different type, "
            "or the stored pointer is invalid. Please check the type of the element stored at this z_order.",
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

bool Components::Layer::swap(uint32_t z_order1, uint32_t z_order2) {
    if (!_elements.contains(z_order1) || !_elements.contains(z_order2)) {
        SDL_Log("[ERROR] Swap failed! One of the specified z_order is not valid! ");
        return false;
    }
    auto temp = _elements.at(z_order1);
    _elements.at(z_order1) = _elements.at(z_order2);
    _elements.at(z_order2) = temp;
    return true;
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
            FrameAnimation* t = std::get<2>(_ele.second).get();
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
        if (viewport_mode) _painter->setViewport(_viewport_geometry, _scaled);
        if (clip_mode) _painter->setClipView(_clipViewport_geometry);
        update();
        if (viewport_mode) _painter->setViewport({0, 0, 0, 0});
        if (clip_mode) _painter->setClipView({0, 0, 0, 0});
        return;
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

Components::Scene::Scene(const std::string &name) : _name(name), _event() {}

void Components::Scene::setName(const std::string &name) {
    _name = name;
}

const std::string &Components::Scene::name() const {
    return _name;
}

bool Components::Scene::appendLayer(uint32_t z_order, const std::string &name) {
    if (_layers.contains(z_order)) {
        SDL_Log("[ERROR] The specified z_order is already exist!");
        return false;
    }
    auto idx = indexOf(name);
    if (idx > 0) {
        SDL_Log("[ERROR] The specified layer '%s' is in index %u (already exist)!", name.c_str(), idx);
        return false;
    }
    _layers.emplace(z_order, std::make_shared<Layer>(name));
    _layers.at(z_order)->_z_order = z_order;
    _layers_find_string_map.emplace(_layers.at(z_order)->name(), _layers.at(z_order));
    return true;
}

bool Components::Scene::appendLayer(uint32_t z_order, Components::Layer *layer) {
    if (_layers.contains(z_order)) {
        SDL_Log("[ERROR] The specified z_order is already exist!");
        return false;
    }
    auto idx = indexOf(layer);
    if (idx > 0) {
        SDL_Log("[ERROR] The specified layer '%s' is in index %u (already exist)!", layer->name().c_str(), idx);
        return false;
    }
    layer->_z_order = z_order;
    _layers.emplace(z_order, std::shared_ptr<Layer>(layer));
    _layers_find_string_map.emplace(layer->name(), _layers.at(z_order));
    return true;
}

bool Components::Scene::removeLayer(uint32_t z_order) {
    if (!_layers.contains(z_order)) {
        SDL_Log("[ERROR] The specified z_order is not exist!");
        return false;
    }
    _layers.erase(z_order);
    return true;
}

uint32_t Components::Scene::indexOf(const std::string &layer_name) const {
    if (!_layers_find_string_map.contains(layer_name)) return 0;
    return _layers_find_string_map.at(layer_name)->_z_order;
}

uint32_t Components::Scene::indexOf(const Components::Layer *layer) const {
    for (auto& _one : _layers) {
        if (_one.second.get() == layer) {
            return _one.first;
        }
    }
    return 0;
}

Components::Layer *Components::Scene::layer(const std::string &layer_name) const {
    if (!_layers_find_string_map.contains(layer_name)) {
        SDL_Log("[ERROR] The specified layer '%s' is not found!", layer_name.c_str());
        return nullptr;
    }
    return _layers_find_string_map.at(layer_name).get();
}

Components::Layer *Components::Scene::layer(uint32_t z_order) const {
    if (!_layers.contains(z_order)) {
        SDL_Log("[ERROR] The specified z_order is not exist!");
        return nullptr;
    }
    return _layers.at(z_order).get();
}

bool Components::Scene::swapLayer(uint32_t z_order1, uint32_t z_order2) {
    if (!_layers.contains(z_order1) || !_layers.contains(z_order2)) {
        SDL_Log("[ERROR] The specified z_order1 or z_order2 is not exist!");
        return false;
    }
    auto _tmp = _layers.at(z_order1);
    _layers.at(z_order1) = _layers.at(z_order2);
    _layers.at(z_order2) = _tmp;
    return true;
}

bool Components::Scene::swapLayer(const std::string &layer_name1, const std::string &layer_name2) {
    if (!_layers_find_string_map.contains(layer_name1)) {
        SDL_Log("[ERROR] The specified layer '%s' is not found!", layer_name1.c_str());
        return false;
    }
    if (!_layers_find_string_map.contains(layer_name2)) {
        SDL_Log("[ERROR] The specified layer '%s' is not found!", layer_name1.c_str());
        return false;
    }
    auto layer1_zorder = _layers_find_string_map.at(layer_name1)->_z_order;
    auto layer2_zorder = _layers_find_string_map.at(layer_name2)->_z_order;

    auto _tmp = _layers.at(layer1_zorder);
    _layers.at(layer1_zorder) = _layers.at(layer2_zorder);
    _layers.at(layer2_zorder) = _tmp;
    return true;
}

bool Components::Scene::setZOrder(uint32_t old_z_order, uint32_t new_z_order) {
    if (!_layers.contains(old_z_order)) {
        SDL_Log("[ERROR] The specified old_z_order is not found!");
        return false;
    }
    if (_layers.contains(new_z_order)) {
        SDL_Log("[ERROR] The specified new_z_order is already exist! Please use `swap()` function!");
        return false;
    }
    _layers.emplace(new_z_order, _layers.at(old_z_order));
    _layers.erase(old_z_order);
    return true;
}

bool Components::Scene::setZOrder(const std::string &layer_name, uint32_t new_z_order) {
    auto idx = indexOf(layer_name);
    if (!_layers_find_string_map.contains(layer_name)) {
        SDL_Log("[ERROR] The specified layer '%s' is not found!", layer_name.c_str());
        return false;
    }
    if (_layers.contains(new_z_order)) {
        SDL_Log("[ERROR] The specified new_z_order is already exist!");
        return false;
    }
    auto& layer = _layers_find_string_map.at(layer_name);
    _layers.emplace(new_z_order, layer);
    _layers.erase(layer->_z_order);
    layer->_z_order = new_z_order;
    return true;
}

void Components::Scene::setSceneEvent(const std::function<void()> &event) {
    _event = event;
}

bool Components::Scene::renameLayer(const std::string &layer_name, const std::string &new_layer_name) {
    if (!_layers_find_string_map.contains(layer_name)) {
        SDL_Log("[ERROR] Can't find the specified layer '%s'.", layer_name.c_str());
        return false;
    }
    if (_layers_find_string_map.contains(new_layer_name)) {
        SDL_Log("[ERROR] Can't rename the specified layer '%s' to new name '%s'.\n"
                "Exception: The new layer name is already exist at index %u.",
                layer_name.c_str(), new_layer_name.c_str(), _layers_find_string_map.at(layer_name)->_z_order);
        return false;
    }
    _layers_find_string_map.emplace(new_layer_name, _layers_find_string_map.at(layer_name));
    _layers_find_string_map.at(layer_name)->setName(new_layer_name);
    _layers_find_string_map.erase(layer_name);
    return true;
}

bool Components::Scene::renameLayer(uint32_t z_order, const std::string &new_layer_name) {
    if (!_layers.contains(z_order)) {
        SDL_Log("[ERROR] Can't find the specified z_order!");
        return false;
    }
    auto& layer = _layers.at(z_order);
    if (_layers_find_string_map.contains(new_layer_name)) {
        SDL_Log("[ERROR] Can't rename the specified layer '%s' to new name '%s'.\n"
                "Exception: The new layer name is already exist at index %u.",
                layer->name().c_str(), new_layer_name.c_str(), z_order);
        return false;
    }
    _layers_find_string_map.emplace(new_layer_name, layer);
    _layers_find_string_map.erase(layer->name());
    layer->setName(new_layer_name);
    return true;
}

void Components::Scene::drawLayers() {
    if (_event) _event();
    for (auto& _layer : _layers) {
        _layer.second->draw(true, true);
    }
}

SceneManager::SceneManager() : _leave_delayer(new Components::Timer()) {
    EventSystem::global()->addSceneManager(this);
};

bool SceneManager::append(Components::Scene *scene, uint32_t index) {
    if (_scenes.contains(index)) {
        SDL_Log("[ERROR] The specified index is already appended!");
        return false;
    }
    auto _idx = indexOf(scene);
    if (_idx > 0) {
        SDL_Log("[ERROR] The specified scene '%s' is already appended!", scene->name().c_str());
        return false;
    }
//    if (_scenes.empty()) {
//        _current_changer = _new_changer = index;
//    }
    _scenes.emplace(index, Property(std::shared_ptr<Components::Scene>(scene)));
    return true;
}

bool SceneManager::remove(uint32_t index) {
    if (!_scenes.contains(index)) {
        SDL_Log("[ERROR] The specified index is not exist!");
        return false;
    }
    _scenes.erase(index);
    return true;
}

bool SceneManager::setEnterSceneEvent(uint32_t index, const std::function<void()> &event) {
    if (!_scenes.contains(index)) {
        SDL_Log("[ERROR] The specified index is not exist!");
        return false;
    }
    _scenes.at(index).enter_scene_event = event;
    return true;
}

bool SceneManager::removeEnterSceneEvent(uint32_t index) {
    if (!_scenes.contains(index)) {
        SDL_Log("[ERROR] The specified index is not exist!");
        return false;
    }
    _scenes.at(index).enter_scene_event = {};
    return true;
}


bool SceneManager::setLeaveSceneEvent(uint32_t index, uint32_t delay_to_change, const std::function<void()> &event) {
    if (!_scenes.contains(index)) {
        SDL_Log("[ERROR] The specified index is not exist!");
        return false;
    }
    _scenes.at(index).leave_delay = delay_to_change;
    _scenes.at(index).leave_scene_event = event;
    return true;
}

bool SceneManager::removeLeaveSceneEvent(uint32_t index) {
    if (!_scenes.contains(index)) {
        SDL_Log("[ERROR] The specified index is not exist!");
        return false;
    }
    _scenes.at(index).leave_scene_event = {};
    return true;
}

void SceneManager::changeScene(uint32_t index) {
    if (index == 0 || !_scenes.contains(index)) {
        SDL_Log("[ERROR] The specified index is not exist!");
        return;
    }
    if (index == _current_changer || index == _new_changer) {
        SDL_Log("[ERROR] The specified index is already changing!");
        return;
    }
    if (!EventSystem::global()->handlerEnabled()) {
        SDL_Log("[ERROR] The event system is not enabled! Can not change scene now!");
        return;
    }
    _new_changer = index;
    if (_scenes.contains(_current_changer)) {
        if (_scenes.at(_current_changer).leave_scene_event) {
            _scenes.at(_current_changer).leave_scene_event();
            _leave_delayer->setDelay(_scenes.at(_current_changer).leave_delay);
            _leave_delayer->setEvent([&] {
                auto &new_scene = _scenes.at(_new_changer);
                if (new_scene.enter_scene_event) new_scene.enter_scene_event();
                _current_changer = _new_changer;
            });
            _leave_delayer->start();
        } else {
            _current_changer = _new_changer;
            auto &cur_scene = _scenes.at(_current_changer);
            if (cur_scene.enter_scene_event) cur_scene.enter_scene_event();
        }
    } else if (_scenes.contains(_new_changer)) {
        auto& scene = _scenes.at(_new_changer);
        _current_changer = _new_changer;
        if (scene.enter_scene_event) {
            scene.enter_scene_event();
        }
    }
}

uint32_t SceneManager::indexOf(const Components::Scene *scene) const {
    for (auto& _scene : _scenes) {
        if (_scene.second.scene.get() == scene) return _scene.first;
    }
    return 0;
}

uint32_t SceneManager::indexOf(const std::string &scene_name) const {
    for (auto& _scene : _scenes) {
        if (_scene.second.scene->name() == scene_name) return _scene.first;
    }
    return 0;
}

uint32_t SceneManager::currentSceneIndex() const {
    return _current_changer;
}

Components::Scene *SceneManager::scene(uint64_t index) const {
    if (!_scenes.contains(index)) {
        SDL_Log("[ERROR] The specified index is not exist!");
        return nullptr;
    } else {
        return _scenes.at(index).scene.get();
    }
}

Components::Scene *SceneManager::currentScene() const {
    if (_scenes.contains(_current_changer))
        return _scenes.at(_current_changer).scene.get();
    else
        return nullptr;
}

