# 🐳 C++ Docker 开发环境实践项目

这是一个完整的 C++ Docker 开发环境示例项目，展示了如何使用 Docker 容器化 C++ 开发流程。

## 📋 项目特性

- ✅ 完整的 C++ 项目结构
- 🐳 Docker 容器化开发环境
- 📊 数学工具库示例
- 🔧 多种构建和调试脚本
- 🎨 代码格式化和静态分析
- 📦 CMake 构建系统
- 🚀 一键构建和运行

## 📁 项目结构

```
docker_learning/
├── README.md                 # 项目说明文档
├── CMakeLists.txt           # CMake 构建配置
├── Makefile                 # 统一的构建和管理工具
├── .gitignore               # Git 忽略文件
│
├── src/                     # 源代码目录
│   ├── main.cpp            # 主程序
│   └── math_utils.cpp      # 数学工具库实现
│
├── include/                 # 头文件目录
│   └── my_project/
│       └── math_utils.hpp  # 数学工具库头文件
│
├── tests/                   # 测试目录
│   └── test_math_utils.cpp # 单元测试
│
└── docker/                  # Docker 配置
    ├── docker-compose.yaml # Docker Compose 配置
    └── ubuntu-local/
        └── Dockerfile      # Docker 镜像定义
```

## 🚀 快速开始

### 前置要求

- Docker
- Docker Compose

### 1. 克隆项目

```bash
git clone <repository-url>
cd docker_learning
```

### 2. 构建并运行

```bash
# 一键 Docker 构建（推荐）
./docker_build.sh

# 或者在容器内手动构建
./auto_build.sh --all
```

## 🛠️ 脚本说明

### 主要脚本

- **`docker_build.sh`** - Docker 构建脚本，启动容器并执行构建
- **`auto_build.sh`** - 自动构建脚本，支持多种构建选项

### auto_build.sh 选项

```bash
# 查看帮助
./auto_build.sh --help

# 执行所有步骤（构建 + 运行 + 测试）
./auto_build.sh --all

# 仅构建项目
./auto_build.sh --build

# 仅运行主程序
./auto_build.sh --run

# 仅运行测试
./auto_build.sh --test
```

### 手动构建（在容器内）

```bash
# 进入容器
docker exec -it my-cpp-dev bash

# 手动构建
mkdir -p build
cd build
cmake ..
make -j$(nproc)

# 运行程序
./my_app
```

## 🐳 Docker 环境特性

### 开发工具

容器内包含以下开发工具：

- **构建工具**: GCC, Clang, CMake, Ninja
- **调试工具**: GDB, Valgrind
- **代码质量**: clang-format, clang-tidy
- **版本控制**: Git
- **编译缓存**: ccache
- **实用工具**: vim, nano, tree, htop

### 安全特性

- 使用非 root 用户 `developer`
- 挂载只读配置文件
- 持久化编译缓存

### 性能优化

- ccache 编译缓存
- 多核并行编译
- 持久化数据卷

## 📊 项目功能

这个示例项目实现了一个数学工具库，包含以下功能：

- ✅ 基本统计计算（求和、平均值、最大值、最小值）
- 📈 高级统计（方差、标准差）
- 🔧 工具函数（向量打印）
- ⚠️ 错误处理和异常安全

### 示例输出

```
🐳 Hello from Docker container!
📊 C++ Math Utils Demo
======================
数据集: [1.50, 2.30, 3.70, 4.10, 5.90, 2.80, 6.20, 1.90, 4.50, 3.30]

📈 基本统计信息:
   总和: 36.200
   平均值: 3.620
   最大值: 6.200
   最小值: 1.500
   方差: 2.840
   标准差: 1.685

✅ 所有计算完成!
```

## 🔧 高级用法

### 调试模式

```bash
# 构建调试版本并启动 GDB
./debug.sh
```

### 代码格式化

```bash
# 格式化所有 C++ 代码
./format.sh
```

### 内存检查

```bash
# 进入容器
./shell.sh

# 使用 Valgrind 检查内存泄漏
cd build
valgrind --leak-check=full ./my_app
```

### 静态分析

```bash
# 运行 clang-tidy 静态分析
./format.sh
```

## 📝 开发最佳实践

### 代码组织

- 头文件和实现文件分离
- 使用命名空间避免冲突
- 异常安全的错误处理

### Docker 最佳实践

- 多阶段构建优化镜像大小
- 非 root 用户提高安全性
- 持久化数据卷保存缓存
- 环境变量配置

### 构建系统

- CMake 现代化配置
- 支持多种构建类型
- 编译命令导出（IDE 支持）

## 🤝 贡献指南

1. Fork 项目
2. 创建特性分支 (`git checkout -b feature/amazing-feature`)
3. 提交更改 (`git commit -m 'Add amazing feature'`)
4. 推送到分支 (`git push origin feature/amazing-feature`)
5. 创建 Pull Request

## 📄 许可证

本项目采用 MIT 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情。

## 🔗 相关资源

- [Docker 官方文档](https://docs.docker.com/)
- [CMake 文档](https://cmake.org/documentation/)
- [C++ 核心指南](https://isocpp.github.io/CppCoreGuidelines/)
- [clang-format 配置](https://clang.llvm.org/docs/ClangFormat.html)

---

**享受 Docker 化的 C++ 开发体验！** 🚀