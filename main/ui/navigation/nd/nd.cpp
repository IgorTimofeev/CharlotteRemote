#include "nd.h"
#include <sstream>
#include <cmath>
#include "../../../rc.h"

namespace pizdanc {
	ND::ND() {
		setClipToBounds(true);
	}

	void ND::onRender(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();
		auto& remoteData = rc.getRemoteData();
		auto& computedData = rc.getComputedData();

		const auto distanceFromAircraftToEarthCenter = _earthEquatorialRadius + remoteData.getAltitude();

		const auto aircraftPosition = geographicToCartesian(
			distanceFromAircraftToEarthCenter,
			computedData.getLatitudeSinAndCos(),
			computedData.getLongitudeSinAndCos()
		);


	}

	Vector3F ND::geographicToCartesian(float distanceFromAircraftToEarthCenter, const SinAndCos& latitude, const SinAndCos& longitude) {
		return {
			distanceFromAircraftToEarthCenter * latitude.getCos() * longitude.getCos(),
			distanceFromAircraftToEarthCenter * latitude.getCos() * longitude.getSin(),
			distanceFromAircraftToEarthCenter * latitude.getSin()
		};
	}


}