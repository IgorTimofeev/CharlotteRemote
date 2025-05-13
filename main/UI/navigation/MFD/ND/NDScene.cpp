#include "NDScene.h"

#include "../../../../rc.h"

#include <numbers>
#include <format>
#include <esp_log.h>
#include "sceneElements/NDRunwayMesh.h"
#include "sceneElements/waypointElement.h"
#include "sceneElements/routeElement.h"

namespace pizda {
	NDScene::NDScene() {
		auto& rc = RC::getInstance();

		setClipToBounds(true);
		setFOV(toRadians(90));

		createSceneElementsFromNavigationData();
	}

	NDScene::~NDScene() {
		deleteSceneElements();
	}

	GeographicCoordinates NDScene::_cameraOffset = {
		0,
		0,
		2500
	};

	void NDScene::onTick() {
		Scene::onTick();

		auto& rc = RC::getInstance();
		const auto& settings = rc.getSettings();
		const auto& ad = rc.getAircraftData();

		// Aircraft
		_aircraftElement->setVisible(isCameraShiftedLaterally());

		if (_aircraftElement->isVisible()) {
			_aircraftElement->setPosition(
				GeographicCoordinates(
					ad.geographicCoordinates.getLatitude(),
					ad.geographicCoordinates.getLongitude(),
					0
				)
				.toCartesian()
			);
		}

		// Camera
		_cameraCoordinates.setLatitude(ad.geographicCoordinates.getLatitude() + _cameraOffset.getLatitude());
		_cameraCoordinates.setLongitude(ad.geographicCoordinates.getLongitude() + _cameraOffset.getLongitude());
		_cameraCoordinates.setAltitude(_cameraOffset.getAltitude());

		setCameraPosition(_cameraCoordinates.toCartesian());

		setCameraRotation(Vector3F(
			-_cameraCoordinates.getLatitude(),
			settings.interface.MFD.ND.mode == SettingsInterfaceMFDNDMode::mapNorthUp ? 0 : ad.computed.yaw,
			toRadians(90) + _cameraCoordinates.getLongitude()
		));

		invalidate();
	}

	void NDScene::onBoundsChanged() {
		Scene::onBoundsChanged();

		updatePivot();
	}

	void NDScene::onRender(Renderer* renderer) {
		Scene::onRender(renderer);

		const auto& bounds = getBounds();
		auto& rc = RC::getInstance();
		const auto& settings = rc.getSettings();
		const auto& ad = rc.getAircraftData();

		// Compass
		{
			const auto centerX = bounds.getXCenter();
			const auto centerY = bounds.getYCenter();

			const auto yawDeg = toDegrees(ad.computed.yaw);

			const auto isLandscape = bounds.isLandscape();

			uint16_t circleRadius;
			uint16_t tickMarksRadius;
			int16_t tickAngleFromDeg;
			int16_t tickAngleToDeg;
			uint16_t y;

			const uint16_t circleMarginTopPixels = bounds.getHeight() * _compassHeadingTextMarginTopPct / 100;
			const uint16_t circleMarginBottomPixels = bounds.getHeight() * _compassCircleMarginBottomPct / 100;

			// Arc
			if (settings.interface.MFD.ND.mode == SettingsInterfaceMFDNDMode::arcHeadingUp) {
				circleRadius =
					bounds.getHeight()
					- circleMarginTopPixels
					- Theme::fontNormal.getHeight()
					- _compassHeadingTextHorizontalLineOffset
					- 1
					- _compassHeadingTextVerticalLineHeight
					- _compassCircleMarginTopPx
					- circleMarginBottomPixels;

				tickMarksRadius = circleRadius;

				y = bounds.getY2() - circleMarginBottomPixels;

				tickAngleFromDeg = -_compassArcViewportHalfDeg + _compassTickMarkUnitsDeg;
				tickAngleToDeg = _compassArcViewportHalfDeg;

				renderer->renderArc(
					Point(centerX, y),
					circleRadius,
					(90 - _compassArcViewportHalfDeg) * 255 / 360,
					(90 + _compassArcViewportHalfDeg) * 255 / 360,
					&Theme::fg1
				);
			}
			// Map / north up
			else {
				const uint16_t landscapeRadius =
					bounds.getHeight() / 2
					- circleMarginTopPixels
					- Theme::fontNormal.getHeight()
					- _compassHeadingTextHorizontalLineOffset
					- 1
					- _compassHeadingTextVerticalLineHeight
					- _compassCircleMarginTopPx;

				const uint16_t portraitRadius = (bounds.getWidth() - (bounds.getWidth() * _compassCircleMarginHorizontalPct / 100) * 2) / 2;

				circleRadius =
					isLandscape
					? landscapeRadius
					: std::min(landscapeRadius, portraitRadius);

				tickMarksRadius = circleRadius - 1;

				y = centerY;

				tickAngleFromDeg = 0;
				tickAngleToDeg = 360 - _compassTickMarkUnitsDeg;
			}

			if (isCameraShiftedLaterally()) {
				// Cross
				renderer->renderHorizontalLine(
					Point(
						centerX - _compassLateralOffsetCrossSize / 2,
						y
					),
					_compassLateralOffsetCrossSize,
					&Theme::fg1
				);

				renderer->renderVerticalLine(
					Point(
						centerX,
						y - _compassLateralOffsetCrossSize / 2
					),
					_compassLateralOffsetCrossSize,
					&Theme::fg1
				);
			}
			else {
				// Aircraft indicator
				AircraftElement::render(renderer, Point(
					centerX,
					y
				));
			}

			// North up
			if (settings.interface.MFD.ND.mode == SettingsInterfaceMFDNDMode::mapNorthUp) {

			}
			else {
				// Tick marks
				float stepUnitsPerYawDegIntPart;
				const float stepUnitsPerYawDegFractPart = std::modff(yawDeg / _compassTickMarkUnitsDeg, &stepUnitsPerYawDegIntPart);
				const int32_t yawSnappedInt = static_cast<int32_t>(stepUnitsPerYawDegIntPart) * _compassTickMarkUnitsDeg;

				for (int16_t angleDeg = tickAngleFromDeg; angleDeg <= tickAngleToDeg; angleDeg += _compassTickMarkUnitsDeg) {
					const uint16_t shownAngleDeg = normalizeAngle360(yawSnappedInt + angleDeg);
					const auto isBig = shownAngleDeg % _compassTickMarkUnitsBigDeg == 0;

					const auto angleEndVec = Vector2F(0, tickMarksRadius).rotate(-toRadians(angleDeg - stepUnitsPerYawDegFractPart * _compassTickMarkUnitsDeg));
					const auto angleEndVecNorm = angleEndVec.normalize();
					const auto angleStartVec = angleEndVec - angleEndVecNorm * (isBig ? _compassTickMarkBigLength : _compassTickMarkSmallLength);

					renderer->renderLine(
						Point(
							centerX + static_cast<int32_t>(angleStartVec.getX()),
							y - static_cast<int32_t>(angleStartVec.getY())
						),
						Point(
							centerX + static_cast<int32_t>(angleEndVec.getX()),
							y - static_cast<int32_t>(angleEndVec.getY())
						),
						&Theme::fg1
					);

					if (isBig) {
						const auto text = std::to_wstring(shownAngleDeg / 10);
						const auto textWidth = Theme::fontSmall.getWidth(text);
						const auto textDiagonal = std::sqrt(textWidth * textWidth + Theme::fontSmall.getHeight() * Theme::fontSmall.getHeight());
						const auto textCenterVec = angleStartVec - angleEndVecNorm * (_compassTickMarkTextOffset + textDiagonal / 2);

						renderer->renderString(
							Point(
								centerX + static_cast<int32_t>(textCenterVec.getX()) - textWidth / 2,
								y - static_cast<int32_t>(textCenterVec.getY()) - Theme::fontSmall.getHeight() / 2
							),
							&Theme::fontSmall,
							&Theme::fg1,
							text
						);
					}
				}

				// Heading text
				const auto yawDegText = std::format(L"{:03}", static_cast<int32_t>(normalizeAngle360(yawDeg)));
				const uint16_t yawDegTextWidth = Theme::fontNormal.getWidth(yawDegText);

				y -= circleRadius + _compassCircleMarginTopPx + _compassHeadingTextVerticalLineHeight - 1;

				// Vertical line
				renderer->renderVerticalLine(
					Point(
						centerX,
						y
					),
					_compassHeadingTextVerticalLineHeight,
					&Theme::fg1
				);

				y -= 1;

				// Horizontal line
				renderer->renderHorizontalLine(
					Point(
						centerX - yawDegTextWidth / 2,
						y
					),
					yawDegTextWidth,
					&Theme::fg1
				);

				y -= _compassHeadingTextHorizontalLineOffset + Theme::fontNormal.getHeight();

				// Heading text
				renderer->renderString(
					Point(
						centerX - yawDegTextWidth / 2,
						y
					),
					&Theme::fontNormal,
					&Theme::fg1,
					yawDegText
				);
			}
		}

		// Cursor
		if (isCursorVisible()) {
			constexpr static uint8_t lineAreaOffset = 5;
			constexpr static uint8_t lineAreaSize = 8;

			// Center
			const auto cursorCenter = bounds.getPosition() + _cursorPosition;

			renderer->renderPixel(cursorCenter, &Theme::yellow);

			// Upper
			renderer->renderLine(
				Point(cursorCenter.getX(), cursorCenter.getY() - lineAreaOffset),
				Point(cursorCenter.getX() - lineAreaSize, cursorCenter.getY() - lineAreaOffset - lineAreaSize),
				&Theme::yellow
			);

			renderer->renderLine(
				Point(cursorCenter.getX(), cursorCenter.getY() - lineAreaOffset),
				Point(cursorCenter.getX() + lineAreaSize, cursorCenter.getY() - lineAreaOffset - lineAreaSize),
				&Theme::yellow
			);

			// Lower
			renderer->renderLine(
				Point(cursorCenter.getX(), cursorCenter.getY() + lineAreaOffset),
				Point(cursorCenter.getX() - lineAreaSize, cursorCenter.getY() + lineAreaOffset + lineAreaSize),
				&Theme::yellow
			);

			renderer->renderLine(
				Point(cursorCenter.getX(), cursorCenter.getY() + lineAreaOffset),
				Point(cursorCenter.getX() + lineAreaSize, cursorCenter.getY() + lineAreaOffset + lineAreaSize),
				&Theme::yellow
			);
		}
	}

	void NDScene::onEvent(Event* event) {
		Scene::onEvent(event);

		if (event->getTypeID() == TouchDownEvent::typeID) {
			const auto touchDownEvent = static_cast<TouchDownEvent*>(event);

			focus();
			capture();

			_touchDownPosition = touchDownEvent->getPosition();
			_cursorPosition = touchDownEvent->getPosition() - getBounds().getPosition();

			event->setHandled(true);
		}
		else if (event->getTypeID() == TouchDragEvent::typeID) {
			const auto touchDragEventEvent = static_cast<TouchDragEvent*>(event);

//			ESP_LOGI("ND", "------------- Drag -------------");

			auto& rc = RC::getInstance();
			const auto yaw = rc.getSettings().interface.MFD.ND.mode == SettingsInterfaceMFDNDMode::mapNorthUp ? 0 : rc.getAircraftData().computed.yaw;
			const auto& deltaPixels = (touchDragEventEvent->getPosition() - _touchDownPosition).rotate(yaw);
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
			removeCapture();

			event->setHandled(true);
		}
		else if (event->getTypeID() == PinchDownEvent::typeID) {
			const auto pinchDownEvent = static_cast<PinchDownEvent*>(event);

			_pinchLength = pinchDownEvent->getLength();
			hideCursor();

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

			hideCursor();

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

				hideCursor();

				event->setHandled(true);
			}
		}
		else if (event->getTypeID() == EncoderPushEvent::typeID) {
			if (isFocused()) {
				const auto pushEvent = static_cast<EncoderPushEvent*>(event);

				if (pushEvent->isDown()) {
					resetCameraLateralOffset();
					hideCursor();

					event->setHandled(true);
				}
			}
		}
	}

	float NDScene::getEquatorialRadiansPerPixel() const {
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

	const GeographicCoordinates& NDScene::getCameraOffset() const {
		return _cameraOffset;
	}

	GeographicCoordinates NDScene::getCameraCoordinates() const {
		return _cameraCoordinates;
	}

	void NDScene::setCameraOffset(const GeographicCoordinates& value) {
		_cameraOffset = value;

		invalidate();
	}

	void NDScene::resetCameraLateralOffset() {
		setCameraOffset(GeographicCoordinates(0, 0, _cameraOffset.getAltitude()));
	}

	bool NDScene::isCameraShiftedLaterally() const {
		return _cameraOffset.getLatitude() != 0 || _cameraOffset.getLongitude() != 0;
	}

	void NDScene::deleteSceneElements() {
		for (const auto element : getSceneElements()) {
			delete element;
		}
		getSceneElements().clear();
	}

	void NDScene::createSceneElementsFromNavigationData() {
		auto& rc = RC::getInstance();
		const auto& settings = rc.getSettings();
		const auto& nd = rc.getNavigationData();

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
		if (settings.interface.MFD.ND.earth)
			addElement(new LinearSphere(Vector3F(), GeographicCoordinates::equatorialRadiusMeters, 16, 16, &Theme::bg3));

		// Flight plan routes
		for (const auto& route : nd.flightPlan.routes) {
			addElement(new RouteElement(&route, &Theme::purple));
		}

		// Airports
		for (const auto& airport : nd.airports) {
			addElement(new WaypointElement(&airport));

			// Runways
			for (const auto& runway : airport.runways) {
				addElement(new NDRunwayMesh(&runway, &Theme::fg1));
			}
		}

		// RNAV waypoints
		for (const auto& waypoint : nd.RNAVWaypoints) {
			addElement(new WaypointElement(&waypoint));
		}

		// Aircraft
		_aircraftElement = new AircraftElement();
		addElement(_aircraftElement);
	}

	void NDScene::renderWaypointStar(Renderer* renderer, const NavigationWaypointData* waypointData, const Point& center, const Color* color) {
		renderer->renderRectangle(
			Bounds(
				center.getX() - 1,
				center.getY() - 1,
				3,
				3
			),
			color
		);

		renderer->renderHorizontalLine(
			Point(
				center.getX() - 3,
				center.getY()
			),
			2,
			color
		);

		renderer->renderHorizontalLine(
			Point(
				center.getX() + 2,
				center.getY()
			),
			2,
			color
		);

		renderer->renderVerticalLine(
			Point(
				center.getX(),
				center.getY() - 3
			),
			2,
			color
		);

		renderer->renderVerticalLine(
			Point(
				center.getX(),
				center.getY() + 2
			),
			2,
			color
		);

		renderer->renderString(
			Point(center.getX() + 7, center.getY() - 7),
			&Theme::fontSmall,
			color,
			waypointData->name
		);
	}

	bool NDScene::isCursorVisible() const {
		return _cursorPosition.getX() >= 0 && _cursorPosition.getY() >= 0;
	}

	void NDScene::hideCursor() {
		_cursorPosition = { -1, -1 };
	}

	void NDScene::updatePivot() {
		const auto& bounds = getBounds();
		auto& rc = RC::getInstance();
		const auto& settings = rc.getSettings();

		setPivotOffset(
			settings.interface.MFD.ND.mode == SettingsInterfaceMFDNDMode::arcHeadingUp
			? Point(0, bounds.getHeight() / 2 - bounds.getHeight() * _compassCircleMarginBottomPct / 100)
			: Point()
		);
	}
}
