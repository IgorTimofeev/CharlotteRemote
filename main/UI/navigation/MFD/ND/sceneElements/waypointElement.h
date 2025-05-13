#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/UI/spatial.h>

#include "../../../../../settings/settings.h"

namespace pizda {
	using namespace YOBA;
	using namespace YOBA::spatial;

	class WaypointElement : public SceneElement {
		public:
			explicit WaypointElement(const SettingsNavigationWaypoint* waypoint);

			void onRender(Renderer* renderer, const Scene& scene, const Vector3F* vertices) override;
			const Vector3F* getVertices() override;
			uint16_t getVertexCount() override;

		private:
			Vector3F _vertex;
			const SettingsNavigationWaypoint* _waypoint;
	};
}
