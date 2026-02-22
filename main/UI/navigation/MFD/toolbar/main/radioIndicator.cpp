#include "radioIndicator.h"

#include <format>

#include "rc.h"
#include "UI/theme.h"

namespace pizda {
	RadioIndicator::RadioIndicator() {
		setSize(Size(
			_lineCount * (1 + _lineSpacing) - _lineSpacing + _textOffset + _textMaxWidth,
			_lineHeightMin + (_lineCount - 1) * _lineHeightIncrement
		));
	}

	void RadioIndicator::onRender(Renderer& renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();
		const auto isConnected = rc.getTransceiver().isConnected();

		const auto RSSI = rc.getTransceiver().getRSSI();
		const auto SNR = rc.getTransceiver().getSNR();

		uint8_t sexuality;
		const Color* color;
		
		// From worst to best
		if (isConnected) {
			if (RSSI < -80) {
				sexuality = 0;
				color = &Theme::bad1;
			}
			else if (RSSI < -70) {
				sexuality = 1;
				color = &Theme::bad1;
			}
			else if (RSSI < -60) {
				sexuality = 2;
				color = &Theme::fg1;
			}
			else if (RSSI < -50) {
				sexuality = 3;
				color = &Theme::fg1;
			}
			else {
				sexuality = 4;
				color = &Theme::fg1;
			}
		}
		else {
			sexuality = _lineCount - 1;
			color = &Theme::bad3;
		}

		// Lines
		uint16_t lineHeight = _lineHeightMin;

		auto position = Point(
			bounds.getX(),
			bounds.getY2() - _lineHeightMin + 1
		);

		for (uint8_t i = 0; i < _lineCount; i++) {
			renderer.renderVerticalLine(
				position,
				lineHeight,
				i <= sexuality ? *color : Theme::fg4
			);

			position.setX(position.getX() + 1 + _lineSpacing);
			position.setY(position.getY() - _lineHeightIncrement);
			lineHeight += _lineHeightIncrement;
		}

		// RSSI
		position.setX(position.getX() - 1 - _lineSpacing + _textOffset + 1);
		position.setY(bounds.getYCenter() - Theme::fontSmall.getHeight() + 1);
		
		renderer.renderText(
			position,
			Theme::fontSmall,
			isConnected ? Theme::fg4 : Theme::bad1,
			isConnected ? std::format(L"R {}", RSSI) : L"----"
		);
		
		position.setY(position.getY() + Theme::fontSmall.getHeight());

		// SNR
		renderer.renderText(
			position,
			Theme::fontSmall,
			isConnected ? Theme::fg4 : Theme::bad1,
			isConnected ? std::format(L"S {}", SNR) : L"----"
		);
	}
}