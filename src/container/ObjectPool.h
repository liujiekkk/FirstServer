#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <cstdio>

namespace fstServ {

	class ObjectPool {

	public:

		template<typename Object>
		Object& get(const std::string& key) {
			auto iter = m_data.find(key);
			if (iter != m_data.end()) {
				return *(std::static_pointer_cast<Object>((*iter).second));
			}
			char buf[100];
			std::sprintf(buf, "ObjectPool get key %s is not exists.", key.c_str());
			throw std::invalid_argument(buf);
		}

		template<typename Object>
		void set(const std::string& key, std::shared_ptr<Object> ptr)
		{
			if (ptr == nullptr) {
				throw "ObjectPool set a null object pointer";
			}
			m_data[key] = ptr;
		}

		bool has(const std::string& key)
		{
			auto iter = m_data.find(key);
			if (iter != m_data.end()) {
				return true;
			}
			return false;
		}

		template<typename Object>
		auto operator[](const std::string& key) -> decltype(get<Object>(key))
		{
			return get<Object>(key);
		}

	private:

		std::unordered_map<std::string, std::shared_ptr<void>> m_data;
	};
}