#include "SocketHandler.h"
#include "StreamSocket.h"
#include "server/AbstractServer.h"
#include <stdexcept>
#include <process.h>
#include <iostream>
#include "protocol/Protocol.h"

fstServ::SocketHandler::SocketHandler(AbstractServer& server)
	: m_server(server), m_acceptor(server.getPoller())
{
}

void fstServ::SocketHandler::handle(StreamSocket& ss)
{
	// �ж��Ƿ��Ǽ����׽����¼�.
	if (ss == m_server.getListenStreamSocket()) {
		// ���������ӣ��ŵ� select readset ���м���.
		auto ssNew = m_acceptor.accept(ss);
		// ��ӵ��������Ŀͻ����б�.
		m_server.addClient(ssNew);
		// �����׽�����ӵ�������.
		m_server.getPoller().addReadStream(ssNew);
		// ������������ز���.
		m_server.onConnect(*ssNew);
	}
	else {
		// ͨ���׽���.��ȡ�ͻ��˷�����������.
		if (ss.getEvents() & StreamSocket::EV_RD) {
			// ����ɶ��¼�.
			m_server.onRead(ss, *this);
		}
		else if (ss.getEvents() & StreamSocket::EV_WR) {
			// ����д�¼�.
			m_server.onWrite(ss, *this);
		}
	}
}

fstServ::AbstractServer& fstServ::SocketHandler::getServer()
{
	return m_server;
}

int fstServ::SocketHandler::readn(socket_t fd, char* buff, int len, int flags)
{
	// ���ö�ȡʣ��left,ÿ�ζ�������ret.
	int left = len, ret = 0, ready = 0;
	// ����������׽��ֻ����������ݺ��ȴ���������.
	// ������������׽��ֻ����������ݺ�᷵�� -1
	while (left > 0 && (ret = ::recv(fd, buff + ready, left, flags)) > 0) {
		left -= ret;
		ready += ret;
	}
	// ��ʾ������ȡ���.
	if (left == 0) {
		return len;
	}
	// ��ʾ�ͻ��˶Ͽ���.
	if (ret == 0) {
		throw std::runtime_error("peer disconnect while read.");
	}
	else {
		// ��ʾ������-1(���ܶ�ȡ�쳣��������E_AGAIN)
		switch (WSAGetLastError()) {
			case WSAEWOULDBLOCK: // ���������,��������Դ����.
				break;
			case WSAECONNRESET: // ���ӱ�Զ������ǿ�ƹر�.
				throw std::runtime_error("Connection has be closed by remote client.");
				break;
			default:
				throw std::runtime_error("peer read error...");
		}
	}
	return len - left;
}

int fstServ::SocketHandler::writen(socket_t fd, const char* buff, int len, int flags)
{
	int left = len, ret = 0, ready = 0;
	while (left > 0 && (ret = send(fd, buff + ready, left, flags)) > 0) {
		left -= ret;
		ready += ret;
	}
	if (left == 0) {
		return len;
	}
	if (ret == 0) {
		throw std::runtime_error("Connection reset by peer.");
	}
	else {
		// ��ʾ������-1(���ܶ�ȡ�쳣��������E_AGAIN)
		if (WSAGetLastError() != WSAEWOULDBLOCK) {
			throw std::runtime_error("peer write error...");
		}
	}
	return len - left;
}



