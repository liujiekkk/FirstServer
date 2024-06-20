#include "Client.h"
#include "Connection.h"
#include <iostream>

fstServ::Client::Client(std::string& serverIp, unsigned short port, bool isIpv6)
	: m_serverIp(serverIp), m_port(port), m_enableIpv6(isIpv6)
{
}

std::string fstServ::Client::getServerIp() const
{
	return m_serverIp;
}

unsigned short fstServ::Client::getPort() const
{
	return m_port;
}

bool fstServ::Client::isEnableIpv6() const
{
	return m_enableIpv6;
}

fstServ::Connection fstServ::Client::createConnection()
{
	return Connection(*this);
}

