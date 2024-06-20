#include "ApplicationInterface.h"
#include "processor/AbstractProcessor.h"
#include "processor/EnvProcessor.h"
#include "processor/ConfigProcessor.h"
#include "processor/ApplicationProcessor.h"
#include "processor/EventProcessor.h"
#include "processor/ConsoleProcessor.h"
#include "AbstractConfig.h"
#include "ServerFactory.h"

#include <iostream>

namespace fstServ {

	

	AbstractApplication::AbstractApplication(): m_processor(nullptr), m_config(nullptr), m_server(nullptr)
	{
		init();
	}

	AbstractApplication::~AbstractApplication()
	{
		if (m_config) {
			delete m_config;
		}
		if (m_server) {
			delete m_server;
		}
		if (m_processor) {
			delete m_processor;
		}
	}

	void AbstractApplication::init()
	{
		m_processor = new ApplicationProcessor(*this);
		// ������Ϣ��ʼ��
		m_processor->addProcessor(std::make_shared<EnvProcessor>(*this));
		// ������Ϣ��ʼ��
		m_processor->addProcessor(std::make_shared<ConfigProcessor>(*this));
		// �¼�ѭ����ʼ��
		m_processor->addProcessor(std::make_shared<EventProcessor>(*this));
		// ����̨��ʼ��
		m_processor->addProcessor(std::make_shared<ConsoleProcessor>(*this));
	}

	void AbstractApplication::run()
	{
		try {
			m_processor->handle();

			// @todo test code for server.
			// ��ʼ���������.
			m_server = ServerFactory::getServer(*m_config);
			m_server->start();
		}
		catch (std::string e) {
			std::cout << e << std::endl;
		}
		
	}
	void AbstractApplication::setConfig(AbstractConfig* config)
	{
		m_config = config;
	}
}