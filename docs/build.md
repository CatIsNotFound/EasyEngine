# Easy Engine 构建向导

此文以最详细地方式来讲述如何通过源代码克隆编译的方式以部署此项目。

### 系统要求

请详细按照如下要求进行检查：

- 编译器：支持 C++23 的现代编译器
- CMake 至少版本为 3.28+
- 依赖库：
    - [SDL3 (图形和窗口管理)](https://github.com/libsdl-org/SDL)
    - [SDL3_image (图像加载)](https://github.com/libsdl-org/SDL_image)
    - [SDL3_ttf (字体渲染)](https://github.com/libsdl-org/SDL_ttf)
    - [SDL3_mixer (音频播放)](https://github.com/libsdl-org/SDL_ttf)
    - [SDL3_gfx (图形处理)](https://github.com/sabdul-khabir/SDL3_gfx)
    - [fmt (格式化库)](https://github.com/fmtlib/fmt)

以上第三方库均可在 Github 上找到对应项目，具体可直接点击上面提到的任意一个库。

TIPS: 可通过以下链接获取对应第三方依赖库下载：

- [Github 下载第三方依赖库](https://github.com/CatIsNotFound/EasyEngine/releases/tag/3rdLibs)
- [Gitee 下载第三方依赖库](https://gitee.com/CatIsNotFound/EasyEngine/releases/tag/3rdLibs)

### 构建步骤

克隆本项目，可通过 Github 或 Gitee 等渠道克隆本项目。

```bash
# 克隆项目（Github 下）
git clone https://github.com/CatIsNotFound/EasyEngine.git
# 克隆项目（Gitee 下）
git clone https://gitee.com/CatIsNotFound/EasyEngine.git
#  进入项目目录
cd EasyEngine
```

编辑 `CMakeLists.txt` 文件，并修改以下内容：

```cmake
# TODO：请记得在此处修改目录
set(SDL_DIR       "/path/to/SDL")
set(SDL_IMAGE_DIR "/path/to/SDL3_image")
set(SDL_TTF_DIR   "/path/to/SDL3_ttf")
set(SDL_MIXER_DIR "/path/to/SDL3_mixer")
set(SDL_GFX_DIR   "/path/to/SDL3_gfx")
set(FMT_DIR       "/path/to/FMT")
```

配置与安装项目，用于编译并安装此第三方库的位置。

```bash
# 配置构建
cmake -B build -DCMAKE_INSTALL_PREFIX=/path/to/install
```

注：这里的 `/path/to/install` 请替换为你希望安装的路径。

编译项目并安装此库

```bash
# 编译项目
cmake --build build --target install

# Linux 下需要执行如下：
sudo cmake --build build --target install
```
