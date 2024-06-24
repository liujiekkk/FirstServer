# CMakeList.txt: FirstServer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("FirstServer")

set(source_path ${PROJECT_SOURCE_DIR}/src)

# 设置头文件目录.
set(include_path 
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/src/buff
	${PROJECT_SOURCE_DIR}/src/client
	${PROJECT_SOURCE_DIR}/src/container
	${PROJECT_SOURCE_DIR}/src/loop
	${PROJECT_SOURCE_DIR}/src/poller
	${PROJECT_SOURCE_DIR}/src/processor
	${PROJECT_SOURCE_DIR}/src/protocol
	${PROJECT_SOURCE_DIR}/src/server
)

# 递归获取源文件.
file(GLOB_RECURSE source_list *.cpp)

# 设置静态库存储路径.
set(lib_path ${PROJECT_SOURCE_DIR}/lib)

# 生成静态库需要包含头文件
include_directories(${include_path})

# 设置输出目录
set(LIBRARY_OUTPUT_PATH ${lib_path})

# 生成静态库.
add_library("FirstServer" STATIC ${source_list})

# 添加子目录.
add_subdirectory("tests")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FirstServer PROPERTY CXX_STANDARD 11)
endif()

# TODO: 如有需要，请添加测试并安装目标。
