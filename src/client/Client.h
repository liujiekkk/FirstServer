#pragma once

#include <string>
#include "../Sock.h"

namespace fstServ {

	class Connection;

	class Client {

	public:

		Client(std::string& serverIp, unsigned short port, bool isIpv6 = false);

		std::string getServerIp() const;

		unsigned short getPort() const;

		bool isEnableIpv6() const;

		Connection createConnection();

	private:

		std::string m_serverIp;

		unsigned short m_port;

		bool m_enableIpv6;

	};
}