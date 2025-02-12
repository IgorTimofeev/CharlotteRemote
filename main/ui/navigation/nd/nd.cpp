#include "nd.h"
#include "../../../rc.h"

namespace pizda {
	ND::ND() {
		setClipToBounds(true);

		*this += &_spatialView;

		const auto distanceFromObjectsToEarthCenter = _earthEquatorialRadius;

		_spatialView.addObject(new Cube(distanceFromObjectsToEarthCenter * 2, &Theme::fg1));

		_spatialView.addObject(new Label(
			geographicToCartesian(
				SinAndCos(yoba::toRadians(60.01483325540486f)),
				SinAndCos(yoba::toRadians(29.69835915766679f)),
				_earthEquatorialRadius
			),
			&Theme::fontNormal,
			&Theme::yellow,
			L"Left"
		));

		_spatialView.addObject(new Label(
			geographicToCartesian(
				SinAndCos(yoba::toRadians(60.014390616612474f)),
				SinAndCos(yoba::toRadians(29.706975357970624f)),
				distanceFromObjectsToEarthCenter
			),
			&Theme::fontNormal,
			&Theme::yellow,
			L"Right"
		));
	}

	void ND::onTick() {
		Layout::onTick();

		auto& rc = RC::getInstance();
		auto& remoteData = rc.getRemoteData();
		auto& computedData = rc.getComputedData();

		auto rotationLatitude = yoba::toRadians(remoteData.getLatitude()) + _cameraOffset.getX();
		auto rotationLongitude = yoba::toRadians(remoteData.getLatitude()) + _cameraOffset.getX();

		auto cameraPosition = geographicToCartesian(
			SinAndCos(rotationLatitude),
			SinAndCos(rotationLongitude),
			_earthEquatorialRadius + _cameraOffset.getZ()
		);

		auto cameraRotation = Vector3F(
			rotationLongitude,
			rotationLatitude,
			0
		);

//		ESP_LOGI("ND", "--------------");
//		ESP_LOGI("ND", "Camera: %f, %f, %f", cameraPosition.getX(), cameraPosition.getY(), cameraPosition.getZ());

		_spatialView.getCamera().setPosition(cameraPosition);
		_spatialView.getCamera().setRotation(cameraRotation);

		invalidate();
	}

	void ND::onRender(Renderer* renderer, const Bounds& bounds) {
		Layout::onRender(renderer, bounds);

		auto position = bounds.getTopLeft() + Point(5, 5);
		auto spacing = Point(0, Theme::fontNormal.getHeight() + 1);

		renderer->renderString(
			position,
			&Theme::fontNormal,
			&Theme::yellow,
			std::format(L"Camera offset X: {}", yoba::toDegrees(_cameraOffset.getX()))
		);

		position += spacing;

		renderer->renderString(
			position,
			&Theme::fontNormal,
			&Theme::yellow,
			std::format(L"Camera offset Y: {}", yoba::toDegrees(_cameraOffset.getY()))
		);

		position += spacing;

		renderer->renderString(
			position,
			&Theme::fontNormal,
			&Theme::yellow,
			std::format(L"Camera offset |: {}", _cameraOffset.getZ())
		);

		position += spacing;
	}

	void ND::onEvent(Event& event) {
		Layout::onEvent(event);

		if (event.getTypeID() == PinchDownEvent::typeID) {
			auto pinchDownEvent = (PinchDownEvent&) event;

			_pinchDownPixelsPerMeter = _metersPerPixel;
			_pinchDownLength = (pinchDownEvent.getPosition2() - pinchDownEvent.getPosition1()).getLength();

			event.setHandled(true);
		}
		else if (event.getTypeID() == PinchDragEvent::typeID) {
			auto pinchDragEvent = (PinchDragEvent&) event;

			const auto pinchFactor = (float) (pinchDragEvent.getPosition2() - pinchDragEvent.getPosition1()).getLength() / (float) _pinchDownLength;

			_metersPerPixel = _pinchDownPixelsPerMeter * pinchFactor;

			_cameraOffset.setZ(_cameraOffset.getZ() + (pinchFactor > 1 ? 10.f : -10.f));

			event.setHandled(true);
		}
		else if (event.getTypeID() == TouchDownEvent::typeID) {
			auto touchDownEvent = (TouchDownEvent&) event;

			setCaptured(true);

			_touchDownPosition = touchDownEvent.getPosition();

			event.setHandled(true);
		}
		else if (event.getTypeID() == TouchDragEvent::typeID) {
			auto touchDragEventEvent = (TouchDragEvent&) event;

			const auto position = touchDragEventEvent.getPosition();

			auto deltaPixels= position - _touchDownPosition;
			ESP_LOGI("ND", "deltaPixels: %ld, %ld", deltaPixels.getX(), deltaPixels.getY());

//			auto deltaMeters= (Vector2F) deltaPixels * _metersPerPixel;
//			ESP_LOGI("ND", "deltaMeters: %f, %f", deltaMeters.getX(), deltaMeters.getY());
//
//			auto deltaAngles = Vector2F(
//				yoba::toRadians(deltaMeters.getX() / _earthMetersPer1EquatorialDeg),
//				yoba::toRadians(deltaMeters.getY() / _earthMetersPer1EquatorialDeg)
//			);

			auto deltaAngles = Vector2F(
				yoba::toRadians(yoba::toRadians(deltaPixels.getX() > 0 ? 1 : -1)),
				yoba::toRadians(yoba::toRadians(deltaPixels.getY() > 0 ? 1 : -1))
			);

			ESP_LOGI("ND", "deltaAngles: %f, %f", deltaAngles.getX(), deltaAngles.getY());

			// Lat
			_cameraOffset.setX(_cameraOffset.getX() + (float) deltaAngles.getX());
			// Lon
			_cameraOffset.setY(_cameraOffset.getY() + (float) deltaAngles.getY());

			ESP_LOGI("ND", "_cameraOffset: %f, %f", _cameraOffset.getX(), _cameraOffset.getY());

			_touchDownPosition = position;

			event.setHandled(true);
		}
		else if (event.getTypeID() == TouchUpEvent::typeID) {
			setCaptured(false);

			event.setHandled(true);
		}
	}

	Vector3F ND::geographicToCartesian(const SinAndCos& latitude, const SinAndCos& longitude, float distanceToEarthCenter) {
		return {
			distanceToEarthCenter * latitude.getCos() * longitude.getCos(),
			distanceToEarthCenter * latitude.getCos() * longitude.getSin(),
			distanceToEarthCenter * latitude.getSin()
		};
	}
}