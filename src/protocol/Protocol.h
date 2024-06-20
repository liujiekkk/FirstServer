#pragma once

#include <string>
#include <vector>
#include "../buff/Buff.h"

namespace fstServ {

	class Protocol {

	public:

		struct Header {

			size_t m_length; // 包体长度（不包括包头）.

			size_t m_uid; // 用户id

			char m_sid[64]; // 会话id

			char m_rid[64]; // 请求id

			Header(size_t lenght = 0, size_t uid = 0, const char* sid = "", const char* rid = "");

			// 设置会话ID数据.
			void setSid(const char(*)[64]);

			// 设置请求ID数据.
			void setRid(const char(*)[64]);

			// 重置所有数据为默认值.
			void reset();

			// 获取头大小.
			static int size();
		};

		Protocol();

		const char* pack(const std::string&);

		const std::string& unpack(const char*, size_t size);

		const Header& unpackHeader(const char*, size_t size);

		const size_t size() const;

		void clear();

	private:

		Header m_header;

		Buff m_buff;
	};
}