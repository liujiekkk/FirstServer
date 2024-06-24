cmake_minimum_required(VERSION 3.8)

project("tests")

# 添加头文件路径.
# include_directories(${include_path})

# 客户端源文件列表
set(client_sources "client.cpp")

# 服务端源文件列表
set(server_sources "server.cpp")

# 设置生成路径.
# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

# 生成客户端可执行文件
add_executable("client" ${client_sources})
# 生成服务端可执行文件
add_executable("server" ${server_sources})

# 添加头文件目录
target_include_directories("server" PRIVATE ${include_path})
target_include_directories("client" PRIVATE ${include_path})

target_link_libraries("client" "FirstServer")
target_link_libraries("server" "FirstServer")

target_link_directories("client" PRIVATE ${lib_path})
target_link_directories("server" PRIVATE ${lib_path})

set_target_properties(client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
