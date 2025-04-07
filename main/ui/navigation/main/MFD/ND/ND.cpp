#include "ND.h"
#include "../../../../../rc.h"
#include "objects/runway.h"
#include <numbers>
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

//		// Cube
//		addObject(new CubeLinearMesh(Vector3F(), GeographicCoordinates::equatorialRadiusMeters * 2, &Theme::fg1));

		// Sphere
		addObject(new SphereLinearMesh(Vector3F(), GeographicCoordinates::equatorialRadiusMeters, 10, 10, &Theme::bg5));

		// Airfields
		addObject(new Label(
			GeographicCoordinates(toRadians(60.014907051555966f), toRadians(29.69815561737486f), 0).toCartesian(),
			&Theme::fontSmall,
			&Theme::fg2,
			L"ULLY"
		));

		addObject(new Runway(
			new Vector3F[] {
				GeographicCoordinates(toRadians(60.014907051555966f), toRadians(29.69815561737486f), 0).toCartesian(),
				GeographicCoordinates(toRadians(60.0145197943842f), toRadians(29.707064330304686f), 0).toCartesian(),
				GeographicCoordinates(toRadians(60.01425827484923f), toRadians(29.707021476564208f), 0).toCartesian(),
				GeographicCoordinates(toRadians(60.0146386127534f), toRadians(29.698106061255945f), 0).toCartesian()
			},
			&Theme::fg2,
			L"ULLY"
		));

		addObject(new Runway(
			new Vector3F[] {
				GeographicCoordinates(toRadians(59.81000515459139f), toRadians(30.245549866082925f), 0).toCartesian(),
				GeographicCoordinates(toRadians(59.801365049765245f), toRadians(30.303604969331097f), 0).toCartesian(),
				GeographicCoordinates(toRadians(59.80086133025612f), toRadians(30.303302737716518f), 0).toCartesian(),
				GeographicCoordinates(toRadians(59.80948999001452f), toRadians(30.245252504683034f), 0).toCartesian()
			},
			&Theme::fg2,
			L"ULLI10L"
		));

		addObject(new Runway(
			new Vector3F[] {
				GeographicCoordinates(toRadians(59.80017069061603f), toRadians(30.218472732217464f), 0).toCartesian(),
				GeographicCoordinates(toRadians(59.79058757720595f), toRadians(30.283053765300696f), 0).toCartesian(),
				GeographicCoordinates(toRadians(59.7900828284425f), toRadians(30.282753357896627f), 0).toCartesian(),
				GeographicCoordinates(toRadians(59.79966144346482f), toRadians(30.218174663302346f), 0).toCartesian()
			},
			&Theme::fg2,
			L"ULLI10R"
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
				0
			)
			.toCartesian()
		);

		const auto& cameraCoordinates = GeographicCoordinates(
			aircraftCoordinates.getLatitude() + _cameraCoordinates.getLatitude(),
			aircraftCoordinates.getLongitude() + _cameraCoordinates.getLongitude(),
			_cameraCoordinates.getAltitude()
		);

		getCamera().setPosition(cameraCoordinates.toCartesian());

		getCamera().setRotation(Vector3F(
			-cameraCoordinates.getLatitude(),
			rc.getYawInterpolator().getValue(),
			toRadians(-90 + 180) + cameraCoordinates.getLongitude()
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

//			ESP_LOGI("ND", "------------- Drag -------------");

			const auto& bounds = getBounds();
			const auto& position = touchDragEventEvent->getPosition();
			const auto& deltaPixels = ((Vector2F) (position - _touchDownPosition)).rotate(RC::getInstance().getYawInterpolator().getValue());
			_touchDownPosition = position;

			// viewport rad - height px
			// x rad - 1 px
			const auto aspectRatio = (float) bounds.getWidth() / (float) bounds.getHeight();

			const auto radPerPixelX = getRadiansPerPixelX();
			const auto radPerPixelY = radPerPixelX / aspectRatio;

			const auto deltaRadX = deltaPixels.getX() * radPerPixelX;
			const auto deltaRadY = deltaPixels.getY() * radPerPixelY;

//			ESP_LOGI("ND", "deltaDeg: %f lat, %f lon", toDegrees(deltaRadLat), toDegrees(deltaRadLon));

			setCameraCoordinates(GeographicCoordinates(
				_cameraCoordinates.getLatitude() + deltaRadY,
				_cameraCoordinates.getLongitude() - deltaRadX,
				_cameraCoordinates.getAltitude()
			));

//			ESP_LOGI("ND", "cameraOffset: %f deg, %f deg, %f m", toDegrees(_cameraCoordinates.getLatitude()), toDegrees(_cameraCoordinates.getLongitude()), _cameraCoordinates.getAltitude());

//			setCameraOffset(GeographicCoordinates(
//				_cameraOffset.getLatitude() + toRadians(deltaPixels.getX() >= 0 ? -5 : 5),
//				_cameraOffset.getLongitude() + toRadians(deltaPixels.getY() >= 0 ? -5 : 5),
//				_cameraOffset.getAltitude()
//			));
//
//			ESP_LOGI("ND", "cameraOffset: %f, %f, %f", _cameraOffset.getLatitude(), _cameraOffset.getLongitude(), _cameraOffset.getAltitude());

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

//			ESP_LOGI("ND", "------------- Pinch -------------");

			const auto pinchLength = (pinchDragEvent->getPosition2() - pinchDragEvent->getPosition1()).getLength();
			const auto pinchDelta = pinchLength - _pinchLength;
			_pinchLength = pinchLength;

//			ESP_LOGI("ND", "pinchDelta: %f px", pinchDelta);

			const auto metersPerPixelX = getMetersPerPixelX();
//			ESP_LOGI("ND", "metersPerPixelX: %f m", metersPerPixelX);

			const auto pinchDeltaMeters = metersPerPixelX * -pinchDelta;
//			ESP_LOGI("ND", "pinchDeltaMeters: %f m", pinchDeltaMeters);

			const auto newEquatorialLength = GeographicCoordinates::equatorialLengthMeters + pinchDeltaMeters;
//			ESP_LOGI("ND", "newEquatorialLength: %f m", newEquatorialLength);

			const auto equatorialLengthFactor = newEquatorialLength / GeographicCoordinates::equatorialLengthMeters;
//			ESP_LOGI("ND", "equatorialLengthFactor: %f", equatorialLengthFactor);

			const auto newAltitude = _cameraCoordinates.getAltitude() + GeographicCoordinates::equatorialRadiusMeters * equatorialLengthFactor - GeographicCoordinates::equatorialRadiusMeters;
//			ESP_LOGI("ND", "newAltitude: %f m", newAltitude);

			setCameraCoordinates(GeographicCoordinates(
				_cameraCoordinates.getLatitude(),
				_cameraCoordinates.getLongitude(),
				std::clamp(newAltitude, (float) cameraAltitudeMinimum, (float) cameraAltitudeMaximum)
			));

//			ESP_LOGI("ND", "cameraOffset: %f deg, %f deg, %f m", toDegrees(_cameraCoordinates.getLatitude()), toDegrees(_cameraCoordinates.getLongitude()), _cameraCoordinates.getAltitude());

//			setCameraOffset(GeographicCoordinates(
//				_cameraOffset.getLatitude(),
//				_cameraOffset.getLongitude(),
//				_cameraOffset.getAltitude() + (pinchFactor > 1 ? 500000.f : -500000.f)
//			));
//
//			ESP_LOGI("ND", "cameraOffset: %f, %f, %f", _cameraOffset.getLatitude(), _cameraOffset.getLongitude(), _cameraOffset.getAltitude());


			event->setHandled(true);
		}
	}

	float ND::getRadiansPerPixelX() {
		// Assuming that camera is placed in the center of circle, so camera altitude defines circle radius
		const auto radiusFactor = _cameraCoordinates.getAltitude() / GeographicCoordinates::equatorialRadiusMeters;
//		ESP_LOGI("ND", "radiusFactor: %f", radiusFactor);

		const auto viewportRad = getCamera().getFOV() * radiusFactor;
//		ESP_LOGI("ND", "viewportDeg: %f", toDegrees(viewportRad));

		// viewport rad - width px
		// x rad - 1 px
		return viewportRad / (float) getBounds().getWidth();
	}

	float ND::getMetersPerPixelX() {
		const auto radPerPixelX = getRadiansPerPixelX();

		return GeographicCoordinates::equatorialLengthMeters * radPerPixelX / std::numbers::pi_v<float> * 2.f;
	}

	const GeographicCoordinates& ND::getCameraCoordinates() const {
		return _cameraCoordinates;
	}

	void ND::setCameraCoordinates(const GeographicCoordinates& value) {
		_cameraCoordinates = value;

		invalidate();
	}
}