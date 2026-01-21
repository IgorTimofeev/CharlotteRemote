#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/UI/spatial.h>

#include "types/settings/settings.h"
#include "types/navigationData.h"

namespace pizda {
	using namespace YOBA;
	using namespace YOBA::spatial;

	class RouteElement : public SceneElement {
		public:
			RouteElement(const Vector3F& from, const Vector3F& to, const Color* color);

			const Vector3F* getVertices() override;
			uint16_t getVertexCount() override;
			void onRender(Renderer* renderer, const Scene& scene, const Vector3F* projectedVertices) override;

		private:
			Vector3F _vertices[2];
			const Color* _color;
	};
}
