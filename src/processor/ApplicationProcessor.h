#pragma once

#include "AbstractProcessor.h"
#include "../AbstractApplication.h"
#include <memory>
#include <vector>

namespace fstServ {

	class AbstractApplication;

	class ApplicationProcessor : public AbstractProcessor {
	
	public:

		ApplicationProcessor(AbstractApplication& app);

		void handle() override;

		void addProcessor(std::shared_ptr<AbstractProcessor> processor);

	private:

		std::vector<std::shared_ptr<AbstractProcessor>> m_processList;

	};
}