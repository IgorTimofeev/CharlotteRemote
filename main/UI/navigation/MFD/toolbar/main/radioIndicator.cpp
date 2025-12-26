#include "radioIndicator.h"

#include <format>

#include "rc.h"
#include "UI/theme.h"

namespace pizda {
	RadioIndicator::RadioIndicator() {
		setSize(Size(
			_lineCount * (_lineThickness + _lineSpacing) - _lineSpacing + _textOffset + _textMaxWidth,
			_lineHeightMin + (_lineCount - 1) * _lineHeightIncrement
		));
	}

	void RadioIndicator::onRender(Renderer* renderer, const Bounds& bounds) {
		const auto rssi = static_cast<int>(RC::getInstance().getAircraftData().computed.transceiverRSSIDBm);

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
		uint16_t lineHeight = _lineHeightMin;

		auto position = Point(
			bounds.getX(),
			bounds.getY() + _lineHeightMin + (_lineCount - 1) * _lineHeightIncrement - 1
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

		// RSSI
		position.setX(position.getX() - _lineSpacing + _textOffset);
		position.setY(bounds.getYCenter() - Theme::fontSmall.getHeight());
		
		renderer->renderString(
			position,
			&Theme::fontSmall,
			&Theme::fg4,
			std::format(L"R {}", rssi)
		);
		
		position.setY(position.getY() + Theme::fontSmall.getHeight());

		// SNR
		renderer->renderString(
			position,
			&Theme::fontSmall,
			&Theme::fg4,
			std::format(L"S {}", 100)
		);
	}
}