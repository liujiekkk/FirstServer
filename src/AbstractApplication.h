#pragma once 

#include "ApplicationInterface.h"

namespace fstServ {

	class AbstractServer;
	class ApplicationProcessor;
	class AbstractConfig;

	class AbstractApplication : public ApplicationInterface {

	public:

		AbstractApplication();
		
		~AbstractApplication();

		void init();

		void run() override;

		void setConfig(AbstractConfig*);

	protected:

		AbstractConfig* m_config;

		AbstractServer* m_server;

		ApplicationProcessor* m_processor;
	};
}