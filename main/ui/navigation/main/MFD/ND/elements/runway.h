#pragma once

#include <numbers>

#include "src/main.h"
#include "src/ui.h"

#include "../../../../../../vector3.h"
#include "../../../../../../sinAndCos.h"
#include "../../../../../../geographicCoordinates.h"
#include "../../../../../../runway.h"

#include "../../../../../spatial/element.h"
#include "../../../../../spatial/spatialView.h"
#include "../../../../../spatial/camera.h"

#include "../../../../../theme.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;


	class RunwayElement : public LinearMesh {
		public:
			RunwayElement(const Runway& runway, const Color* color) : _runway(runway) {
				// 0     1                           0     1
				// |--|--|                           |--|--|
				// |--|--|                          |--|--|
				// |--|--|                         |--|--|
				// |--C--|  > 10 deg heading >    |--C--|
				// |--|--|                       |--|--|
				// |--|--|                      |--|--|
				// |--|--|                     |--|--|
				// 2     3                     3     2
				const auto headingRadians = toRadians((float) _runway.getHeadingDegrees());
				const auto& headingVectorNorm = Vector2F(std::sinf(headingRadians), std::cosf(headingRadians));
				const auto center01Meters = headingVectorNorm * ((float) runway.getLength() / 2.f);
				const auto corner1OffsetMeters = headingVectorNorm.clockwisePerpendicular() * ((float) runway.getWidth() / 2.f);
				const auto corner0Radians = (center01Meters - corner1OffsetMeters) * GeographicCoordinates::radiansPerMeter;
				const auto corner1Radians = (center01Meters + corner1OffsetMeters) * GeographicCoordinates::radiansPerMeter;

				_vertices[0] = radialCornerToVertex(corner0Radians);
				_vertices[1] = radialCornerToVertex(corner1Radians);
				_vertices[2] = radialCornerToVertex(-corner0Radians);
				_vertices[3] = radialCornerToVertex(-corner1Radians);

				setVertices(_vertices);
				setVertexCount(4);

				setLineVertexIndices(_lineVertexIndices);
				setLineVertexIndicesCount(8);

				setColor(color);
			}

		private:
			Vector3F _vertices[4];

			constexpr static const uint16_t _lineVertexIndices[] = {
				0, 1,
				1, 2,
				2, 3,
				0, 3
			};

			Runway _runway;

			Vector3F radialCornerToVertex(Vector2F corner) {
				return GeographicCoordinates(
					_runway.getCenter().getLatitude() + corner.getY(),
					_runway.getCenter().getLongitude() + corner.getX(),
					_runway.getCenter().getAltitude()
				).toCartesian();
			}
	};
}