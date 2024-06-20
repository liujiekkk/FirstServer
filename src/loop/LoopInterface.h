#pragma once

namespace fstServ {
		
	class LoopInterface
	{
	public:

		virtual ~LoopInterface() {};

		virtual void loop() const = 0;
	};
}