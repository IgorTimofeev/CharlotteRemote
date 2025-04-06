#pragma once

#include "src/main.h"
#include "src/ui.h"

#include "../../../../../../vector3.h"
#include "../../../../../../sinAndCos.h"
#include "../../../../../../geographicCoordinates.h"

#include "../../../../../spatial/object.h"
#include "../../../../../spatial/spatialView.h"
#include "../../../../../spatial/camera.h"

#include "../../../../../theme.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class Runway : public LinearMesh {
		public:
			Runway(const Vector3F* vertices, const std::wstring_view& code, const Color* color) : _code(code) {
				setVertices(vertices);
				setVertexCount(4);

				setLineVertexIndices(_lineVertexIndices);
				setLineVertexIndicesCount(8);

				setColor(color);
			}

		private:
			constexpr static const uint16_t _lineVertexIndices[] = {
				0, 1,
				1, 2,
				2, 3,
				0, 3
			};

			const std::wstring_view _code;
	};
}