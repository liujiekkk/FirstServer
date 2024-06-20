#pragma once

#include "AbstractProcessor.h"

namespace fstServ {

	class AbstractApplication;

	class EventProcessor : public AbstractProcessor {

	public:

		EventProcessor(AbstractApplication& app);

		void handle() override;

	};
}