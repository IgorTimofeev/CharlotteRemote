#include "ND.h"

#include "../../../../rc.h"

#include <numbers>
#include <format>
#include <esp_log.h>
#include "../../../elements/spatial/runwayElement.h"

namespace pizda {
	ND::ND() {
		auto& rc = RC::getInstance();
		const auto& sd = rc.getSpatialData();

		setClipToBounds(true);

		// Axis
//		addElement(new SpatialLine(
//			Vector3F(0, 0, 0),
//			Vector3F(GeographicCoordinates::equatorialRadiusMeters, 0, 0),
//			&Theme::red
//		));
//
//		addElement(new SpatialLine(
//			Vector3F(0, 0, 0),
//			Vector3F(0, GeographicCoordinates::equatorialRadiusMeters, 0),
//			&Theme::green
//		));
//
//		addElement(new SpatialLine(
//			Vector3F(0, 0, 0),
//			Vector3F(0, 0, GeographicCoordinates::equatorialRadiusMeters),
//			&Theme::blue
//		));

		// Sphere
		addElement(new LinearSphere(Vector3F(), GeographicCoordinates::equatorialRadiusMeters, 16, 16, &Theme::bg4));

		// Runways
		for (const auto& runway : sd.runways) {
			addElement(new RunwayElement(
				runway,
				&Theme::fg1
			));
		}

		// Aircraft
		_aircraftElement = new AircraftElement();
		addElement(_aircraftElement);
	}

	ND::~ND() {
		for (const auto element : getSceneElements()) {
			delete element;
		}
	}

	GeographicCoordinates ND::_cameraOffset = {
		0,
		0,
		500
	};

	void ND::onTick() {
		Scene::onTick();

		auto& rc = RC::getInstance();
		const auto& settings = rc.getSettings();
		const auto& ad = rc.getAircraftData();

		// Aircraft
		_aircraftElement->setPosition(
			GeographicCoordinates(
				ad.geographicCoordinates.getLatitude(),
				ad.geographicCoordinates.getLongitude(),
				0
			)
			.toCartesian()
		);

		// Camera
		setFOV(toRadians(90));

		_cameraCoordinates.setLatitude(ad.geographicCoordinates.getLatitude() + _cameraOffset.getLatitude());
		_cameraCoordinates.setLongitude(ad.geographicCoordinates.getLongitude() + _cameraOffset.getLongitude());
		_cameraCoordinates.setAltitude(_cameraOffset.getAltitude());

		setCameraPosition(_cameraCoordinates.toCartesian());

		setCameraRotation(Vector3F(
			-_cameraCoordinates.getLatitude(),
			settings.interface.MFD.ND.northUp ? 0 : ad.computed.yaw,
			toRadians(90) + _cameraCoordinates.getLongitude()
		));

		invalidate();
	}

	void ND::onRender(Renderer* renderer, const Bounds& bounds) {
		Scene::onRender(renderer, bounds);

		// Cursor
		if (_cursorPosition.getX() >= 0 &&  _cursorPosition.getY() >= 0) {
			constexpr static uint8_t lineAreaOffset = 5;
			constexpr static uint8_t lineAreaSize = 8;

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
	}

	void ND::onEvent(Event* event) {
		Scene::onEvent(event);

		if (event->getTypeID() == TouchDownEvent::typeID) {
			const auto touchDownEvent = static_cast<TouchDownEvent*>(event);

			setFocused(true);
			setCaptured(true);

			_touchDownPosition = touchDownEvent->getPosition();
			_cursorPosition = touchDownEvent->getPosition() - getBounds().getPosition();

			event->setHandled(true);
		}
		else if (event->getTypeID() == TouchDragEvent::typeID) {
			const auto touchDragEventEvent = static_cast<TouchDragEvent*>(event);

//			ESP_LOGI("ND", "------------- Drag -------------");

			const auto& deltaPixels = (touchDragEventEvent->getPosition() - _touchDownPosition).rotate(RC::getInstance().getAircraftData().computed.yaw);
			_touchDownPosition = touchDragEventEvent->getPosition();
			_cursorPosition = touchDragEventEvent->getPosition() - getBounds().getPosition();

//			ESP_LOGI("ND", "deltaPixels: %ld, %ld", deltaPixels.getX(), deltaPixels.getY());

			// viewport rad - height px
			// x rad - 1 px
			const auto equatorialRadiansPerPixel = getEquatorialRadiansPerPixel();

//			ESP_LOGI("ND", "camera lat: %f", cameraCoordinates.getLatitude());
//			ESP_LOGI("ND", "camera lat cos: %f", std::cosf(cameraCoordinates.getLatitude()));
//			ESP_LOGI("ND", "deltaPixelsX with coorection: %f", static_cast<float>(deltaPixels.getX()) / std::cosf(cameraCoordinates.getLatitude()));

			const auto deltaRadLon = static_cast<float>(deltaPixels.getX()) * equatorialRadiansPerPixel / std::cosf(_cameraCoordinates.getLatitude());
			const auto deltaRadLat = static_cast<float>(deltaPixels.getY()) * equatorialRadiansPerPixel;

//			ESP_LOGI("ND", "deltaDeg: %f lat, %f lon", toDegrees(deltaRadLat), toDegrees(deltaRadLon));

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
			const auto pinchDownEvent = static_cast<PinchDownEvent*>(event);

			_pinchLength = pinchDownEvent->getLength();
			_cursorPosition = { -1, -1 };

			event->setHandled(true);
		}
		else if (event->getTypeID() == PinchDragEvent::typeID) {
			const auto pinchDragEvent = static_cast<PinchDragEvent*>(event);

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
					static_cast<float>(cameraAltitudeMinimum),
					static_cast<float>(cameraAltitudeMaximum)
				)
			));

			event->setHandled(true);
		}
		else if (event->getTypeID() == EncoderRotateEvent::typeID) {
			if (isFocused()) {
				const auto rotateEvent = static_cast<EncoderRotateEvent*>(event);
				const auto scaleFactor = rotateEvent->getRPS() > 60 ? 1.5f : 2.f;

				setCameraOffset(GeographicCoordinates(
					_cameraOffset.getLatitude(),
					_cameraOffset.getLongitude(),
					std::clamp(
						rotateEvent->getRPS() >= 0
						? _cameraOffset.getAltitude() / scaleFactor
						: _cameraOffset.getAltitude() * scaleFactor,
						static_cast<float>(cameraAltitudeMinimum),
						static_cast<float>(cameraAltitudeMaximum)
					)
				));

				event->setHandled(true);
			}
		}
		else if (event->getTypeID() == EncoderPushEvent::typeID) {
			if (isFocused()) {
				const auto pushEvent = static_cast<EncoderPushEvent*>(event);

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
		const auto viewportRad = getFOV() * radiusFactor;
//		ESP_LOGI("ND", "viewportDeg: %f", toDegrees(viewportRad));

		// And then we can calculate how many equatorial radians of the earth is in 1 pixel of the screen
		// viewport rad - width px
		// x rad - 1 px
		return viewportRad / static_cast<float>(getBounds().getWidth());
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
}