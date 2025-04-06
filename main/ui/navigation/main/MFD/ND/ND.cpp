#include "ND.h"
#include "../../../../../rc.h"
#include "objects/runway.h"
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

		// Airfields
		addObject(new Runway(
			new Vector3F[] {
				GeographicCoordinates(toRadians(60.014907051555966f), toRadians(29.69815561737486f), GeographicCoordinates::equatorialRadiusMeters).toCartesian(),
				GeographicCoordinates(toRadians(60.0145197943842f), toRadians(29.707064330304686f), GeographicCoordinates::equatorialRadiusMeters).toCartesian(),
				GeographicCoordinates(toRadians(60.01425827484923f), toRadians(29.707021476564208f), GeographicCoordinates::equatorialRadiusMeters).toCartesian(),
				GeographicCoordinates(toRadians(60.0146386127534f), toRadians(29.698106061255945f), GeographicCoordinates::equatorialRadiusMeters).toCartesian()
			},
			L"ULLY",
			&Theme::fg2
		));

		addObject(new Runway(
			new Vector3F[] {
				GeographicCoordinates(toRadians(59.81000515459139f), toRadians(30.245549866082925f), GeographicCoordinates::equatorialRadiusMeters).toCartesian(),
				GeographicCoordinates(toRadians(59.801365049765245f), toRadians(30.303604969331097f), GeographicCoordinates::equatorialRadiusMeters).toCartesian(),
				GeographicCoordinates(toRadians(59.80086133025612f), toRadians(30.303302737716518f), GeographicCoordinates::equatorialRadiusMeters).toCartesian(),
				GeographicCoordinates(toRadians(59.80948999001452f), toRadians(30.245252504683034f), GeographicCoordinates::equatorialRadiusMeters).toCartesian()
			},
			L"ULLI10L",
			&Theme::fg2
		));

		addObject(new Runway(
			new Vector3F[] {
				GeographicCoordinates(toRadians(59.80017069061603f), toRadians(30.218472732217464f), GeographicCoordinates::equatorialRadiusMeters).toCartesian(),
				GeographicCoordinates(toRadians(59.79058757720595f), toRadians(30.283053765300696f), GeographicCoordinates::equatorialRadiusMeters).toCartesian(),
				GeographicCoordinates(toRadians(59.7900828284425f), toRadians(30.282753357896627f), GeographicCoordinates::equatorialRadiusMeters).toCartesian(),
				GeographicCoordinates(toRadians(59.79966144346482f), toRadians(30.218174663302346f), GeographicCoordinates::equatorialRadiusMeters).toCartesian()
			},
			L"ULLI10R",
			&Theme::fg2
		));

		// Aircraft
		addObject(&_aircraftObject);
	}

	void ND::onTick() {
		SpatialView::onTick();

		auto& rc = RC::getInstance();

		const auto& aircraftCoordinates = rc.getGeographicCoordinates();

		_aircraftObject.setPosition(
			GeographicCoordinates(
				aircraftCoordinates.getLatitude(),
				aircraftCoordinates.getLongitude(),
				GeographicCoordinates::equatorialRadiusMeters
			)
			.toCartesian()
		);

		const auto& cameraCoordinates = GeographicCoordinates(
			0,
			0,
			GeographicCoordinates::equatorialRadiusMeters + _cameraOffset.getAltitude()
		);

		getCamera().setPosition(cameraCoordinates.toCartesian());

		getCamera().setRotation(Vector3F(
			0,
			0,
			// Geographic longitude uses "X axis - value" for rotation around Z axis, but camera uses "Y + value", so...
			// 90 - rotation + 180 or 270 - rotation
			toRadians(-90 + 180) + _cameraOffset.getLongitude()
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

//			const auto deltaRadLat = toRadians(deltaPixels.getY() >= 0 ? 5 : -5);
//			const auto deltaRadLon = toRadians(deltaPixels.getX() >= 0 ? 5 : -5);

			ESP_LOGI("ND", "deltaDeg: %f lat, %f lon", toDegrees(deltaRadLat), toDegrees(deltaRadLon));

//			setCameraOffset(GeographicCoordinates(
//				_cameraOffset.getLatitude() + deltaRadLat,
//				_cameraOffset.getLongitude() - deltaRadLon,
//				_cameraOffset.getAltitude()
//			));
//
//			ESP_LOGI("ND", "cameraOffset: %f deg, %f deg, %f m", toDegrees(_cameraOffset.getLatitude()), toDegrees(_cameraOffset.getLongitude()), _cameraOffset.getAltitude());

			setCameraOffset(GeographicCoordinates(
				_cameraOffset.getLatitude() + toRadians(deltaPixels.getX() >= 0 ? -5 : 5),
				_cameraOffset.getLongitude() + toRadians(deltaPixels.getY() >= 0 ? -5 : 5),
				_cameraOffset.getAltitude()
			));

			ESP_LOGI("ND", "cameraOffset: %f, %f, %f", _cameraOffset.getLatitude(), _cameraOffset.getLongitude(), _cameraOffset.getAltitude());

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

//			setCameraOffset(GeographicCoordinates(
//				_cameraOffset.getLatitude(),
//				_cameraOffset.getLongitude(),
//				std::clamp(_cameraOffset.getAltitude() + (pinchFactor > 1 ? -100.f : 100.f), (float) cameraOffsetMinimum, (float) cameraOffsetMaximum)
//			));
//
//			ESP_LOGI("ND", "cameraOffset: %f deg, %f deg, %f m", toDegrees(_cameraOffset.getLatitude()), toDegrees(_cameraOffset.getLongitude()), _cameraOffset.getAltitude());

			setCameraOffset(GeographicCoordinates(
				_cameraOffset.getLatitude(),
				_cameraOffset.getLongitude(),
				_cameraOffset.getAltitude() + (pinchFactor > 1 ? 500000.f : -500000.f)
			));

			ESP_LOGI("ND", "cameraOffset: %f, %f, %f", _cameraOffset.getLatitude(), _cameraOffset.getLongitude(), _cameraOffset.getAltitude());

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