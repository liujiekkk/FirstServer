#include "Select.h"
#include <WinSock2.h>
#include "../StreamSocket.h"

fstServ::poller::Select::Select(): m_maxFd(0U)
{
}

fstServ::poller::Select::~Select()
{
}

int fstServ::poller::Select::wait(SSockSet& set)
{
	return wait(set, 0);
}

int fstServ::poller::Select::wait(SSockSet& set, long time)
{
	FD_SET readSet;
	FD_ZERO(&readSet);
	// 添加文件描述符到集合.
	for (auto& ss : getReadStreamSet()) {
		// 将要监听的读套接字添加到 fd_set
		FD_SET(ss.second->getFd(), &readSet);
	}

	FD_SET writeSet;
	FD_ZERO(&writeSet);
	for (auto& ss : getWriteStreamSet()) {
		FD_SET(ss.second->getFd(), &writeSet);
	}

	// 设置时间.
	struct timeval* ptr = nullptr;
	if (time > 0) {
		timeval t;
		t.tv_sec = static_cast<int>(time / 1000000), t.tv_usec = time - t.tv_sec * 1000000;
		ptr = &t;
	}
	int ret = select((int)(m_maxFd + 1), &readSet, &writeSet, nullptr, ptr);
	// 如果有套接字有事件发生，通过参数传递到外部.
	if (ret) {
		for (int i = 0; i <= m_maxFd; ++i) {
			// 如果监听的可读事件套接字集合有事件.
			if (FD_ISSET(i, &readSet)) {
				LpSocket ss = getReadStreamSet().at(i);
				ss->setEvents(fstServ::StreamSocket::EventStatus::EV_RD | ss->getEvents());
				set[i] = ss;
				FD_CLR(i, &readSet);
			}
			// 如果监听可写事件的套接字集合有事件.
			if (FD_ISSET(i, &writeSet)) {
				LpSocket ss = getWriteStreamSet().at(i);
				ss->setEvents(fstServ::StreamSocket::EventStatus::EV_WR | ss->getEvents());
				set[i] = ss;
				FD_CLR(i, &writeSet);
			}
		}
	}

	return ret;
}

// 添加套接字到多路复用器.
int fstServ::poller::Select::addReadStream(LpSocket ls)
{
	if (AbstractPoller::addReadStream(ls)) {
		socket_t fd = ls->getFd();
		if (m_maxFd < fd) {
			m_maxFd = fd;
		}
		return 1;
	}
	return 0;
}

int fstServ::poller::Select::removeReadStream(LpSocket ls)
{
	if (AbstractPoller::removeReadStream(ls)) {
		// 检查当前监听套接字中最大的fd值.
		socket_t fd = getMaxFd();
		if (fd) {
			m_maxFd = fd;
		}
		return 1;
	}
	return 0;
}

int fstServ::poller::Select::addWriteStream(LpSocket ls)
{
	if (AbstractPoller::addWriteStream(ls)) {
		if (ls->getFd() > m_maxFd) {
			m_maxFd = ls->getFd();
		}
		return 1;
	}
	return 0;
}

int fstServ::poller::Select::removeWriteStream(LpSocket ls)
{
	if (AbstractPoller::removeWriteStream(ls)) {
		// 检查当前监听套接字中最大的fd值.
		socket_t fd = getMaxFd();
		if (fd) {
			m_maxFd = fd;
		}
		return 1;
	}
	return 0;
}
