#include "throttleIndicator.h"

#include <format>
#include "../../../../theme.h"
#include "../../../../../rc.h"

namespace pizda {
	void ThrottleIndicator::onRender(Renderer* renderer) {
		const auto& bounds = getBounds();

		constexpr static uint8_t textOffset = 3;
		constexpr static uint8_t lineOffsetX = 2;

		const uint16_t frameHeight = bounds.getHeight() - Theme::fontSmall.getHeight() - textOffset + 3;
		const auto frameY2 = bounds.getY() + frameHeight - 1;
		const auto& settings = RC::getInstance().getSettings();

		// Frame
		renderer->renderRectangle(
			Bounds(
				bounds.getX(),
				bounds.getY(),
				bounds.getWidth(),
				frameHeight
			),
			&Theme::bg6
		);

		const auto& renderOffsetLine = [&renderer, &bounds](int32_t y, const Color* color) {
			renderer->renderHorizontalLine(
				Point(
					bounds.getX() - lineOffsetX,
					y
				),
				bounds.getWidth() + lineOffsetX * 2,
				color
			);
		};

		// Secondary value
		const auto secondaryValue = settings.autopilot.autoThrottle ? _remoteValue : _aircraftValue;

		renderOffsetLine(
			frameY2 - std::max(static_cast<uint16_t>(frameHeight * secondaryValue / 0xFF), static_cast<uint16_t>(1)) + 1,
			&Theme::fg5
		);

		// Primary value
		const auto primaryValue = settings.autopilot.autoThrottle ? _aircraftValue : _remoteValue;
		const auto primaryValueHeight = std::max(static_cast<uint16_t>(frameHeight * primaryValue / 0xFF), static_cast<uint16_t>(1));
		const int32_t primaryValueY = frameY2 - primaryValueHeight + 1;
		const auto primaryValueColor = settings.autopilot.autoThrottle ? &Theme::purple : &Theme::green;

		if (primaryValueHeight > 1) {
			renderer->renderFilledRectangle(
				Bounds(
					bounds.getX(),
					primaryValueY + 1,
					bounds.getWidth(),
					primaryValueHeight - 1
				),
				&Theme::fg1
			);
		}

		renderOffsetLine(primaryValueY, primaryValueColor);

		// Aircraft value text
		const auto text = std::to_wstring(static_cast<int32_t>(primaryValue * 100 / 0xFF));

		renderer->renderString(
			Point(
				bounds.getX() + bounds.getWidth() / 2 - Theme::fontSmall.getWidth(text) / 2 + 1,
				bounds.getY() + frameHeight + textOffset
			),
			&Theme::fontSmall,
			primaryValueColor,
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