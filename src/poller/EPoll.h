#pragma once

#include "AbstractPoller.h"

namespace fstServ {

	namespace poller {
	
		class EPoller : public AbstractPoller {

		public:

			EPoller();

			int wait();

		private:

			int m_epfd;
		};
	}
}