#pragma once

#include "AbstractProcessor.h"

namespace fstServ {

	class AbstractApplication;

	class ConfigProcessor : public AbstractProcessor {

	public:

		ConfigProcessor(AbstractApplication& app);

		void handle() override;
	};
}