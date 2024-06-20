#include "StreamSocket.h"
#include "SocketHandler.h"
#include "protocol/Protocol.h"
#include "server/AbstractServer.h"
#include <string>
#include <iostream>

// 定义静态变量 ReadyState
fstServ::StreamSocket::ReadyState fstServ::StreamSocket::m_ready;

// 定义静态变量 UnReadyState
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
	// 写入数据到缓存.
	m_buffWrite->append(cp, len);
}

void fstServ::StreamSocket::read(SocketHandler& handler)
{
	// 如果一次数据读取完全后，需要继续处理ready状态的数据（具体业务 onReceive 回调）
	do {
		// 获取当前状态
		IState& state = getState();
		// 读取当前 socket 的数据到缓存.
		state.read(*this, handler);
	} while (m_buffStatus & StreamSocket::BuffStatus::BUFF_RD_READY);
}

fstServ::Buff& fstServ::StreamSocket::getReadBuff()
{
	// 将 buff 数据转换成 Package.
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

// 将当前 socket 的数据读到缓存中.
void fstServ::StreamSocket::UnReadyState::read(StreamSocket& ss, SocketHandler& handler)
{
	Protocol protocol;
	// 获取本次要读取的长度.
	size_t length = Protocol::Header::size();
	// 如果本次读取不是协议包的第一次读取.
	if (ss.getReadBuff().size() == length) {
		// 解析出协议头.
		Protocol::Header header = protocol.unpackHeader(ss.getReadBuff().c_str(), length);
		// 目标读取长度变更为(包头 + 包体长度 - 已经读取的长度).
		length = header.size() + header.m_length - ss.getReadBuff().size();
		// 判断包体长度是否超过缓存最大限制.
		if (header.m_length > Buff::MAX_LEN) {
			const char* msg = "The packet received exceed max limit.";
			std::cerr << msg << std::endl;
			throw std::runtime_error(msg);
		}
	}
	// 本次已经读取的长度.
	int ready = 0, left = (int)length;
	// 申请目标长度的缓存.
	char* cp = new char[length] {'\0'};
	// 开始读取数据.
	while (left) {
		int ret = handler.readn(ss.getFd(), cp + ready, left);
		left -= ret;
		ready += ret;
	}
	// 将读取的数据添加到读缓冲区.
	ss.getReadBuff().append(cp, length);
	delete[] cp;
	// 本次读取长度完成.
	if (ss.getReadBuff().size() == Protocol::Header::size()) {
		// 表示包头读完，这时候直接返回，会重新进入此函数继续读包体数据.
		return;
	}
	else {
		// 整个协议读取完全，改变读缓冲状态为 ready.（下一次检测后会触发读取到ready的处理）
		ss.setBuffStatus(ss.getBuffStatus() | StreamSocket::BuffStatus::BUFF_RD_READY);
	}
}

// 将一个 package 数据回调服务器接口.
void fstServ::StreamSocket::ReadyState::read(StreamSocket& ss, SocketHandler& handler)
{
	// 这里可以抽象出 Protocol，用来将 buff 中的数据去掉协议header.
	Protocol protocol;
	const char* data = ss.getReadBuff().c_str();
	// 服务端接收到的请求，进行业务处理的回调.
	handler.getServer().onReceive(ss, protocol.unpack(data, ss.getReadBuff().size()).c_str());
	// 清空缓存数据.
	ss.getReadBuff().clear();
	// 重置 readBuff 状态为 unready.
	ss.setBuffStatus(ss.getBuffStatus() & ~StreamSocket::BuffStatus::BUFF_RD_READY);
	// 将写缓存中的数据通过 protocol 打包.
	const char* tmp = protocol.pack(ss.getWriteBuff().str());
	ss.getWriteBuff().clear();
	// 将打包成协议的数据重新存入缓存.
	ss.getWriteBuff().append(tmp, protocol.size());
	// 将当前接口的写事件加入监听.
	LpSocket ls = handler.getServer().getClient(ss);
	handler.getServer().getPoller().addWriteStream(ls);
}