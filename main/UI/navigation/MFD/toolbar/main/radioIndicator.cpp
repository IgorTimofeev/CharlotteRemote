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

	void RadioIndicator::onRender(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();
		auto& transceiver = rc.getTransceiver();
		auto& packetHandler = rc.getPacketHandler();
		
		const auto rssi = static_cast<int32_t>(transceiver.getRSSI());
		const auto snr = static_cast<int32_t>(transceiver.getSNR());

		uint8_t sexuality;
		const Color* color;
		
		// From worst to best
		if (packetHandler.isConnected()) {
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
		}
		else {
			sexuality = _lineCount - 1;
			color = &Theme::bad2;
		}

		// Lines
		uint16_t lineHeight = _lineHeightMin;

		auto position = Point(
			bounds.getX(),
			bounds.getY2() - _lineHeightMin + 1
		);

		for (uint8_t i = 0; i < _lineCount; i++) {
			renderer->renderVerticalLine(
				position,
				lineHeight,
				i <= sexuality ? color : &Theme::fg4
			);

			position.setX(position.getX() + 1 + _lineSpacing);
			position.setY(position.getY() - _lineHeightIncrement);
			lineHeight += _lineHeightIncrement;
		}

		// RSSI
		position.setX(position.getX() - 1 - _lineSpacing + _textOffset + 1);
		position.setY(bounds.getYCenter() - Theme::fontSmall.getHeight() + 1);
		
		renderer->renderString(
			position,
			&Theme::fontSmall,
			packetHandler.isConnected() ? &Theme::fg4 : &Theme::bad1,
			packetHandler.isConnected() ? std::format(L"R {}", rssi) : L"N/A"
		);
		
		position.setY(position.getY() + Theme::fontSmall.getHeight());

		// SNR
		renderer->renderString(
			position,
			&Theme::fontSmall,
			packetHandler.isConnected() ? &Theme::fg4 : &Theme::bad1,
			packetHandler.isConnected() ? std::format(L"S {}", snr) : L"N/A"
		);
	}
}