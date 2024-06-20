#pragma once

#include <sstream>
#include <unordered_map>
#include <string>

namespace fstServ {

	class AbstractConfig {

	public:

		virtual ~AbstractConfig() {}

		// 加载配置文件，读取配置.
		virtual void load(std::string& fileName);
		
		// 将现有配置写入配置文件.
		virtual void save();

		template<typename T>
		const T getValue(const char* key) const {
			// 找到 key 所在的值.
			// mock 值.
			std::string str = "";
			if (key == "port") {
				str = "5005";
			}
			else if (key == "host") {
				str = "127.0.0.1";
			}
			std::stringstream s(str);
			T t;
			s >> t;
			return t;
		}

		virtual void setValue(const char* key, const char* value) = 0;

	private:

		std::string m_file;

		std::unordered_map<std::string, std::string> m_data;

	};
}