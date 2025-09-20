#!/bin/bash

# 此脚本仅用于下载并编译所需的依赖库
#
# P.S: 执行此脚本前，请检查当前的网络环境是否良好！
#

if [[ $1 == "-h" || $1 == "--help" ]];then
    echo "此脚本仅用于下载并编译所需的依赖库！"
    echo -e "Tips: 编译之前，请编辑 \033[1mvars.sh\033[0m 文件进行配置！"
fi


# 导入设置脚本变量
if [ -f "./vars.sh" ];then
    source ./vars.sh
fi

function print() {
    echo -e "\033[1;32m[`date +%X`]\033[0m \033[1m$*\033[0m"
}

function printError() {
    echo -e "\033[1;31m[`date +%X`]\033[0m \033[33m$*\033[0m"
}

function install_pkg() {
    print "开始下载安装 $* 软件包..."
    if [ -f "/usr/bin/apt" ]; then
        sudo apt-get update
        if [ $? -ne 0 ];then
            return 1
        fi
        sudo apt-get install $* -y > /dev/null
    elif [ -f "/usr/bin/dnf" ]; then
        sudo dnf makecache
        if [ $? -ne 0 ];then
            return 1
        fi
        sudo dnf install $* -y > /dev/null
    elif [ -f "/usr/bin/yum" ]; then
        sudo yum makecache
        if [ $? -ne 0 ];then
            return 1
        fi
        sudo yum install $* -y > /dev/null
    elif [ -f "/usr/bin/pacman" ]; then
        sudo pacman -Sy --no-confirm $* > /dev/null
    fi
    return $?
}

function cloneRepo() {
    print "正在克隆项目..."
    git clone $1
    return $?
}

function compile_install() {
    cd $1
    if [ $2 ];then
        git pull
        if [ $? -ne 0 ];then
            printError "错误：克隆项目时出现问题！"
            exit 6
        fi
    fi
    mkdir -p build
    cd build
    if [ $THIRD_PARTY_PATH ];then
        cmake .. -G "Ninja" -DCMAKE_INSTALL_PREFIX=$THIRD_PARTY_PATH
    else
        cmake .. -G "Ninja"
    fi
    if [ $? -ne 0 ];then
        printError "错误：配置项目时出现问题！"
        return 4
    fi
    ninja install
    if [ $? -ne 0 ];then
        printError "错误：编译项目时出现问题！"
        return 5
    fi
    cd ../../
    return 0
}

if [ ! -f "/usr/bin/git" ]; then
    install_pkg "git"
    if [ $? -ne 0 ];then
      printError "错误：下载时出现错误！构建终止..."
      exit 1
    fi
fi

if [ $CLONE_DIR ];then
     cd $CLONE_DIR
     if [ $? -ne 0 ];then
        printError "错误：指定克隆的路径不存在或无法访问！"
        exit 2
     fi
fi

# 远程仓库克隆依赖库
if [ $CLONE_REPO ];then
    if [ $SDL_GIT ];then
        cloneRepo $SDL_GIT
        if [ $? -ne 0 ];then
            printError "错误：克隆项目时出现问题！构建终止..."
            exit 3
        fi
    fi
    if [ $SDL_MIXER_GIT ];then
        cloneRepo $SDL_MIXER_GIT
        if [ $? -ne 0 ];then
            printError "错误：克隆项目时出现问题！构建终止..."
            exit 3
        fi
    fi
    if [ $SDL_TTF_GIT ];then
        cloneRepo $SDL_TTF_GIT
        if [ $? -ne 0 ];then
            printError "错误：克隆项目时出现问题！构建终止..."
            exit 3
        fi
    fi
    if [ $SDL_IMAGE_GIT ];then
        cloneRepo $SDL_IMAGE_GIT
        if [ $? -ne 0 ];then
            printError "错误：克隆项目时出现问题！构建终止..."
            exit 3
        fi
    fi
    if [ $FMT_GIT ];then
        cloneRepo $FMT_GIT
        if [ $? -ne 0 ];then
            printError "错误：克隆项目时出现问题！构建终止..."
            exit 3
        fi
    fi
    if [ $XML_GIT ];then
        cloneRepo $XML_GIT
        if [ $? -ne 0 ];then
            printError "错误：克隆项目时出现问题！构建终止..."
            exit 3
        fi
    fi
    if [ $JSON_GIT ];then
        cloneRepo $JSON_GIT
        if [ $? -ne 0 ];then
            printError "错误：克隆项目时出现问题！构建终止..."
            exit 3
        fi
    fi
fi


# 检查编译套件
if [ ! -f /usr/bin/cmake ];then
    install_pkg cmake ninja gcc g++ gdb make
fi

function readyToCompile() {
    for i in $*; do
        compile_install $i $CHECK_UPDATE_FROM_REPO
        if [ $? -ne 0 ];then
            printError "错误：配置或编译项目失败！"
            exit 32
        fi
    done
}

# 尝试编译每个项目
readyToCompile SDL3 SDL_mixer SDL_ttf SDL_image fmt pugixml json

print "已完成下载依赖库构建！"