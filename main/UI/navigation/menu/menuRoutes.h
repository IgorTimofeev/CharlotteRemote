#pragma once

#include "MFDMenuView.h"
#include "pagesMenuView.h"

namespace pizda {
	class MenuRoutes {
		public:
			constexpr static ElementRoute<MFDMenuView> MFD = {};
			constexpr static ElementRoute<PagesMenuView> settings = {};
	};
}