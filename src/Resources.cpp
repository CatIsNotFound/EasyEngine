
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
    std::filesystem::path temp = (path.front() == '.' ?
            fmt::format("{}{}", _main_path, path.substr(path.find_first_of('/'))) : path);
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
    std::filesystem::path temp = (path.front() == '.' ?
             fmt::format("{}{}", _main_path, path.substr(path.find_first_of('/'))) : path);
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
    std::filesystem::path temp = (path.front() == '.' ?
            fmt::format("{}{}", _main_path, path.substr(path.find_first_of('/'))) : path);
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
    std::filesystem::path temp = (path.front() == '.' ?
            fmt::format("{}{}", _main_path, path.substr(path.find_first_of('/'))) : path);
    if (std::filesystem::is_regular_file(temp)) {
        std::filesystem::remove(temp);
        return true;
    }
    if (!ignore_error) fmt::println("[ERROR] Can't remove the file '{}'", temp.string());
    return false;
}

bool FileSystem::writeFile(const std::string &context, const std::string &path, bool append_mode, bool ignore_error) {
    std::filesystem::path temp = (path.front() == '.' ?
            fmt::format("{}{}", _main_path, path.substr(path.find_first_of('/'))) : path);
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
    std::filesystem::path temp = (path.front() == '.' ?
            fmt::format("{}{}", _main_path, path.substr(path.find_first_of('/'))) : path);
    std::ifstream file(temp.string(), std::ios::in);
    if (!file.is_open()) {
        if (!ignore_error) fmt::println("[ERROR] File '{}' is not found!", temp.string());
        if (ok) *ok = false;
        return "";
    }
    std::string output;
    char buf[1024] = {'\0'};
    while (true) {
        file.getline(buf, 1024);
        output += buf;
        if (!file.eof()) output += '\n'; else break;
    }
    file.close();
    if (ok) *ok = true;
    return output;
}

bool FileSystem::writeBinaryFile(const std::string &path, bool append_mode,
                                 const std::function<void(std::ofstream &)> &how2WriteFile, bool ignore_error) {
    std::filesystem::path temp = (path.front() == '.' ?
                                  fmt::format("{}{}", _main_path, path.substr(path.find_first_of('/'))) : path);
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

bool FileSystem::readBinaryFile(const std::string &path, const std::function<void(std::ifstream &)> &how2ReadFile,
                                bool ignore_error) {
    std::filesystem::path temp = (path.front() == '.' ?
            fmt::format("{}{}", _main_path, path.substr(path.find_first_of('/'))) : path);
    std::ifstream file(temp.string(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        if (!ignore_error) fmt::println("[ERROR] File '{}' is not found!", temp.string());
        return false;
    }
    if (how2ReadFile) how2ReadFile(file);
    file.close();
    return true;
}

const char* FileSystem::getAbsolutePath(const std::string &path) {
    return (path.front() == '.' ? fmt::format("{}{}", _main_path, path.substr(path.find_first_of('/'))) 
            : path).c_str();
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
ResourceSystem::load(const std::string &name, const std::string &path) {
    std::string real_path = FileSystem::getAbsolutePath(path);
    if (FileSystem::isDir(real_path)) {
        fmt::println("[ERROR] Path '{}' is the directory, not the file!", real_path);
        return false;
    }
    if (!FileSystem::isFile(real_path)) {
        fmt::println("[ERROR] File '{}' is not found!", real_path);
        return false;
    }

    return true;
}

void ResourceSystem::unload(const std::string &name) {
    if (_resource.contains(name)) {
        _resource.erase(name);
        return;
    }
    fmt::println("[ERROR] Can't find the resource \'{}\'!", name);
}

const ResourceSystem::Resource &ResourceSystem::resource(const std::string &name) {
    if (_resource.contains(name))
        return _resource[name];
    else
        throw std::runtime_error(fmt::format("[ERROR] Resourse {} is not found!", name));
}
