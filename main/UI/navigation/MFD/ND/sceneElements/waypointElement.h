#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/UI/spatial.h>

#include <settings/settings.h>
#include <types/navigationData.h>

namespace pizda {
	using namespace YOBA;
	using namespace YOBA::spatial;

	class WaypointElement : public SceneElement {
		public:
			explicit WaypointElement(uint16_t waypointIndex);

			void onRender(Renderer* renderer, const Scene& scene, const Vector3F* vertices) override;
			const Vector3F* getVertices() override;
			uint16_t getVertexCount() override;

		private:
			uint16_t waypointIndex;
	};
}
