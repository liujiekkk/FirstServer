#pragma once

#include "../Sock.h"
#include "../StreamSocket.h"
#include "../poller/AbstractPoller.h"
#include <string>

namespace fstServ {

	class AbstractConfig;

	class SocketHandler;

	struct Package;

	class AbstractServer {

	public:

		AbstractServer(const std::string host, unsigned short port, int sockType = SOCK_STREAM, int sinFamily = AF_INET);

		virtual ~AbstractServer();

		// 获取监听网段.
		const std::string& getHost() const;

		// 获取服务端监听端口号.
		unsigned short getPort() const;

		// 服务端启动函数.
		virtual void start();

		// 服务端停止函数.
		virtual void stop();

		// 新链接到来触发
		virtual void onConnect(StreamSocket&);

		// 连接有数据到来触发
		virtual void onRead(StreamSocket&, SocketHandler&);

		// 套接字有可写事件触发
		virtual void onWrite(StreamSocket&, SocketHandler&);

		// 连接数据解析一个 package 触发.
		virtual void onReceive(StreamSocket&, const char*) = 0;

		// 连接断开前回调此函数.
		virtual void onClose(StreamSocket&);

		// 添加一个新客户端.
		void addClient(LpSocket);

		// 删除客户端连接.
		size_t removeClient(LpSocket);

		// 删除客户端连接.
		size_t removeClient(const StreamSocket&);

		// 删除客户端连接.
		size_t removeClient(socket_t);

		// 获取客户端的套接字实例.
		LpSocket getClient(const StreamSocket&) const;

		// 获取客户端的套接字实例.
		LpSocket getClient(socket_t) const;

		// 获取监听套接字实例.
		StreamSocket& getListenStreamSocket() const;

		// 获取IO多路复用器.
		poller::AbstractPoller& getPoller();

	protected:

		// 通过配置初始化相关组件.
		void init();

		// 创建服务端监听套接字.
		void socket();

		// 绑定监听套接字到服务监听端口.
		void bind() const;

		// 开始监听数据.
		virtual void listen() const;

	private:

		// 请求连接处理队列长度(默认128).
		int m_backLog = 128;

		// 服务器监听的网段
		std::string m_host;

		// 服务器端口地址
		unsigned short m_port;

		// SOCK_STREAM, SOCK_DGRAM
		int m_sockType;

		// 协议类型
		int m_sinFamily;

		// 持有客户端连接集合.
		SSockSet m_clients;

		// 监听套接字.
		LpSocket m_listenStreamSocket;

		// IO多路复用器.
		std::unique_ptr<poller::AbstractPoller> m_poller;

	};
}
