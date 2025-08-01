#!/bin/bash
set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
echo "脚本目录: ${SCRIPT_DIR}"

cd ${SCRIPT_DIR}/docker

echo "🐳 启动 Docker 开发环境..."
docker compose up -d --build

echo "⏳ 等待容器启动..."
sleep 3

echo "📦 在容器中执行构建脚本..."
docker exec -it my-cpp-dev /workspace/auto_build.sh --all

echo ""
echo "🎉 Docker 构建完成!"
echo ""
echo "💡 提示:"
echo "   - 使用 'docker exec -it my-cpp-dev bash' 进入容器"
echo "   - 使用 'docker compose -f ${SCRIPT_DIR}/docker/docker-compose.yaml down' 停止容器"