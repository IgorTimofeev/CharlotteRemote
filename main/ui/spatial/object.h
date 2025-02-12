#pragma once

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"

#include "vector3.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class Object {
		public:
			virtual Vector3F* getVertices() = 0;
			virtual uint16_t getVertexCount() = 0;
			virtual void onRender(Renderer* renderer, Point* points) = 0;
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

			void onRender(Renderer* renderer, Point* points) override {
				renderer->renderLine(points[0], points[1], _color);
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

			void onRender(Renderer* renderer, Point* points) override {
				renderer->renderCircle(points[0], 10, _color);
				renderer->renderString(Point(points[0].getX() + 10, points[0].getY()), _font, _color, _text);
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
			Mesh(Vector3F* vertices, uint16_t vertexCount, uint16_t* triangleVertexIndices, uint16_t triangleVertexIndicesCount, const Color* color) : _color(color), _vertices(vertices), _vertexCount(vertexCount), _triangleVertexIndices(triangleVertexIndices), _triangleVertexIndicesCount(triangleVertexIndicesCount) {}

			Vector3F* getVertices() override {
				return _vertices;
			}

			uint16_t getVertexCount() override {
				return _vertexCount;
			}

			void onRender(Renderer* renderer, Point* points) override {
				for (uint16_t i = 0; i < _triangleVertexIndicesCount; i += 3) {
					renderer->renderLine(
						points[_triangleVertexIndices[i]],
						points[_triangleVertexIndices[i + 1]],
						_color
					);

					renderer->renderLine(
						points[_triangleVertexIndices[i + 1]],
						points[_triangleVertexIndices[i + 2]],
						_color
					);

					renderer->renderLine(
						points[_triangleVertexIndices[i + 2]],
						points[_triangleVertexIndices[i]],
						_color
					);
				}
			}


		private:
			const Color* _color = nullptr;

			Vector3F* _vertices = nullptr;
			uint16_t _vertexCount = 0;

			uint16_t* _triangleVertexIndices = nullptr;
			uint16_t _triangleVertexIndicesCount = 0;
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
	class Cube : public Mesh {
		public:
			Cube(float size, const Color* color) :
				Mesh(
					_vertices,
					8,
					_triangleVertexIndices,
					10,
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
				_triangleVertexIndices[0] = 0;
				_triangleVertexIndices[1] = 1;
				_triangleVertexIndices[2] = 2;

				_triangleVertexIndices[3] = 0;
				_triangleVertexIndices[4] = 3;
				_triangleVertexIndices[5] = 2;

				// Left
				_triangleVertexIndices[6] = 3;
				_triangleVertexIndices[7] = 2;
				_triangleVertexIndices[8] = 5;

				_triangleVertexIndices[9] = 3;
				_triangleVertexIndices[10] = 4;
				_triangleVertexIndices[11] = 5;

				// Back
				_triangleVertexIndices[12] = 4;
				_triangleVertexIndices[13] = 5;
				_triangleVertexIndices[14] = 6;

				_triangleVertexIndices[15] = 4;
				_triangleVertexIndices[16] = 7;
				_triangleVertexIndices[17] = 6;

				// Right
				_triangleVertexIndices[18] = 7;
				_triangleVertexIndices[19] = 6;
				_triangleVertexIndices[20] = 1;

				_triangleVertexIndices[21] = 7;
				_triangleVertexIndices[22] = 0;
				_triangleVertexIndices[23] = 1;

				// Top
				_triangleVertexIndices[24] = 1;
				_triangleVertexIndices[25] = 6;
				_triangleVertexIndices[26] = 5;

				_triangleVertexIndices[27] = 1;
				_triangleVertexIndices[28] = 2;
				_triangleVertexIndices[29] = 5;

				// Bottom
				_triangleVertexIndices[30] = 0;
				_triangleVertexIndices[31] = 7;
				_triangleVertexIndices[32] = 4;

				_triangleVertexIndices[33] = 0;
				_triangleVertexIndices[34] = 3;
				_triangleVertexIndices[35] = 4;
			}

		private:
			Vector3F _vertices[8] {};
			uint16_t _triangleVertexIndices[36] {};
	};
}