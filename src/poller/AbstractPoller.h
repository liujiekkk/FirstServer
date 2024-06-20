#pragma once

#include <unordered_map>
#include "../StreamSocket.h"
#include "../Sock.h"

namespace fstServ {

	namespace poller {
	
		class AbstractPoller {

		public:
			
			virtual ~AbstractPoller() {}

			virtual int addReadStream(LpSocket);

			virtual int removeReadStream(LpSocket);

			virtual int addWriteStream(LpSocket);

			virtual int removeWriteStream(LpSocket);

			virtual int wait(SSockSet& set) = 0;

			virtual int wait(SSockSet& set, long) = 0;

		protected:

			socket_t getMaxFd() const;

			const SSockSet& getReadStreamSet() const;

			const SSockSet& getWriteStreamSet() const;

		private:

			SSockSet m_socketRead;

			SSockSet m_socketWrite;

			socket_t getMaxFd(const SSockSet ss) const;

			// 判断是否存在指定的StreamSocket.
			std::pair<bool, SSockSet::const_iterator> exists(LpSocket, const SSockSet&);
		};
	}
}