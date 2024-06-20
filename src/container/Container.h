#pragma once

#include <string>
#include "ObjectPool.h"

namespace fstServ {

	class Container {

	public:

		enum Scope {SINGLETON, PROTOTYPE, REQUEST, SESSION};

		Container();

		~Container();

		template<typename Object>
		Object& getSingleton(const std::string& name) const;

		template<typename Object>
		Object getPrototype(const std::string& name) const;

		template<typename Object>
		Object& create(std::string name);

		void destructRequest(const std::string&);

		void destructSession(const std::string&);

	private:

		// 用来根据请求id或者会话id存储的对象池.
		using MapPools = std::unordered_map<std::string, std::shared_ptr<ObjectPool>>;

		// 存储单例对象.
		ObjectPool* m_singletonPool;

		// 存储可复制对象.
		ObjectPool* m_prototypePool;

		// 请求对象池，根据请求ID存储.
		MapPools m_requestPool;

		// 会话对象池，根据 sessionId 存储
		MapPools m_sessionPool;

		template<typename Object>
		Object& newInstance(const std::string& name, std::string id = "");

		template<typename Object>
		Object& storeInstance(const std::string& name, Scope scope, std::shared_ptr<Object> spObj, const std::string& id = "");
	};

	template<typename Object>
	inline Object& Container::getSingleton(const std::string& name) const
	{
		if (m_singletonPool->has(name)) {
			return m_singletonPool->get<Object>(name);
		}
	}

	template<typename Object>
	inline Object Container::getPrototype(const std::string& name) const
	{
		if (m_prototypePool->has<Object>(name)) {
			auto origin = m_prototypePool->get<Object>(name);
			return Object(origin);
		}
		// throw exception.
	}

	template<typename Object>
	inline Object& Container::create(std::string name)
	{
		return newInstance<Object>(name);
	}

	template<typename Object>
	inline Object& Container::newInstance(const std::string& name, std::string id)
	{
		// 根据对象类型获取对应的实例. @todo 反射型容器，会根据类的依赖自动生成对象.此处暂只能实现无参构造.
		auto spObj = std::make_shared<Object>();
		// 将生成的实例存储到容器内部. @todo 容器中存储的实例，实例的范围由类自己决定.需要将配置写入定义中.
		return storeInstance(name, Scope::SINGLETON, spObj);
	}

	template<typename Object>
	inline Object& Container::storeInstance(const std::string& name, Scope scope, std::shared_ptr<Object> spObj, const std::string& id)
	{
		switch (scope) {
			case SINGLETON:
				m_singletonPool->set(name, spObj);
				break;
			case PROTOTYPE:
				m_prototypePool->set(name, spObj);
				break;
			case REQUEST:
				m_requestPool[id]->set(name, spObj);
				break;
			case SESSION:
				m_sessionPool[id]->set(name, spObj);
				break;
		}
		return *spObj;
	}
}