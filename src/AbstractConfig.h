#pragma once

#include <sstream>
#include <unordered_map>
#include <string>

namespace fstServ {

	class AbstractConfig {

	public:

		virtual ~AbstractConfig() {}

		// ���������ļ�����ȡ����.
		virtual void load(std::string& fileName);
		
		// ����������д�������ļ�.
		virtual void save();

		template<typename T>
		const T getValue(const char* key) const {
			// �ҵ� key ���ڵ�ֵ.
			// mock ֵ.
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