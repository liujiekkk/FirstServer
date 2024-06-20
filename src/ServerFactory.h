#pragma once

#include "server/AbstractServer.h"
#include "AbstractConfig.h"

namespace fstServ {

	class ServerFactory {

	public:

		static fstServ::AbstractServer* getServer(const AbstractConfig&);
	};
}