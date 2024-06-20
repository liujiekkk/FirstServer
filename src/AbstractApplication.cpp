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
		// 环境信息初始化
		m_processor->addProcessor(std::make_shared<EnvProcessor>(*this));
		// 配置信息初始化
		m_processor->addProcessor(std::make_shared<ConfigProcessor>(*this));
		// 事件循环初始化
		m_processor->addProcessor(std::make_shared<EventProcessor>(*this));
		// 控制台初始化
		m_processor->addProcessor(std::make_shared<ConsoleProcessor>(*this));
	}

	void AbstractApplication::run()
	{
		try {
			m_processor->handle();

			// @todo test code for server.
			// 初始化服务对象.
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