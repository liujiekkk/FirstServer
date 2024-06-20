#include "Buff.h"

#include <algorithm>

fstServ::Buff::Buff() : m_data()
{
}

fstServ::Buff::Buff(const char* vp, size_t size): m_data(vp, size)
{
}

fstServ::Buff::Buff(const Buff& rhs)
{
	m_data = rhs.m_data;
}

fstServ::Buff& fstServ::Buff::operator=(const Buff& rhs)
{
	if (this == &rhs) {
		return *this;
	}
	Buff buf(rhs);
	swap(buf);
	return *this;
}

void fstServ::Buff::swap(Buff& rhs)
{
	if (this == &rhs) {
		return;
	}
	using std::swap;
	swap(m_data, rhs.m_data);
}

fstServ::Buff::~Buff()
{
}

size_t fstServ::Buff::append(const char* vp, size_t size)
{
	// 判断剩余长度.
	size_t left = MAX_LEN - m_data.size();
	// 判断剩余空间是否能够存储目标长度.
	if (left > size) {
		left = size;
	}
	m_data.append(vp, size);
	return left;
}

// 从头部丢弃指定长度的数据.
size_t fstServ::Buff::discard(size_t size)
{
	// 如果要失效数据长度大于已存储数据长度，则释放长度为已存储数据长度.
	if (size > m_data.size()) {
		size = m_data.size();
	}
	// 删除头部数据.
	m_data.erase(m_data.begin(), m_data.begin() + size);
	return size;
}

void fstServ::Buff::clear()
{
	m_data.clear();
}

size_t fstServ::Buff::size() const
{
	return m_data.size();
}

const char* fstServ::Buff::c_str() const
{
	return m_data.c_str();
}

const std::string& fstServ::Buff::str() const
{
	return m_data;
}
