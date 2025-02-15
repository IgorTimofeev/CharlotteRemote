#pragma once

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"

#include "vector3.h"
#include "camera.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class Object {
		public:
			virtual Vector3F* getVertices() = 0;
			virtual uint16_t getVertexCount() = 0;
			virtual void onRender(Renderer* renderer, Camera* camera, Vector3F* vertices) = 0;
	};

	class Line : public Object {
		public:
			Line(const Vector3F& from, const Vector3F& to, const Color* color) : _color(color) {
				_vertices[0] = from;
				_vertices[0] = to;
			}

			Vector3F* getVertices() override {
				return _vertices;
			}

			uint16_t getVertexCount() override {
				return 2;
			}

			void onRender(Renderer* renderer, Camera* camera, Vector3F* vertices) override {
				const auto nearPlane = camera->getNearPlaneDistance();

				if (
					vertices[0].getZ() < nearPlane
					|| vertices[1].getZ() < nearPlane
				)
					return;

				renderer->renderLine(
					Point(
						(int32_t) vertices[0].getX(),
						(int32_t) vertices[0].getY()
					),
					Point(
						(int32_t) vertices[1].getX(),
						(int32_t) vertices[1].getY()
					),
					_color
				);
			}

			Vector3F getFrom() const {
				return _vertices[0];
			}

			void setFrom(const Vector3F& from) {
				_vertices[0] = from;
			}

			Vector3F getTo() const {
				return _vertices[1];
			}

			void setTo(const Vector3F& from) {
				_vertices[1] = from;
			}

			const Color* getColor() const {
				return _color;
			}

			void setColor(const Color* color) {
				_color = color;
			}

		private:
			Vector3F _vertices[2];
			const Color* _color = nullptr;
	};

	class Label : public Object {
		public:
			Label(const Vector3F& position, const Font* font, const Color* color, const std::wstring_view& text) : _position(position), _font(font), _color(color), _text(text) {

			}

			Vector3F* getVertices() override {
				return &_position;
			}

			uint16_t getVertexCount() override {
				return 1;
			}

			void onRender(Renderer* renderer, Camera* camera, Vector3F* vertices) override {
				const auto nearPlane = camera->getNearPlaneDistance();

				if (vertices[0].getZ() < nearPlane)
					return;

				const auto point = Point(
					(int32_t) vertices[0].getX(),
					(int32_t) vertices[0].getY()
				);

				const uint8_t radius = 3;
				const uint8_t textOffset = 5;
				
				renderer->renderFilledCircle(point, radius, _color);
				renderer->renderString(Point(point.getX() + radius + textOffset, point.getY() - _font->getHeight() / 2), _font, _color, _text);
			}

			const Vector3F& getPosition() const {
				return _position;
			}

			void setPosition(const Vector3F& position) {
				_position = position;
			}

			const Color* getColor() const {
				return _color;
			}

			void setColor(const Color* color) {
				_color = color;
			}

			const Font* getFont() const {
				return _font;
			}

			void setFont(const Font* font) {
				_font = font;
			}

			const std::wstring_view& getText() const {
				return _text;
			}

			void setText(const std::wstring_view& text) {
				_text = text;
			}

		private:
			Vector3F _position;
			const Font* _font = nullptr;
			const Color* _color = nullptr;
			std::wstring_view _text;
	};

	class Mesh : public Object {
		public:
			Mesh(Vector3F* vertices, uint16_t vertexCount) : _vertices(vertices), _vertexCount(vertexCount) {}

			Vector3F* getVertices() override {
				return _vertices;
			}

			uint16_t getVertexCount() override {
				return _vertexCount;
			}

		private:
			Vector3F* _vertices = nullptr;
			uint16_t _vertexCount = 0;
	};

	class LinearMesh : public Mesh {
		public:
			LinearMesh(Vector3F* vertices, uint16_t vertexCount, uint16_t* lineVertexIndices, uint16_t lineVertexIndicesCount, const Color* color) :
				Mesh(vertices, vertexCount),
				_lineVertexIndices(lineVertexIndices),
				_lineVertexIndicesCount(lineVertexIndicesCount),
				_color(color)
			{

			}

			void onRender(Renderer* renderer, Camera* camera, Vector3F* vertices) override {
				const auto nearPlane = camera->getNearPlaneDistance();
				Vector3F* vertex0;
				Vector3F* vertex1;

				for (uint16_t i = 0; i < _lineVertexIndicesCount; i += 2) {
					vertex0 = &vertices[_lineVertexIndices[i]];
					vertex1 = &vertices[_lineVertexIndices[i + 1]];

					if (
						vertex0->getZ() < nearPlane
						|| vertex1->getZ() < nearPlane
					)
						continue;

					renderer->renderLine(
						Point(
							(int32_t) vertex0->getX(),
							(int32_t) vertex0->getY()
						),
						Point(
							(int32_t) vertex1->getX(),
							(int32_t) vertex1->getY()
						),
						_color
					);
				}
			}

		private:
			uint16_t* _lineVertexIndices = nullptr;
			uint16_t _lineVertexIndicesCount = 0;

			const Color* _color = nullptr;
	};

	// |    /
	// |  /
	// y z
	// x -----
	//
	// FRONT     LEFT      BACK      RIGHT     TOP       BOTTOM
	// 1######2  2######5  5######6  6######1  6######5  7######4
	// ########  ########  ########  ########  ########  ########
	// 0######3  3######4  4######7  7######0  1######2  0######3
	class LinearCubeMesh : public LinearMesh {
		public:
			LinearCubeMesh(float size, const Color* color) :
				LinearMesh(
					_vertices,
					8,
					_lineVertexIndices,
					24,
					color
				)
			{
				const auto sizeHalf = size / 2;

				_vertices[0] = Vector3F(-sizeHalf, -sizeHalf, -sizeHalf);
				_vertices[1] = Vector3F(-sizeHalf, sizeHalf, -sizeHalf);
				_vertices[2] = Vector3F(sizeHalf, sizeHalf, -sizeHalf);
				_vertices[3] = Vector3F(sizeHalf, -sizeHalf, -sizeHalf);

				_vertices[4] = Vector3F(sizeHalf, -sizeHalf, sizeHalf);
				_vertices[5] = Vector3F(sizeHalf, sizeHalf, sizeHalf);
				_vertices[6] = Vector3F(-sizeHalf, sizeHalf, sizeHalf);
				_vertices[7] = Vector3F(-sizeHalf, -sizeHalf, sizeHalf);

				// Front
				_lineVertexIndices[0] = 0;
				_lineVertexIndices[1] = 1;

				_lineVertexIndices[2] = 1;
				_lineVertexIndices[3] = 2;

				_lineVertexIndices[4] = 2;
				_lineVertexIndices[5] = 3;

				_lineVertexIndices[6] = 0;
				_lineVertexIndices[7] = 3;

				// Back
				_lineVertexIndices[8] = 4;
				_lineVertexIndices[9] = 5;

				_lineVertexIndices[10] = 5;
				_lineVertexIndices[11] = 6;

				_lineVertexIndices[12] = 6;
				_lineVertexIndices[13] = 7;

				_lineVertexIndices[14] = 4;
				_lineVertexIndices[15] = 7;

				// Left
				_lineVertexIndices[16] = 2;
				_lineVertexIndices[17] = 5;

				_lineVertexIndices[18] = 3;
				_lineVertexIndices[19] = 4;

				// Right
				_lineVertexIndices[20] = 1;
				_lineVertexIndices[21] = 6;

				_lineVertexIndices[22] = 0;
				_lineVertexIndices[23] = 7;
			}

		private:
			Vector3F _vertices[8] {};
			uint16_t _lineVertexIndices[24] {};
	};
}