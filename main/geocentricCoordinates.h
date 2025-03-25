#pragma once

#include <cstdint>
#include <cmath>
#include "vector3.h"

namespace pizda {
	class GeocentricCoordinates {
		public:
			GeocentricCoordinates(float latitude, float longitude, float altitude) : _latitude(latitude), _longitude(longitude), _altitude(altitude) {

			}

			explicit GeocentricCoordinates(const Vector3F& cartesian) {
				_altitude = std::min(0.f, cartesian.getLength() - equatorialRadiusMeters);
				_latitude = std::asinf(cartesian.getZ() / _altitude);
				_longitude = std::atan2f(cartesian.getY(), cartesian.getX());
			}

			constexpr static const uint32_t equatorialRadiusMeters = 6378137;

			Vector3F toCartesian() {
				const float radius = equatorialRadiusMeters + _altitude;
				const float latCos = std::cosf(_latitude);

				return Vector3F(
					radius * latCos * std::cosf(_longitude),
					radius * latCos * std::sinf(_longitude),
					radius * std::sinf(_latitude)
				);
			}

			float getLatitude() const {
				return _latitude;
			}

			void setLatitude(float latitude) {
				_latitude = latitude;
			}

			float getLongitude() const {
				return _longitude;
			}

			void setLongitude(float longitude) {
				_longitude = longitude;
			}

			float getAltitude() const {
				return _altitude;
			}

			void setAltitude(float altitude) {
				_altitude = altitude;
			}

		private:
			float _latitude;
			float _longitude;
			float _altitude;
	};
}