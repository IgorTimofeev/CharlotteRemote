#include "ND.h"
#include "../../../../../rc.h"
#include "elements/runway.h"
#include <numbers>
#include <format>
#include <esp_log.h>

namespace pizda {
	ND::ND() {
		setClipToBounds(true);

		// Axis
		addElement(new Line(
			Vector3F(0, 0, 0),
			Vector3F(GeographicCoordinates::equatorialRadiusMeters, 0, 0),
			&Theme::red
		));

		addElement(new Line(
			Vector3F(0, 0, 0),
			Vector3F(0, GeographicCoordinates::equatorialRadiusMeters, 0),
			&Theme::green
		));

		addElement(new Line(
			Vector3F(0, 0, 0),
			Vector3F(0, 0, GeographicCoordinates::equatorialRadiusMeters),
			&Theme::blue
		));

//		// Cube
//		addObject(new CubeLinearMesh(Vector3F(), GeographicCoordinates::equatorialRadiusMeters * 2, &Theme::fg1));

		// Sphere
		addElement(new SphereLinearMesh(Vector3F(), GeographicCoordinates::equatorialRadiusMeters, 16, 16, &Theme::bg4));

		// Airfields
//		addElement(new Label(
//			GeographicCoordinates(toRadians(60.014907051555966f), toRadians(29.69815561737486f), 0).toCartesian(),
//			&Theme::fontSmall,
//			&Theme::fg1,
//			L"ULLY"
//		));
//
		addElement(new RunwayElement(
			Runway(
				GeographicCoordinates(toRadians(60.014581566191914f), toRadians(29.70258579817704f), 0),
				95,
				500,
				30,
				L"ULLY"
			),
			&Theme::fg1
		));

		addElement(new RunwayElement(
			Runway(
				GeographicCoordinates(toRadians(59.79507652101131f), toRadians(30.250945449842572), 0),
				100,
				3780,
				60,
				L"ULLI"
			),
			&Theme::fg1
		));

//		addElement(new RunwayElement(
//			new Vector3F[]{
//				GeographicCoordinates(toRadians(59.81000515459139f), toRadians(30.245549866082925f), 0).toCartesian(),
//				GeographicCoordinates(toRadians(59.801365049765245f), toRadians(30.303604969331097f), 0).toCartesian(),
//				GeographicCoordinates(toRadians(59.80086133025612f), toRadians(30.303302737716518f), 0).toCartesian(),
//				GeographicCoordinates(toRadians(59.80948999001452f), toRadians(30.245252504683034f), 0).toCartesian()
//			},
//			&Theme::fg1,
//			L"ULLI10L"
//		));
//
//		addElement(new RunwayElement(
//			new Vector3F[]{
//				GeographicCoordinates(toRadians(59.80017069061603f), toRadians(30.218472732217464f), 0).toCartesian(),
//				GeographicCoordinates(toRadians(59.79058757720595f), toRadians(30.283053765300696f), 0).toCartesian(),
//				GeographicCoordinates(toRadians(59.7900828284425f), toRadians(30.282753357896627f), 0).toCartesian(),
//				GeographicCoordinates(toRadians(59.79966144346482f), toRadians(30.218174663302346f), 0).toCartesian()
//			},
//			&Theme::fg1,
//			L"ULLI10R"
//		));

		// Aircraft
		addElement(&_aircraftElement);
	}

	void ND::onTick() {
		SpatialView::onTick();

		auto& rc = RC::getInstance();

		const auto& aircraftCoordinates = rc.getGeographicCoordinates();

		_aircraftElement.setPosition(
			GeographicCoordinates(
				aircraftCoordinates.getLatitude(),
				aircraftCoordinates.getLongitude(),
				0
			)
			.toCartesian()
		);

		const auto& cameraCoordinates = getCameraCoordinates();

		getCamera().setPosition(cameraCoordinates.toCartesian());

		getCamera().setRotation(Vector3F(
			-cameraCoordinates.getLatitude(),
			rc.getYawInterpolator().getValue(),
			toRadians(-90 + 180) + cameraCoordinates.getLongitude()
		));

		invalidate();
	}

	void ND::onRender(Renderer* renderer, const Bounds& bounds) {
		SpatialView::onRender(renderer, bounds);

		if (!isFocused())
			return;

		constexpr const uint8_t lineAreaOffset = 5;
		constexpr const uint8_t lineAreaSize = 8;

		// Center
		const auto& center = bounds.getPosition() + _cursorPosition;

		renderer->renderPixel(center, &Theme::yellow);

		// Upper
		renderer->renderLine(
			Point(center.getX(), center.getY() - lineAreaOffset),
			Point(center.getX() - lineAreaSize, center.getY() - lineAreaOffset - lineAreaSize),
			&Theme::yellow
		);

		renderer->renderLine(
			Point(center.getX(), center.getY() - lineAreaOffset),
			Point(center.getX() + lineAreaSize, center.getY() - lineAreaOffset - lineAreaSize),
			&Theme::yellow
		);

		// Lower
		renderer->renderLine(
			Point(center.getX(), center.getY() + lineAreaOffset),
			Point(center.getX() - lineAreaSize, center.getY() + lineAreaOffset + lineAreaSize),
			&Theme::yellow
		);

		renderer->renderLine(
			Point(center.getX(), center.getY() + lineAreaOffset),
			Point(center.getX() + lineAreaSize, center.getY() + lineAreaOffset + lineAreaSize),
			&Theme::yellow
		);
	}

	void ND::onEvent(Event* event) {
		SpatialView::onEvent(event);

		if (event->getTypeID() == TouchDownEvent::typeID) {
			auto touchDownEvent = (TouchDownEvent*) event;

			setFocused(true);
			setCaptured(true);

			_touchDownPosition = touchDownEvent->getPosition();
			_cursorPosition = touchDownEvent->getPosition() - getBounds().getPosition();

			event->setHandled(true);
		}
		else if (event->getTypeID() == TouchDragEvent::typeID) {
			auto touchDragEventEvent = (TouchDragEvent*) event;

			ESP_LOGI("ND", "------------- Drag -------------");

			const auto& deltaPixels = (touchDragEventEvent->getPosition() - _touchDownPosition).rotate(RC::getInstance().getYawInterpolator().getValue());
			_touchDownPosition = touchDragEventEvent->getPosition();
			_cursorPosition = touchDragEventEvent->getPosition() - getBounds().getPosition();

			ESP_LOGI("ND", "deltaPixels: %ld, %ld", deltaPixels.getX(), deltaPixels.getY());

			// viewport rad - height px
			// x rad - 1 px
			const auto equatorialRadiansPerPixel = getEquatorialRadiansPerPixel();
			const auto& cameraCoordinates = getCameraCoordinates();

			ESP_LOGI("ND", "camera lat: %f", cameraCoordinates.getLatitude());
			ESP_LOGI("ND", "camera lat cos: %f", std::cosf(cameraCoordinates.getLatitude()));
			ESP_LOGI("ND", "deltaPixelsX with coorection: %f", (float) deltaPixels.getX() / std::cosf(cameraCoordinates.getLatitude()));

			const auto deltaRadLon = (float) deltaPixels.getX() * equatorialRadiansPerPixel / std::cosf(cameraCoordinates.getLatitude());
			const auto deltaRadLat = (float) deltaPixels.getY() * equatorialRadiansPerPixel;

			ESP_LOGI("ND", "deltaDeg: %f lat, %f lon", toDegrees(deltaRadLat), toDegrees(deltaRadLon));

			setCameraOffset(GeographicCoordinates(
				_cameraOffset.getLatitude() + deltaRadLat,
				_cameraOffset.getLongitude() - deltaRadLon,
				_cameraOffset.getAltitude()
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

			_pinchLength = pinchDownEvent->getLength();
			_cursorPosition = {};

			event->setHandled(true);
		}
		else if (event->getTypeID() == PinchDragEvent::typeID) {
			auto pinchDragEvent = (PinchDragEvent*) event;

//			ESP_LOGI("ND", "------------- Pinch -------------");

//			const auto pinchLength = pinchDragEvent->getLength();
//			const auto pinchDelta = pinchLength - _pinchLength;
//			_pinchLength = pinchLength;
//
//			ESP_LOGI("ND", "pinchDelta: %f px", pinchDelta);
//
//			setAltitudeFromDeltaPixels(-pinchDelta);

			const auto pinchLength = pinchDragEvent->getLength();
			const auto pinchFactor = _pinchLength / pinchLength;
			_pinchLength = pinchLength;

			setCameraOffset(GeographicCoordinates(
				_cameraOffset.getLatitude(),
				_cameraOffset.getLongitude(),
				std::clamp(
					_cameraOffset.getAltitude() * pinchFactor,
					(float) cameraAltitudeMinimum,
					(float) cameraAltitudeMaximum
				)
			));

			event->setHandled(true);
		}
		else if (event->getTypeID() == EncoderRotateEvent::typeID) {
			if (isFocused()) {
				const auto rotateEvent = (EncoderRotateEvent*) event;

				const auto scaleFactor = rotateEvent->getRPS() > 60 ? 1.5f : 2.f;

				setCameraOffset(GeographicCoordinates(
					_cameraOffset.getLatitude(),
					_cameraOffset.getLongitude(),
					std::clamp(
						rotateEvent->getRPS() >= 0
						? _cameraOffset.getAltitude() / scaleFactor
						: _cameraOffset.getAltitude() * scaleFactor,
						(float) cameraAltitudeMinimum,
						(float) cameraAltitudeMaximum
					)
				));

				event->setHandled(true);
			}
		}
		else if (event->getTypeID() == EncoderPushEvent::typeID) {
			if (isFocused()) {
				const auto pushEvent = (EncoderPushEvent*) event;

				if (pushEvent->isDown())
					resetCameraOffsetLatLon();

				event->setHandled(true);
			}
		}
	}

	float ND::getEquatorialRadiansPerPixel() {
		// Imagine that camera is placed in the center of small sphere with radius = camera altitude
//		const float maxViewportRads = 2.f * std::asinf(GeographicCoordinates::equatorialRadiusMeters / (GeographicCoordinates::equatorialRadiusMeters + cameraAltitudeMaximum));

		// First, we need to find out how many times the length of the equator of this sphere is
		// greater than the length of the earth's equator
		const auto radiusFactor = _cameraOffset.getAltitude() / GeographicCoordinates::equatorialRadiusMeters;
//		ESP_LOGI("ND", "radiusFactor: %f", radiusFactor);

		// Since the length of the equator is calculated using 2 * pi * r, the dependence here is linear.
		// This allows us to easily determine how many equatorial radians of the earth our camera can see
		// excluding of FOV limitations
		const auto viewportRad = getCamera().getFOV() * radiusFactor;
//		ESP_LOGI("ND", "viewportDeg: %f", toDegrees(viewportRad));

		// And then we can calculate how many equatorial radians of the earth is in 1 pixel of the screen
		// viewport rad - width px
		// x rad - 1 px
		return viewportRad / (float) getBounds().getWidth();
	}

	const GeographicCoordinates& ND::getCameraOffset() const {
		return _cameraOffset;
	}

	void ND::setCameraOffset(const GeographicCoordinates& value) {
		_cameraOffset = value;

		invalidate();
	}

	void ND::resetCameraOffsetLatLon() {
		setCameraOffset(GeographicCoordinates(0, 0, _cameraOffset.getAltitude()));
	}

	GeographicCoordinates ND::getCameraCoordinates() {
		const auto& aircraftCoordinates = RC::getInstance().getGeographicCoordinates();

		return GeographicCoordinates(
			aircraftCoordinates.getLatitude() + _cameraOffset.getLatitude(),
			aircraftCoordinates.getLongitude() + _cameraOffset.getLongitude(),
			_cameraOffset.getAltitude()
		);
	}
}