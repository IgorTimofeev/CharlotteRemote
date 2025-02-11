#include "nd.h"
#include <sstream>
#include <cmath>
#include "../../../rc.h"

namespace pizda {
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

		for (auto& point : _points) {
			auto pointPosition = geographicToCartesian(
				distanceFromAircraftToEarthCenter,
				SinAndCos(point.getX()),
				SinAndCos(point.getY())
			);

			pointPosition.setX(pointPosition.getX() * std::sinf())

			renderer->renderCircle(
				Point(

				),
				10,
				&Theme::yellow
			);
		}

		// Pixels per meter
		renderer->renderString(
			bounds.getTopLeft(),
			&Theme::fontNormal,
			&Theme::fg1,
			std::format(L"Scale: {}", _pixelsPerMeter)
		);
	}

	void ND::onEvent(Event& event) {
		Element::onEvent(event);

		if (event.getTypeID() == PinchDownEvent::typeID) {
			auto pinchDownEvent = (PinchDownEvent&) event;

			_pinchDownPixelsPerMeter = _pixelsPerMeter;
			_pinchDownLength = (pinchDownEvent.getPosition2() - pinchDownEvent.getPosition1()).getLength();

			event.setHandled(true);
		}
		else if (event.getTypeID() == PinchDragEvent::typeID) {
			auto pinchDragEvent = (PinchDragEvent&) event;

			const auto pinchFactor = (float) (pinchDragEvent.getPosition2() - pinchDragEvent.getPosition1()).getLength() / (float) _pinchDownLength;

			_pixelsPerMeter = _pinchDownPixelsPerMeter * pinchFactor;

			event.setHandled(true);
		}
	}

	Vector3F ND::geographicToCartesian(float distanceFromAircraftToEarthCenter, const SinAndCos& latitude, const SinAndCos& longitude) {
		return {
			distanceFromAircraftToEarthCenter * latitude.getCos() * longitude.getCos(),
			distanceFromAircraftToEarthCenter * latitude.getCos() * longitude.getSin(),
			distanceFromAircraftToEarthCenter * latitude.getSin()
		};
	}
}