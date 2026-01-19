#include "throttleIndicator.h"

#include <format>

#include "UI/theme.h"
#include "rc.h"

namespace pizda {
	ThrottleIndicator::ThrottleIndicator() {
		setSize(Size(42, 5));
	}
	
	void ThrottleIndicator::onRender(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();
		const auto isConnected = rc.getCommunicationManager().isConnected();
		
		constexpr static uint8_t textMaxSize = 11;
		constexpr static uint8_t textOffset = 4;
		
		constexpr static uint8_t lineOffset = 1;
		
		const uint16_t frameSize = bounds.getWidth() - textMaxSize - textOffset;
		
		// Frame
		renderer->renderRectangle(
			Bounds(
				bounds.getX(),
				bounds.getY(),
				frameSize,
				bounds.getHeight()
			),
			isConnected ? &Theme::bg6 : &Theme::bad3
		);

		const auto& renderLine = [&renderer, &bounds](const int32_t pos, const Color* color) {
			renderer->renderVerticalLine(
				Point(
					pos,
					bounds.getY() - lineOffset
				),
				bounds.getHeight() + lineOffset * 2,
				color
			);
		};

		// Remote value
		renderLine(
			bounds.getX() + std::max(static_cast<uint16_t>(frameSize * _remoteValue / 0xFF), static_cast<uint16_t>(1)) - 1,
			isConnected ? &Theme::green1 : &Theme::bad1
		);

		// Aircraft value
		if (isConnected) {
			const auto aircraftValueSize = std::max(static_cast<uint16_t>(frameSize * _aircraftValue / 0xFF), static_cast<uint16_t>(1));
			
			if (aircraftValueSize > 2) {
				renderer->renderFilledRectangle(
					Bounds(
						bounds.getX(),
						bounds.getY(),
						aircraftValueSize - 1,
						bounds.getHeight()
					),
					&Theme::fg1
				);
			}
			
			renderLine(bounds.getX() + aircraftValueSize - 1, &Theme::magenta);
		}

		// Text
		const auto text =
			isConnected
			? std::format(L"{:03}", static_cast<int32_t>(_aircraftValue * 100 / 0xFF))
			: L"---";

		renderer->renderString(
			Point(
				bounds.getX() + frameSize + textOffset,
				bounds.getYCenter() - Theme::fontSmall.getHeight() / 2 + 1
			),
			&Theme::fontSmall,
			isConnected ? &Theme::fg2 : &Theme::bad1,
			text
		);
	}

	uint8_t ThrottleIndicator::getRemoteValue() const {
		return _remoteValue;
	}

	void ThrottleIndicator::setRemoteValue(uint8_t value) {
		_remoteValue = value;

		invalidate();
	}

	uint8_t ThrottleIndicator::getAircraftValue() const {
		return _aircraftValue;
	}

	void ThrottleIndicator::setAircraftValue(uint8_t value) {
		_aircraftValue = value;

		invalidate();
	}
}