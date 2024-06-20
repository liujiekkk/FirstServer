#include "AbstractPoller.h"

int fstServ::poller::AbstractPoller::addReadStream(LpSocket ptr)
{
	// 判断是否已经存在
	if (exists(ptr, m_socketRead).first) {
		return 0;
	}
	m_socketRead[ptr->getFd()] = ptr;
	return 1;
}

int fstServ::poller::AbstractPoller::removeReadStream(LpSocket ptr)
{
	auto result = exists(ptr, m_socketRead);
	if (result.first) {
		m_socketRead.erase(result.second);
		return 1;
	}
	return 0;
}

int fstServ::poller::AbstractPoller::addWriteStream(LpSocket ptr)
{
	if (exists(ptr, m_socketWrite).first) {
		return 0;
	}
	m_socketWrite[ptr->getFd()] = ptr;
	return 1;
}

int fstServ::poller::AbstractPoller::removeWriteStream(LpSocket ptr)
{
	auto result = exists(ptr, m_socketWrite);
	if (result.first) {
		m_socketWrite.erase(result.second);
		return 1;
	}
	return 0;
}

// 获取读、写套接字集合中最大的fd值.
socket_t fstServ::poller::AbstractPoller::getMaxFd() const
{
	socket_t rfd = getMaxFd(m_socketRead);
	socket_t wfd = getMaxFd(m_socketWrite);
	return rfd > wfd ? rfd : wfd;
}

const fstServ::SSockSet& fstServ::poller::AbstractPoller::getReadStreamSet() const
{
	return m_socketRead;
}

const fstServ::SSockSet& fstServ::poller::AbstractPoller::getWriteStreamSet() const
{
	return m_socketWrite;
}

socket_t fstServ::poller::AbstractPoller::getMaxFd(const SSockSet ss) const
{
	auto it = std::max_element(ss.begin(), ss.end(), [](const auto& e1, const auto& e2) {
		return e1.first < e2.first;
		});
	if (it != ss.end()) {
		return it->first;
	}
	return 0U;
}

std::pair<bool, fstServ::SSockSet::const_iterator> fstServ::poller::AbstractPoller::exists(LpSocket ptr, const SSockSet& set)
{
	// 判断是否已经存在
	auto iter = set.find(ptr->getFd());
	if (iter != set.end()) {
		return std::make_pair(true, iter);
	}
	return std::make_pair(false, set.cend());
}

