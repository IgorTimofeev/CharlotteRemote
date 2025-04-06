#include "spatialView.h"
#include <esp_log.h>

namespace pizda {
	void SpatialView::onRender(Renderer* renderer, const Bounds& bounds) {
		const auto projectionPlaneDistance = (float) bounds.getWidth() / 2.f / std::tanf(_camera.getFOV() / 2.f);

		for (auto object : _objects) {
			const auto vertices = object->getVertices();
			const auto verticesCount = object->getVertexCount();

			auto screenPositions = new Vector3F[verticesCount];

			for (uint32_t i = 0; i < verticesCount; i++) {
				auto vertex = vertices[i];

//				ESP_LOGI("ND", "Point: %f, %f, %f", vertex.getX(), vertex.getY(), vertex.getZ());

				// Пиздим кодыч с майноси. КАКОВА ПРАВА!1 ГОВОРИЛА ОНА...........
				// OCGL.translate(-scene.camera.position[1], -scene.camera.position[2], -scene.camera.position[3])
				// OCGL.rotate(OCGL.rotateVectorRelativeToYAxis, -scene.camera.rotation[2])
				// OCGL.rotate(OCGL.rotateVectorRelativeToXAxis, -scene.camera.rotation[1])

				// Translating point to camera
				vertex -= _camera.getPosition();

				// Rotating point around camera
				if (_camera.getRotation().getZ() != 0)
					vertex = vertex.rotateAroundZAxis(-_camera.getRotation().getZ());

				if (_camera.getRotation().getX() != 0)
					vertex = vertex.rotateAroundXAxis(-_camera.getRotation().getX());

				if (_camera.getRotation().getY() != 0)
					vertex = vertex.rotateAroundYAxis(-_camera.getRotation().getY());

				// Applying perspective projection
				// From WebGL wiki:
				// https://learnwebgl.brown37.net/08_projections/projections_perspective.html
				// x' = (x * near) / z
				// y' = (y * near) / z
				screenPositions[i] = Vector3F(
					(float) bounds.getXCenter() + (vertex.getX() * projectionPlaneDistance / vertex.getZ()),
					(float) bounds.getYCenter() - (vertex.getY() * projectionPlaneDistance / vertex.getZ()),
					vertex.getZ()
				);
			}

			object->onRender(renderer, &_camera, screenPositions);

			delete[] screenPositions;
		}
	}

	Camera& SpatialView::getCamera() {
		return _camera;
	}

	void SpatialView::addObject(SpatialObject* object) {
		_objects.push_back(object);
	}
}