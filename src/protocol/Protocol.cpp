#include "Protocol.h"
#include <cstring>
#include <stdexcept>

fstServ::Protocol::Protocol() : m_header(), m_buff()
{
}

const char* fstServ::Protocol::pack(const std::string& data)
{
	clear();
	// 设置包长度.
	m_header.m_length = data.size();
	m_buff.append((char*) &m_header, Header::size());
	m_buff.append(data.c_str(), data.size());
	return m_buff.c_str();
}

const std::string& fstServ::Protocol::unpack(const char* data, size_t size)
{
	unpackHeader(data, size);
	// m_buff.append((char*) & m_header, m_header.size());
	m_buff.append(data + m_header.size(), m_header.m_length);
	return m_buff.str();
}

const fstServ::Protocol::Header& fstServ::Protocol::unpackHeader(const char* data, size_t size)
{
	if (size < sizeof(Header)) {
		throw std::length_error("Protocol unpack param data lenght is too short.");
	}
	clear();
	std::memcpy(&m_header, data, Header::size());
	return m_header;
}

const size_t fstServ::Protocol::size() const
{
	return m_buff.size();
}

void fstServ::Protocol::clear()
{
	// 重置头.
	m_header.reset();
	// 清空缓冲区，将数据复制到缓存.
	m_buff.clear();
}

fstServ::Protocol::Header::Header(size_t lenght, size_t uid, const char* sid, const char* rid)
	: m_length(lenght), m_uid(uid), m_sid{0}, m_rid{0}
{
	std::memcpy(m_sid, sid, strlen(sid));
	std::memcpy(m_rid, rid, strlen(rid));
}

void fstServ::Protocol::Header::setSid(const char(*ptr)[64])
{
	std::memcpy(m_sid, *ptr, sizeof(*ptr));
}

void fstServ::Protocol::Header::setRid(const char(*ptr)[64])
{
	std::memcpy(m_rid, *ptr, sizeof(*ptr));
}

void fstServ::Protocol::Header::reset()
{
	m_length = 0U;
	m_uid = 0U;
	std::memset(m_sid, 0, sizeof(m_sid));
	std::memset(m_rid, 0, sizeof(m_rid));
}

int fstServ::Protocol::Header::size()
{
	return sizeof(Header);
}
