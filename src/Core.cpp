
#include "Core.h"

#include <memory>

SDL_WindowID EasyEngine::Engine::_main_window_id = 0;
bool EasyEngine::Engine::_is_stopped = false;
std::function<bool(SEvent)> EasyEngine::EventSystem::_my_event_handler = nullptr;
std::unique_ptr<EasyEngine::AudioSystem> EasyEngine::AudioSystem::_instance = nullptr;
std::unique_ptr<EasyEngine::EventSystem> EasyEngine::EventSystem::_instance = nullptr;
std::unique_ptr<EasyEngine::Cursor> EasyEngine::Cursor::_instance = nullptr;

EasyEngine::Cursor::Cursor() {
    _cursor = SDL_CreateSystemCursor(SStdCursor(0));
    SDL_SetCursor(_cursor);
}

EasyEngine::Cursor::~Cursor() = default;

EasyEngine::Cursor *EasyEngine::Cursor::global() {
    if (!_instance) {
        _instance = std::unique_ptr<Cursor>(new Cursor());
    }
    return _instance.get();
}

EasyEngine::Vector2 EasyEngine::Cursor::globalPosition() const {
    Vector2 temp_pos;
    SDL_GetGlobalMouseState(&temp_pos.x, &temp_pos.y);
    return temp_pos;
}

EasyEngine::Vector2 EasyEngine::Cursor::position() {
    Vector2 temp_pos;
    SDL_GetMouseState(&temp_pos.x, &temp_pos.y);
    return temp_pos;
}

void EasyEngine::Cursor::move(const EasyEngine::Vector2 &pos, const EasyEngine::Window *window) {
    if (window) {
        SDL_WarpMouseInWindow(window->window, pos.x, pos.y);
    } else {
        SDL_WarpMouseGlobal(pos.x, pos.y);
    }
}

void EasyEngine::Cursor::move(float x, float y, const EasyEngine::Window *window) {
    if (window) {
        SDL_WarpMouseInWindow(window->window, x, y);
    } else {
        SDL_WarpMouseGlobal(x, y);
    }
}

void EasyEngine::Cursor::moveToCenter(const EasyEngine::Window *window) {
    Vector2 pos;
    if (window) {
        int w, h;
        SDL_GetWindowSize(window->window, &w, &h);
        pos.reset((float)w / 2, (float)h / 2);
        SDL_WarpMouseInWindow(window->window, pos.x, pos.y);
    } else {
        SDL_Rect rect;
        SDL_GetDisplayBounds(SDL_GetPrimaryDisplay(), &rect);
        pos.reset((float)rect.w / 2, (float)rect.h / 2);
        SDL_WarpMouseGlobal(pos.x, pos.y);
    }
}

void EasyEngine::Cursor::setCursor(const EasyEngine::Cursor::StdCursor &cursor) {
    if (cursor == _std_cursor) return;
    if (_custom_cursor && _user_custom.contains(cursor)) {
        auto user_cursor = _user_custom.at(cursor);
        if (user_cursor.cursor) {
            SDL_DestroyCursor(_cursor);
            _cursor = SDL_CreateColorCursor(user_cursor.cursor.get(),
                                            user_cursor.hot_point.x, user_cursor.hot_point.y);
            SDL_SetCursor(_cursor);
            return;
        }
    }
    _std_cursor = cursor;
    SDL_DestroyCursor(_cursor);
    if (_surface) {
        SDL_DestroySurface(_surface);
        _surface = nullptr;
    }
    _cursor = SDL_CreateSystemCursor(SStdCursor(_std_cursor));
    SDL_SetCursor(_cursor);
}

void EasyEngine::Cursor::setCursor(const std::string &path, int hot_x, int hot_y) {
    SSurface *temp = IMG_Load(path.data());
    if (!temp) {
        SDL_Log("[ERROR] Can't load image while setting cursor: %s", path.data());
        return;
    }
    SDL_DestroyCursor(_cursor);
    if (_surface) {
        SDL_DestroySurface(_surface);
    }
    _surface = temp;
    _std_cursor = Custom;
    _cursor = SDL_CreateColorCursor(_surface, hot_x, hot_y);
    SDL_SetCursor(_cursor);
}

void EasyEngine::Cursor::setUserCustomEnabled(bool enabled) {
    _custom_cursor = enabled;
}

bool EasyEngine::Cursor::userCustomEnabled() const { return _custom_cursor; }

void EasyEngine::Cursor::setCustomCursor(const EasyEngine::Cursor::StdCursor &stdCursor, const std::string &path,
                                         const UserCustom::HotPoint &hot_point) {
    auto new_cursor = IMG_Load(path.c_str());
    if (!new_cursor) {
        SDL_Log("[ERROR] Can't set custom cursor, because the current path \"%s\" is not valid!", path.c_str());
        return;
    }
    if (_user_custom.contains(stdCursor)) {
        if (_user_custom.at(stdCursor).cursor) SDL_DestroySurface(_user_custom.at(stdCursor).cursor.get());
        _user_custom.at(stdCursor).cursor = std::unique_ptr<SDL_Surface>(new_cursor);
        _user_custom.at(stdCursor).hot_point = hot_point;
    } else {
        _user_custom.emplace(stdCursor, UserCustom(hot_point, new_cursor));
    }
}

EasyEngine::Cursor::StdCursor EasyEngine::Cursor::cursor() const {
    return _std_cursor;
}

void EasyEngine::Cursor::setVisible(bool visible) {
    _visible = visible;
    if (_visible) {
        SDL_ShowCursor();
    } else {
        SDL_HideCursor();
    }
}

bool EasyEngine::Cursor::visible() const {
    return _visible;
}

void EasyEngine::Cursor::unload() {
    SDL_DestroyCursor(_cursor);
    if (_surface) SDL_DestroySurface(_surface);
}


EasyEngine::Engine::Engine(const std::string& title, uint32_t width, uint32_t height) {
    SDL_Log("%s v%d.%d.%d-%s (Based by SDL %d.%d.%d)\n"
            "For more information, see https://github.com/CatIsNotFound/EasyEngine.\n\n",
            EASYENGINE_NAME, EASYENGINE_MAJOR_VERSION, EASYENGINE_MINOR_VERSION, EASYENGINE_MACRO_VERSION,
            EASYENGINE_VERSION, SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD | SDL_INIT_CAMERA)) {
        SDL_Log("[ERROR] Initializing Engine failed!\n");
        throw std::runtime_error("Runtime Error: Initialized Engine failed!\n");
    }
    if (!init(title.c_str(), width, height, &_main_window_id)) {
        SDL_Log("[ERROR] Initializing window failed!\n");
        SDL_Quit();
        throw std::runtime_error("Runtime Error: Initializing window failed!\n");
    }
    AudioSystem::global()->init();
    Cursor::global();
    SDL_Log("Enjoy! (*^_^*)\n\n");
}

int EasyEngine::Engine::exec() {
    int exit_code = 0;
    _is_running = true;
    exit_code = run();
    return exit_code;
}

EasyEngine::Engine::~Engine() {
    cleanUp();
}

bool EasyEngine::Engine::show(SDL_WindowID window_id) {
    if (!_sdl_window_list.contains(window_id)) {
        SDL_Log("[ERROR] wID %u is not found! \n", window_id);
        return false;
    }
    bool _ok = SDL_ShowWindow(_sdl_window_list.at(window_id)->window);
    if (!_ok) {
        SDL_Log("[ERROR] wID %u can't be showed! Code: %s\n", window_id, SDL_GetError());
    }
    SDL_GetWindowSize(_sdl_window_list.at(window_id)->window,
                      &_sdl_window_list.at(window_id)->geometry.width,
                      &_sdl_window_list.at(window_id)->geometry.height);
    SDL_GetWindowPosition(_sdl_window_list.at(window_id)->window,
                          &_sdl_window_list.at(window_id)->geometry.x,
                          &_sdl_window_list.at(window_id)->geometry.y);
    return _ok;
}

int EasyEngine::Engine::showAll() {
    int _ret = 0;
    for (auto& _win : _sdl_window_list) {
        bool _ok = SDL_ShowWindow(_win.second->window);
        if (!_ok) {
            SDL_Log("[ERROR] wID %u can't be showed! Code: %s\n", _win.first, SDL_GetError());
            _ret += 1;
        }
        SDL_GetWindowSize(_win.second->window,
                          &_win.second->geometry.width,
                          &_win.second->geometry.height);
        SDL_GetWindowPosition(_win.second->window,
                              &_win.second->geometry.x,
                              &_win.second->geometry.y);
    }
    return _ret;
}

bool EasyEngine::Engine::hide(SDL_WindowID window_id) {
    if (!_sdl_window_list.contains(window_id)) {
        SDL_Log("[ERROR] wID %u is not found! \n", window_id);
        return false;
    }
    bool _ok = SDL_HideWindow(_sdl_window_list.at(window_id)->window);
    if (!_ok) {
        SDL_Log("[ERROR] wID %u can't be hidden! Code: %s\n", window_id, SDL_GetError());
    }
    return _ok;
}

bool EasyEngine::Engine::resize(int width, int height, SWindowID window_id) {
    if (!_sdl_window_list.contains(window_id)) {
        SDL_Log("[ERROR] wID %u is not found!\n", window_id);
        return false;
    }
    auto _ok = SDL_SetWindowSize(_sdl_window_list[window_id]->window, width, height);
    if (!_ok) {
        SDL_Log("[ERROR] wID %u can't resized the window! Code: %s\n", window_id, SDL_GetError());
    } else {
        _sdl_window_list[window_id]->geometry.resize(width, height);
    }
    return _ok;
}

bool EasyEngine::Engine::move(int x, int y, SWindowID window_id) {
    if (!_sdl_window_list.contains(window_id)) {
        SDL_Log("[ERROR] wID %u is not found!\n", window_id);
        return false;
    }
    auto _ok = SDL_SetWindowPosition(_sdl_window_list[window_id]->window, x, y);
    if (!_ok) {
        SDL_Log("[ERROR] wID %u can't moved the window! Code: %s\n", window_id, SDL_GetError());
    } else {
        _sdl_window_list[window_id]->geometry.setPosition(x, y);
    }
    return _ok;
}

bool EasyEngine::Engine::setGeometry(int x, int y, int width, int height, SWindowID window_id) {
    if (!_sdl_window_list.contains(window_id)) {
        SDL_Log("[ERROR] wID %u is not found!\n", window_id);
        return false;
    }
    auto _ok = SDL_SetWindowPosition(_sdl_window_list[window_id]->window, x, y);
    if (!_ok) {
        SDL_Log("[ERROR] wID %u can't moved the window! Code: %s\n", window_id, SDL_GetError());
        return false;
    } else {
        _sdl_window_list[window_id]->geometry.setPosition(x, y);
    }
    _ok = SDL_SetWindowSize(_sdl_window_list[window_id]->window, width, height);
    if (!_ok) {
        SDL_Log("[ERROR] wID %u can't resized the window! Code: %s\n", window_id, SDL_GetError());
    } else {
        _sdl_window_list[window_id]->geometry.resize(width, height);
    }
    return _ok;
}

bool EasyEngine::Engine::setResizable(bool resizeable, SDL_WindowID window_id) {
    if (!_sdl_window_list.contains(window_id)) {
        SDL_Log("[ERROR] wID %u is not found! \n", window_id);
        return false;
    }
    bool _ok = SDL_SetWindowResizable(_sdl_window_list.at(window_id)->window, resizeable);
    if (!_ok) {
        SDL_Log("[ERROR] wID %u can't be set resizable! Code: %s\n", window_id, SDL_GetError());
    }
    return _ok;
}

bool EasyEngine::Engine::setBorderlessWindow(bool enabled, SDL_WindowID window_id) {
    if (!_sdl_window_list.contains(window_id)) {
        SDL_Log("[ERROR] wID %u is not found! \n", window_id);
        return false;
    }
    bool _ok = SDL_SetWindowBordered(_sdl_window_list.at(window_id)->window, !enabled);
    if (!_ok) {
        SDL_Log("[ERROR] wID %u can't be set bordered! Code: %s\n", window_id, SDL_GetError());
    }
    return _ok;
}

bool EasyEngine::Engine::setFullScreen(bool enabled, bool move_cursor_to_center, SDL_WindowID window_id) {
    if (!_sdl_window_list.contains(window_id)) {
        SDL_Log("[ERROR] wID %u is not found! \n", window_id);
        return false;
    }
    if (!move_cursor_to_center) _cursor_old_pos = Cursor::global()->globalPosition();
    bool _ok = SDL_SetWindowFullscreen(_sdl_window_list.at(window_id)->window, enabled);
    if (!_ok) {
        SDL_Log("[ERROR] wID %u can't be set full-screened! Code: %s\n", window_id, SDL_GetError());
    }
    if (move_cursor_to_center) {
        Cursor::global()->moveToCenter((enabled ? window(window_id) : nullptr));
    } else {
        Cursor::global()->move(_cursor_old_pos, (enabled ? window(window_id) : nullptr));
    }
    return _ok;
}

bool EasyEngine::Engine::setWindowTitle(const std::string& title, SWindowID window_id) {
    if (!_sdl_window_list.contains(window_id)) {
        SDL_Log("[ERROR] wID %u is not found! \n", window_id);
        return false;
    }
    bool _ok = SDL_SetWindowTitle(_sdl_window_list.at(window_id)->window, title.c_str());
    if (!_ok) {
        SDL_Log("[ERROR] wID %u can't be set window title! Code: %s\n", window_id, SDL_GetError());
    }
    return _ok;
}

std::string EasyEngine::Engine::windowTitle(SWindowID window_id) {
    if (!_sdl_window_list.contains(window_id)) {
        SDL_Log("[ERROR] wID %u is not found! \n", window_id);
        return {};
    }
    std::string ret = SDL_GetWindowTitle(_sdl_window_list.at(window_id)->window);
    return ret;
}

bool EasyEngine::Engine::init(const char *title, uint32_t width, uint32_t height, uint32_t *wID) {
    SDL_Window* _sdl_window = SDL_CreateWindow(title, width, height, SDL_WINDOW_HIDDEN);

    if (!_sdl_window) {
        SDL_Log("[ERROR] Create window failed! Code: %s\n", SDL_GetError());
        return false;
    }
    SDL_WindowID id = SDL_GetWindowID(_sdl_window);
    SRenderer* _sdl_renderer = SDL_CreateRenderer(_sdl_window, nullptr);
    if (!_sdl_renderer) {
        SDL_Log("[ERROR] wID %u created failed! Code: %s\n", id, SDL_GetError());
        SDL_DestroyWindow(_sdl_window);
        return false;
    }
    _sdl_window_list[id] = std::make_shared<Window>(Window(_sdl_window, _sdl_renderer));
    _sdl_window_list[id]->geometry.setGeometry(0, 0, width, height);
    _renderer_list[id] = std::make_unique<Painter>(_sdl_window_list[id].get());
    if (wID) *wID = id;
    _window_count++;
    return true;
}

bool EasyEngine::Engine::unload(SDL_WindowID window_id) {
    if (!_sdl_window_list.contains(window_id)) {
        SDL_Log("[ERROR] wID %u is not found!\n", window_id);
        return false;
    }
    if (_renderer_list.at(window_id)) {
        _renderer_list.erase(window_id);
    }
    SDL_DestroyRenderer(_sdl_window_list[window_id]->renderer);
    SDL_DestroyWindow(_sdl_window_list[window_id]->window);
    _sdl_window_list.erase(window_id);
    _window_count--;
    _main_window_id = (_sdl_window_list.empty() ? 0 : _sdl_window_list.begin()->first);
    return true;
}

int EasyEngine::Engine::run() {
    // 高精度计时器初始化
    const uint64_t performance_freq = SDL_GetPerformanceFrequency();
    
    // 事件循环专用计时器 - 固定1000Hz事件处理
    const uint64_t event_interval = performance_freq / 1000; // 1ms事件间隔
    uint64_t last_event_time = SDL_GetPerformanceCounter();
    
    // 渲染循环计时器
    uint64_t last_render_time = SDL_GetPerformanceCounter();
    uint64_t frames_in_a_second = 0;
    uint64_t latest_fps_check = SDL_GetPerformanceCounter();
    
    // 丢帧检测变量
    uint64_t consecutive_slow_frames = 0;
    uint64_t dropped_frames = 0;
    // 渲染循环状态
    bool render_needed = true;
    while (_is_running) {
        const uint64_t now = SDL_GetPerformanceCounter();

        // 事件处理循环，每隔 1ms 处理一次
        if (now - last_event_time >= event_interval) {
            _is_running = EventSystem::global()->handler(); // 高频事件处理
            last_event_time = now;
        }
        // 决定是否渲染画面
        if (_is_allowed_stop_render && _is_stopped) continue;
        // 渲染循环，根据 FPS 动态调整渲染频率
        bool should_render = false;
        if (_fps > 0) {
            const uint64_t render_interval = performance_freq / _fps;
            if (now - last_render_time >= render_interval) {
                should_render = true;
                last_render_time = now;
            }
        } else {
            // 无限制帧率 - 每帧都渲染
            should_render = true;
        }

        if (should_render && _is_running) {
            // 渲染前更新阈值
            if (_fps > 0) {
                max_frame_duration = (performance_freq * 15) / (10 * _fps);
                target_frame_duration = performance_freq / _fps;
            } else {
                max_frame_duration = performance_freq / 40;
            }

            uint64_t render_start = SDL_GetPerformanceCounter();

            {
                std::lock_guard<std::mutex> lock(_mutex);

                // 检查丢帧
                bool should_skip_frame = false;
                if (consecutive_slow_frames >= max_consecutive_slow_frames) {
                    should_skip_frame = true;
                    consecutive_slow_frames = 0;
                    dropped_frames += 1;
                    // SDL_Log("[DROP] Frame skipped due to slow rendering");
                }

                if (!should_skip_frame) {
                    if (_window_count == 1) {
                        _renderer_list.begin()->second->update();
                    } else if (_window_count > 1) {
                        for (auto &_renderer: _renderer_list) {
                            _renderer.second->update();
                        }
                    }
                }
            }

            // 丢帧检测
            uint64_t render_end = SDL_GetPerformanceCounter();
            uint64_t render_duration = render_end - render_start;

            if (render_duration > max_frame_duration) {
                consecutive_slow_frames++;
            } else {
                consecutive_slow_frames = 0;
            }

            // FPS统计
            frames_in_a_second++;
            if (now - latest_fps_check >= performance_freq) {
                _real_fps = frames_in_a_second;
                latest_fps_check = now;
                frames_in_a_second = 0;
                if (dropped_frames) SDL_Log("[WARNING] Frame skipped due to slow rendering! count: %d", dropped_frames);
                dropped_frames = 0;
            }
        }

        // 防止CPU占用过高
        if (!should_render) {
            SDL_DelayNS(1000000ULL); // 1ms睡眠
        }
    }
    return _is_running;
}

void EasyEngine::Engine::cleanUp() {
    EventSystem::global()->clearTimer();
    EventSystem::global()->clearTrigger();
    if (_clean_up_function) _clean_up_function();
    AudioSystem::global()->unload();
    Cursor::global()->unload();
    for (auto& _win : _sdl_window_list) {
        if (_win.second->renderer) SDL_DestroyRenderer(_win.second->renderer);
        if (_win.second->window) SDL_DestroyWindow(_win.second->window);
    }
    SDL_Quit();
}

uint32_t EasyEngine::Engine::newWindow(const std::string& title, uint32_t width, uint32_t height) {
    uint32_t ret = 0;
    return init(title.c_str(), width, height, &ret) ? ret : 0;
}

bool EasyEngine::Engine::removeWindow(SDL_WindowID window_id) {
    return unload(window_id);
}

const EasyEngine::Window *EasyEngine::Engine::window(SWindowID window_id) {
    if (_sdl_window_list.contains(window_id)) {
        return _sdl_window_list.at(window_id).get();
    }
    return nullptr;
}

std::vector<uint32_t> EasyEngine::Engine::windowIDList() {
    std::vector<uint32_t> ret;
    for (auto& _win : _sdl_window_list) {
        ret.push_back(_win.first);
    }
    return ret;
}

uint32_t EasyEngine::Engine::windowCount() const {
    return _window_count;
}

void EasyEngine::Engine::installEventHandler(std::function<bool(SEvent)> event_handler) {
    EasyEngine::EventSystem::_my_event_handler = std::move(event_handler);
}

EasyEngine::Painter* EasyEngine::Engine::painter(SDL_WindowID window_id) {
    if (_renderer_list.contains(window_id)) return _renderer_list[window_id].get();
    return nullptr;
}

void EasyEngine::Engine::setFPS(uint32_t fps) {
    _fps = fps;
    
    // 更新丢帧检测参数
    const uint64_t performance_freq = SDL_GetPerformanceFrequency();
    if (_fps > 0) {
        max_frame_duration = (performance_freq * 15) / (10 * _fps);
    } else {
        max_frame_duration = performance_freq / 40;
    }
    if (_fps > 0) {
        target_frame_duration = performance_freq / _fps;
    }
}

uint32_t EasyEngine::Engine::fps() const {
    return _real_fps;
}

void EasyEngine::Engine::setFrameDropTolerance(float ratio) {
    // 确保比例在合理范围内（1.2-3.0倍）
    ratio = std::max(1.2f, std::min(3.0f, ratio));
    
    const uint64_t performance_freq = SDL_GetPerformanceFrequency();
    if (_fps > 0) {
        max_frame_duration = (performance_freq * static_cast<uint64_t>(ratio * 10)) / (10 * _fps);
    } else {
        max_frame_duration = performance_freq / 40; // 无限制帧率时的默认值
    }
}

float EasyEngine::Engine::frameDropTolerance() const {
    const uint64_t performance_freq = SDL_GetPerformanceFrequency();
    if (_fps > 0) {
        return static_cast<float>(max_frame_duration * _fps) / static_cast<float>(performance_freq);
    } else {
        return static_cast<float>(performance_freq) / (40.0f * static_cast<float>(max_frame_duration));
    }
}

void EasyEngine::Engine::setFrameDropThreshold(uint32_t count) {
    max_consecutive_slow_frames = std::max(2u, std::min(10u, count));
}

uint32_t EasyEngine::Engine::frameDropThreshold() const {
    return max_consecutive_slow_frames;
}

void EasyEngine::Engine::setBackgroundRenderingEnabled(bool enabled) {
    _is_allowed_stop_render = !enabled;
}

bool EasyEngine::Engine::backgroundRenderingEnabled() const {
    return _is_allowed_stop_render;
}

void EasyEngine::Engine::installCleanUpEvent(const std::function<void()> &function) {
    _clean_up_function = function;
}

EasyEngine::Geometry EasyEngine::Engine::screenGeometry() {
    SDL_Rect rect;
    auto _primary_screen = SDL_GetPrimaryDisplay();
    SDL_GetDisplayBounds(_primary_screen, &rect);
    return Geometry(rect.x, rect.y, rect.w, rect.h);
}

EasyEngine::Painter::Painter(EasyEngine::Window* window) : _window(window), paint_function(nullptr), _thickness(1) {}

EasyEngine::Painter::~Painter() = default;

const EasyEngine::Window *EasyEngine::Painter::window() const {
    return _window;
}

void EasyEngine::Painter::setThickness(uint16_t value) {
    _thickness = value;
}

uint16_t EasyEngine::Painter::thickness() const {
    return _thickness;
}

void EasyEngine::Painter::paintEvent() {
    if (paint_function) paint_function(*this);
}

void EasyEngine::Painter::update() {
    SDL_GetWindowSize(_window->window, &_window->geometry.width, &_window->geometry.height);
    SDL_GetWindowPosition(_window->window, &_window->geometry.x, &_window->geometry.y);
    SDL_Rect _win_rect(0, 0, _window->geometry.width, _window->geometry.height);
    SDL_SetRenderViewport(_window->renderer, &_win_rect);
    SDL_SetRenderDrawBlendMode(_window->renderer, SDL_BLENDMODE_NONE);
    for (auto& cmd : command_list) {
        cmd->exec(_window->renderer, thickness());
    }
    SDL_RenderPresent(_window->renderer);
    command_list.clear();
    paintEvent();
}

void EasyEngine::Painter::fillBackColor(const SDL_Color &color) {
    command_list.emplace_back(std::make_unique<FillCMD>(color));
}

void EasyEngine::Painter::drawPoint(const EasyEngine::Graphics::Point &point) {
    command_list.emplace_back(std::make_unique<PointCMD>(point));
}

void EasyEngine::Painter::drawLine(const EasyEngine::Graphics::Line &line) {
    command_list.emplace_back(std::make_unique<LineCMD>(line));
}

void EasyEngine::Painter::drawRectangle(const EasyEngine::Graphics::Rectangle &rect) {
    command_list.emplace_back(std::make_unique<RectCMD>(rect));
}

void EasyEngine::Painter::drawEllipse(const EasyEngine::Graphics::Ellipse &ellipse) {
    command_list.emplace_back(std::make_unique<EllipseCMD>(ellipse));
}

void EasyEngine::Painter::drawSprite(const Components::Sprite &sprite, const Vector2 &pos) {
    auto spriteCMD = new SpriteCMD(sprite, pos);
    spriteCMD->_pos = pos + sprite.properties()->position;
    spriteCMD->_is_clip = sprite.properties()->clip_mode;
    spriteCMD->_clip_pos = sprite.properties()->clip_pos;
    spriteCMD->_clip_size = sprite.properties()->clip_size;
    spriteCMD->_rotate = sprite.properties()->rotate;
    spriteCMD->_rotate_center = sprite.properties()->rotate_center;
    spriteCMD->_scaled = sprite.properties()->scaled;
    spriteCMD->_scaled_center = sprite.properties()->scaled_center;
    spriteCMD->_flip_mode = (sprite.properties()->flip_mode == Components::Sprite::FlipMode::None ? SDL_FLIP_NONE :
                             (sprite.properties()->flip_mode == Components::Sprite::FlipMode::VFlip ? SDL_FLIP_VERTICAL
                                                                                                : SDL_FLIP_HORIZONTAL));
    spriteCMD->_color_alpha = sprite.properties()->color_alpha;
    command_list.emplace_back(std::unique_ptr<SpriteCMD>(spriteCMD));
}

void EasyEngine::Painter::drawSprite(const Components::Sprite &sprite,
                                     const Components::Sprite::Properties &properties) {
    auto spiritCmd = new SpriteCMD(sprite, properties.position);
    spiritCmd->_is_clip = properties.clip_mode;
    spiritCmd->_clip_pos = properties.clip_pos;
    spiritCmd->_clip_size = properties.clip_size;
    spiritCmd->_rotate = properties.rotate;
    spiritCmd->_rotate_center = properties.rotate_center;
    spiritCmd->_scaled = properties.scaled;
    spiritCmd->_scaled_center = properties.scaled_center;
    spiritCmd->_flip_mode = (properties.flip_mode == Components::Sprite::FlipMode::None ? SDL_FLIP_NONE :
                             (properties.flip_mode == Components::Sprite::FlipMode::VFlip ? SDL_FLIP_VERTICAL
                                                                                    : SDL_FLIP_HORIZONTAL));
    spiritCmd->_color_alpha = properties.color_alpha;
    command_list.emplace_back(std::unique_ptr<SpriteCMD>(spiritCmd));
}

void EasyEngine::Painter::drawSprite(const EasyEngine::Components::Sprite &sprite,
                                     const EasyEngine::Components::Sprite::Properties *properties) {
    auto spiritCmd = new SpriteCMD(sprite, properties->position);
    spiritCmd->_is_clip = properties->clip_mode;
    spiritCmd->_clip_pos = properties->clip_pos;
    spiritCmd->_clip_size = properties->clip_size;
    spiritCmd->_rotate = properties->rotate;
    spiritCmd->_rotate_center = properties->rotate_center;
    spiritCmd->_scaled = properties->scaled;
    spiritCmd->_scaled_center = properties->scaled_center;
    spiritCmd->_flip_mode = (properties->flip_mode == Components::Sprite::FlipMode::None ? SDL_FLIP_NONE :
            (properties->flip_mode == Components::Sprite::FlipMode::VFlip ? SDL_FLIP_VERTICAL : SDL_FLIP_HORIZONTAL));
    spiritCmd->_color_alpha = properties->color_alpha;
    command_list.emplace_back(std::unique_ptr<SpriteCMD>(spiritCmd));
}

void EasyEngine::Painter::clear() {
    command_list.clear();
}

void EasyEngine::Painter::installPaintEvent(std::function<void(Painter&)> function) {
    paint_function = std::move(function);
}

void EasyEngine::Painter::PointCMD::exec(SRenderer *renderer, uint32_t thickness) {
    if (thickness > 1) {
        filledCircleRGBA(renderer, pt.pos.x, pt.pos.y, thickness - 1, pt.color.r, pt.color.g, pt.color.b, pt.color.a);
    } else {
        SDL_SetRenderDrawColor(renderer, pt.color.r, pt.color.g, pt.color.b, pt.color.a);
        SDL_RenderPoint(renderer, pt.pos.x, pt.pos.y);
    }
}

void EasyEngine::Painter::LineCMD::exec(SRenderer *renderer, uint32_t thickness) {
    if (line.width > 1) {
        thickLineRGBA(renderer, line.start.x, line.start.y, line.end.x, line.end.y, line.width,
                 line.color.r, line.color.g, line.color.b, line.color.a);

    } else {
        SDL_SetRenderDrawColor(renderer, line.color.r, line.color.g, line.color.b, line.color.a);
        SDL_RenderLine(renderer, line.start.x, line.start.y, line.end.x, line.end.y);
    }
}

void EasyEngine::Painter::RectCMD::exec(SRenderer *renderer, uint32_t thickness) {
    SDL_FRect r = {rect.pos.x, rect.pos.y, (float)rect.size.width, (float)rect.size.height},
              rb = {rect.pos.x - thickness + 1, rect.pos.y - thickness + 1,
                    rect.size.width + (float)((thickness - 1) * 2),
                    rect.size.height + (float)((thickness - 1) * 2)};

    if (rect.filled_mode && !rect.bordered_mode) {
        SDL_SetRenderDrawColor(renderer, rect.back_color.r, rect.back_color.g, rect.back_color.b, rect.back_color.a);
        SDL_RenderFillRect(renderer, &r);
    } else if (rect.filled_mode) {
        SDL_SetRenderDrawColor(renderer, rect.fore_color.r, rect.fore_color.g, rect.fore_color.b, rect.fore_color.a);
        SDL_RenderFillRect(renderer, &rb);
        SDL_SetRenderDrawColor(renderer, rect.back_color.r, rect.back_color.g, rect.back_color.b, rect.back_color.a);
        SDL_RenderFillRect(renderer, &r);
    } else if (rect.bordered_mode) {
        SDL_SetRenderDrawColor(renderer, rect.fore_color.r, rect.fore_color.g, rect.fore_color.b, rect.fore_color.a);
        SDL_RenderRect(renderer, &r);
    }
}

void EasyEngine::Painter::EllipseCMD::exec(SRenderer *renderer, uint32_t thickness) {
    auto tmp_thickness = static_cast<int16_t>(thickness);
    auto t_area_w = static_cast<int16_t>(ellipse.area.width),
         t_area_h = static_cast<int16_t>(ellipse.area.height);
    auto tmp_width = t_area_w / 2 + (t_area_w % 2),
         tmp_height = t_area_h / 2 + (t_area_h % 2);
    if (!ellipse.bordered_mode && ellipse.filled_mode) {
        filledEllipseRGBA(renderer, ellipse.pos.x, ellipse.pos.y,
                    tmp_width,tmp_height,
                    ellipse.back_color.r, ellipse.back_color.g,
                    ellipse.back_color.b, ellipse.back_color.a);

    } else if (ellipse.bordered_mode && ellipse.filled_mode) {
        filledEllipseRGBA(renderer, ellipse.pos.x, ellipse.pos.y,
                          tmp_width + tmp_thickness,tmp_height + tmp_thickness,
                          ellipse.fore_color.r, ellipse.fore_color.g,
                          ellipse.fore_color.b, ellipse.fore_color.a);
        filledEllipseRGBA(renderer, ellipse.pos.x, ellipse.pos.y,
                          tmp_width,tmp_height,
                          ellipse.back_color.r, ellipse.back_color.g,
                          ellipse.back_color.b, ellipse.back_color.a);
    } else {
        // 暂时单画边框圆
        ellipseRGBA(renderer, ellipse.pos.x, ellipse.pos.y,
                          tmp_width + tmp_thickness,tmp_height + tmp_thickness,
                          ellipse.fore_color.r, ellipse.fore_color.g,
                          ellipse.fore_color.b, ellipse.fore_color.a);
    }
}

void EasyEngine::Painter::FillCMD::exec(SRenderer *renderer, uint32_t) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void EasyEngine::Painter::SpriteCMD::exec(SRenderer *renderer, uint32_t) {
    SDL_SetTextureColorMod(_sprite, _color_alpha.r, _color_alpha.g, _color_alpha.b);
    if (_color_reversed) SDL_SetTextureBlendMode(_sprite, SDL_BLENDMODE_MOD);
    Vector2 _global_center_pos(_pos.x + _scaled_center.x, _pos.y + _scaled_center.y);
    Vector2 newPos((_pos.x - _global_center_pos.x) * _scaled + _global_center_pos.x,
                   (_pos.y - _global_center_pos.y) * _scaled + _global_center_pos.y);
    Vector2 newScaled(_size.width * _scaled, _size.height * _scaled);
    SDL_FPoint center = {_rotate_center.x * _scaled, _rotate_center.y * _scaled};
    SDL_FRect _dst = {newPos.x, newPos.y, newScaled.x, newScaled.y};

    if (_is_clip) {
        SDL_FRect _tmp = {_clip_pos.x, _clip_pos.y, _clip_size.width, _clip_size.height};
        SDL_RenderTextureRotated(renderer, _sprite, &_tmp, &_dst,
                                 _rotate, &center, _flip_mode);
    } else {
        SDL_RenderTextureRotated(renderer, _sprite, nullptr, &_dst,
                                 _rotate, &center, _flip_mode);
    }
}

EasyEngine::EventSystem::~EventSystem() {};

EasyEngine::EventSystem *EasyEngine::EventSystem::global() {
    if (!_instance) {
        _instance = std::make_unique<EventSystem>();
    }
    return _instance.get();
}

bool EasyEngine::EventSystem::handler() {
    static SEvent ev;
    static bool ret = true;
    if (SDL_PollEvent(&ev)) {
        if (ev.window.type == SDL_EVENT_QUIT) {
            return false;
        }
        if (ev.window.type == SDL_EVENT_WINDOW_FOCUS_LOST) {
            Engine::_is_stopped = true;

        } else if (ev.window.type == SDL_EVENT_WINDOW_FOCUS_GAINED) {
            Engine::_is_stopped = false;
        }

        Vector2 cursor_pos = Cursor::global()->position();
        const uint64_t CLICK_DELAY = 500;
        for (auto& _control : _control_list) {
            auto _container = _control.second;
            if (!_container->enabled()) continue;
            if (_container->active() &&
                (ev.key.key == SDLK_SPACE || ev.key.key == SDLK_RETURN || ev.key.key == SDLK_KP_ENTER)) {
                static bool _is_key_down = false;
                if (ev.key.down) {
                    if (!_is_key_down) {
                        _container->__updateEvent(Components::Control::Event::KeyDown);
                        _container->__updateStatus(Components::Control::Status::Pressed);
                        _is_key_down = true;
                    }
                } else {
                    _is_key_down = false;
                    _container->__updateEvent(Components::Control::Event::KeyUp);
                    _container->__updateStatus(Components::Control::Status::Active);
                }
                continue;
            }
            bool is_cursor_on_control = Algorithm::comparePosRect(cursor_pos, _container->hotArea()) > -1;
            static bool is_hovered_one = false;
            static uint64_t last_click_time = 0, current_click_time = 0, click_count = 0;
            static Vector2 old_cursor_pos = cursor_pos;
            if (is_cursor_on_control) {
                if (!is_hovered_one) {
                    is_hovered_one = true;
                    _container->__updateEvent(Components::Control::Event::MouseHover);
                    _container->__updateStatus(Components::Control::Status::Hovered);
                } else if (ev.button.down) {
                    _container->__updateEvent(Components::Control::Event::MouseDown);
                    _container->__updateStatus(Components::Control::Status::Pressed);
                    old_cursor_pos = cursor_pos;
                } else if (
                    _container->__currentStatus() == Components::Control::Status::Pressed) {
                    current_click_time = SDL_GetTicks();
                    if (!last_click_time) {
                        last_click_time = current_click_time;
                        click_count += 1;
                    } else if (current_click_time - last_click_time < CLICK_DELAY) {
                        click_count += 1;
                    } else {
                        last_click_time = current_click_time;
                        click_count = 1;
                    }
                    if (click_count == 1) {
                        _container->__updateEvent(Components::Control::Event::Clicked);
                    } else if (click_count == 2) {
                        _container->__updateEvent(Components::Control::Event::DblClicked);
                    } else {
                        _container->__updateEvent(Components::Control::Event::MouseUp);
                    }
                    _container->__updateStatus(Components::Control::Status::Hovered);
                }
            } else if (_container->__currentStatus() == Components::Control::Status::Hovered) {
                is_hovered_one = false;
                _container->__updateEvent(Components::Control::Event::MouseLeave);
                _container->__updateStatus(_container->active() ?
                    Components::Control::Status::Active : Components::Control::Status::Default);
            } else if (_container->__currentStatus() == Components::Control::Status::Pressed) {
                is_hovered_one = false;
                _container->__updateEvent(Components::Control::Event::MouseLeave);
                _container->__updateStatus(_container->active() ?
                                           Components::Control::Status::Active : Components::Control::Status::Default);
            }
        }
        if (_my_event_handler) {
            ret = _my_event_handler(ev);
        }
    }
    for (auto& _timer : _timer_list) {
        _timer.second->update();
    }
    for (auto& _trigger : _trigger_list) {
        _trigger.second->__update();
    }
    return ret;
}

uint64_t EasyEngine::EventSystem::addTimer(EasyEngine::Components::Timer *timer) {
    if (!timer) {
        SDL_Log("[ERROR] The specified timer is not valid!");
        return UINT64_MAX;
    }
    
    auto it = std::find_if(_timer_list.begin(), _timer_list.end(),
        [timer](const auto& pair) {
            return timer == pair.second.get();
        });
    
    if (it != _timer_list.end()) {
        return it->first;
    }
    
    _timer_list[++_timer_id] = std::unique_ptr<Components::Timer>(timer);
    return _timer_id;
}

void EasyEngine::EventSystem::replaceTimer(uint64_t id, EasyEngine::Components::Timer *timer) {
    if (!timer) {
        SDL_Log("[ERROR] The specified timer is not valid!");
        return;
    }
    if (_timer_list.contains(id)) _timer_list[id].reset(timer);
}

void EasyEngine::EventSystem::removeTimer(uint64_t id) {
    if (!_timer_list.contains(id)) {
        SDL_Log("[ERROR] The Specified timer ID is not exist!");
        return;
    }
    if (_timer_list.contains(id)) _timer_list.erase(id);
}

void EasyEngine::EventSystem::removeTimer(EasyEngine::Components::Timer *timer) {
    auto iter = std::find_if(_timer_list.begin(), _timer_list.end(),
                 [timer](const auto& pair) {
                     return timer == pair.second.get();
                 });
    if (iter != _timer_list.end()) {
        _timer_list.erase(iter);
    }
}

void EasyEngine::EventSystem::clearTimer() {
    _timer_list.clear();
}

uint64_t EasyEngine::EventSystem::addTrigger(EasyEngine::Components::Trigger *trigger) {
    if (!trigger) {
        SDL_Log("[ERROR] The specified trigger is not valid!");
        return UINT64_MAX;
    }
    auto it = std::find_if(_trigger_list.begin(), _trigger_list.end(),
                           [trigger](const auto& pair) {
                               return trigger == pair.second.get();
                           });
    if (it != _trigger_list.end()) {
        return it->first;
    }
    _trigger_list[++_trigger_id] = std::unique_ptr<Components::Trigger>(trigger);
    return _trigger_id;
}

void EasyEngine::EventSystem::replaceTrigger(uint64_t id, EasyEngine::Components::Trigger *trigger) {
    if (!trigger) {
        SDL_Log("[ERROR] The specified trigger is not valid!");
        return;
    }
    if (_trigger_list.contains(id)) _trigger_list[id].reset(trigger);
}

void EasyEngine::EventSystem::removeTrigger(uint64_t id) {
    if (!_trigger_list.contains(id)) {
        SDL_Log("[ERROR] The Specified trigger ID is not exist!");
        return;
    }
    if (_trigger_list.contains(id)) _trigger_list.erase(id);
}

void EasyEngine::EventSystem::clearTrigger() {
    _trigger_list.clear();
}

uint64_t EasyEngine::EventSystem::addControl(EasyEngine::Components::Control *control) {
    if (!control) {
        SDL_Log("[ERROR] The specified control is not valid!");
        return UINT64_MAX;
    }
    auto it = std::find_if(_control_list.begin(), _control_list.end(),
                           [control](const auto& pair) {
                               return control == pair.second;
                           });
    if (it != _control_list.end()) {
        return it->first;
    }
    _control_list[++_control_id] = control;
    return _control_id;
}

void EasyEngine::EventSystem::replaceControl(uint64_t id, EasyEngine::Components::Control *control) {
    if (!control) {
        SDL_Log("[ERROR] The specified control is not valid!");
        return;
    }
    if (_control_list.contains(id)) _control_list[id] = control;
}

void EasyEngine::EventSystem::removeControl(uint64_t id) {
    if (_control_list.contains(id)) {
        _control_list.erase(id);
    }
}

void EasyEngine::EventSystem::clearControls() {
    _control_list.clear();
}

EasyEngine::AudioSystem::AudioSystem() : _audio_spec(StdAudioSpec::Stereo), _bgm_mixer(nullptr), _sfx_mixer(nullptr) {
    if (!MIX_Init()) {
        SDL_Log("[ERROR] Failed to load audio system! Code: %s", SDL_GetError());
        return;
    }
    _is_init = true;
}

EasyEngine::AudioSystem::~AudioSystem() {
    if (_is_init) MIX_Quit();
}

EasyEngine::AudioSystem *EasyEngine::AudioSystem::global() {
    if (!_instance) {
        _instance = std::unique_ptr<AudioSystem>(new AudioSystem());
    }
    return _instance.get();
}

bool EasyEngine::AudioSystem::init() {
    if (!_is_init && !MIX_Init()) {
        SDL_Log("[ERROR] Failed to load audio system! Code: %s", SDL_GetError());
        return false;
    }
    if (!_is_loaded) {
        _bgm_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &_audio_spec);
        _sfx_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &_audio_spec);
        if (!_bgm_mixer) {
            _audio_spec = StdAudioSpec::Mono;
            _bgm_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &_audio_spec);
            if (!_bgm_mixer) {
                SDL_Log("[ERROR] Failed to load audio system! Code: %s", SDL_GetError());
                return false;
            }
        }
        if (!_sfx_mixer) {
            _audio_spec = StdAudioSpec::Mono;
            _sfx_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &_audio_spec);
            if (!_sfx_mixer) {
                SDL_Log("[ERROR] Failed to load audio system! Code: %s", SDL_GetError());
                return false;
            }
        }
        _is_loaded = true;
    }
    return _is_loaded;
}

void EasyEngine::AudioSystem::unload() {
    if (_is_loaded) {
        unloadAllChannel();
        if (_bgm_mixer) MIX_DestroyMixer(_bgm_mixer);
        if (_sfx_mixer) MIX_DestroyMixer(_sfx_mixer);
    }
}

void EasyEngine::AudioSystem::setAudioSpec(const SAudioSpec &spec = StdAudioSpec::Stereo) {
    if ((spec.freq == _audio_spec.freq) && (spec.channels == _audio_spec.channels)
                                        && (spec.format == _audio_spec.format))   return;
    MIX_DestroyMixer(_bgm_mixer);
    MIX_DestroyMixer(_sfx_mixer);
    _bgm_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &_audio_spec);
    _sfx_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &_audio_spec);
    if (_bgm_mixer && _sfx_mixer) {
        _audio_spec.format = spec.format;
        _audio_spec.freq = spec.freq;
        _audio_spec.channels = spec.channels;
        SDL_Log("[WARNING] Current audio format has changed!");
    } else {
        SDL_Log("[ERROR] Can't change the audio format! Code: %s", SDL_GetError());
        if (!_bgm_mixer) _bgm_mixer = MIX_CreateMixer(&_audio_spec);
        if (!_sfx_mixer) _sfx_mixer = MIX_CreateMixer(&_audio_spec);
    }
}

const SAudioSpec& EasyEngine::AudioSystem::audioSpec() const {
    return _audio_spec;
}

void EasyEngine::AudioSystem::setBGMVolume(float volume) {
    if (volume >= 0 && volume <= 20.01f) _bgm_volume = volume;
    MIX_SetMasterGain(_bgm_mixer, _bgm_volume);
}

float EasyEngine::AudioSystem::bgmVolume() const {
    return _bgm_volume;
}

void EasyEngine::AudioSystem::setSFXVolume(float volume) {
    _sfx_volume = volume;
    MIX_SetMasterGain(_sfx_mixer, _sfx_volume);
}

float EasyEngine::AudioSystem::sfxVolume() const {
    return _sfx_volume;
}

int16_t EasyEngine::AudioSystem::loadBGM(const Components::BGM &bgm) {
    uint8_t ret = 0;
    bool _f = false;
    for (auto& ch : _bgm_channels) {
        if (ch.status == Audio::Unload || ch.status == Audio::Failed) {
            if (ch.status == Audio::Failed) {
                unloadBGM(ret);
            }
            ch.status = Audio::Loading;
            ch.audio = MIX_LoadAudio(_bgm_mixer, bgm.path().data(), false);
            if (!ch.audio) {
                SDL_Log("[ERROR] Failed to load BGM: %s\n        Code: %s", bgm.path().data(), SDL_GetError());
                ch.status = Audio::Failed;
                return -1;
            }
            ch.status = Audio::Loaded;
            ch.url = bgm.path();
            ch.is_stream = true;
            ch.Stream.position = 0;
            ch.Stream.duration = 0;

            _bgm_played_list.push_back(ret);
            _f = true;
            break;
        }
        ret += 1;
    }
    if (!_f) {
        uint8_t ch = _bgm_played_list.front();
        _bgm_played_list.pop_front();
        _bgm_played_list.push_back(ch);
        unloadBGM(ch);
        ret = loadBGM(bgm);
    }
    return ret;
}

int16_t EasyEngine::AudioSystem::loadSFX(const Components::SFX &sfx) {
    uint8_t ret = 0;
    bool _f = false;
    for (auto& ch : _sfx_channels) {
        if (ch.status == Audio::Unload || ch.status == Audio::Failed) {
            if (ch.status == Audio::Failed) {
                unloadSFX(ret);
            }
            ch.status = Audio::Loading;
            ch.audio = MIX_LoadAudio(_bgm_mixer, sfx.path().data(), true);
            if (!ch.audio) {
                SDL_Log("[ERROR] Failed to load SFX: %s\n        Code: %s", sfx.path().data(), SDL_GetError());
                ch.status = Audio::Failed;
                return -1;
            }
            ch.status = Audio::Loaded;
            ch.url = sfx.path();
            ch.is_stream = false;
            _sfx_played_list.push_back(ret);
            _f = true;
            break;
        }
        ret += 1;
    }
    if (!_f) {
        uint8_t ch = _sfx_played_list.front();
        _sfx_played_list.pop_front();
        _sfx_played_list.push_back(ch);
        unloadSFX(ch);
        ret = loadSFX(sfx);
    }
    return ret;
}

bool EasyEngine::AudioSystem::playBGM(uint8_t channel, bool loop) {
    if (_bgm_channels[channel].status == Audio::Loaded) {
        _bgm_channels[channel].Stream.track = MIX_CreateTrack(_bgm_mixer);
        if (!_bgm_channels[channel].Stream.track) {
            SDL_Log("[ERROR] Failed to init the track! \n        Code: %s", SDL_GetError());
            _bgm_channels[channel].status = Audio::Failed;
            return false;
        }
        bool _played = MIX_SetTrackAudio(_bgm_channels[channel].Stream.track, _bgm_channels[channel].audio);
        if (!_played) {
            SDL_Log("[ERROR] Can't played the bgm: %s, \n        Code: %s", 
                    _bgm_channels[channel].url.c_str(), SDL_GetError());
            _bgm_channels[channel].status = Audio::Failed;
            return false;
        }
    }

    SDL_PropertiesID id = 1;
    SDL_SetNumberProperty(id, MIX_PROP_PLAY_LOOPS_NUMBER, (loop ? -1 : 0));
    if (_bgm_channels[channel].status == Audio::Loaded &&
        !MIX_PlayTrack(_bgm_channels[channel].Stream.track, id)) {
        SDL_Log("[ERROR] Can't played the bgm: %s, \n        Code: %s",
                _bgm_channels[channel].url.c_str(), SDL_GetError());
        _bgm_channels[channel].status = Audio::Failed;
        return false;
    }
    if (_bgm_channels[channel].status == Audio::Paused && !MIX_ResumeTrack(_bgm_channels[channel].Stream.track)) {
        SDL_Log("[ERROR] Can't played the bgm: %s, \n        Code: %s",
                _bgm_channels[channel].url.c_str(), SDL_GetError());
        _bgm_channels[channel].status = Audio::Failed;
        return false;
    }
    _bgm_channels[channel].Stream.duration = MIX_AudioFramesToMS(
            _bgm_channels[channel].audio, MIX_GetAudioDuration(_bgm_channels[channel].audio)
    );
    _bgm_channels[channel].status = Audio::Playing;
    return true;
}

bool EasyEngine::AudioSystem::playSFX(uint8_t channel) {
    if (_sfx_channels[channel].status == Audio::Loaded) {
        _sfx_channels[channel].Stream.track = MIX_CreateTrack(_sfx_mixer);
        if (!_sfx_channels[channel].Stream.track) {
            SDL_Log("[ERROR] Failed to init the track! \n        Code: %s", SDL_GetError());
            _sfx_channels[channel].status = Audio::Failed;
            return false;
        }
        bool _played = MIX_SetTrackAudio(_sfx_channels[channel].Stream.track, _sfx_channels[channel].audio);
        if (!_played) {
            SDL_Log("[ERROR] Can't played the sfx: %s, \n        Code: %s",
                    _sfx_channels[channel].url.c_str(), SDL_GetError());
            _sfx_channels[channel].status = Audio::Failed;
            return false;
        }
        _sfx_channels[channel].Stream.duration = MIX_AudioFramesToMS(
            _sfx_channels[channel].audio, MIX_GetAudioDuration(_sfx_channels[channel].audio)
        );
    }
    if (_sfx_channels[channel].status == Audio::Loaded &&
        !MIX_PlayTrack(_sfx_channels[channel].Stream.track, 0)) {
        SDL_Log("[ERROR] Can't played the sfx: %s, \n        Code: %s",
                _sfx_channels[channel].url.c_str(), SDL_GetError());
        _sfx_channels[channel].status = Audio::Failed;
        return false;
    }

    return true;
}

void EasyEngine::AudioSystem::stopBGM(uint8_t channel, bool pause, int64_t fade_out_duration) {
    if (pause) {
        MIX_PauseTrack(_bgm_channels[channel].Stream.track);
        _bgm_channels[channel].status = Audio::Paused;
    } else {
        MIX_StopTrack(_bgm_channels[channel].Stream.track, fade_out_duration);
        _bgm_channels[channel].status = Audio::Loaded;
    }
}

void EasyEngine::AudioSystem::stopSFX(uint8_t channel) {
    MIX_StopTrack(_sfx_channels[channel].Stream.track, 0);
}

void EasyEngine::AudioSystem::stopAllBGM() {
    for (uint8_t ch = 0; ch < 16; ++ch) {
        stopBGM(ch, false);
    }
}

void EasyEngine::AudioSystem::stopAllSFX() {
    for (uint16_t ch = 0; ch <= 255; ++ch) {
        stopSFX(ch);
    }
}

void EasyEngine::AudioSystem::unloadBGM(uint8_t channel) {
    if (channel > 15) return;
    stopBGM(channel, false, 0);
    MIX_DestroyTrack(_bgm_channels[channel].Stream.track);
    MIX_DestroyAudio(_bgm_channels[channel].audio);
    _bgm_channels[channel].url.clear();
    _bgm_channels[channel].status = Audio::Unload;
}

void EasyEngine::AudioSystem::unloadSFX(uint8_t channel) {
    stopSFX(channel);
    MIX_DestroyTrack(_sfx_channels[channel].Stream.track);
    MIX_DestroyAudio(_sfx_channels[channel].audio);
    _sfx_channels[channel].url.clear();
    _sfx_channels[channel].status = Audio::Unload;
}

void EasyEngine::AudioSystem::unloadAllChannel() {
    for (uint16_t ch = 0; ch <= 255; ++ch) {
        if (ch < 16) unloadBGM(ch);
        unloadSFX(ch);
    }
}

const EasyEngine::AudioSystem::Audio &EasyEngine::AudioSystem::bgmChannel(uint8_t channel) {
    if (channel <= 15)
        return _bgm_channels[channel];
    else
        return _bgm_channels[15];
}

const EasyEngine::AudioSystem::Audio &EasyEngine::AudioSystem::sfxChannel(uint8_t channel) {
    return _sfx_channels[channel];
}


