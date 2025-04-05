#include "ND.h"
#include "../../../../../rc.h"
#include <format>
#include <esp_log.h>

namespace pizda {
	ND::ND() {
		setClipToBounds(true);

		// Axis
		addObject(new Line(
			Vector3F(0, 0, 0),
			Vector3F(GeographicCoordinates::equatorialRadiusMeters, 0, 0),
			&Theme::red
		));

		addObject(new Line(
			Vector3F(0, 0, 0),
			Vector3F(0, GeographicCoordinates::equatorialRadiusMeters, 0),
			&Theme::green
		));

		addObject(new Line(
			Vector3F(0, 0, 0),
			Vector3F(0, 0, GeographicCoordinates::equatorialRadiusMeters),
			&Theme::blue
		));

		// Cube
		addObject(new CubeLinearMesh(Vector3F(), GeographicCoordinates::equatorialRadiusMeters * 2, &Theme::fg1));

		const Vector3F* pizda = new Vector3F[]{
			GeographicCoordinates(toRadians(60.014907051555966f), toRadians(29.69815561737486f), GeographicCoordinates::equatorialRadiusMeters).toCartesian(),
			GeographicCoordinates(toRadians(60.0145197943842f), toRadians(29.707064330304686f), GeographicCoordinates::equatorialRadiusMeters).toCartesian(),
			GeographicCoordinates(toRadians(60.01425827484923f), toRadians(29.707021476564208f), GeographicCoordinates::equatorialRadiusMeters).toCartesian(),
			GeographicCoordinates(toRadians(60.0146386127534f), toRadians(29.698106061255945f), GeographicCoordinates::equatorialRadiusMeters).toCartesian()
		};

		addObject(new PlaneLinearMesh(
			pizda,
			&Theme::fg3
		));

		// Labels
		addObject(&_planeObject);
	}

	void ND::onTick() {
		SpatialView::onTick();

		auto& rc = RC::getInstance();

		const auto rotationLatitude = rc.getGeographicCoordinates().getLatitude();
		const auto rotationLongitude = rc.getGeographicCoordinates().getLongitude();

		_planeObject.setPosition(
			GeographicCoordinates(
				rotationLatitude,
				rotationLongitude,
				GeographicCoordinates::equatorialRadiusMeters
			)
			.toCartesian()
		);

		getCamera().setPosition(
			GeographicCoordinates(
				rotationLatitude + _cameraOffset.getLatitude(),
				rotationLongitude - _cameraOffset.getLongitude(),
				GeographicCoordinates::equatorialRadiusMeters + _cameraOffset.getAltitude()
			)
			.toCartesian()
		);

		getCamera().setRotation(Vector3F(
			rotationLatitude,
			0,
			// Longitude uses "X axis - value" for Y rotation, but camera uses "Y + value", so...
			// 90 - rotation + 180 or 270 - rotation
			yoba::toRadians(270) - rotationLongitude
		));

		invalidate();
	}

	void ND::onEvent(Event* event) {
		SpatialView::onEvent(event);

		if (event->getTypeID() == TouchDownEvent::typeID) {
			auto touchDownEvent = (TouchDownEvent*) event;

			setCaptured(true);

			_touchDownPosition = touchDownEvent->getPosition();

			event->setHandled(true);
		}
		else if (event->getTypeID() == TouchDragEvent::typeID) {
			auto touchDragEventEvent = (TouchDragEvent*) event;

			const auto position = touchDragEventEvent->getPosition();

			const auto deltaPixels= position - _touchDownPosition;
			_touchDownPosition = position;

			ESP_LOGI("ND", "----------------------------");

			ESP_LOGI("ND", "deltaPixels: %ld x , %ld y", deltaPixels.getX(), deltaPixels.getY());

			// Assuming that camera is placed in the center of circle, so camera altitude defines circle radius
			const auto radiusFactor = _cameraOffset.getAltitude() / GeographicCoordinates::equatorialRadiusMeters;
			ESP_LOGI("ND", "radiusFactor: %f", radiusFactor);

			const auto viewportRad = getCamera().getFOV() * radiusFactor;
			ESP_LOGI("ND", "viewportDeg: %f", toDegrees(viewportRad));

			// viewport rad - height px
			// x rad - 1 px
			const auto aspectRatio = (float) getBounds().getWidth() / (float) getBounds().getHeight();

			const auto radPerPixelX = viewportRad / (float) getBounds().getWidth();
			const auto radPerPixelY = radPerPixelX / aspectRatio;
			ESP_LOGI("ND", "degPerPixel: %f x, %f y", toDegrees(radPerPixelX), toDegrees(radPerPixelY));

			const auto deltaRadLat = (float) deltaPixels.getY() * radPerPixelY;
			const auto deltaRadLon = (float) deltaPixels.getX() * radPerPixelX;

			ESP_LOGI("ND", "deltaDeg: %f lat, %f lon", toDegrees(deltaRadLat), toDegrees(deltaRadLon));

			setCameraOffset(GeographicCoordinates(
				_cameraOffset.getLatitude() + deltaRadLat,
				_cameraOffset.getLongitude() + deltaRadLon,
				_cameraOffset.getAltitude()
			));

			event->setHandled(true);
		}
		else if (event->getTypeID() == TouchUpEvent::typeID) {
			setCaptured(false);

			event->setHandled(true);
		}
		else if (event->getTypeID() == PinchDownEvent::typeID) {
			auto pinchDownEvent = (PinchDownEvent*) event;

			_pinchLength = (pinchDownEvent->getPosition2() - pinchDownEvent->getPosition1()).getLength();

			event->setHandled(true);
		}
		else if (event->getTypeID() == PinchDragEvent::typeID) {
			auto pinchDragEvent = (PinchDragEvent*) event;

			const auto pinchLength = (pinchDragEvent->getPosition2() - pinchDragEvent->getPosition1()).getLength();

			const auto pinchFactor = (float) pinchLength / (float) _pinchLength;
			_pinchLength = pinchLength;

			setCameraOffset(GeographicCoordinates(
				_cameraOffset.getLatitude(),
				_cameraOffset.getLongitude(),
				std::clamp(_cameraOffset.getAltitude() + (pinchFactor > 1 ? -100.f : 100.f), (float) cameraOffsetMinimum, (float) cameraOffsetMaximum)
			));

			event->setHandled(true);
		}
	}

	const GeographicCoordinates& ND::getCameraOffset() const {
		return _cameraOffset;
	}

	void ND::setCameraOffset(const GeographicCoordinates& value) {
		_cameraOffset = value;

		invalidate();
	}
}