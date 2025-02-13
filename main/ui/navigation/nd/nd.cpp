#include "nd.h"
#include "../../../rc.h"

namespace pizda {
	ND::ND() {
		setClipToBounds(true);

		const auto distanceFromObjectsToEarthCenter = _earthEquatorialRadius;

		addObject(new Cube(distanceFromObjectsToEarthCenter * 2, &Theme::fg1));

		addObject(new Label(
			geographicToCartesian(
				SinAndCos(yoba::toRadians(60.01483325540486f)),
				SinAndCos(yoba::toRadians(29.69835915766679f)),
				_earthEquatorialRadius
			),
			&Theme::fontNormal,
			&Theme::yellow,
			L"Left"
		));

		addObject(new Label(
			geographicToCartesian(
				SinAndCos(yoba::toRadians(60.014390616612474f)),
				SinAndCos(yoba::toRadians(29.706975357970624f)),
				distanceFromObjectsToEarthCenter
			),
			&Theme::fontNormal,
			&Theme::yellow,
			L"Right"
		));

		addObject(new Line(
			Vector3F(0, 0, 0),
			Vector3F(_earthEquatorialRadius, 0, 0),
			&Theme::red
		));

		// Axis
		addObject(new Line(
			Vector3F(0, 0, 0),
			Vector3F(_earthEquatorialRadius, 0, 0),
			&Theme::red
		));

		addObject(new Line(
			Vector3F(0, 0, 0),
			Vector3F(0, _earthEquatorialRadius, 0),
			&Theme::green
		));

		addObject(new Line(
			Vector3F(0, 0, 0),
			Vector3F(0, 0, _earthEquatorialRadius),
			&Theme::blue
		));
	}

	void ND::onTick() {
		SpatialView::onTick();

		auto& rc = RC::getInstance();
		auto& remoteData = rc.getRemoteData();
		auto& computedData = rc.getComputedData();

		auto rotationLatitude =_cameraOffset.getX();
		auto rotationLongitude = _cameraOffset.getY();

		auto cameraPosition = Vector3F(
			_earthEquatorialRadius * 2,
			_earthEquatorialRadius * 2,
			-_earthEquatorialRadius * 3 + _cameraOffset.getZ()
		);

		auto cameraRotation = Vector3F(
			rotationLongitude,
			rotationLatitude,
			0
		);

//		ESP_LOGI("ND", "--------------");
//		ESP_LOGI("ND", "Camera: %f, %f, %f", cameraPosition.getX(), cameraPosition.getY(), cameraPosition.getZ());

		getCamera().setPosition(cameraPosition);
		getCamera().setRotation(cameraRotation);

		invalidate();
	}

	void ND::onRender(Renderer* renderer, const Bounds& bounds) {
		SpatialView::onRender(renderer, bounds);

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
			std::format(L"Camera offset Z: {}", _cameraOffset.getZ())
		);

		position += spacing;
	}

	void ND::onEvent(Event* event) {
		SpatialView::onEvent(event);

		if (event->getTypeID() == PinchDownEvent::typeID) {
			auto pinchDownEvent = (PinchDownEvent*) event;

			_pinchDownPixelsPerMeter = _metersPerPixel;
			_pinchDownLength = (pinchDownEvent->getPosition2() - pinchDownEvent->getPosition1()).getLength();

			event->setHandled(true);
		}
		else if (event->getTypeID() == PinchDragEvent::typeID) {
			auto pinchDragEvent = (PinchDragEvent*) event;

			const auto pinchLength = (pinchDragEvent->getPosition2() - pinchDragEvent->getPosition1()).getLength();

			const auto pinchFactor = (float) pinchLength / (float) _pinchDownLength;
			_pinchDownLength = pinchLength;

			_metersPerPixel = _pinchDownPixelsPerMeter * pinchFactor;

			_cameraOffset.setZ(_cameraOffset.getZ() + (pinchFactor > 1 ? 10.f : -10.f));

			event->setHandled(true);
		}
		else if (event->getTypeID() == TouchDownEvent::typeID) {
			auto touchDownEvent = (TouchDownEvent*) event;

			setCaptured(true);

			_touchDownPosition = touchDownEvent->getPosition();

			event->setHandled(true);
		}
		else if (event->getTypeID() == TouchDragEvent::typeID) {
			auto touchDragEventEvent = (TouchDragEvent*) event;

			const auto position = touchDragEventEvent->getPosition();

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
				yoba::toRadians((float) deltaPixels.getX() * 0.2f),
				yoba::toRadians((float) deltaPixels.getY() * 0.2f)
			);

			ESP_LOGI("ND", "deltaAngles: %f, %f", deltaAngles.getX(), deltaAngles.getY());

			// Lat
			_cameraOffset.setX(_cameraOffset.getX() + (float) deltaAngles.getX());
			// Lon
			_cameraOffset.setY(_cameraOffset.getY() + (float) deltaAngles.getY());

			ESP_LOGI("ND", "_cameraOffset: %f, %f", _cameraOffset.getX(), _cameraOffset.getY());

			_touchDownPosition = position;

			event->setHandled(true);
		}
		else if (event->getTypeID() == TouchUpEvent::typeID) {
			setCaptured(false);

			event->setHandled(true);
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