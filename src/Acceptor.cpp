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
	// ��ȡͨ���׽��� @todo peer ��ϸ��Ϣ������
	socket_t fd = ::accept((SOCKET)ss.getFd(), (sockaddr*)&in, &addrLen);
	if (fd == INVALID_SOCKET) {
		// ��ӡ������Ϣ.�׳��쳣.
		std::stringstream sstring;
		sstring << "Acceptor.accept new conn" << ss.getFd() << "error." << std::endl;
		throw std::runtime_error(sstring.str());
	}
	// ��ӡ�ͻ�����Ϣ.
	/*char tmp[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &in.sin_addr, tmp, INET_ADDRSTRLEN);
	std::cout << tmp << ":" << ntohs(in.sin_port) << std::endl;*/
	return std::make_shared<StreamSocket>(fd);
}
