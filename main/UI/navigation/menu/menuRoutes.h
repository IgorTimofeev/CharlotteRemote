#pragma once

#include "MFDMenuView.h"
#include "pagesMenuView.h"

namespace pizda {
	class MenuRoutes {
		public:
			constexpr static const ElementRoute<MFDMenuView> MFD = {};
			constexpr static const ElementRoute<PagesMenuView> settings = {};
	};
}