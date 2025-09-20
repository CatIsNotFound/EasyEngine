
#pragma once
#ifndef EASYENGINE_RESOURCES_H
#define EASYENGINE_RESOURCES_H

/**
 * @file Resources.h
 * @brief 资源管理
 *
 * 包含资源系统、文件系统的用于管理资源的主要模块
 */

#include "Basic.h"

namespace EasyEngine {
    /**
     * @class FileSystem
     * @brief 文件系统
     *
     */
    class FileSystem {
    public:
        /**
         * @brief 设置当前的目录
         * @param main_directory 设定目录
         */
        static bool setCurrentPath(const std::string& main_directory);
        /**
         * @brief 获取当前的目录
         *
         */
        static const std::string& currentPath();
        /**
         * @brief 指定文件是否存在
         * @param path 指定文件路径
         * @note 若指定相对目录，则会从主目录下遍历！
         */
        static bool isFile(const std::string& path);
        /**
         * @brief 指定一个目录是否存在？
         * @param path 指定目录
         * @note 若指定相对目录，则会从主目录下遍历！
         */
        static bool isDir(const std::string& path);
        /**
         * @brief 创建一个空目录
         * @param path 指定新的空目录
         * @param ignore_error 忽略目录存在的错误
         * @param recursive_create 是否递归创建目录
         * @return 返回是否成功创建一个空目录
         */
        static bool mkDir(const std::string& path, bool ignore_error = false, bool recursive_create = false);
        /**
         * @brief 删除指定的目录
         * @param path             指定目录
         * @param ignore_error     忽略目录存在的错误
         * @param recursive_remove 递归删除此目录下的所有文件及目录
         * @warning 请不要随意使用 recursive_remove 参数，如指定目录下包含重要文件，使用此参数后将一同删除！
         */
        static bool rmDir(const std::string& path, bool ignore_error = false, bool recursive_remove = false);
        /**
         * @brief 创建一个空白文件
         * @param path                   指定文件路径
         * @param auto_create_directory  是否自动创建目录（当目录不存在时递归创建）
         * @param ignore_error           忽略存在的错误
         *
         */
        static bool mkFile(const std::string &path, bool auto_create_directory = false, bool ignore_error = false);
        /**
         * @brief 删除指定的文件
         * @param path          指定文件路径
         * @param ignore_error  忽略文件存在的错误
         */
        static bool rmFile(const std::string& path, bool ignore_error = false);
        /**
         * @brief 写入内容到指定文件
         * @param context       指定写入内容
         * @param path          指定文件路径
         * @param append_mode   是否使用追加写入
         * @param ignore_error  忽略存在的错误
         */
        static bool writeFile(const std::string &context, const std::string &path,
                              bool append_mode, bool ignore_error = false);
        /**
         * @brief 从指定文件中读取内容
         * @param path          指定文件路径
         * @param ignore_error  忽略存在的错误
         * @param ok            用于检查当前是否存在错误
         * @return 返回读取到的文件的所有内容
         */
        static std::string readFile(const std::string &path, bool ignore_error = false, bool *ok = nullptr);

        /**
         * @brief 写入二进制内容到指定文件
         * @param path              指定输出文件路径
         * @param append_mode       是否使用追加写入模式
         * @param how2WriteFile     指定写入文件的方法
         * @param ignore_error      忽略存在的错误
         * @return 返回是否成功写入文件
         */
        static bool writeBinaryFile(const std::string& path, bool append_mode,
                                    const std::function<void(std::ofstream& file)>& how2WriteFile,
                                    bool ignore_error = false);
        /**
         * @brief 写入二进制内容到指定文件中
         * @param binaries      指定存储的二进制内容
         * @param path          指定文件输出路径
         * @param append_mode   指定是否使用追加写入模式
         * @param ignore_error  忽略存在的错误
         * @return 返回是否成功写入文件
         */
        static bool writeBinaryFile(const std::vector<uint8_t>& binaries, const std::string& path,
                                    bool append_mode, bool ignore_error = false);

        /**
         * @brief 从指定文件中读取二进制内容
         * @param path          指定文件路径
         * @param how2ReadFile  指定读取文件的方法
         * @param ignore_error  忽略存在的错误
         * @return 返回是否成功读取二进制内容
         */
        static bool readBinaryFile(const std::string& path,
                       const std::function<void(std::ifstream& file)>& how2ReadFile,
                       bool ignore_error = false);
        /**
         * @brief 从指定文件中读取二进制内容
         * @param path          指定文件路径
         * @param ignore_error  忽略存在的错误
         * @param ok            执行是否存在问题
         * @return 返回完整的二进制内容
         */
        static std::vector<uint8_t> readBinaryFile(const std::string& path, bool ignore_error = false, bool* ok = nullptr);
        /**
         * @brief 获取绝对路径
         * @param path 指定路径
         * @return 将返回完整的绝对路径
         */
        static std::string getAbsolutePath(const std::string& path);

        FileSystem() = delete;
        FileSystem(FileSystem&) = delete;
        FileSystem& operator=(const FileSystem&) = delete;
        ~FileSystem() = delete;
    private:
        static std::deque<std::string> getPathUntilExist(const std::string& path);
        static std::deque<std::string> getPathUntilNotExist(const std::string &path);
        static std::string _main_path;
    };

    /**
     * @class ResourceSystem
     * @brief 资源系统
     *
     */
    class ResourceSystem {
    public:
        /**
         * @struct Resource
         * @brief 资源
         */
        struct Resource {
            /**
             * @enum Type
             * @brief 资源类型
             */
            enum Type {
                /// 无，用于占位
                None,
                /// 文本文档
                Text = 0x10,
                /// 图片
                Image = 0x20,
                /// 字体
                Font = 0x30,
                /// 音频
                Audio = 0x40,
                /// 视频
                Video = 0x50,
                /// 其它二进制文件
                Binary = 0x60
            };
            /// 资源类型
            Type type;
            /// 资源所在路径
            std::string url;
            /// 是否加载
            bool is_loaded;
            /// 元数据
            std::variant<std::monostate, std::string, SSurface*, char*, void*, std::vector<uint8_t>> meta_data;
        };
    public:
        /**
         * @brief 获取全局资源系统
         */
        static ResourceSystem* global();
        /**
         * @brief 设置根目录
         * @param path 指定目录
         *
         * 用于设置整个资源系统的根目录
         */
        void setRootPath(const std::string& path);
        /**
         * @brief 从文件系统中加载资源
         * @param name 命名新的资源名称
         * @param path 指定路径
         * @param type 指定资源类型
         * @return 返回是否成功载入资源
         */
        bool load(const std::string &name, const std::string &path, const Resource::Type &type);
        /**
         * @brief 指定现有的资源并加载
         * @param name 指定的资源名称
         * @return 返回是否成功加载资源
         */
        bool load(const std::string &name);
        /**
         * @brief 预加载多个现有的资源
         * @param resource_names 指定的资源名列表
         * @return 返回加载失败的个数。若为 0，则加载所有资源成功！
         * @see load
         * @see asyncLoad
         */
        uint64_t preload(const std::vector<std::string>& resource_names);
        /**
         * @brief 异步加载多个现有的资源
         * @param resource_names 指定的资源名列表
         * @return 返回加载失败的个数。若为 0，则加载所有资源成功！
         * @see load
         * @see preload
         */
        uint64_t asyncLoad(const std::vector<std::string>& resource_names);
        /**
         * @brief 卸载指定资源
         * @param name 指定卸载的资源名
         */
        bool unload(const std::string& name);
        /**
         * @brief 卸载全部资源
         */
        void unloadAll();
        /**
         * @brief 从文件系统中添加资源
         * @param name 命名新的资源名称
         * @param path 指定路径
         * @param type 指定资源类型
         */
        bool append(const std::string &name, const std::string &path, const ResourceSystem::Resource::Type &type);
        /**
         * @brief 从资源系统中移除指定资源
         * @param name 指定的资源名称
         *
         * 将自动卸载并完全移除该资源
         */
        void remove(const std::string &name);
        /**
         * @brief 指定现有的资源并替换
         * @param name 指定的资源名称
         * @param path 指定新的资源路径
         * @param type 指定新的资源类型
         */
        bool replace(const std::string &name, const std::string &path, const Resource::Type &type);
        /**
         * @brief 从指定资源中获取元数据
         * @param name 指定的资源名称
         * @return 返回对应的元数据
         */
        const std::variant<std::monostate, std::string, SSurface *, char *, void *, std::vector<uint8_t>>&
                    metaData(const std::string &name) const;
        /**
         * @brief 从指定资源中获取资源所在路径
         */
        const std::string &resourcePath(const std::string &name) const;
        /**
         * @brief 查看指定资源是否被加载
         * @param name 指定的资源名称
         */
        bool isLoaded(const std::string &name) const;
        /**
         * @brief 查看当前资源系统中是否存在指定资源
         * @param name 指定查找的资源名
         */
        bool isContain(const std::string &name) const;

        ResourceSystem(ResourceSystem&) = delete;
        ResourceSystem& operator=(const ResourceSystem&) = delete;
    private:
        explicit ResourceSystem() = default;
        static std::unique_ptr<ResourceSystem> _instance;
        std::map<std::string, Resource> _resource;
        std::string _null_str{};
    };
}

#endif //EASYENGINE_RESOURCES_H
