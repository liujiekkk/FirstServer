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

		// 尾部追加数据.
		size_t append(const char *, size_t size);

		// 使头部size长的数据失效.
		size_t discard(size_t size);

		// 清空buff数据.
		void clear();

		// 返回buff 的大小.
		size_t size() const;

		// 获取 byte 数据.
		const char* c_str() const;

		const std::string& str() const;

	private:

		std::string m_data;
	};

	void toPackage(const Buff&, Package*);

}