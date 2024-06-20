#include "SocketHandler.h"
#include "StreamSocket.h"
#include "server/AbstractServer.h"
#include <stdexcept>
#include <process.h>
#include <iostream>
#include "protocol/Protocol.h"

fstServ::SocketHandler::SocketHandler(AbstractServer& server)
	: m_server(server), m_acceptor(server.getPoller())
{
}

void fstServ::SocketHandler::handle(StreamSocket& ss)
{
	// 判断是否是监听套接字事件.
	if (ss == m_server.getListenStreamSocket()) {
		// 接收新链接，放到 select readset 进行监听.
		auto ssNew = m_acceptor.accept(ss);
		// 添加到服务器的客户端列表.
		m_server.addClient(ssNew);
		// 将新套接字添加到读监听.
		m_server.getPoller().addReadStream(ssNew);
		// 处理新链接相关操作.
		m_server.onConnect(*ssNew);
	}
	else {
		// 通信套接字.读取客户端发送来的数据.
		if (ss.getEvents() & StreamSocket::EV_RD) {
			// 处理可读事件.
			m_server.onRead(ss, *this);
		}
		else if (ss.getEvents() & StreamSocket::EV_WR) {
			// 处理写事件.
			m_server.onWrite(ss, *this);
		}
	}
}

fstServ::AbstractServer& fstServ::SocketHandler::getServer()
{
	return m_server;
}

int fstServ::SocketHandler::readn(socket_t fd, char* buff, int len, int flags)
{
	// 设置读取剩余left,每次读出数据ret.
	int left = len, ret = 0, ready = 0;
	// 阻塞情况，套接字缓冲区无数据后会等待后续数据.
	// 非阻塞情况，套接字缓冲区无数据后会返回 -1
	while (left > 0 && (ret = ::recv(fd, buff + ready, left, flags)) > 0) {
		left -= ret;
		ready += ret;
	}
	// 表示正常读取完毕.
	if (left == 0) {
		return len;
	}
	// 表示客户端断开了.
	if (ret == 0) {
		throw std::runtime_error("peer disconnect while read.");
	}
	else {
		// 表示返回了-1(可能读取异常，可能是E_AGAIN)
		switch (WSAGetLastError()) {
			case WSAEWOULDBLOCK: // 非阻塞情况,缓冲区资源不足.
				break;
			case WSAECONNRESET: // 连接被远程主机强制关闭.
				throw std::runtime_error("Connection has be closed by remote client.");
				break;
			default:
				throw std::runtime_error("peer read error...");
		}
	}
	return len - left;
}

int fstServ::SocketHandler::writen(socket_t fd, const char* buff, int len, int flags)
{
	int left = len, ret = 0, ready = 0;
	while (left > 0 && (ret = send(fd, buff + ready, left, flags)) > 0) {
		left -= ret;
		ready += ret;
	}
	if (left == 0) {
		return len;
	}
	if (ret == 0) {
		throw std::runtime_error("Connection reset by peer.");
	}
	else {
		// 表示返回了-1(可能读取异常，可能是E_AGAIN)
		if (WSAGetLastError() != WSAEWOULDBLOCK) {
			throw std::runtime_error("peer write error...");
		}
	}
	return len - left;
}



