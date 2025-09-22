
#include "Resources.h"
using namespace EasyEngine;

std::string FileSystem::_main_path = std::filesystem::absolute(".").string();
std::unique_ptr<ResourceSystem> ResourceSystem::_instance = nullptr;

bool FileSystem::setCurrentPath(const std::string &main_directory) {
    if (std::filesystem::is_directory(main_directory)) {
        _main_path = std::filesystem::absolute(main_directory).string();
        return true;
    }
    fmt::println("[ERROR] Path '{}' is not found!", main_directory);
    return false;
}

const std::string& FileSystem::currentPath() {
    return _main_path;
}

bool FileSystem::isFile(const std::string &path) {
    if (path.front() != '.') {
        return std::filesystem::is_regular_file(path);
    } else {
        auto real_path = fmt::format("{}{}", _main_path, path.substr(path.find_first_of('/')));
        return std::filesystem::is_regular_file(real_path);
    }
}

bool FileSystem::isDir(const std::string &path) {
    if (path.front() != '.') {
        return std::filesystem::is_directory(path);
    } else {
        auto real_path = fmt::format("{}{}", _main_path, path.substr(path.find_first_of('/')));
        return std::filesystem::is_directory(real_path);
    }
}

bool FileSystem::mkDir(const std::string &path, bool ignore_error, bool recursive_create) {
    std::filesystem::path temp = getAbsolutePath(path);
    if (std::filesystem::is_directory(temp)) {
        if (ignore_error) fmt::println("[ERROR] Directory '{}' is exist!", temp.string());
        return false;
    } else {
        if (recursive_create) {
            auto mk_path = getPathUntilExist(temp.string());
            for (auto& m_path : mk_path) {
                std::filesystem::create_directory(m_path);
            }
        } else {
            if (!std::filesystem::is_directory(temp)) {
                try {
                    std::filesystem::create_directory(temp);
                } catch (const std::exception& e) {
                    if (ignore_error) return false;
                    fmt::println("[ERROR] Directory '{}' can not be created!\n"
                                 "Exception: {}", temp.string(), e.what());
                    return false;
                }
            }
            else {
                if (ignore_error) fmt::println("[ERROR] Directory '{}' is exist!", path);
                return false;
            }
        }
    }
    return true;
}

bool FileSystem::rmDir(const std::string &path, bool ignore_error, bool recursive_remove) {
    std::filesystem::path temp = getAbsolutePath(path);
    if (!std::filesystem::is_directory(temp)) {
        if (!ignore_error) fmt::println("[ERROR] Directory '{}' is not exist!", temp.string());
        return false;
    } else {
        if (recursive_remove) {
            auto rm_paths = getPathUntilNotExist(temp.string());
            if (rm_paths.empty()) {
                if (ignore_error) fmt::println("[ERROR] Directory '{}' can not be removed!", temp.string());
                return false;
            }
            for (int64_t idx = rm_paths.size() - 1; idx >= 0; idx--) {
                fmt::println("Delete: {}", rm_paths[idx]);
                std::filesystem::remove(rm_paths[idx]);
            }
        } else {
            if (std::filesystem::is_directory(temp)) {
                try {
                    std::filesystem::remove(temp);
                } catch (const std::exception &e) {
                    if (!ignore_error)
                        fmt::println("[ERROR] Directory '{}' can not be removed!\n"
                                     "Exception: {}", temp.string(), e.what());
                    return false;
                }
            } else {
                if (!ignore_error) fmt::println("[ERROR] Directory '{}' is not exist!", temp.string());
                return false;
            }
        }
    }
    return true;
}

bool FileSystem::mkFile(const std::string &path, bool auto_create_directory, bool ignore_error) {
    std::filesystem::path temp = getAbsolutePath(path);
    if (auto_create_directory) {
        auto dir = temp.string().substr(0, temp.string().find_last_of('/'));
        if (!std::filesystem::is_directory(dir)) {
            mkDir(dir, ignore_error, true);
        }
    }
    std::ofstream file(temp.string());
    if (!file.is_open()) {
        if (!ignore_error) fmt::println("[ERROR] Can't create file '{}'!", temp.string());
    }
    file << "";
    file.close();
    return true;
}

bool FileSystem::rmFile(const std::string &path, bool ignore_error) {
    std::filesystem::path temp = getAbsolutePath(path);
    if (std::filesystem::is_regular_file(temp)) {
        std::filesystem::remove(temp);
        return true;
    }
    if (!ignore_error) fmt::println("[ERROR] Can't remove the file '{}'", temp.string());
    return false;
}

bool FileSystem::writeFile(const std::string &context, const std::string &path, bool append_mode, bool ignore_error) {
    std::filesystem::path temp = getAbsolutePath(path);
    std::ofstream file(temp.string(),((append_mode ? (std::ios::in | std::ios::app) : std::ios::in)));
    if (!file.is_open()) {
        if (!ignore_error) fmt::println("[ERROR] Can't create file '{}'!", temp.string());
        return false;
    }
    file << context;
    file.close();
    return true;
}

std::string FileSystem::readFile(const std::string &path, bool ignore_error, bool *ok) {
    std::filesystem::path temp = getAbsolutePath(path);
    std::ifstream file(temp.string(), std::ios::in);
    if (!file.is_open()) {
        if (!ignore_error) fmt::println("[ERROR] File '{}' is not found!", temp.string());
        if (ok) *ok = false;
        return "";
    }
    std::string output;
    size_t line = 0;
    try {
        char buf[1024] = {'\0'};
        while (true) {
            line += 1;
            file.getline(buf, 1024);
            output += buf;
            if (!file.eof()) output += '\n'; else break;
        }
    } catch (const std::exception &exception) {
        SDL_Log("[ERROR] Read file '%s' failed at line %zu!", temp.string().c_str(), line);
    }
    file.close();
    if (ok) *ok = true;
    return output;
}

bool FileSystem::writeBinaryFile(const std::string &path, bool append_mode,
                                 const std::function<void(std::ofstream &)> &how2WriteFile, bool ignore_error) {
    std::filesystem::path temp = getAbsolutePath(path);
    std::ofstream file(temp.string(),((append_mode ? (std::ios::in | std::ios::app | std::ios::binary) :
                                            std::ios::in | std::ios::binary)));
    if (!file.is_open()) {
        if (!ignore_error) fmt::println("[ERROR] Can't create file '{}'!", temp.string());
        return false;
    }
    if (how2WriteFile) how2WriteFile(file);
    file.close();
    return true;
}

bool FileSystem::writeBinaryFile(const std::vector<uint8_t> &binaries, const std::string &path, bool append_mode,
                                 bool ignore_error) {
    std::filesystem::path temp = getAbsolutePath(path);
    std::ofstream file(temp.string(),((append_mode ? (std::ios::in | std::ios::app | std::ios::binary) :
                                       std::ios::in | std::ios::binary)));
    if (!file.is_open()) {
        if (!ignore_error) fmt::println("[ERROR] Can't create file '{}'!", temp.string());
        return false;
    }
    for (auto& bin : binaries) {
        file.write(reinterpret_cast<char*>(bin), sizeof(uint8_t));
    }
    file.close();
    return true;
}

bool FileSystem::readBinaryFile(const std::string &path, const std::function<void(std::ifstream &)> &how2ReadFile,
                                bool ignore_error) {
    std::filesystem::path temp = getAbsolutePath(path);
    std::ifstream file(temp.string(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        if (!ignore_error) fmt::println("[ERROR] File '{}' is not found!", temp.string());
        return false;
    }
    if (how2ReadFile) how2ReadFile(file);
    file.close();
    return true;
}

std::vector<uint8_t> FileSystem::readBinaryFile(const std::string &path, bool ignore_error, bool *ok) {
    std::filesystem::path temp = getAbsolutePath(path);
    std::ifstream file(temp.string(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        if (!ignore_error) fmt::println("[ERROR] File '{}' is not found!", temp.string());
        if (ok) *ok = false;
        return {};
    }
    std::vector<uint8_t> ret(1024);
    char temp_buf;
    while (!file.eof()) {
        file.get(temp_buf);
        ret.emplace_back(static_cast<uint8_t>(temp_buf));
    }
    file.close();
    if (ok) *ok = true;
    return ret;
}

std::string FileSystem::getAbsolutePath(const std::string &path) {
    if (path[1] == ':') return path;
    if (path.front() == '.') return fmt::format("{}{}", _main_path, path.substr(path.find_first_of('/')));
    else return fmt::format("{}/{}", _main_path, path);
//    return (path.front() != '/' ? fmt::format("{}{}", _main_path, path.substr(path.find_first_of('/')))
//            : path);
}

std::deque<std::string> FileSystem::getPathUntilExist(const std::string &path) {
    std::deque<std::string> paths;
    std::string temp_path = path;
    do {
        paths.push_front(temp_path);
        temp_path = temp_path.substr(0, temp_path.find_last_of('/'));
    } while (!std::filesystem::is_directory(temp_path));
    return paths;
}

std::deque<std::string> FileSystem::getPathUntilNotExist(const std::string &path) {
    std::deque<std::string> paths;
    std::string temp_path = path;
    if (std::filesystem::is_directory(temp_path)) {
        paths.push_back(temp_path);
        auto files = std::filesystem::directory_iterator(temp_path);
        for (auto& f : files) {
            if (f.is_directory()) {
                auto new_paths = getPathUntilNotExist(f.path().string());
                for (auto& sub_path : new_paths) {
                    paths.push_back(sub_path);
                }
            } else {
                paths.push_back(f.path().string());
            }
        }
    }
    return paths;
}

ResourceSystem *ResourceSystem::global() {
    if (!_instance) {
        _instance = std::unique_ptr<ResourceSystem>(new ResourceSystem());
    }
    return _instance.get();
}

void ResourceSystem::setRootPath(const std::string &path) {
    FileSystem::setCurrentPath(path);
}

bool
ResourceSystem::load(const std::string &name, const std::string &path, const Resource::Type &type) {
    if (!append(name, path, type)) return false;
    return load(name);
}

bool ResourceSystem::load(const std::string &name) {
    if (!isContain(name)) {
        SDL_Log("[ERROR] Resource '%s' is not found!", name.c_str());
        return false;
    }
    auto& resource = _resource.at(name);
    if (resource.is_loaded) {
        SDL_Log("[ERROR] Resource '%s' is already loaded!", name.c_str());
        return false;
    }
    if (resource.type == Resource::Text) {
        bool is_error;
        resource.meta_data = FileSystem::readFile(resource.url, false, &is_error);
        if (!is_error) {
            SDL_Log("[ERROR] Resource '%s' loaded failed!", name.c_str());
            return false;
        }
    } else if (resource.type == Resource::Image) {
        resource.meta_data = IMG_Load(resource.url.c_str());
        if (!std::get<SSurface*>(resource.meta_data)) {
            SDL_Log("[ERROR] Resource '%s' loaded failed!\nException: Load image file '%s' failed!\n",
                    name.c_str(), resource.url.c_str());
            return false;
        }
    } else if (resource.type == Resource::Font) {
        TTF_Font* font = TTF_OpenFont(resource.url.c_str(), 12.0f);
        if (!font) {
            SDL_Log("[ERROR] Resource '%s' loaded failed!\nException: Load font file '%s' failed!\n",
                    name.c_str(), resource.url.c_str());
            return false;
        }
        resource.meta_data = reinterpret_cast<void*>(font);
    } else if (resource.type == Resource::Audio) {
        MIX_Audio* audio = MIX_LoadAudio(AudioSystem::global()->mixer(), resource.url.c_str(), true);
        if (!audio) {
            SDL_Log("[ERROR] Resource '%s' loaded failed!\nException: Load audio file '%s' failed!\n",
                    name.c_str(), resource.url.c_str());
            return false;
        }
        resource.meta_data = reinterpret_cast<void*>(audio);
    } else {
        bool ret;
        resource.meta_data = FileSystem::readBinaryFile(resource.url, false, &ret);
        if (!ret) {
            SDL_Log("[ERROR] Resource '%s' loaded failed!\nException: Read binary file '%s' failed!\n",
                    name.c_str(), resource.url.c_str());
            return false;
        }
    }
    resource.is_loaded = true;
    return true;
}

uint64_t ResourceSystem::preload(const std::vector<std::string> &resource_names) {
    uint64_t err = 0;
    if (resource_names.empty()) {
        for (auto& name: _resource) err += (!load(name.first));
    } else {
        for (auto &name: resource_names) err += (!load(name));
    }
    return err;
}

void ResourceSystem::asyncLoad(const std::vector<std::string> &resource_names,
                               const std::function<void(uint64_t)>& callback) {
    const std::vector<std::string>& copy_res_name = resource_names;
    auto is_ok = std::async(std::launch::async, [this, copy_res_name, callback]() -> bool {
        uint64_t ret = 0;
        if (copy_res_name.empty()) {
            for (auto& name : _resource) ret += (!load(name.first));
        } else {
            for (auto& name : copy_res_name) ret += (!load(name));
        }
        if (callback) callback(ret);
        return ret == 0;
    });
}

bool ResourceSystem::unload(const std::string &name) {
    if (!isContain(name)) {
        SDL_Log("[ERROR] Can't find the resource '%s'!", name.c_str());
        return false;
    }
    auto& resource = _resource.at(name);
    if (!resource.is_loaded) return true;
    if (resource.type == Resource::Image) {
        if (std::get<SSurface*>(resource.meta_data))
            SDL_DestroySurface(std::get<SSurface*>(resource.meta_data));
    } else if (resource.type == Resource::Font) {
        auto font = reinterpret_cast<TTF_Font*>(std::get<void*>(resource.meta_data));
        if (font) TTF_CloseFont(font);
    } else if (resource.type == Resource::Audio) {
        auto audio = reinterpret_cast<MIX_Audio*>(std::get<void*>(resource.meta_data));
        if (audio) MIX_DestroyAudio(audio);
    }
    resource.meta_data = {};
    resource.is_loaded = false;
    SDL_Log("[INFO] Unload Resource: '%s'", name.c_str());
    return true;
}

void ResourceSystem::unloadAll() {
    for (auto& res : _resource) {
        unload(res.first);
    }
}

bool
ResourceSystem::append(const std::string &name, const std::string &path, const Resource::Type &type) {
    std::string real_path = FileSystem::getAbsolutePath(path);
    if (FileSystem::isDir(real_path)) {
        SDL_Log("[ERROR] Path '%s' is the directory, not the file!", real_path.c_str());
        return false;
    }
    if (!FileSystem::isFile(real_path)) {
        SDL_Log("[ERROR] File '%s' is not found!", real_path.c_str());
        return false;
    }
    if (isContain(name)) {
        SDL_Log("[ERROR] Resource '%s' is already exist! Did you mean to use `replace()`?", name.c_str());
        return false;
    }
    _resource.emplace(name, Resource{type, real_path, false, {}});
    return true;
}

void ResourceSystem::remove(const std::string &name) {
    if (unload(name)) {
        _resource.erase(name);
    }
}

bool
ResourceSystem::replace(const std::string &name, const std::string &path, const Resource::Type &type) {
    if (!isContain(name)) {
        SDL_Log("[ERROR] Resource '%s' is not found!", name.c_str()); return false;
    }
    unload(name);
    std::string real_path = FileSystem::getAbsolutePath(path);
    if (FileSystem::isDir(real_path)) {
        SDL_Log("[ERROR] Path '%s' is the directory, not the file!", real_path.c_str());
        return false;
    }
    if (!FileSystem::isFile(real_path)) {
        SDL_Log("[ERROR] File '%s' is not found!", real_path.c_str());
        return false;
    }
    _resource.at(name).type = type;
    _resource.at(name).url = real_path;
    _resource.at(name).is_loaded = false;
    _resource.at(name).meta_data = {};
    return true;
}

const std::variant<std::monostate, std::string, SSurface *, char *, void *, std::vector<uint8_t>> &
ResourceSystem::metaData(const std::string &name) const {
    if (!isContain(name)) {
        SDL_Log("[ERROR] Resource '%s' is not found!", name.c_str());
        throw std::runtime_error(fmt::format("[ERROR] Resource '{}' is not found!", name));
    } else if (!_resource.at(name).is_loaded) {
        SDL_Log("[ERROR] Resource '%s' is not loaded!", name.c_str());
        throw std::runtime_error(fmt::format("[ERROR] Resource '{}' is not found!", name));
    }
    return _resource.at(name).meta_data;
}

const std::string &ResourceSystem::resourcePath(const std::string &name) const {
    if (isContain(name)) return _resource.at(name).url;
    SDL_Log("[ERROR] Resource '%s' is not found!", name.c_str());
    return _null_str;
}

Resource::Type ResourceSystem::resourceType(const std::string &name) const {
    if (isContain(name)) return _resource.at(name).type;
    SDL_Log("[ERROR] Resource '%s' is not found!", name.c_str());
    return Resource::None;
}

bool ResourceSystem::isLoaded(const std::string &name) const {
    if (isContain(name)) return _resource.at(name).is_loaded;
    SDL_Log("[ERROR] Resource '%s' is not found!", name.c_str());
    return false;
}

bool ResourceSystem::isContain(const std::string &name) const {
    return _resource.contains(name);
}
