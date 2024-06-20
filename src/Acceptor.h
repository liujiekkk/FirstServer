#pragma once

#include "StreamSocket.h"

namespace fstServ {

	namespace poller {

		class AbstractPoller;

	}

	class Acceptor {

	public:

		Acceptor(poller::AbstractPoller&);

		LpSocket accept(StreamSocket& ss);

	private:

		poller::AbstractPoller& m_poller;

	};
}