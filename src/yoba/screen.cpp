#include "Arduino.h"
#include "screen.h"
#include "yoba/elements/workspace.h"
#include "yoba/hardware/touch/FT6336U.h"

namespace yoba {
	Screen::Screen(
		const Size &resolution,
		const uint8_t &touchRstPin,
		const uint8_t &touchIntPin
	) :
		_resolution(resolution),
		_touchIntPin(touchIntPin),
		_touchPanel(FT6336U(
			touchRstPin,
			touchIntPin
		))
	{

	}


	uint16_t _palette[16];

	void Screen::begin() {
		// TFT
		_tft.init();
		_tft.setRotation(1);

//		_buffer.setAttribute(PSRAM_ENABLE, true);
//		_buffer.setColorDepth(8);
//		_buffer.createSprite((int16_t) _resolution.getWidth(), (int16_t) _resolution.getHeight());

		_buffer.setColorDepth(4);
		_buffer.createSprite((int16_t) _resolution.getWidth(), (int16_t) _resolution.getHeight());

		// Touch
		_touchPanel.begin();

		pinMode(_touchIntPin, INPUT_PULLUP);
		attachInterrupt(digitalPinToInterrupt(_touchIntPin), Screen::onTouchInterrupted, CHANGE);
	}

	void Screen::tick() {
		readTouch();
	}

	const Size &Screen::getResolution() const {
		return _resolution;
	}

	// -------------------------------- Rendering --------------------------------

	Size Screen::measureText(const String &text) {
		return {
			(uint16_t) _buffer.textWidth(text),
			(uint16_t) _buffer.fontHeight()
		};
	}

	void Screen::setDefaultFont(const uint8_t *value) {
		_defaultFont = value;
	}

	void Screen::setFont(const uint8_t* value) {
		if (!value)
			value = _defaultFont;

		if (value == _font)
			return;

		_font = value;
		_buffer.loadFont(_font);
	}

	void Screen::renderImage(const Bounds &bounds, const uint16_t *data) {
		_buffer.pushImage(
			bounds.getX(),
			bounds.getY(),
			bounds.getWidth(),
			bounds.getHeight(),
			data
		);
	}

	void Screen::renderCircle(const Point &position, int32_t radius, const Color* color) {
		_buffer.fillSmoothCircle(
			(int16_t) position.getX(),
			(int16_t) position.getY(),
			(int16_t) radius,
			color->to16Bit()
		);
	}

	void Screen::renderText(const Point &position, const Color* color, const String &text) {
		_buffer.setTextColor(color->to16Bit());
		_buffer.setCursor((int16_t) position.getX(), (int16_t) position.getY());
		_buffer.print(text);
	}

	void Screen::renderRectangle(const Bounds &bounds, uint16_t radius, const Color* color) {
		_buffer.fillSmoothRoundRect(
			(int16_t) bounds.getX(),
			(int16_t) bounds.getY(),
			(int16_t) bounds.getWidth(),
			(int16_t) bounds.getHeight(),
			(int16_t) radius,
			color->to16Bit()
		);
	}

	void Screen::renderRectangle(const Bounds &bounds, const Color* color) {
		_buffer.fillRect(
			(int16_t) bounds.getX(),
			(int16_t) bounds.getY(),
			(int16_t) bounds.getWidth(),
			(int16_t) bounds.getHeight(),
			color->to16Bit()
		);
	}

	void Screen::clear() {
		_buffer.fillSprite(TFT_BLACK);
	}

	void Screen::flush() {
		_buffer.pushSprite(0, 0);
	}

	void Screen::renderHorizontalLine(const Point &position, uint16_t size, const Color* color) {
		_buffer.drawFastHLine(
			position.getX(),
			position.getY(),
			size,
			color->to16Bit()
		);
	}

	void Screen::renderVerticalLine(const Point &position, uint16_t size, const Color* color) {
		_buffer.drawFastVLine(
			position.getX(),
			position.getY(),
			size,
			color->to16Bit()
		);
	}

	// -------------------------------- Touch --------------------------------

	volatile bool Screen::_touchInterrupted = false;
	
	TouchPoint &Screen::getTouchPoint1() {
		return _touchPoints[0];
	}

	TouchPoint &Screen::getTouchPoint2() {
		return _touchPoints[1];
	}

	void Screen::onTouchInterrupted() {
		_touchInterrupted = true;
	}

	Point Screen::rotateTouchPoint(uint16_t x, uint16_t y) {
		switch (_tft.getRotation()) {
			// 270
			case 1:
				auto tmp = x;
				x = y;
				y = _resolution.getHeight() - tmp;

				break;
		}

		return {x, y};
	}

	Point Screen::readTouchPoint1() {
		return rotateTouchPoint(_touchPanel.read_touch1_x(), _touchPanel.read_touch1_y());
	}

	Point Screen::readTouchPoint2() {
		return rotateTouchPoint(_touchPanel.read_touch2_x(), _touchPanel.read_touch2_y());
	}

	void Screen::onTouchDown(const Point& point) {

	}

	void Screen::onTouchDrag(const Point& point) {

	}

	void Screen::onTouchUp(const Point& point) {

	}

	void Screen::onPinchDown(const Point& point1, const Point& point2) {

	}

	void Screen::onPinchDrag(const Point& point1, const Point& point2) {

	}

	void Screen::onPinchUp(const Point& point1, const Point& point2) {

	}

	void Screen::callOnTouchDown() {
		auto& point = _touchPoints[0].getPosition();

		onTouchDown(point);
		_onTouchDown.call(point);
	}

	void Screen::callOnTouchDrag() {
		auto& point = _touchPoints[0].getPosition();

		onTouchDrag(point);
		_onTouchDrag.call(point);
	}

	void Screen::callOnTouchUp() {
		auto& point = _touchPoints[0].getPosition();

		onTouchUp(point);
		_onTouchUp.call(point);
	}

	void Screen::callOnPinchDown() {
		auto& point1 = _touchPoints[0].getPosition();
		auto& point2 = _touchPoints[1].getPosition();

		onPinchDown(point1, point2);
		_onPinchDown.call(point1, point2);
	}

	void Screen::callOnPinchDrag() {
		auto& point1 = _touchPoints[0].getPosition();
		auto& point2 = _touchPoints[1].getPosition();

		onPinchDrag(point1, point2);
		_onPinchDrag.call(point1, point2);
	}

	void Screen::callOnPinchUp() {
		auto& point1 = _touchPoints[0].getPosition();
		auto& point2 = _touchPoints[1].getPosition();

		onPinchUp(point1, point2);
		_onPinchUp.call(point1, point2);
	}

	void Screen::readTouch() {
		if (!_touchInterrupted)
			return;

		_touchInterrupted = false;

		auto isDown1 = _touchPanel.read_touch1_event() == 2;
		auto isDown2 = _touchPanel.read_touch2_event() == 2;

		if (isDown1) {
			if (isDown2) {
				// Pinch drag
				if (_isPinched) {
					auto point1 = readTouchPoint1();
					auto point2 = readTouchPoint2();

					if (
						point1 != _touchPoints[0].getPosition()
						|| point2 != _touchPoints[1].getPosition()
					) {
						_touchPoints[0].setPosition(point1);
						_touchPoints[1].setPosition(point2);

						callOnPinchDrag();
					}
				}
				// Pinch down
				else {
					_isPinched = true;

					_touchPoints[0].setDown(true);
					_touchPoints[0].setPosition(readTouchPoint1());

					_touchPoints[1].setDown(true);
					_touchPoints[1].setPosition(readTouchPoint2());

					callOnPinchDown();
				}
			}
			else {
				// Pinch up
				if (_isPinched) {
					_isPinched = false;

					_touchPoints[1].setDown(false);
					_touchPoints[1].setPosition(readTouchPoint2());

					callOnPinchUp();
				}
				// Touch drag
				else if (_isTouched) {
					auto point1 = readTouchPoint1();

					if (point1 != _touchPoints[0].getPosition()) {
						_touchPoints[0].setPosition(point1);

						callOnTouchDrag();
					}
				}
				// Touch down
				else {
					_isTouched = true;

					_touchPoints[0].setDown(true);
					_touchPoints[0].setPosition(readTouchPoint1());

					callOnTouchDown();
				}
			}
		}
		else {
			// Pinch up
			if (_isPinched) {
				_isPinched = false;

				_touchPoints[1].setDown(false);
				_touchPoints[1].setPosition(readTouchPoint2());

				callOnPinchUp();
			}

			// Touch up
			if (_isTouched) {
				_isTouched = false;

				_touchPoints[0].setDown(false);
				_touchPoints[0].setPosition(readTouchPoint1());

				callOnTouchUp();
			}
		}
	}

	void Screen::renderLine(const Point &from, const Point &to, const Color *color) {
		_buffer.drawLine(
			from.getX(),
			from.getY(),

			to.getX(),
			to.getY(),

			color->to16Bit()
		);
	}

	void Screen::renderTriangle(const Point &p1, const Point &p2, const Point &p3, const Color *color) {
		_buffer.fillTriangle(
			p1.getX(),
			p1.getY(),

			p2.getX(),
			p2.getY(),

			p3.getX(),
			p3.getY(),

			color->to16Bit()
		);
	}

	void Screen::setPalette(uint16_t* palette) {
		_buffer.createPalette(palette);
	}

	TFT_eSprite& Screen::getBuffer() {
		return _buffer;
	}

	Bounds Screen::getViewport() {
		return {
			_buffer.getViewportX(),
			_buffer.getViewportY(),
			_buffer.getViewportWidth(),
			_buffer.getViewportHeight()
		};
	}

	void Screen::setViewport(const Bounds &bounds) {
		_buffer.setViewport(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), false);
	}

	void Screen::resetViewport() {
		_buffer.resetViewport();
	}
}
