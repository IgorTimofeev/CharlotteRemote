#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/UI/spatial.h>

#include <settings/settings.h>
#include <types/navigationData.h>

namespace pizda {
	using namespace YOBA;
	using namespace YOBA::spatial;

	class RouteElement : public SceneElement {
		public:
			RouteElement(const NavigationWaypointData* from, const NavigationWaypointData* to, const Color* color);

			const Vector3F* getVertices() override;
			uint16_t getVertexCount() override;
			void onRender(Renderer* renderer, const Scene& scene, const Vector3F* vertices) override;

		private:
			const NavigationWaypointData* _from;
			const NavigationWaypointData* _to;
			Vector3F _vertices[2];
			const Color* _color;
	};
}
