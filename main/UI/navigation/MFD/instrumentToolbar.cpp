#include "instrumentToolbar.h"

namespace pizda {
	InstrumentToolbar::InstrumentToolbar() {
		// Titles
		topPanel.setHeight(topPanelHeight);
		topPanel.setVerticalAlignment(Alignment::start);
		topPanel.setFillColor(&Theme::bg2);
		*this += &topPanel;

		// Row
		row.setOrientation(Orientation::horizontal);
		row.setHorizontalAlignment(Alignment::center);
		*this += &row;

		// Menu button
		menuButton.setWidth(11);
		menuButton.setMargin(Margin(0, topPanelHeight, 0, 0));
		menuButton.setHorizontalAlignment(Alignment::end);

		menuButton.setDefaultBackgroundColor(&Theme::bg3);
		menuButton.setDefaultTextColor(&Theme::fg5);

		menuButton.setPressedBackgroundColor(&Theme::fg1);
		menuButton.setPressedTextColor(&Theme::bg1);

		menuButton.setFont(&Theme::fontSmall);
		menuButton.setText(L"^");

		*this += &menuButton;
	}
}