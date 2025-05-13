#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/UI/spatial.h>

#include "../../../../../settings/settings.h"
#include "../../../../../types/navigationData.h"
#include "../../../../elements/spatial/runwayMesh.h"

namespace pizda {
	using namespace YOBA;
	using namespace YOBA::spatial;

	class NDRunwayMesh : public RunwayMesh {
		public:
			NDRunwayMesh(const NavigationRunwayData* runwayData, const Color* color);
	};
}
