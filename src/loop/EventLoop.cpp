#include "EventLoop.h"

fstServ::EventLoop::EventLoop()
{
}

void fstServ::EventLoop::loop() const
{
	for (auto ptr : m_loops) {
		ptr->loop();
	}
}
