#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/UI/spatial.h>

#include "UI/elements/spatial/runwayElement.h"

namespace pizda {
	using namespace YOBA;
	using namespace YOBA::spatial;

	class NDRunwayElement : public RunwayElement {
		public:
			NDRunwayElement(uint16_t runwayIndex);

			void onRender(Renderer* renderer, const Scene& scene, const Vector3F* projectedVertices) override;
	};
}
