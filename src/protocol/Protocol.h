#pragma once

#include <string>
#include <vector>
#include "../buff/Buff.h"

namespace fstServ {

	class Protocol {

	public:

		struct Header {

			size_t m_length; // ���峤�ȣ���������ͷ��.

			size_t m_uid; // �û�id

			char m_sid[64]; // �Ựid

			char m_rid[64]; // ����id

			Header(size_t lenght = 0, size_t uid = 0, const char* sid = "", const char* rid = "");

			// ���ûỰID����.
			void setSid(const char(*)[64]);

			// ��������ID����.
			void setRid(const char(*)[64]);

			// ������������ΪĬ��ֵ.
			void reset();

			// ��ȡͷ��С.
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