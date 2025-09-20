
# 【远程仓库】
# 这里将定义远程仓库变量，
# 你可以完全修改对应 Git 的地址，这样确实能加快网络下载速度！
# 若有不需要克隆的远程仓库，请直接注释当前行即可
SDL_GIT="https://github.com/libsdl-org/SDL.git"
SDL_MIXER_GIT="https://github.com/libsdl-org/SDL_mixer.git"
SDL_TTF_GIT="https://github.com/libsdl-org/SDL_ttf.git"
SDL_IMAGE_GIT="https://github.com/libsdl-org/SDL_image.git"

FMT_GIT="https://github.com/fmtlib/fmt.git"
XML_GIT="https://github.com/zeux/pugixml.git"
JSON_GIT="https://github.com/nlohmann/json.git"


# 【构建选项】
# 下方选项影响了整个构建脚本的运行，
# 如有不需要执行的，请直接注释当前行即可

# 定义默认克隆项目路径，所有克隆项目都将被克隆至此目录
# CLONE_DIR=""

# 是否克隆依赖库？若已安装过，请注释此行
CLONE_REPO=1

# 从远程仓库中检查更新？对于需要更新第三方项目的，请取消注释此行！
# CHECK_UPDATE_FROM_REPO=1

# 定义安装依赖库的路径，注释此行后，将安装至默认路径
# THIRD_PARTY_PATH=
