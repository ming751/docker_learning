#!/bin/bash
set -e #遇到错误立即退出

helpFunction()
{
    echo "===== 帮助信息 ====="
    echo "用法: $0 [选项]"
    echo
    echo "选项:"
    echo "  --all                       执行所有构建步骤"
    echo "  --build                     仅构建项目"
    echo "  --test                      仅运行测试"
    echo "  --run                       仅运行主程序"
    echo "  -h, --help                  显示此帮助信息"
}

if [ "$#" -eq 0 ]; then
    helpFunction
    exit 1
fi

# 定义选项
SHORT_OPTIONS="h"
LONG_OPTIONS="all,build,test,run,help"

# 使用 getopt 解析参数
PARSED=$(getopt -o "$SHORT_OPTIONS" -l "$LONG_OPTIONS" -n "$0" -- "$@")

if [ $? -ne 0 ]; then
    echo "参数解析错误"
    helpFunction
    exit 2
fi

eval set -- "$PARSED"

# 初始化变量
BUILD_PROJECT=false
RUN_TESTS=false
RUN_PROGRAM=false

# 处理所有选项
while true; do
    case "$1" in
        --all)
            BUILD_PROJECT=true
            RUN_TESTS=true
            RUN_PROGRAM=true
            echo "执行所有构建步骤"
            shift
            ;;
        --build)
            BUILD_PROJECT=true
            echo "仅构建项目"
            shift
            ;;
        --test)
            RUN_TESTS=true
            echo "仅运行测试"
            shift
            ;;
        --run)
            RUN_PROGRAM=true
            echo "仅运行主程序"
            shift
            ;;
        -h|--help)
            helpFunction
            exit 0
            ;;
        --)
            shift
            break
            ;;
        *)
            echo "遇到未知选项: $1"
            exit 3
            ;;
    esac
done

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
echo "脚本目录: ${SCRIPT_DIR}"

# 切换到项目根目录
cd ${SCRIPT_DIR}

if [ ! -f "CMakeLists.txt" ]; then
    echo "❌ 错误: 找不到 CMakeLists.txt"
    exit 1
fi

width=$(tput cols)
separator_line=$(printf '%*s' "$width" '' | tr ' ' '=')
separationLine()
{
    echo "$separator_line"
    echo "$1"
    echo "$separator_line"
}

if [ "${BUILD_PROJECT}" == true ]; then
    separationLine "构建 C++ 项目"
    
    mkdir -p build
    cd build
    
    if [ -f "CMakeCache.txt" ]; then
        rm "./CMakeCache.txt"
    fi
    
    echo "⚙️  配置项目..."
    cmake ..
    
    echo "🔨 编译项目..."
    nice -n20 make -j$(nproc)
    
    echo "✅ 构建完成!"
    echo "📍 可执行文件位置: $(pwd)/my_app"
    echo "📍 测试文件位置: $(pwd)/test_math_utils"
    
    cd ${SCRIPT_DIR}
fi

if [ "${RUN_PROGRAM}" == true ]; then
    separationLine "运行主程序"
    cd ${SCRIPT_DIR}/build
    ./my_app
fi

if [ "${RUN_TESTS}" == true ]; then
    separationLine "运行测试"
    cd ${SCRIPT_DIR}/build
    ./test_math_utils
    echo ""
    echo "运行 CTest:"
    ctest --verbose
fi

echo ""
separationLine "所有任务完成!"