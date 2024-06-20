#pragma once

#include <memory>
#include "Acceptor.h"
#include "Sock.h"

namespace fstServ {

	class StreamSocket;

	class AbstractServer;

	class SocketHandler {

	public:

		// ���캯��.
		SocketHandler(AbstractServer&);

		void handle(StreamSocket&);

		AbstractServer& getServer();
		// ��ָ���׽����ж�ȡָ�����ȵ�����.
		static int readn(socket_t, char*, int, int = 0);

		// ���׽�����д��ָ�����ȵ�����.
		static int writen(socket_t, const char*, int, int = 0);

	private:

		AbstractServer& m_server;

		Acceptor m_acceptor;
	};
}