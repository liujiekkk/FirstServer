#pragma once

#include "AbstractProcessor.h"

namespace fstServ {

	class ConsoleProcessor : public AbstractProcessor {

	public:

		ConsoleProcessor(AbstractApplication& app);

		void handle() override;
	};
}