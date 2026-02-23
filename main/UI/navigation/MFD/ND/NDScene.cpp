#include "NDScene.h"

#include "rc.h"

#include <format>
#include <esp_log.h>

#include "UI/navigation/MFD/ND/sceneElements/NDRunwayElement.h"
#include "UI/navigation/MFD/ND/sceneElements/waypointElement.h"
#include "UI/navigation/MFD/ND/sceneElements/routeElement.h"
#include "UI/navigation/MFD/PFD/PFD.h"

namespace pizda {
	NDScene::NDScene() {
		setClipToBounds(true);
		setFOV(toRadians(90));

		createSceneElementsFromNavigationData();
	}

	NDScene::~NDScene() {
		deleteSceneElements();
	}

	GeoCoordinates NDScene::_cameraOffset = {
		0,
		0,
		2500
	};

	void NDScene::onTick() {
		Scene::onTick();

		auto& rc = RC::getInstance();
		
		// Aircraft
		_aircraftElement->setVisible(isCameraShiftedLaterally());

		if (_aircraftElement->isVisible()) {
			_aircraftElement->setPosition(
				GeoCoordinates(
					rc.getAircraftData().computed.coordinates.getLatitude(),
					rc.getAircraftData().computed.coordinates.getLongitude(),
					0
				)
				.toCartesian()
			);
		}

		// Camera
		_cameraCoordinates.setLatitude(rc.getAircraftData().computed.coordinates.getLatitude() + _cameraOffset.getLatitude());
		_cameraCoordinates.setLongitude(rc.getAircraftData().computed.coordinates.getLongitude() + _cameraOffset.getLongitude());
		_cameraCoordinates.setAltitude(_cameraOffset.getAltitude());

		setCameraPosition(_cameraCoordinates.toCartesian());

		setCameraRotation(Vector3F(
			-_cameraCoordinates.getLatitude(),
			-rc.getAircraftData().computed.yawRad,
			toRadians(90) + _cameraCoordinates.getLongitude()
		));

		invalidate();
	}

	void NDScene::onBoundsChanged() {
		Scene::onBoundsChanged();

		updatePivot();
	}

	void NDScene::onRender(Renderer& renderer, const Bounds& bounds) {
		Scene::onRender(renderer, bounds);

		auto& rc = RC::getInstance();

		const auto pivot = bounds.getCenter() + getPivotOffset();

		const auto isLandscape = bounds.isLandscape();

		const uint16_t circleMarginTopPixels = bounds.getHeight() * compassCircleMarginTopPct / 100;
		const uint16_t circleMarginBottomPixels = bounds.getHeight() * compassCircleMarginBottomPct / 100;

		uint16_t circleRadius;
		uint16_t tickMarksRadius;
		int16_t tickAngleFromDeg;
		int16_t tickAngleToDeg;

		// Arc
		if (rc.getSettings().personalization.MFD.ND.mode == PersonalizationSettingsMFDNDMode::arc) {
			circleRadius =
				bounds.getHeight()
				- circleMarginTopPixels
				- circleMarginBottomPixels;

			tickMarksRadius = circleRadius;

			tickAngleFromDeg = -compassArcViewportHalfDeg + compassTickMarkUnitsDeg;
			tickAngleToDeg = compassArcViewportHalfDeg;
		}
		// Map
		else {
			const uint16_t landscapeRadius =
				(
					bounds.getHeight()
					- circleMarginTopPixels
					- circleMarginBottomPixels
				) / 2;

			const uint16_t portraitRadius =
				(
					bounds.getWidth()
					- bounds.getWidth() * compassCircleMarginHorizontalPct / 100 * 2
				)
				/ 2;

			circleRadius =
				isLandscape
				? landscapeRadius
				: std::min(landscapeRadius, portraitRadius);

			tickMarksRadius = circleRadius - 1;

			tickAngleFromDeg = 0;
			tickAngleToDeg = 360 - compassTickMarkUnitsDeg;
		}

		// Aircraft indicator
		if (isCameraShiftedLaterally()) {
			// Cross
			renderer.renderHorizontalLine(
				Point(
					pivot.getX() - compassLateralOffsetCrossSize / 2,
					pivot.getY()
				),
				compassLateralOffsetCrossSize,
				&Theme::fg1
			);

			renderer.renderVerticalLine(
				Point(
					pivot.getX(),
					pivot.getY() - compassLateralOffsetCrossSize / 2
				),
				compassLateralOffsetCrossSize,
				&Theme::fg1
			);
		}
		else {
			// Triangle
			AircraftElement::render(renderer, pivot);
		}

		// Enough space for rose
		if (bounds.getHeight() >= 80) {
			// Arc
			if (rc.getSettings().personalization.MFD.ND.mode == PersonalizationSettingsMFDNDMode::arc) {
				renderer.renderArc(
					pivot,
					circleRadius,
					(90 - compassArcViewportHalfDeg) * 255 / 360,
					(90 + compassArcViewportHalfDeg) * 255 / 360,
					&Theme::fg1
				);
			}
			// Map
			else {

			}

			// Tick marks
			{
				float stepUnitsPerYawDegIntPart;
				const float stepUnitsPerYawDegFractPart = std::modff(rc.getAircraftData().computed.headingDeg / compassTickMarkUnitsDeg, &stepUnitsPerYawDegIntPart);
				const int32_t yawSnappedInt = static_cast<int32_t>(stepUnitsPerYawDegIntPart) * compassTickMarkUnitsDeg;

				for (int16_t angleDeg = tickAngleFromDeg; angleDeg <= tickAngleToDeg; angleDeg += compassTickMarkUnitsDeg) {
					const uint16_t shownAngleDeg = normalizeAngleDeg360(yawSnappedInt + angleDeg);
					const auto isBig = shownAngleDeg % compassTickMarkUnitsBigDeg == 0;

					const auto angleEndVec = Vector2F(0, tickMarksRadius).rotate(-toRadians(angleDeg - stepUnitsPerYawDegFractPart * compassTickMarkUnitsDeg));
					const auto angleEndVecNorm = angleEndVec.normalize();
					const auto angleStartVec = angleEndVec - angleEndVecNorm * (isBig ? compassTickMarkBigLength : compassTickMarkSmallLength);

					renderer.renderLine(
						Point(
							pivot.getX() + static_cast<int32_t>(angleStartVec.getX()),
							pivot.getY() - static_cast<int32_t>(angleStartVec.getY())
						),
						Point(
							pivot.getX() + static_cast<int32_t>(angleEndVec.getX()),
							pivot.getY() - static_cast<int32_t>(angleEndVec.getY())
						),
						&Theme::fg1
					);

					if (isBig) {
						const auto text = std::to_wstring(shownAngleDeg / 10);
						const auto textWidth = Theme::fontSmall.getWidth(text);
						const auto textDiagonal = std::sqrt(textWidth * textWidth + Theme::fontSmall.getHeight() * Theme::fontSmall.getHeight());
						const auto textCenterVec = angleStartVec - angleEndVecNorm * (compassTickMarkTextOffset + textDiagonal / 2);

						renderer.renderString(
							Point(
								pivot.getX() + static_cast<int32_t>(textCenterVec.getX() - textWidth / 2),
								pivot.getY() - static_cast<int32_t>(textCenterVec.getY()) - Theme::fontSmall.getHeight() / 2
							),
							&Theme::fontSmall,
							&Theme::fg1,
							text
						);
					}
				}
			}

			// Triangle
			renderer.renderFilledTriangle(
				Point(
					pivot.getX(),
					pivot.getY() - circleRadius
				),
				Point(
					pivot.getX() - compassArcTriangleWidth / 2,
					pivot.getY() - circleRadius + compassArcTriangleHeight
				),
				Point(
					pivot.getX() + compassArcTriangleWidth / 2,
					pivot.getY() - circleRadius + compassArcTriangleHeight
				),
				&Theme::fg1
			);
		}
	}

	void NDScene::onEvent(Event* event) {
		Scene::onEvent(event);

		if (event->getTypeID() == PointerDownEvent::typeID) {
			const auto pointerDownEvent = static_cast<PointerDownEvent*>(event);

			setFocused(true);
			setCaptured(true);

			_pointerDownPosition = pointerDownEvent->getPosition();

			event->setHandled(true);
		}
		else if (event->getTypeID() == PointerDragEvent::typeID) {
			const auto pointerDragEventEvent = static_cast<PointerDragEvent*>(event);

//			ESP_LOGI("ND", "------------- Drag -------------");

			auto& rc = RC::getInstance();
			const auto yaw = -rc.getAircraftData().computed.yawRad;
			const auto& deltaPixels = (pointerDragEventEvent->getPosition() - _pointerDownPosition).rotate(yaw);
			_pointerDownPosition = pointerDragEventEvent->getPosition();

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

			setCameraOffset(GeoCoordinates(
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
		else if (event->getTypeID() == PointerUpEvent::typeID) {
			setCaptured(false);

			event->setHandled(true);
		}
		else if (event->getTypeID() == PinchDownEvent::typeID) {
			_pinchLength = reinterpret_cast<PinchDownEvent*>(event)->getLength();

			event->setHandled(true);
		}
		else if (event->getTypeID() == PinchDragEvent::typeID) {

			//			ESP_LOGI("ND", "------------- Pinch -------------");

			//			const auto pinchLength = pinchDragEvent->getLength();
			//			const auto pinchDelta = pinchLength - _pinchLength;
			//			_pinchLength = pinchLength;
			//
			//			ESP_LOGI("ND", "pinchDelta: %f px", pinchDelta);
			//
			//			setAltitudeFromDeltaPixels(-pinchDelta);

			const auto pinchLength = reinterpret_cast<PinchDragEvent*>(event)->getLength();
			const auto pinchFactor = _pinchLength / pinchLength;
			_pinchLength = pinchLength;

			setCameraOffset(GeoCoordinates(
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
		else if (event->getTypeID() == PinchUpEvent::typeID) {

		}
		else if (event->getTypeID() == EncoderValueChangedEvent::typeID) {
			if (isFocused()) {
				const auto rotateEvent = static_cast<EncoderValueChangedEvent*>(event);
				const auto scaleFactor = std::abs(rotateEvent->getDPS()) > 80 ? 1.5f : 1.25f;

				setCameraOffset(GeoCoordinates(
					_cameraOffset.getLatitude(),
					_cameraOffset.getLongitude(),
					std::clamp(
						rotateEvent->getDPS() >= 0
							? _cameraOffset.getAltitude() / scaleFactor
							: _cameraOffset.getAltitude() * scaleFactor,
						static_cast<float>(cameraAltitudeMinimum),
						static_cast<float>(cameraAltitudeMaximum)
					)
				));

				event->setHandled(true);
			}
		}
		else if (event->getTypeID() == PushButtonEncoderDownEvent::typeID) {
			if (isFocused()) {
				resetCameraLateralOffset();

				event->setHandled(true);
			}
		}
	}

	float NDScene::getEquatorialRadiansPerPixel() const {
		// Imagine that camera is placed in the center of small sphere with radius = camera altitude
//		const float maxViewportRads = 2.f * std::asinf(GeographicCoordinates::equatorialRadiusMeters / (GeographicCoordinates::equatorialRadiusMeters + cameraAltitudeMaximum));

		// First, we need to find out how many times the length of the equator of this sphere is
		// greater than the length of the earth's equator
		const auto radiusFactor = _cameraOffset.getAltitude() / GeoCoordinates::equatorialRadiusMeters;
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

	const GeoCoordinates& NDScene::getCameraOffset() const {
		return _cameraOffset;
	}

	GeoCoordinates NDScene::getCameraCoordinates() const {
		return _cameraCoordinates;
	}

	void NDScene::setCameraOffset(const GeoCoordinates& value) {
		_cameraOffset = value;

		invalidate();
	}

	void NDScene::resetCameraLateralOffset() {
		setCameraOffset(GeoCoordinates(0, 0, _cameraOffset.getAltitude()));
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

		// // Axes
		// addElement(new Line(
		// 	Vector3F(0, 0, 0),
		// 	Vector3F(GeographicCoordinates::equatorialRadiusMeters, 0, 0),
		// 	&Theme::red
		// ));
		//
		// addElement(new Line(
		// 	Vector3F(0, 0, 0),
		// 	Vector3F(0, GeographicCoordinates::equatorialRadiusMeters, 0),
		// 	&Theme::green1
		// ));
		//
		// addElement(new Line(
		// 	Vector3F(0, 0, 0),
		// 	Vector3F(0, 0, GeographicCoordinates::equatorialRadiusMeters),
		// 	&Theme::sky1
		// ));

		// Sphere
		if (rc.getSettings().personalization.MFD.ND.earth)
			addElement(new LinearSphere(Vector3F(), GeoCoordinates::equatorialRadiusMeters, 16, 16, &Theme::bg2));

		// Flight plan
		if (rc.getNavigationData().flightPlan.legs.size() > 1) {
			// Subsequent
			for (uint16_t legIndex = 1; legIndex < rc.getNavigationData().flightPlan.legs.size(); legIndex++) {
				addElement(new RouteElement(legIndex));
			}
		}

		// Runways
		for (uint16_t i = 0; i < rc.getNavigationData().runways.size(); ++i) {
			addElement(new NDRunwayElement(i));
		}

		// Enroute waypoints
		for (const auto& waypoint : rc.getNavigationData().enrouteWaypoints) {
			addElement(new WaypointElement(waypoint.waypointIndex));
		}

		// Aircraft
		_aircraftElement = new AircraftElement();
		addElement(_aircraftElement);
	}

	void NDScene::updatePivot() {
		const auto& bounds = getBounds();
		auto& rc = RC::getInstance();

		setPivotOffset(
			rc.getSettings().personalization.MFD.ND.mode == PersonalizationSettingsMFDNDMode::arc
			? Point(0, bounds.getHeight() / 2 - bounds.getHeight() * compassCircleMarginBottomPct / 100)
			: Point(0, 0)
		);
	}
}
