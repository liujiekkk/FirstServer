#pragma once

#include "AbstractServer.h"

namespace fstServ {

	class EchoServer : public AbstractServer {

	public:

		EchoServer(const std::string host, unsigned short port, int sockType = SOCK_STREAM, int sinFamily = AF_INET);

		void onConnect(StreamSocket&) override;

		// �˺�������ͻ��˷��͵�����.
		void onReceive(StreamSocket&, const char*) override;

		void onWrite(StreamSocket&, SocketHandler&) override;

		void onRead(StreamSocket&, SocketHandler&) override;

	};
}