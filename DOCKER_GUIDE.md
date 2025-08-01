# 🐳 Docker 详细工作流程指南

本文档详细解释本项目中Docker的工作流程、命令含义和最佳实践。

## 📋 目录

- [Docker 基本概念](#docker-基本概念)
- [项目Docker架构](#项目docker架构)
- [Dockerfile 详解](#dockerfile-详解)
- [Docker Compose 详解](#docker-compose-详解)
- [脚本命令详解](#脚本命令详解)
- [工作流程分析](#工作流程分析)
- [常用Docker命令](#常用docker命令)
- [故障排除](#故障排除)

## 🔧 Docker 基本概念

### 核心概念

1. **镜像 (Image)**: 只读的模板，包含运行应用所需的代码、运行时、库、环境变量和配置文件
2. **容器 (Container)**: 镜像的运行实例，是轻量级、可移植的执行环境
3. **Dockerfile**: 用于构建镜像的文本文件，包含一系列指令
4. **Docker Compose**: 用于定义和运行多容器Docker应用的工具

### 优势

- ✅ **环境一致性**: 开发、测试、生产环境完全一致
- ✅ **快速部署**: 秒级启动，比虚拟机快得多
- ✅ **资源高效**: 共享主机内核，资源占用少
- ✅ **版本控制**: 镜像可以版本化管理
- ✅ **隔离性**: 应用间相互隔离，互不影响

## 🏗️ 项目Docker架构

```
项目架构图:
┌─────────────────────────────────────────────────────────────┐
│                        宿主机 (Host)                          │
│  ┌─────────────────────────────────────────────────────────┐ │
│  │                Docker 容器 (Container)                   │ │
│  │  ┌─────────────────────────────────────────────────────┐ │ │
│  │  │              Ubuntu 20.04 环境                      │ │ │
│  │  │  ┌─────────────────────────────────────────────────┐ │ │ │
│  │  │  │           开发工具                               │ │ │ │
│  │  │  │  • GCC/G++                                     │ │ │ │
│  │  │  │  • CMake                                       │ │ │ │
│  │  │  │  • Git                                         │ │ │ │
│  │  │  │  • GDB (调试器)                                 │ │ │ │
│  │  │  │  • Valgrind (内存检查)                          │ │ │ │
│  │  │  │  • clang-format (代码格式化)                     │ │ │ │
│  │  │  └─────────────────────────────────────────────────┘ │ │ │
│  │  │                                                       │ │ │
│  │  │  工作目录: /workspace                                  │ │ │
│  │  │  用户: developer (非root，安全)                        │ │ │
│  │  └─────────────────────────────────────────────────────┘ │ │
│  └─────────────────────────────────────────────────────────┘ │
│                                                               │
│  卷映射 (Volume Mounts):                                      │
│  • 项目目录 → /workspace                                      │
│  • ccache缓存 → /workspace/.ccache                           │
│  • Git配置 → /home/developer/.gitconfig                      │
└─────────────────────────────────────────────────────────────┘
```

## 📄 Dockerfile 详解

让我们逐行分析 `docker/ubuntu-local/Dockerfile`:

```dockerfile
# 基于 Ubuntu 20.04 LTS
FROM ubuntu:focal
```
**作用**: 指定基础镜像。`ubuntu:focal` 是Ubuntu 20.04 LTS的代号。

```dockerfile
# 设置非交互式环境（避免安装时的交互提示）
ENV DEBIAN_FRONTEND=noninteractive
```
**作用**: 设置环境变量，防止apt安装包时出现交互式提示，确保自动化安装。

```dockerfile
# 设置时区
ENV TZ=Asia/Shanghai
```
**作用**: 设置容器时区为上海时区（东八区）。

```dockerfile
# 创建非root用户（安全最佳实践）
RUN useradd -m -s /bin/bash developer && \
    usermod -aG sudo developer
```
**作用**: 
- `useradd -m -s /bin/bash developer`: 创建用户developer，创建家目录(-m)，设置shell为bash(-s)
- `usermod -aG sudo developer`: 将developer用户添加到sudo组，获得管理员权限

```dockerfile
# 更新包列表并安装开发工具
RUN apt-get update && apt-get install -y \
    # 基础构建工具
    build-essential \
    cmake \
    ninja-build \
    pkg-config \
    # 版本控制
    git \
    # 调试和分析工具
    gdb \
    valgrind \
    # 代码质量工具
    clang \
    clang-format \
    clang-tidy \
    # 编译缓存
    ccache \
    # 实用工具
    curl \
    wget \
    vim \
    nano \
    tree \
    htop \
    # 清理缓存
    && rm -rf /var/lib/apt/lists/*
```
**作用**: 安装所有必需的开发工具和实用程序，最后清理包缓存以减小镜像大小。

```dockerfile
# 设置 ccache 路径（加速重复编译）
ENV PATH=/usr/lib/ccache:$PATH
ENV CCACHE_DIR=/workspace/.ccache
```
**作用**: 配置ccache编译缓存，加速重复编译过程。

```dockerfile
# 配置 Git（可选）
RUN git config --global init.defaultBranch main
```
**作用**: 设置Git默认分支名为main。

```dockerfile
# 验证安装是否成功
RUN gcc --version && \
    g++ --version && \
    cmake --version && \
    clang --version
```
**作用**: 验证关键工具安装成功，如果失败会导致镜像构建失败。

```dockerfile
# 设置工作目录
WORKDIR /workspace
```
**作用**: 设置容器内的默认工作目录。

```dockerfile
# 切换到非root用户
USER developer
```
**作用**: 切换到非特权用户，提高安全性。

```dockerfile
# 设置默认命令
CMD ["/bin/bash"]
```
**作用**: 设置容器启动时的默认命令。

## 🐙 Docker Compose 详解

让我们分析 `docker/docker-compose.yaml`:

```yaml
version: '3.8'   # Docker Compose 版本
```
**作用**: 指定Docker Compose文件格式版本。

```yaml
services:        # 定义服务
  dev-environment:  # 服务名称
```
**作用**: 定义服务，可以有多个服务组成一个应用栈。

```yaml
    build:          # 构建配置
      context: ./ubuntu-local   # Dockerfile所在目录
      args:
        - USER_ID=${USER_ID:-1000}
        - GROUP_ID=${GROUP_ID:-1000}
```
**作用**: 
- `context`: 指定构建上下文目录
- `args`: 构建参数，可以传递给Dockerfile中的ARG指令

```yaml
    image: my-cpp-project:ubuntu-focal  # 生成的镜像名称
    container_name: my-cpp-dev          # 容器名称
```
**作用**: 指定镜像名称和容器名称。

```yaml
    volumes:
      # 挂载项目目录到容器
      - ..:/workspace
      # 挂载 ccache 缓存目录（加速编译）
      - ccache-data:/workspace/.ccache
      # 挂载 Git 配置（可选）
      - ${HOME}/.gitconfig:/home/developer/.gitconfig:ro
```
**作用**: 卷挂载配置
- `..:/workspace`: 将项目根目录挂载到容器的/workspace
- `ccache-data:/workspace/.ccache`: 持久化编译缓存
- `${HOME}/.gitconfig:/home/developer/.gitconfig:ro`: 只读挂载Git配置

```yaml
    working_dir: /workspace
```
**作用**: 设置容器启动时的工作目录。

```yaml
    environment:
      # 设置环境变量
      - TERM=xterm-256color
      - CCACHE_DIR=/workspace/.ccache
```
**作用**: 设置容器内的环境变量。

```yaml
    # 保持容器运行
    tty: true           # 分配伪终端
    stdin_open: true    # 保持标准输入开放
```
**作用**: 
- `tty: true`: 分配一个伪终端
- `stdin_open: true`: 保持STDIN开放，即使没有附加

```yaml
    restart: unless-stopped
```
**作用**: 重启策略，除非手动停止，否则总是重启。

```yaml
volumes:
  ccache-data:  # 持久化 ccache 数据
```
**作用**: 定义命名卷，用于持久化数据。

## 🔍 脚本命令详解

### docker_build.sh 详解

```bash
#!/bin/bash
set -e
```
**作用**: 
- `#!/bin/bash`: 指定使用bash解释器
- `set -e`: 遇到错误立即退出

```bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
echo "脚本目录: ${SCRIPT_DIR}"
```
**作用**: 获取脚本所在的绝对路径，无论从哪里调用脚本都能正确工作。

```bash
cd ${SCRIPT_DIR}/docker
```
**作用**: 切换到docker目录，因为docker-compose.yaml在这里。

```bash
docker compose up -d --build
```
**作用**: 
- `docker compose up`: 启动服务
- `-d`: 后台运行（detached模式）
- `--build`: 强制重新构建镜像

```bash
docker exec -it my-cpp-dev /workspace/auto_build.sh --all
```
**作用**: 
- `docker exec`: 在运行的容器中执行命令
- `-it`: 交互式终端
- `my-cpp-dev`: 容器名称
- `/workspace/auto_build.sh --all`: 要执行的命令

### auto_build.sh 详解

```bash
PARSED=$(getopt -o "$SHORT_OPTIONS" -l "$LONG_OPTIONS" -n "$0" -- "$@")
```
**作用**: 使用getopt解析命令行参数，支持短选项和长选项。

```bash
width=$(tput cols)
separator_line=$(printf '%*s' "$width" '' | tr ' ' '=')
```
**作用**: 
- `tput cols`: 获取终端宽度
- `printf '%*s' "$width" ''`: 创建指定宽度的空字符串
- `tr ' ' '='`: 将空格替换为等号，创建分隔线

```bash
nice -n20 make -j$(nproc)
```
**作用**: 
- `nice -n20`: 以低优先级运行，不影响系统其他进程
- `make -j$(nproc)`: 并行编译，使用所有CPU核心

## 🔄 工作流程分析

### 完整工作流程

```
1. 用户执行: ./docker_build.sh
   ↓
2. 脚本获取自身路径: SCRIPT_DIR
   ↓
3. 切换到docker目录: cd ${SCRIPT_DIR}/docker
   ↓
4. 启动Docker服务: docker compose up -d --build
   ├─ 构建镜像 (如果不存在或需要更新)
   ├─ 创建网络
   ├─ 创建卷
   └─ 启动容器
   ↓
5. 等待容器启动: sleep 3
   ↓
6. 在容器中执行构建: docker exec -it my-cpp-dev /workspace/auto_build.sh --all
   ├─ 解析命令行参数
   ├─ 创建build目录
   ├─ 运行cmake配置
   ├─ 编译项目
   ├─ 运行主程序
   └─ 运行测试
   ↓
7. 构建完成，显示提示信息
```

### 镜像构建过程

```
Docker镜像构建流程:
1. FROM ubuntu:focal
   └─ 下载Ubuntu 20.04基础镜像
   
2. 安装系统包
   ├─ 更新包列表: apt-get update
   ├─ 安装开发工具: build-essential, cmake, git...
   └─ 清理缓存: rm -rf /var/lib/apt/lists/*
   
3. 配置用户和环境
   ├─ 创建developer用户
   ├─ 设置环境变量
   └─ 配置工具
   
4. 设置工作目录和默认命令
   ├─ WORKDIR /workspace
   ├─ USER developer
   └─ CMD ["/bin/bash"]
```

### 容器运行过程

```
容器运行流程:
1. docker compose up -d --build
   ├─ 读取docker-compose.yaml
   ├─ 构建或拉取镜像
   ├─ 创建网络: docker_default
   ├─ 创建卷: docker_ccache-data
   └─ 启动容器: my-cpp-dev
   
2. 卷挂载
   ├─ 项目目录 → /workspace
   ├─ ccache缓存 → /workspace/.ccache
   └─ Git配置 → /home/developer/.gitconfig
   
3. 容器启动
   ├─ 以developer用户身份运行
   ├─ 工作目录: /workspace
   └─ 保持运行状态 (tty: true, stdin_open: true)
```

## 🛠️ 常用Docker命令

### 镜像管理

```bash
# 查看本地镜像
docker images

# 删除镜像
docker rmi my-cpp-project:ubuntu-focal

# 构建镜像
docker build -t my-cpp-project:ubuntu-focal docker/ubuntu-local/

# 清理未使用的镜像
docker image prune
```

### 容器管理

```bash
# 查看运行中的容器
docker ps

# 查看所有容器（包括停止的）
docker ps -a

# 启动容器
docker start my-cpp-dev

# 停止容器
docker stop my-cpp-dev

# 删除容器
docker rm my-cpp-dev

# 进入容器
docker exec -it my-cpp-dev bash

# 查看容器日志
docker logs my-cpp-dev

# 查看容器详细信息
docker inspect my-cpp-dev
```

### Docker Compose 命令

```bash
# 启动服务（前台）
docker compose -f docker/docker-compose.yaml up

# 启动服务（后台）
docker compose -f docker/docker-compose.yaml up -d

# 停止并删除服务
docker compose -f docker/docker-compose.yaml down

# 重新构建并启动
docker compose -f docker/docker-compose.yaml up --build

# 查看服务状态
docker compose -f docker/docker-compose.yaml ps

# 查看服务日志
docker compose -f docker/docker-compose.yaml logs

# 在服务中执行命令
docker compose -f docker/docker-compose.yaml exec dev-environment bash
```

### 卷和网络管理

```bash
# 查看卷
docker volume ls

# 删除卷
docker volume rm docker_ccache-data

# 查看网络
docker network ls

# 删除网络
docker network rm docker_default
```

## 🔧 故障排除

### 常见问题及解决方案

#### 1. 容器启动失败

**问题**: `docker compose up` 失败
```bash
# 查看详细错误信息
docker compose -f docker/docker-compose.yaml logs

# 检查镜像是否存在
docker images | grep my-cpp-project

# 强制重新构建
docker compose -f docker/docker-compose.yaml up --build --force-recreate
```

#### 2. 文件权限问题

**问题**: 容器内无法访问挂载的文件
```bash
# 检查文件权限
ls -la

# 修改文件权限
chmod +x docker_build.sh auto_build.sh

# 检查用户ID匹配
docker exec -it my-cpp-dev id
```

#### 3. 编译错误

**问题**: 编译失败
```bash
# 清理构建缓存
docker exec -it my-cpp-dev bash -c "cd /workspace && rm -rf build"

# 检查依赖
docker exec -it my-cpp-dev bash -c "gcc --version && cmake --version"

# 手动调试
docker exec -it my-cpp-dev bash
cd /workspace
mkdir build && cd build
cmake ..
make VERBOSE=1
```

#### 4. 网络问题

**问题**: 无法访问外网
```bash
# 检查DNS
docker exec -it my-cpp-dev nslookup google.com

# 重启Docker服务
sudo systemctl restart docker
```

#### 5. 磁盘空间不足

**问题**: 磁盘空间不足
```bash
# 清理未使用的镜像
docker image prune -a

# 清理未使用的容器
docker container prune

# 清理未使用的卷
docker volume prune

# 清理所有未使用的资源
docker system prune -a
```

### 调试技巧

```bash
# 1. 查看容器资源使用情况
docker stats my-cpp-dev

# 2. 进入容器调试
docker exec -it my-cpp-dev bash

# 3. 检查挂载点
docker exec -it my-cpp-dev df -h

# 4. 查看环境变量
docker exec -it my-cpp-dev env

# 5. 检查网络连接
docker exec -it my-cpp-dev ping google.com
```

## 📚 最佳实践

### 1. 镜像优化
- 使用多阶段构建减小镜像大小
- 清理包管理器缓存
- 合并RUN指令减少层数

### 2. 安全性
- 使用非root用户运行容器
- 只读挂载敏感文件
- 定期更新基础镜像

### 3. 性能优化
- 使用编译缓存(ccache)
- 持久化数据卷
- 合理设置资源限制

### 4. 开发效率
- 热重载代码变更
- 统一开发环境
- 自动化构建和测试

---

**🎉 恭喜！您现在已经掌握了Docker的核心概念和本项目的完整工作流程！**

如果您有任何问题或需要进一步的解释，请随时询问！