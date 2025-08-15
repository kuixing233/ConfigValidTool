cmake_minimum_required(VERSION 3.10)
project(MyProject LANGUAGES CXX)      # 显式指定 C++ 项目

# 全局配置
set(CMAKE_CXX_STANDARD 11)            # C++11 标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)   # 必须支持该标准
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # 生成 compile_commands.json（供工具链使用）

# 可选：输出目录设置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)  # 可执行文件输出到 ./bin/
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)  # 动态库输出到 ./lib/

# 可选：设置编译属性
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -fcoroutines-ts -Wall -Wextra -O2")


# 添加可执行文件（支持多个源文件）
add_executable(my_app
    src/main.cc
    src/output.cc
)

# 可选：添加子目录（例如包含库的代码）
# add_subdirectory(lib) 


# 可选：包含头文件目录
target_include_directories(my_app PRIVATE include)

# 可选：链接库（如 pthread、OpenMP）
 target_link_libraries(my_app PRIVATE pthread)
