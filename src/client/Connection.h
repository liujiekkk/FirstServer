#pragma once

#include <string>
#include "../Sock.h"
#include "../StreamSocket.h"

namespace fstServ {

	class Client;

	class Connection {

	public:

		Connection(Client&);

		~Connection();

		bool connect();

		std::string send(std::string&);

	protected:

		int getAfType() const;

		std::string recv();

	private:

		Client& m_client;

		// 服务器的文件描述符.
		LpSocket m_socket;

	};
}