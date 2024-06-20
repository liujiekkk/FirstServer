#include "StreamSocket.h"
#include "SocketHandler.h"
#include "protocol/Protocol.h"
#include "server/AbstractServer.h"
#include <string>
#include <iostream>

// ���徲̬���� ReadyState
fstServ::StreamSocket::ReadyState fstServ::StreamSocket::m_ready;

// ���徲̬���� UnReadyState
fstServ::StreamSocket::UnReadyState fstServ::StreamSocket::m_unReady;

fstServ::StreamSocket::StreamSocket(socket_t fd)
	: m_fd(fd), m_events(0U), m_buffStatus(0U), m_buffRead(new Buff()), m_buffWrite(new Buff)
{
}

fstServ::StreamSocket::~StreamSocket()
{
	if (m_fd) {
		::closesocket(m_fd);
	}
}

fstServ::StreamSocket::StreamSocket(StreamSocket&& ss) noexcept 
	: m_fd(ss.m_fd), m_events(ss.m_events), m_buffStatus(ss.m_buffStatus), m_buffRead(std::move(ss.m_buffRead)), m_buffWrite(std::move(ss.m_buffWrite))
{
}

fstServ::StreamSocket& fstServ::StreamSocket::operator==(StreamSocket&& ss)
{
	swap(ss);
	return *this;
}

void fstServ::StreamSocket::swap(StreamSocket& ss)
{
	if (this == &ss) {
		return;
	}
	std::swap(m_fd, ss.m_fd);
	std::swap(m_events, ss.m_events);
	std::swap(m_buffRead, ss.m_buffRead);
	std::swap(m_buffWrite, ss.m_buffWrite);
}

socket_t fstServ::StreamSocket::getFd() const
{
	return m_fd;
}

void fstServ::StreamSocket::setEvents(const unsigned events)
{
	m_events = events;
}

unsigned fstServ::StreamSocket::getEvents() const
{
	return m_events;
}

void fstServ::StreamSocket::setBuffStatus(const unsigned status)
{
	m_buffStatus = status;
}

unsigned fstServ::StreamSocket::getBuffStatus() const
{
	return m_buffStatus;
}

void fstServ::StreamSocket::reset()
{
	m_events = EventStatus::EV_NONE;
}

void fstServ::StreamSocket::resetBuffStatus()
{
	m_buffStatus = BuffStatus::BUFF_NONE;
}

bool fstServ::StreamSocket::operator==(const StreamSocket& ss) const
{
	return this == &ss;
}

void fstServ::StreamSocket::write(const char* cp, size_t len)
{
	// д�����ݵ�����.
	m_buffWrite->append(cp, len);
}

void fstServ::StreamSocket::read(SocketHandler& handler)
{
	// ���һ�����ݶ�ȡ��ȫ����Ҫ��������ready״̬�����ݣ�����ҵ�� onReceive �ص���
	do {
		// ��ȡ��ǰ״̬
		IState& state = getState();
		// ��ȡ��ǰ socket �����ݵ�����.
		state.read(*this, handler);
	} while (m_buffStatus & StreamSocket::BuffStatus::BUFF_RD_READY);
}

fstServ::Buff& fstServ::StreamSocket::getReadBuff()
{
	// �� buff ����ת���� Package.
	return *m_buffRead;
}

fstServ::Buff& fstServ::StreamSocket::getWriteBuff()
{
	return *m_buffWrite;
}

fstServ::StreamSocket::IState& fstServ::StreamSocket::getState()
{
	if (m_buffStatus & BuffStatus::BUFF_RD_READY) {
		return StreamSocket::m_ready;
	}
	else {
		return StreamSocket::m_unReady;
	}
}

// ����ǰ socket �����ݶ���������.
void fstServ::StreamSocket::UnReadyState::read(StreamSocket& ss, SocketHandler& handler)
{
	Protocol protocol;
	// ��ȡ����Ҫ��ȡ�ĳ���.
	size_t length = Protocol::Header::size();
	// ������ζ�ȡ����Э����ĵ�һ�ζ�ȡ.
	if (ss.getReadBuff().size() == length) {
		// ������Э��ͷ.
		Protocol::Header header = protocol.unpackHeader(ss.getReadBuff().c_str(), length);
		// Ŀ���ȡ���ȱ��Ϊ(��ͷ + ���峤�� - �Ѿ���ȡ�ĳ���).
		length = header.size() + header.m_length - ss.getReadBuff().size();
		// �жϰ��峤���Ƿ񳬹������������.
		if (header.m_length > Buff::MAX_LEN) {
			const char* msg = "The packet received exceed max limit.";
			std::cerr << msg << std::endl;
			throw std::runtime_error(msg);
		}
	}
	// �����Ѿ���ȡ�ĳ���.
	int ready = 0, left = (int)length;
	// ����Ŀ�곤�ȵĻ���.
	char* cp = new char[length] {'\0'};
	// ��ʼ��ȡ����.
	while (left) {
		int ret = handler.readn(ss.getFd(), cp + ready, left);
		left -= ret;
		ready += ret;
	}
	// ����ȡ��������ӵ���������.
	ss.getReadBuff().append(cp, length);
	delete[] cp;
	// ���ζ�ȡ�������.
	if (ss.getReadBuff().size() == Protocol::Header::size()) {
		// ��ʾ��ͷ���꣬��ʱ��ֱ�ӷ��أ������½���˺�����������������.
		return;
	}
	else {
		// ����Э���ȡ��ȫ���ı������״̬Ϊ ready.����һ�μ���ᴥ����ȡ��ready�Ĵ���
		ss.setBuffStatus(ss.getBuffStatus() | StreamSocket::BuffStatus::BUFF_RD_READY);
	}
}

// ��һ�� package ���ݻص��������ӿ�.
void fstServ::StreamSocket::ReadyState::read(StreamSocket& ss, SocketHandler& handler)
{
	// ������Գ���� Protocol�������� buff �е�����ȥ��Э��header.
	Protocol protocol;
	const char* data = ss.getReadBuff().c_str();
	// ����˽��յ������󣬽���ҵ����Ļص�.
	handler.getServer().onReceive(ss, protocol.unpack(data, ss.getReadBuff().size()).c_str());
	// ��ջ�������.
	ss.getReadBuff().clear();
	// ���� readBuff ״̬Ϊ unready.
	ss.setBuffStatus(ss.getBuffStatus() & ~StreamSocket::BuffStatus::BUFF_RD_READY);
	// ��д�����е�����ͨ�� protocol ���.
	const char* tmp = protocol.pack(ss.getWriteBuff().str());
	ss.getWriteBuff().clear();
	// �������Э����������´��뻺��.
	ss.getWriteBuff().append(tmp, protocol.size());
	// ����ǰ�ӿڵ�д�¼��������.
	LpSocket ls = handler.getServer().getClient(ss);
	handler.getServer().getPoller().addWriteStream(ls);
}