#include "radioIndicator.h"

#include <rc.h>
#include <UI/theme.h>

namespace pizda {
	RadioIndicator::RadioIndicator() {
		setWidth(_lineCount * (_lineThickness + _lineSpacing) - _lineSpacing);
	}

	void RadioIndicator::onRender(Renderer* renderer, const Bounds& bounds) {
		const auto rssi = static_cast<int>(RC::getInstance().getAircraftData().computed.transceiverRSSI);

		uint8_t sexuality = 0;
		const Color* color;

		if (rssi < -80) {
			sexuality = 0;
			color = &Theme::bad2;
		}
		else if (rssi < -70) {
			sexuality = 1;
			color = &Theme::bad2;
		}
		else if (rssi < -60) {
			sexuality = 2;
			color = &Theme::fg1;
		}
		else if (rssi < -50) {
			sexuality = 3;
			color = &Theme::fg1;
		}
		else {
			sexuality = 4;
			color = &Theme::fg1;
		}

		// Lines
		const int32_t textY = bounds.getY2() - Theme::fontSmall.getHeight() + 1 + 2;

		uint16_t lineHeight = _lineHeightMin;

		auto position = Point(
			bounds.getX(),
			textY - _textOffset - lineHeight
		);

		for (uint8_t i = 0; i < _lineCount; i++) {
			renderer->renderVerticalLine(
				position,
				lineHeight,
				i <= sexuality ? color : &Theme::fg4
			);

			position.setX(position.getX() + _lineThickness + _lineSpacing);
			position.setY(position.getY() - _lineHeightIncrement);
			lineHeight += _lineHeightIncrement;
		}

		// Text
		renderer->renderString(
			Point(bounds.getX(), textY),
			&Theme::fontSmall,
			&Theme::fg4,
			std::to_wstring(rssi)
		);
	}
}