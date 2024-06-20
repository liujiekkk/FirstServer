#pragma once

#include "../Sock.h"
#include "../StreamSocket.h"
#include "../poller/AbstractPoller.h"
#include <string>

namespace fstServ {

	class AbstractConfig;

	class SocketHandler;

	struct Package;

	class AbstractServer {

	public:

		AbstractServer(const std::string host, unsigned short port, int sockType = SOCK_STREAM, int sinFamily = AF_INET);

		virtual ~AbstractServer();

		// ��ȡ��������.
		const std::string& getHost() const;

		// ��ȡ����˼����˿ں�.
		unsigned short getPort() const;

		// �������������.
		virtual void start();

		// �����ֹͣ����.
		virtual void stop();

		// �����ӵ�������
		virtual void onConnect(StreamSocket&);

		// ���������ݵ�������
		virtual void onRead(StreamSocket&, SocketHandler&);

		// �׽����п�д�¼�����
		virtual void onWrite(StreamSocket&, SocketHandler&);

		// �������ݽ���һ�� package ����.
		virtual void onReceive(StreamSocket&, const char*) = 0;

		// ���ӶϿ�ǰ�ص��˺���.
		virtual void onClose(StreamSocket&);

		// ���һ���¿ͻ���.
		void addClient(LpSocket);

		// ɾ���ͻ�������.
		size_t removeClient(LpSocket);

		// ɾ���ͻ�������.
		size_t removeClient(const StreamSocket&);

		// ɾ���ͻ�������.
		size_t removeClient(socket_t);

		// ��ȡ�ͻ��˵��׽���ʵ��.
		LpSocket getClient(const StreamSocket&) const;

		// ��ȡ�ͻ��˵��׽���ʵ��.
		LpSocket getClient(socket_t) const;

		// ��ȡ�����׽���ʵ��.
		StreamSocket& getListenStreamSocket() const;

		// ��ȡIO��·������.
		poller::AbstractPoller& getPoller();

	protected:

		// ͨ�����ó�ʼ��������.
		void init();

		// ��������˼����׽���.
		void socket();

		// �󶨼����׽��ֵ���������˿�.
		void bind() const;

		// ��ʼ��������.
		virtual void listen() const;

	private:

		// �������Ӵ�����г���(Ĭ��128).
		int m_backLog = 128;

		// ����������������
		std::string m_host;

		// �������˿ڵ�ַ
		unsigned short m_port;

		// SOCK_STREAM, SOCK_DGRAM
		int m_sockType;

		// Э������
		int m_sinFamily;

		// ���пͻ������Ӽ���.
		SSockSet m_clients;

		// �����׽���.
		LpSocket m_listenStreamSocket;

		// IO��·������.
		std::unique_ptr<poller::AbstractPoller> m_poller;

	};
}
