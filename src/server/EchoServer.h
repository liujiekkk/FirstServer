#pragma once

#include "AbstractServer.h"

namespace fstServ {

	class EchoServer : public AbstractServer {

	public:

		EchoServer(const std::string host, unsigned short port, int sockType = SOCK_STREAM, int sinFamily = AF_INET);

		void onConnect(StreamSocket&) override;

		// 此函数处理客户端发送的数据.
		void onReceive(StreamSocket&, const char*) override;

		void onWrite(StreamSocket&, SocketHandler&) override;

		void onRead(StreamSocket&, SocketHandler&) override;

	};
}