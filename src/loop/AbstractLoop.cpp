#include "AbstractLoop.h"

#include <algorithm>

fstServ::AbstractLoop::AbstractLoop() : m_loops()
{
}

fstServ::AbstractLoop::~AbstractLoop()
{
	for (auto& ptr : m_loops)
	{
		delete ptr;
	}
}

void fstServ::AbstractLoop::addLoop(const LoopInterface* pLoop)
{
	m_loops.insert(pLoop);
}

void fstServ::AbstractLoop::removeLoop(const LoopInterface* pLoop)
{
	auto iter = std::find(m_loops.begin(), m_loops.end(), pLoop);
	if (iter != m_loops.end()) {
		m_loops.erase(iter);
	}
}
