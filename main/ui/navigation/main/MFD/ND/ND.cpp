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
		addObject(new CubeLinearMesh(GeographicCoordinates::equatorialRadiusMeters * 2, &Theme::fg1));

		const auto& airfield1 = GeographicCoordinates(
			yoba::toRadians(60.01483325540486f),
			yoba::toRadians(29.69835915766679f),
			GeographicCoordinates::equatorialRadiusMeters
		).toCartesian();

		const auto& airfield2 = GeographicCoordinates(
			yoba::toRadians(60.014390616612474f),
			yoba::toRadians(29.706975357970624f),
			GeographicCoordinates::equatorialRadiusMeters
		).toCartesian();

		// Labels
		addObject(new Label(
			airfield1,
			&Theme::fontNormal,
			&Theme::yellow,
			L"Left"
		));

		addObject(new Label(
			airfield2,
			&Theme::fontNormal,
			&Theme::yellow,
			L"Right"
		));
	}

	void ND::onTick() {
		SpatialView::onTick();

		auto& rc = RC::getInstance();

		const auto rotationLatitude = rc.getGeographicCoordinates().getLatitude() + _cameraOffset.getX();
		const auto rotationLongitude = rc.getGeographicCoordinates().getLongitude() - _cameraOffset.getY();

		getCamera().setPosition(
			GeographicCoordinates(
				rotationLatitude,
				rotationLongitude,
				GeographicCoordinates::equatorialRadiusMeters + _cameraOffset.getZ()
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

	void ND::onRender(Renderer* renderer, const Bounds& bounds) {
		SpatialView::onRender(renderer, bounds);

		auto position = bounds.getTopLeft() + Point(5, 5);
		auto spacing = Point(0, Theme::fontNormal.getHeight() + 1);

		// Offset
		renderer->renderString(
			position,
			&Theme::fontNormal,
			&Theme::yellow,
			std::format(L"Camera off: {} x {} x {}", yoba::round(yoba::toDegrees(_cameraOffset.getX()), 2), yoba::round(yoba::toDegrees(_cameraOffset.getY()), 2), yoba::round(_cameraOffset.getZ(), 2))
		);

		position += spacing;

		// Rot
		renderer->renderString(
			position,
			&Theme::fontNormal,
			&Theme::green,
			std::format(L"Camera rot: {} x {} x {}", yoba::round(yoba::toDegrees(getCamera().getRotation().getX()), 2), yoba::round(yoba::toDegrees(getCamera().getRotation().getY()), 2), yoba::round(yoba::toDegrees(getCamera().getRotation().getZ()), 2))
		);

		position += spacing;
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

			auto deltaPixels= position - _touchDownPosition;
//			ESP_LOGI("ND", "deltaPixels: %ld, %ld", deltaPixels.getX(), deltaPixels.getY());

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
			_cameraOffset.setX(_cameraOffset.getX() + (float) deltaAngles.getY());
			// Long
			_cameraOffset.setY(_cameraOffset.getY() + (float) deltaAngles.getX());

			_touchDownPosition = position;

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

			_cameraOffset.setZ(std::max(_cameraOffset.getZ() + (pinchFactor > 1 ? -100.f : 100.f), 0.f));

			event->setHandled(true);
		}
	}

	const Vector3F& ND::getCameraOffset() const {
		return _cameraOffset;
	}

	void ND::setCameraOffset(const Vector3F& value) {
		_cameraOffset = value;
	}
}