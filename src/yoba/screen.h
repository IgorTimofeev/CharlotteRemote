#pragma once

#include <cstdint>
#include "Arduino.h"
#include "TFT_eSPI.h"

#include "size.h"
#include "point.h"
#include "bounds.h"
#include "color.h"
#include "touch_point.h"
#include "elements/workspace.h"
#include "yoba/hardware/touch/ft6336u/FT6336U.h"

namespace yoba {
	class Screen {
		public:
			Screen(
				const Size &resolution,
				const uint8_t &tftLedPin,
				const uint8_t &touchSdaPin,
				const uint8_t &touchSclPin,
				const uint8_t &touchRstPin,
				const uint8_t &touchIntPin
			);

			const Size &getResolution() const;
			void setBrightness(const uint8_t &value) const;
			virtual void begin();
			virtual void tick();

			// -------------------------------- Rendering --------------------------------

			virtual void clear();

			void setDefaultFont(const uint8_t* value);
			void setFont(const uint8_t *value);

			virtual Size measureText(const String &text);

			virtual void renderText(const Point &position, const Color* color, const String &text);
			virtual void renderCircle(const Point& position, int32_t radius, const Color* color);
			virtual void renderRectangle(const Bounds& bounds, const Color* color);
			virtual void renderRectangle(const Bounds& bounds, uint16_t radius, const Color* color);
			virtual void renderHorizontalLine(const Point& position, uint16_t size, const Color* color);
			virtual void renderVerticalLine(const Point& position, uint16_t size, const Color* color);
			virtual void renderImage(const Bounds& bounds, const uint16_t* data);
			virtual void flush();

			// -------------------------------- Touch --------------------------------

			TouchPoint& getTouchPoint1();
			TouchPoint& getTouchPoint2();

			void addOnTouchDown(const std::function<void(const Point&)>& callback) {
				_onTouchDown.add(callback);
			}

			void addOnTouchDrag(const std::function<void(const Point&)>& callback) {
				_onTouchDrag.add(callback);
			}

			void addOnTouchUp(const std::function<void(const Point&)>& callback) {
				_onTouchUp.add(callback);
			}

			void addOnPinchDown(const std::function<void(const Point&, const Point&)>& callback) {
				_onPinchDown.add(callback);
			}

			void addOnPinchDrag(const std::function<void(const Point&, const Point&)>& callback) {
				_onPinchDrag.add(callback);
			}

			void addOnPinchUp(const std::function<void(const Point&, const Point&)>& callback) {
				_onPinchUp.add(callback);
			}

		protected:
			virtual void onTouchDown(const Point& point);
			virtual void onTouchDrag(const Point& point);
			virtual void onTouchUp(const Point& point);
			virtual void onPinchDown(const Point& point1, const Point& point2);
			virtual void onPinchDrag(const Point& point1, const Point& point2);
			virtual void onPinchUp(const Point& point1, const Point& point2);

		private:
			static volatile bool _touchInterrupted;

			const Size _resolution;
			const uint8_t _tftLedPin;
			const uint8_t _touchIntPin;

			TFT_eSPI _tft = TFT_eSPI();
			TFT_eSprite _buffer = TFT_eSprite(&_tft);
			FT6336U _touchPanel;

			const uint8_t* _defaultFont = nullptr;
			const uint8_t* _font = nullptr;

			bool _isTouched = false;
			bool _isPinched = false;

			TouchPoint _touchPoints[2] {
				TouchPoint(),
				TouchPoint()
			};

			static void onTouchInterrupted();

			Action<const Point&> _onTouchDown;
			Action<const Point&> _onTouchDrag;
			Action<const Point&> _onTouchUp;
			Action<const Point&, const Point&> _onPinchDown;
			Action<const Point&, const Point&> _onPinchDrag;
			Action<const Point&, const Point&> _onPinchUp;

			Point rotateTouchPoint(uint16_t x, uint16_t y);
			Point readTouchPoint1();
			Point readTouchPoint2();
			void callOnTouchDown();
			void callOnTouchDrag();
			void callOnTouchUp();
			void callOnPinchDown();
			void callOnPinchDrag();
			void callOnPinchUp();
			void readTouch();
	};
}
