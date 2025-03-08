#pragma once

#include <functional>
#include "page.h"

namespace pizda {
	class Route {
		public:
			virtual Page* newPage() = 0;
	};

	template<typename TPage>
	class PageRoute : public Route {
		public:
			Page* newPage() override;
	};

	template<typename TPage>
	Page* PageRoute<TPage>::newPage() {
		return new TPage();
	}
}