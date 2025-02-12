#include "spatialView.h"
#include <esp_log.h>

namespace pizda {
	void SpatialView::onRender(Renderer* renderer, const Bounds& bounds) {
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
				vertex = vertex.rotateAroundYAxis(-_camera.getRotation().getY());
				vertex = vertex.rotateAroundXAxis(-_camera.getRotation().getX());

				// Applying perspective projection
				// camera.projectionSurface = camera.farClippingSurface - camera.FOV / math.rad(180) * (camera.farClippingSurface - camera.nearClippingSurface)
				// zProjectionDivZ = math.abs(renderer.viewport.projectionSurface / OCGL.vertices[vertexIndex][3])
				// OCGL.vertices[vertexIndex][1] = zProjectionDivZ * OCGL.vertices[vertexIndex][1]
				// OCGL.vertices[vertexIndex][2] = zProjectionDivZ * OCGL.vertices[vertexIndex][2]

				// From WebGL wiki:
				// https://learnwebgl.brown37.net/08_projections/projections_perspective.html
				// x' = (x*near)/z
				// y' = (y*near)/z
				const auto perspectiveFactor = std::abs(_camera.getNearPlaneDistance() / vertex.getZ()) * ((float) M_PI / _camera.getFieldOfView());

				const auto screenPosition = Vector3F(
					(float) bounds.getXCenter() + vertex.getX() * perspectiveFactor,
					(float) bounds.getYCenter() + vertex.getY() * perspectiveFactor,
					-vertex.getZ()
				);

//				ESP_LOGI("ND", "Screen: %ld, %ld", screenPosition.getX(), screenPosition.getY());

				screenPositions[i] = screenPosition;
			}

			object->onRender(renderer, &_camera, screenPositions);

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