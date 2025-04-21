#pragma once

#include <numbers>
#include <esp_log.h>

#include "src/main.h"
#include "src/ui.h"

#include "../../../../../vector3.h"
#include "../../../../../sinAndCos.h"
#include "../../../../../geographicCoordinates.h"
#include "../../../../../runway.h"

#include "../../../../spatial/element.h"
#include "../../../../spatial/spatialView.h"
#include "../../../../spatial/camera.h"

#include "../../../../theme.h"

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
				// 3     2                     3     2
				const auto headingRadians = toRadians((float) _runway.getHeadingDegrees());
				const auto& headingVectorNorm = Vector2F(std::sinf(headingRadians), std::cosf(headingRadians));
				const auto center01Meters = headingVectorNorm * ((float) _runway.getLength() / 2.f);
				const auto corner0OffsetMeters = headingVectorNorm.counterClockwisePerpendicular() * ((float) _runway.getWidth() / 2.f);
				const auto corner0Radians = (center01Meters + corner0OffsetMeters) * GeographicCoordinates::radiansPerMeter;
				const auto corner1Radians = (center01Meters - corner0OffsetMeters) * GeographicCoordinates::radiansPerMeter;

//				ESP_LOGI("Runway", "---------------------");
//				ESP_LOGI("Runway", "heading (deg): %d", _runway.getHeadingDegrees());
//				ESP_LOGI("Runway", "headingVectorNorm: %f, %f", headingVectorNorm.getX(), headingVectorNorm.getY());
//				ESP_LOGI("Runway", "center01Meters: %f, %f", center01Meters.getX(), center01Meters.getY());
//				ESP_LOGI("Runway", "corner0OffsetMeters: %f, %f", corner0OffsetMeters.getX(), corner0OffsetMeters.getY());
//				ESP_LOGI("Runway", "corner0Radians: %f, %f", corner0Radians.getX(), corner0Radians.getY());
//				ESP_LOGI("Runway", "corner1Radians: %f, %f", corner1Radians.getX(), corner1Radians.getY());

				_vertices[0] = cornerToVertex(corner0Radians);
				_vertices[1] = cornerToVertex(corner1Radians);
				_vertices[2] = cornerToVertex(-corner0Radians);
				_vertices[3] = cornerToVertex(-corner1Radians);

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

			Vector3F cornerToVertex(const Vector2F& corner) {
				const auto lat = _runway.getCenter().getLatitude() + corner.getY();

				return GeographicCoordinates(
					lat,
					_runway.getCenter().getLongitude() + corner.getX() / std::cosf(lat),
					_runway.getCenter().getAltitude()
				).toCartesian();
			}
	};
}