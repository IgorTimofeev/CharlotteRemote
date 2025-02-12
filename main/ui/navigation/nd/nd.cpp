#include "nd.h"
#include <sstream>
#include <cmath>
#include "../../../rc.h"

namespace pizda {
	ND::ND() {
		setClipToBounds(true);
	}

	void ND::onRender(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();
		auto& remoteData = rc.getRemoteData();
		auto& computedData = rc.getComputedData();

//		const auto distanceFromAircraftToEarthCenter = _earthEquatorialRadius + remoteData.getAltitude();
		const auto distanceFromAircraftToEarthCenter = 100;

		const auto aircraftPosition = geographicToCartesian(
			computedData.getLatitudeSinAndCos(),
			computedData.getLongitudeSinAndCos(),
			distanceFromAircraftToEarthCenter
		);

		auto cameraPosition =  geographicToCartesian(
			SinAndCos(remoteData.getLatitude() + _cameraOffset.getX()),
			SinAndCos(remoteData.getLongitude() + _cameraOffset.getY()),
			distanceFromAircraftToEarthCenter
		);

		auto cameraRotation = -aircraftPosition;

//		ESP_LOGI("ND", "--------------");
//		ESP_LOGI("ND", "Camera: %f, %f, %f", cameraPosition.getX(), cameraPosition.getY(), cameraPosition.getZ());

		for (auto& point : _points) {
			auto pointPosition = geographicToCartesian(
				SinAndCos(point.getX()),
				SinAndCos(point.getY()),
				distanceFromAircraftToEarthCenter
			);

//			ESP_LOGI("ND", "Point: %f, %f, %f", pointPosition.getX(), pointPosition.getY(), pointPosition.getZ());

			// Пиздим кодыч с майноси. КАКОВА ПРАВА!1 ГОВОРИЛА ОНА...........
			// OCGL.translate(-scene.camera.position[1], -scene.camera.position[2], -scene.camera.position[3])
			// OCGL.rotate(OCGL.rotateVectorRelativeToYAxis, -scene.camera.rotation[2])
			// OCGL.rotate(OCGL.rotateVectorRelativeToXAxis, -scene.camera.rotation[1])

			// Translating camera to point
			pointPosition -= cameraPosition;

			// Rotating camera
			auto cameraRotationXSinCos = SinAndCos(cameraRotation.getX());
			auto cameraRotationZSinCos = SinAndCos(cameraRotation.getZ());

			pointPosition = pointPosition.rotateAroundZAxis(
				-cameraRotationZSinCos.getSin(),
				-cameraRotationZSinCos.getCos()
			);

			pointPosition = pointPosition.rotateAroundXAxis(
				-cameraRotationXSinCos.getSin(),
				-cameraRotationXSinCos.getCos()
			);

			// Applying perspective projection

//			zProjectionDivZ = math.abs(renderer.viewport.projectionSurface / OCGL.vertices[vertexIndex][3])
//			OCGL.vertices[vertexIndex][1] = zProjectionDivZ * OCGL.vertices[vertexIndex][1]
//			OCGL.vertices[vertexIndex][2] = zProjectionDivZ * OCGL.vertices[vertexIndex][2]

//			const auto zFactor = std::abs(cameraPosition.getZ() / pointPosition.getZ());
			const auto zFactor = 1.f;

			const auto screenPosition = Point(
				bounds.getXCenter() + (int32_t) (pointPosition.getX() * zFactor),
				bounds.getYCenter() + (int32_t) (pointPosition.getY() * zFactor)
			);

//			ESP_LOGI("ND", "Screen: %ld, %ld", screenPosition.getX(), screenPosition.getY());

			renderer->renderCircle(
				screenPosition,
				10,
				&Theme::yellow
			);
		}

		// Pixels per meter
		renderer->renderString(
			bounds.getTopLeft(),
			&Theme::fontNormal,
			&Theme::fg1,
			std::format(L"Scale: {}", _metersPerPixel)
		);
	}

	void ND::onEvent(Event& event) {
		Element::onEvent(event);

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
			auto deltaMeters= (Vector2F) deltaPixels * _metersPerPixel;
			auto deltaAngles = Vector2F(
				yoba::toRadians(deltaMeters.getX() / _earthMetersPer1EquatorialDeg),
				yoba::toRadians(deltaMeters.getY() / _earthMetersPer1EquatorialDeg)
			);

			ESP_LOGI("ND", "deltaPixels: %ld, %ld", deltaPixels.getX(), deltaPixels.getY());
			ESP_LOGI("ND", "deltaMeters: %f, %f", deltaMeters.getX(), deltaMeters.getY());
			ESP_LOGI("ND", "deltaAngles: %f, %f", deltaAngles.getX(), deltaAngles.getY());

			// Lat
			_cameraOffset.setX(_cameraOffset.getX() + (float) deltaAngles.getY());
			// Lon
			_cameraOffset.setY(_cameraOffset.getY() + (float) deltaAngles.getX());

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