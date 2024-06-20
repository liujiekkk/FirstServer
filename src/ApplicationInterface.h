#pragma once

namespace fstServ {

	class ApplicationInterface {

	public:

		virtual ~ApplicationInterface() {};
		virtual void run() = 0;
	};
}