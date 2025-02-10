#pragma once

#include "../page.h"
#include "nd.h"

namespace pizdanc {
	using namespace yoba;

	class NDPage : public Page {
		public:
			NDPage();

		private:
			ND _nd;
	};
}