#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/UI/spatial.h>

#include "../../../../../settings/settings.h"

namespace pizda {
	using namespace YOBA;
	using namespace YOBA::spatial;

	class RouteElement : public SceneElement {
		public:
			RouteElement(uint16_t waypointIndexFrom, uint16_t waypointIndexTo, const Color* color);

			const Vector3F* getVertices() override;
			uint16_t getVertexCount() override;
			void onRender(Renderer* renderer, const Scene& scene, const Vector3F* vertices) override;

		private:
			const Color* _color;
			Vector3F _vertices[2] {};
	};
}
