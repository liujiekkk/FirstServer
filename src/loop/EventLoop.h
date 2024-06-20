#pragma once

#include "AbstractLoop.h"

namespace fstServ {


	class EventLoop : AbstractLoop {

	public:

		EventLoop();

		void loop() const;

	};

}