#pragma once

#include "../AbstractApplication.h"

namespace fstServ {

	class AbstractProcessor
	{

	public:

		AbstractProcessor(AbstractApplication& app): m_application(app) {}

		virtual ~AbstractProcessor() {}

		virtual void handle() = 0;

	protected:

		AbstractApplication& m_application;

	};
}