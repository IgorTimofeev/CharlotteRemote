#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/UI/spatial.h>

#include "../../../theme.h"

namespace pizda {
	using namespace YOBA;
	using namespace YOBA::spatial;

	class AircraftElement : public SceneElement {
		public:
			Vector3F* getVertices() override {
				return &_position;
			}

			uint16_t getVertexCount() override {
				return 1;
			}

			void onRender(Renderer* renderer, const Scene& scene, const Vector3F* vertices) override {
				if (vertices[0].getZ() < scene.getNearPlaneDistance())
					return;

				constexpr static uint8_t triangleWidth = 8;
				constexpr static uint8_t triangleHeight = 6;

				const auto position = Point(
					static_cast<int32_t>(vertices[0].getX()),
					static_cast<int32_t>(vertices[0].getY())
				);

				renderer->renderLine(
					position,
					Point(position.getX(), scene.getBounds().getY()),
					&Theme::purple
				);

				renderer->renderTriangle(
					position,
					Point(position.getX() - triangleWidth / 2, position.getY() + triangleHeight),
					Point(position.getX() + triangleWidth / 2, position.getY() + triangleHeight),
					&Theme::fg1
				);
			}

			const Vector3F& getPosition() const {
				return _position;
			}

			void setPosition(const Vector3F& position) {
				_position = position;
			}

		private:
			Vector3F _position {};
	};
}