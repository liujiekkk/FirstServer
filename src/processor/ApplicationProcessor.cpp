#include "ApplicationProcessor.h"

fstServ::ApplicationProcessor::ApplicationProcessor(AbstractApplication& app) : AbstractProcessor(app) {}

void fstServ::ApplicationProcessor::handle()
{
	// Ӧ����ز���.
	for (auto& p : m_processList) {
		p->handle();
	}
}

void fstServ::ApplicationProcessor::addProcessor(std::shared_ptr<AbstractProcessor> processor)
{
	m_processList.push_back(processor);
}
