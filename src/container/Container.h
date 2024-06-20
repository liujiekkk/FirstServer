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

		// ������������id���߻Ựid�洢�Ķ����.
		using MapPools = std::unordered_map<std::string, std::shared_ptr<ObjectPool>>;

		// �洢��������.
		ObjectPool* m_singletonPool;

		// �洢�ɸ��ƶ���.
		ObjectPool* m_prototypePool;

		// �������أ���������ID�洢.
		MapPools m_requestPool;

		// �Ự����أ����� sessionId �洢
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
		// ���ݶ������ͻ�ȡ��Ӧ��ʵ��. @todo �������������������������Զ����ɶ���.�˴���ֻ��ʵ���޲ι���.
		auto spObj = std::make_shared<Object>();
		// �����ɵ�ʵ���洢�������ڲ�. @todo �����д洢��ʵ����ʵ���ķ�Χ�����Լ�����.��Ҫ������д�붨����.
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