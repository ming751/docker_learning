# 设置最低 CMake 版本
cmake_minimum_required(VERSION 3.16)

# 项目名称和版本
project(my-cpp-project VERSION 1.0.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 生成 compile_commands.json (用于 clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 设置安装路径
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install)
endif()

# 包含目录
include_directories(include)

# 创建库
add_library(math_utils
    src/math_utils.cpp
)

# 设置库的头文件路径
target_include_directories(math_utils PUBLIC include)

# 创建可执行文件
add_executable(my_app
    src/main.cpp
)

# 链接库
target_link_libraries(my_app math_utils)

# 启用测试
enable_testing()

# 创建测试可执行文件
add_executable(test_math_utils
    tests/test_math_utils.cpp
)

# 链接测试到库
target_link_libraries(test_math_utils math_utils)

# 添加测试
add_test(NAME MathUtilsTests COMMAND test_math_utils)

# 安装规则
install(TARGETS my_app math_utils
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)