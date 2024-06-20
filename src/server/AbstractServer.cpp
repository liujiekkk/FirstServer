#include "AbstractServer.h"
#include "../StreamSocket.h"
#include "../AbstractConfig.h"
#include "../poller/Select.h"
#include "../SocketHandler.h"
#include <string>
#include <iostream>

fstServ::AbstractServer::AbstractServer(const std::string host, unsigned short port, int sockType, int sinFamily)
	:m_host(host), m_port(port), m_sockType(sockType), m_sinFamily(sinFamily), m_clients(), m_listenStreamSocket(nullptr), m_poller(nullptr)
{
#ifdef __SYS_WINDOWS
	WSADATA wsaData;
	int wsaerr = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsaerr != 0) {
		throw std::runtime_error("Windows server WSAStartup error!!!");
	}
#endif // IS_WINDOWS

	// 初始化操作.
	init();
}

fstServ::AbstractServer::~AbstractServer()
{
#ifdef __SYS_WINDOWS
	WSACleanup();
#endif // __SYS_WINDOWS
}

const std::string& fstServ::AbstractServer::getHost() const
{
	return m_host;
}

unsigned short fstServ::AbstractServer::getPort() const
{
	return m_port;
}

void fstServ::AbstractServer::start()
{
	// 创建监听套接字.
	socket();
	// 绑定协议、端口、地址到套接字.
	bind();
	// 开始监听.
	listen();
	// StreamSocket 处理器.
	SocketHandler handler(*this);
	// 增加监听套接字.
	m_poller->addReadStream(m_listenStreamSocket);
	// poller 事件监听套接字结果集合.
	SSockSet result;
	// 进入循环.
	while (true) {
		int ret = m_poller->wait(result);
		// 有错误直接退出. 
		if (ret < 0) {
			std::cerr << "select wait error." << std::endl;
			return;
		}
		else if (ret == 0) {
			// 表示 select 超时.
			std::cout << "Timeout ..." << std::endl;
		}
		else {
			// 监听套接字有事件响应.
			for (auto& ptr : result) {
				try {
					// 通信套接字事件处理.
					handler.handle(*ptr.second);
					// 清理套接字的事件状态.
					ptr.second->reset();
				}
				catch (std::runtime_error e) {
					// 回调 onClose 函数
					onClose(*ptr.second);
					m_poller->removeReadStream(ptr.second);
					m_poller->removeWriteStream(ptr.second);
					removeClient(ptr.second);
				}
			}
		}
		// 清理结果集.
		result.clear();
	}
}

void fstServ::AbstractServer::stop()
{
}

void fstServ::AbstractServer::onConnect(StreamSocket& ss)
{
	// TODO: 这里处理新连接相关操作.
}

void fstServ::AbstractServer::onRead(StreamSocket& ss, SocketHandler& handler)
{
	// 处理读事件.
	ss.read(handler);
}

void fstServ::AbstractServer::onWrite(StreamSocket& ss, SocketHandler& handler)
{
	// 要写入的数据长度.
	int left = (int)ss.getWriteBuff().size(), ready = 0;
	while (left > 0) {
		int ret = handler.writen(ss.getFd(), ss.getWriteBuff().c_str() + ready, left);
		left -= ret;
		ready += ret;
	}
	// 删除已发送数据.
	ss.getWriteBuff().discard(ss.getWriteBuff().size());
	// 写入完毕.移除写事件.
	LpSocket ls = getClient(ss);
	getPoller().removeWriteStream(ls);
}

void fstServ::AbstractServer::onClose(StreamSocket& ss)
{
}

void fstServ::AbstractServer::addClient(LpSocket ptr)
{
	m_clients[ptr->getFd()] = ptr;
}

size_t fstServ::AbstractServer::removeClient(const StreamSocket& ss)
{
	return removeClient(ss.getFd());
}

size_t fstServ::AbstractServer::removeClient(socket_t fd)
{
	return m_clients.erase(fd);
}

size_t fstServ::AbstractServer::removeClient(LpSocket ls)
{
	return removeClient(*ls);
}

fstServ::LpSocket fstServ::AbstractServer::getClient(const StreamSocket& ss) const
{
	return getClient(ss.getFd());
}

fstServ::LpSocket fstServ::AbstractServer::getClient(socket_t fd) const
{
	auto iter = m_clients.find(fd);
	if (iter == m_clients.end()) {
		return std::shared_ptr<StreamSocket>(nullptr);
	}
	return iter->second;
}

fstServ::StreamSocket& fstServ::AbstractServer::getListenStreamSocket() const
{
	return *m_listenStreamSocket;
}

fstServ::poller::AbstractPoller& fstServ::AbstractServer::getPoller()
{
	return *m_poller;
}

void fstServ::AbstractServer::init()
{
	// 设置多路复用器.
	m_poller.reset(new poller::Select());
}

void fstServ::AbstractServer::socket()
{
	socket_t fd = ::socket(m_sinFamily, m_sockType, 0);
	if (fd == SOCKET_ERROR) {
		throw std::runtime_error("create a win socket failed.");
	}
	m_listenStreamSocket = std::make_shared<StreamSocket>(fd);
}

void fstServ::AbstractServer::bind() const
{
	// 创建网络地址协议簇.
	sockaddr_in in;
	std::memset(&in, 0, sizeof(in));
	in.sin_port = htons(m_port);
	in.sin_family = m_sinFamily;
	// 转换IPV4(IPV6)地址为网络字节序.
	inet_pton(m_sinFamily, m_host.c_str(), &in.sin_addr);
	// 绑定端口到 fd.
	int ret = ::bind(m_listenStreamSocket->getFd(), (sockaddr*)&in, sizeof(in));
	if (ret < 0) {
		throw std::runtime_error("windows server bind return -1.");
	}
}

void fstServ::AbstractServer::listen() const
{
	int ret = ::listen(m_listenStreamSocket->getFd(), m_backLog);
	if (ret < 0) {
		throw std::runtime_error("windows server listen return -1");
	}
}
