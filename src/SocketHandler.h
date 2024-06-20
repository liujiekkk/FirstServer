#pragma once

#include <memory>
#include "Acceptor.h"
#include "Sock.h"

namespace fstServ {

	class StreamSocket;

	class AbstractServer;

	class SocketHandler {

	public:

		// 构造函数.
		SocketHandler(AbstractServer&);

		void handle(StreamSocket&);

		AbstractServer& getServer();
		// 从指定套接字中读取指定长度的内容.
		static int readn(socket_t, char*, int, int = 0);

		// 向套接字中写入指定长度的内容.
		static int writen(socket_t, const char*, int, int = 0);

	private:

		AbstractServer& m_server;

		Acceptor m_acceptor;
	};
}