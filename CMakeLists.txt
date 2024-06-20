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

# 将源代码添加到此项目的可执行文件。
add_executable (FirstServer 
"FirstServer.cpp" 
"FirstServer.h"  
"src/server/AbstractServer.h"
"src/server/AbstractServer.cpp"  
"src/StreamSocket.h" 
"src/StreamSocket.cpp" 
"src/ServerFactory.h" 
"src/ServerFactory.cpp" 
"src/loop/LoopInterface.h" 
"src/loop/AbstractLoop.h" 
"src/loop/AbstractLoop.cpp" 
"src/loop/EventLoop.h" 
"src/loop/EventLoop.cpp" 
"src/poller/AbstractPoller.h" 
"src/poller/EPoll.h" 
"src/poller/EPoll.cpp"  
"src/poller/Select.cpp" 
"src/poller/Select.h" 
"src/poller/AbstractPoller.cpp" 
"src/server/EchoServer.cpp" 
"src/server/EchoServer.h" 
"src/AbstractConfig.h" 
"src/processor/AbstractProcessor.h" 
"src/processor/ConfigProcessor.h" 
"src/processor/ConfigProcessor.cpp"
"src/processor/EnvProcessor.h" 
"src/processor/EnvProcessor.cpp"
"src/processor/ApplicationProcessor.h" 
"src/processor/ApplicationProcessor.cpp" 
"src/processor/EventProcessor.h"  
"src/processor/EventProcessor.cpp"
"src/processor/ConsoleProcessor.h" 
"src/processor/ConsoleProcessor.cpp"
"src/ApplicationInterface.h" 
"src/AbstractApplication.h" 
"src/AbstractApplication.cpp" 
"src/container/ObjectPool.h" 
"src/container/Container.h"  
"src/container/Container.cpp"  
"src/buff/Buff.cpp" 
"src/buff/Buff.h"   
"src/SocketHandler.h" 
"src/SocketHandler.cpp"
"src/Acceptor.h"
"src/Acceptor.cpp"
"src/Sock.h" 
"src/protocol/Protocol.h"
"src/protocol/Protocol.cpp")

add_executable(client "src/client/Client.h"
"src/client/Client.cpp" 
"src/client/Connection.h" 
"src/client/Connection.cpp"
"src/server/AbstractServer.h"
"src/server/AbstractServer.cpp"  
"src/StreamSocket.h" 
"src/StreamSocket.cpp" 
"src/ServerFactory.h" 
"src/ServerFactory.cpp" 
"src/loop/LoopInterface.h" 
"src/loop/AbstractLoop.h" 
"src/loop/AbstractLoop.cpp" 
"src/loop/EventLoop.h" 
"src/loop/EventLoop.cpp" 
"src/poller/AbstractPoller.h" 
"src/poller/EPoll.h" 
"src/poller/EPoll.cpp"  
"src/poller/Select.cpp" 
"src/poller/Select.h" 
"src/poller/AbstractPoller.cpp" 
"src/server/EchoServer.cpp" 
"src/server/EchoServer.h" 
"src/AbstractConfig.h" 
"src/processor/AbstractProcessor.h" 
"src/processor/ConfigProcessor.h" 
"src/processor/ConfigProcessor.cpp"
"src/processor/EnvProcessor.h" 
"src/processor/EnvProcessor.cpp"
"src/processor/ApplicationProcessor.h" 
"src/processor/ApplicationProcessor.cpp" 
"src/processor/EventProcessor.h"  
"src/processor/EventProcessor.cpp"
"src/processor/ConsoleProcessor.h" 
"src/processor/ConsoleProcessor.cpp"
"src/ApplicationInterface.h" 
"src/AbstractApplication.h" 
"src/AbstractApplication.cpp" 
"src/container/ObjectPool.h" 
"src/container/Container.h"  
"src/container/Container.cpp"  
"src/buff/Buff.cpp" 
"src/buff/Buff.h"   
"src/SocketHandler.h" 
"src/SocketHandler.cpp"
"src/Acceptor.h"
"src/Acceptor.cpp"
"src/Sock.h"  
"src/protocol/Protocol.h" 
"src/protocol/Protocol.cpp" 
"Client.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FirstServer PROPERTY CXX_STANDARD 11)
endif()

# TODO: 如有需要，请添加测试并安装目标。
