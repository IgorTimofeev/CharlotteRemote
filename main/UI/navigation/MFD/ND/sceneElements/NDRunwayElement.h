#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/UI/spatial.h>

#include "types/navigationData.h"
#include "UI/elements/spatial/runwayElement.h"

namespace pizda {
	using namespace YOBA;
	using namespace YOBA::spatial;

	class NDRunwayElement : public RunwayElement {
		public:
			NDRunwayElement(const NavigationRunwayData* runwayData, const Color* color);
	};
}
