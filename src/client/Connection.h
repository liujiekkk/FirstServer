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

		// ���������ļ�������.
		LpSocket m_socket;

	};
}