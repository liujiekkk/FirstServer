#pragma once

#include "LoopInterface.h"
#include <set>

namespace fstServ {

	class AbstractLoop : public LoopInterface
	{
	public:

		AbstractLoop();
			
		~AbstractLoop();

		void addLoop(const LoopInterface*);

		void removeLoop(const LoopInterface*);

	protected:

		std::set<const LoopInterface*> m_loops;

	};
}