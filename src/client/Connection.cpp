#include "Connection.h"
#include <stdexcept>
#include <iostream>
#include "Client.h"
#include "../SocketHandler.h"
#include "../protocol/Protocol.h"

fstServ::Connection::Connection(Client& client): m_client(client), m_socket(new StreamSocket(SOCKET_ERROR))
{
	WSADATA wsaData;
	int wsResult = WSAStartup(MAKEWORD(2, 2), & wsaData);
	if (wsResult != 0)
	{
		std::cerr << "Can't start Winsock, Err #" << wsResult << std::endl;
	}
}

fstServ::Connection::~Connection()
{
	WSACleanup();
}

bool fstServ::Connection::connect()
{
	// �����׽���
	socket_t sock = socket(getAfType(), SOCK_STREAM, 0);
	if (sock == SOCKET_ERROR) {
		throw std::runtime_error("client socket create failed.");
	}
	struct sockaddr_in in;
	std::memset(&in, 0, sizeof(in));
	in.sin_port = htons(m_client.getPort());
	in.sin_family = getAfType();
	// ��ip��ַת��Ϊָ��Э�����͵������ֽ�������.
	inet_pton(getAfType(), m_client.getServerIp().c_str(), &in.sin_addr);

	// ���ӵ�������.
	int ret = ::connect(sock, (const sockaddr*)&in, sizeof(in));
	if (ret == SOCKET_ERROR) {
		throw std::runtime_error("client connect error.");
	}
	m_socket = std::make_shared<StreamSocket>(sock);
	// ���ӳɹ�.
	return true;
}

std::string fstServ::Connection::send(std::string& msg)
{
	Protocol protocol;
	const char* data = protocol.pack(msg);
	// �����˷�������.
	size_t left = protocol.size();
	do {
		int ret = SocketHandler::writen(m_socket->getFd(), data, (int)left);
		if (ret) {
			left -= ret;
		}
		else {
			std::cerr << "writen error." << std::endl;
			break;
		}
	} while (left);
	// д������ɺ󣬶�ȡ���ؽ��.
	if (left == 0) {
		Buff buff;
		// �ȶ�ȡpacketͷ����.
		size_t packetLeft = Protocol::Header::size();
		do {
			char* pHeader = new char[packetLeft] {'\0'};
			int ret = SocketHandler::readn(m_socket->getFd(), pHeader, (int)packetLeft);
			// ����ȡ�������ݷ��뻺��.
			buff.append(pHeader, ret);
			// ʣ��Ҫ��ȡ�ĳ���.
			packetLeft -= ret;
			if (buff.size() == Protocol::Header::size()) {
				// ��ȡ��Ӧͷ.
				packetLeft = protocol.unpackHeader(pHeader, Protocol::Header::size()).m_length;
			}
		} while (packetLeft > 0);
		return protocol.unpack(buff.c_str(), buff.size());
	}
	return std::string();
}

int fstServ::Connection::getAfType() const
{
	if (m_client.isEnableIpv6()) {
		return AF_INET6;
	}
	else {
		return AF_INET;
	}
}

std::string fstServ::Connection::recv()
{
	return std::string();
}
