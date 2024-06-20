#include "EchoServer.h"
#include <sys/types.h>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <iostream>

#include "AbstractServer.h"
#include "../StreamSocket.h"
#include "../poller/Select.h"
#include "EchoServer.h"
#include "../Acceptor.h"
#include "../SocketHandler.h"

fstServ::EchoServer::EchoServer(const std::string host, unsigned short port, int sockType, int sinFamily): AbstractServer(host, port, sockType, sinFamily)
{
}

void fstServ::EchoServer::onConnect(StreamSocket& ss)
{
	// ��������ز��� TODO
	std::string msg = "welcome!!!\n";
	ss.write(msg.c_str(), msg.size());
}

// �˺�������ͻ��˷��͵�����.
void fstServ::EchoServer::onReceive(StreamSocket& ss, const char* data)
{
	std::cout << "Receive:" << std::string(data, strlen(data)) << std::endl;
	std::string msg = "hello client, this is server message!";
	ss.write(msg.c_str(), msg.size());
}

void fstServ::EchoServer::onWrite(StreamSocket& ss, SocketHandler& handler)
{
	// д�뻺��������.
	AbstractServer::onWrite(ss, handler);
	auto ls = getClient(ss.getFd());
	// ������Ӧ��ȥ����������ɾ���׽���ֱ�ӶϿ�����.
	getPoller().removeReadStream(ls);
	// ע�⣬�˳�ɾ�����������������ͷţ�select->wait�����һ���������ж���
	removeClient(ls);
}

void fstServ::EchoServer::onRead(StreamSocket& ss, SocketHandler& handler)
{
	AbstractServer::onRead(ss, handler);
}
