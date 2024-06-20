#include "EchoServer.h"
#include <sys/types.h>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <iostream>

#include "AbstractServer.h"
#include "../StreamSocket.h"
#include "../poller/Select.h"
#include "EchoServer.h"
#include "../Acceptor.h"
#include "../SocketHandler.h"

fstServ::EchoServer::EchoServer(const std::string host, unsigned short port, int sockType, int sinFamily): AbstractServer(host, port, sockType, sinFamily)
{
}

void fstServ::EchoServer::onConnect(StreamSocket& ss)
{
	// 新链接相关操作 TODO
	std::string msg = "welcome!!!\n";
	ss.write(msg.c_str(), msg.size());
}

// 此函数处理客户端发送的数据.
void fstServ::EchoServer::onReceive(StreamSocket& ss, const char* data)
{
	std::cout << "Receive:" << std::string(data, strlen(data)) << std::endl;
	std::string msg = "hello client, this is server message!";
	ss.write(msg.c_str(), msg.size());
}

void fstServ::EchoServer::onWrite(StreamSocket& ss, SocketHandler& handler)
{
	// 写入缓存中数据.
	AbstractServer::onWrite(ss, handler);
	auto ls = getClient(ss.getFd());
	// 返回响应后去掉读监听，删除套接字直接断开连接.
	getPoller().removeReadStream(ls);
	// 注意，此出删除并不会引起对象的释放（select->wait后会有一个拷贝持有对象）
	removeClient(ls);
}

void fstServ::EchoServer::onRead(StreamSocket& ss, SocketHandler& handler)
{
	AbstractServer::onRead(ss, handler);
}
