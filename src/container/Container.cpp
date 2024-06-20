#include "Container.h"

fstServ::Container::Container(): m_singletonPool(new ObjectPool()), m_prototypePool(new ObjectPool()), m_requestPool(), m_sessionPool()
{

}

fstServ::Container::~Container()
{
	if (m_singletonPool) {
		delete m_singletonPool;
	}
	if (m_prototypePool) {
		delete m_prototypePool;
	}
}

void fstServ::Container::destructRequest(const std::string& id)
{
	m_requestPool.erase(id);
}

void fstServ::Container::destructSession(const std::string& id)
{
	m_sessionPool.erase(id);
}
