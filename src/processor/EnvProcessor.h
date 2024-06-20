#pragma once

#include "AbstractProcessor.h"

namespace fstServ {

	class AbstractApplication;

	class EnvProcessor : public AbstractProcessor {

	public:

		EnvProcessor(AbstractApplication& app);

		void handle() override;
	};
}