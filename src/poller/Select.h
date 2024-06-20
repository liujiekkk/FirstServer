#pragma once

#include "AbstractPoller.h"

namespace fstServ {

	namespace poller {
	
		class Select : public AbstractPoller {

		public:

			Select();

			~Select();

			int wait(SSockSet&);

			int wait(SSockSet&, long);

			int addReadStream(LpSocket) override;

			int removeReadStream(LpSocket) override;

			int addWriteStream(LpSocket) override;

			int removeWriteStream(LpSocket) override;

		private:

			socket_t m_maxFd;
		};
	}
}