#include "ServerFactory.h"
#include "server/EchoServer.h"
#include "AbstractConfig.h"

fstServ::AbstractServer* fstServ::ServerFactory::getServer(const AbstractConfig& config)
{
	auto sHost = config.getValue<std::string>("host");
	unsigned short port = config.getValue<unsigned short>("port");
	return new fstServ::EchoServer(sHost, port);
}
