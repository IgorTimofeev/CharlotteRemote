#include "spatialView.h"
#include <esp_log.h>

namespace pizda {
	void SpatialView::onRender(Renderer* renderer, const Bounds& bounds) {
		for (auto object : _objects) {
			const auto vertices = object->getVertices();
			const auto verticesCount = object->getVertexCount();

			auto screenPositions = new Point[verticesCount];

			for (uint32_t i = 0; i < verticesCount; i++) {
				auto vertex = vertices[i];

//				ESP_LOGI("ND", "Point: %f, %f, %f", vertex.getX(), vertex.getY(), vertex.getZ());

				// Пиздим кодыч с майноси. КАКОВА ПРАВА!1 ГОВОРИЛА ОНА...........
				// OCGL.translate(-scene.camera.position[1], -scene.camera.position[2], -scene.camera.position[3])
				// OCGL.rotate(OCGL.rotateVectorRelativeToYAxis, -scene.camera.rotation[2])
				// OCGL.rotate(OCGL.rotateVectorRelativeToXAxis, -scene.camera.rotation[1])

				// Translating camera to point
				vertex -= _camera.getPosition();

				// Rotating camera
				vertex = vertex.rotateAroundYAxis(-_camera.getRotation().getY());
				vertex = vertex.rotateAroundXAxis(-_camera.getRotation().getX());

				// Applying perspective projection
				// zProjectionDivZ = math.abs(renderer.viewport.projectionSurface / OCGL.vertices[vertexIndex][3])
				// OCGL.vertices[vertexIndex][1] = zProjectionDivZ * OCGL.vertices[vertexIndex][1]
				// OCGL.vertices[vertexIndex][2] = zProjectionDivZ * OCGL.vertices[vertexIndex][2]

				const auto zFactor = std::abs(_camera.getPosition().getZ() / vertex.getZ());

				const auto screenPosition = Point(
					bounds.getXCenter() + (int32_t) (vertex.getX() * zFactor),
					bounds.getYCenter() + (int32_t) (vertex.getY() * zFactor)
				);

//				ESP_LOGI("ND", "Screen: %ld, %ld", screenPosition.getX(), screenPosition.getY());

				screenPositions[i] = screenPosition;
			}

			object->onRender(renderer, screenPositions);

			delete[] screenPositions;
		}
	}

	Camera& SpatialView::getCamera() {
		return _camera;
	}

	void SpatialView::addObject(Object* object) {
		_objects.push_back(object);
	}
}