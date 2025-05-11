#pragma once

#include <numbers>
#include <esp_log.h>

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/UI/spatial.h>

#include "../../../rc.h"
#include "../../../settings/settings.h"

namespace pizda {
	using namespace YOBA;
	using namespace YOBA::spatial;

	class RunwayMesh : public LinearMesh {
		public:
			RunwayMesh(const SettingsNavigationRunway* runway, const Color* color) : _runway(runway) {
				_waypoint = &RC::getInstance().getSettings().navigation.waypoints[runway->waypointIndex];

				// 0     1                           0     1
				// |--|--|                           |--|--|
				// |--|--|                          |--|--|
				// |--|--|                         |--|--|
				// |--C--|  > 10 deg heading >    |--C--|
				// |--|--|                       |--|--|
				// |--|--|                      |--|--|
				// |--|--|                     |--|--|
				// 3     2                     3     2
				const auto headingRad = toRadians(static_cast<float>(_runway->headingDeg));
				const auto& headingVectorNorm = Vector2F(std::sinf(headingRad), std::cosf(headingRad));
				const auto center01M = headingVectorNorm * (static_cast<float>(_runway->lengthM) / 2.f);
				const auto corner0OffsetM = headingVectorNorm.counterClockwisePerpendicular() * (static_cast<float>(_runway->widthM) / 2.f);
				const auto corner0Rad = (center01M + corner0OffsetM) * GeographicCoordinates::equatorialRadiansPerMeter;
				const auto corner1Rad = (center01M - corner0OffsetM) * GeographicCoordinates::equatorialRadiansPerMeter;

				ESP_LOGI("Runway", "---------------------");
				ESP_LOGI("Runway", "heading (deg): %d", _runway->headingDeg);
				ESP_LOGI("Runway", "headingVectorNorm: %f, %f", headingVectorNorm.getX(), headingVectorNorm.getY());
				ESP_LOGI("Runway", "center01Meters: %f, %f", center01M.getX(), center01M.getY());
				ESP_LOGI("Runway", "corner0OffsetMeters: %f, %f", corner0OffsetM.getX(), corner0OffsetM.getY());
				ESP_LOGI("Runway", "corner0Radians: %f, %f", corner0Rad.getX(), corner0Rad.getY());
				ESP_LOGI("Runway", "corner1Radians: %f, %f", corner1Rad.getX(), corner1Rad.getY());

				_vertices[0] = cornerToVertex(corner0Rad);
				_vertices[1] = cornerToVertex(corner1Rad);
				_vertices[2] = cornerToVertex(-corner0Rad);
				_vertices[3] = cornerToVertex(-corner1Rad);

				setVertices(_vertices);
				setVertexCount(4);

				setLineVertexIndices(_lineVertexIndices);
				setLineVertexIndicesCount(8);

				setColor(color);
			}

		private:
			Vector3F _vertices[4];

			constexpr static uint16_t _lineVertexIndices[] = {
				0, 1,
				1, 2,
				2, 3,
				0, 3
			};

			const SettingsNavigationRunway* _runway;
			const SettingsNavigationWaypoint* _waypoint;

			Vector3F cornerToVertex(const Vector2F& corner) const {
				const auto lat = _runway->coordinates.getLatitude() + corner.getY();

				return GeographicCoordinates(
					lat,
					_runway->coordinates.getLongitude() + corner.getX() / std::cosf(lat),
					_runway->coordinates.getAltitude()
				).toCartesian();
			}
	};
}