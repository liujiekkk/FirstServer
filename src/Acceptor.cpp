#include "Acceptor.h"
#include "StreamSocket.h"
#include "poller/AbstractPoller.h"

#include <stdexcept>
#include <sstream>
#include <memory>

#include "Sock.h"
#include <iostream>

fstServ::Acceptor::Acceptor(poller::AbstractPoller& poller): m_poller(poller)
{
}

fstServ::LpSocket fstServ::Acceptor::accept(StreamSocket& ss)
{
	struct sockaddr_in in;
	int addrLen = sizeof(in);
	// 获取通信套接字 @todo peer 详细信息待保存
	socket_t fd = ::accept((SOCKET)ss.getFd(), (sockaddr*)&in, &addrLen);
	if (fd == INVALID_SOCKET) {
		// 打印错误信息.抛出异常.
		std::stringstream sstring;
		sstring << "Acceptor.accept new conn" << ss.getFd() << "error." << std::endl;
		throw std::runtime_error(sstring.str());
	}
	// 打印客户端信息.
	/*char tmp[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &in.sin_addr, tmp, INET_ADDRSTRLEN);
	std::cout << tmp << ":" << ntohs(in.sin_port) << std::endl;*/
	return std::make_shared<StreamSocket>(fd);
}
