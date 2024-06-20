#pragma once

#include <string>

namespace fstServ {

	struct Package;

	class Buff
	{
		friend void toPackage(const Buff&, Package*);

	public:

		static const size_t MAX_LEN = 8196;

		Buff();

		Buff(const char*, size_t);

		Buff(const Buff&);

		Buff& operator=(const Buff&);

		void swap(Buff&);

		~Buff();

		// β��׷������.
		size_t append(const char *, size_t size);

		// ʹͷ��size��������ʧЧ.
		size_t discard(size_t size);

		// ���buff����.
		void clear();

		// ����buff �Ĵ�С.
		size_t size() const;

		// ��ȡ byte ����.
		const char* c_str() const;

		const std::string& str() const;

	private:

		std::string m_data;
	};

	void toPackage(const Buff&, Package*);

}