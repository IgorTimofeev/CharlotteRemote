#include "instrumentToolbar.h"

namespace pizda {
	InstrumentToolbar::InstrumentToolbar() {
		// Titles
		topPanel.setHeight(InstrumentToolbar::topPanelHeight);
		topPanel.setVerticalAlignment(Alignment::start);
		topPanel.setFillColor(&Theme::bg2);
		*this += &topPanel;

		buttonAndRowLayout.setOrientation(Orientation::horizontal);

		// Row
		row.setOrientation(Orientation::horizontal);
		row.setHorizontalAlignment(Alignment::center);
		buttonAndRowLayout += &row;

		// Menu button
		menuButton.setWidth(13);

		menuButton.setDefaultBackgroundColor(&Theme::bg3);
		menuButton.setDefaultTextColor(&Theme::fg5);

		menuButton.setPressedBackgroundColor(&Theme::fg1);
		menuButton.setPressedTextColor(&Theme::bg1);

		menuButton.setFont(&Theme::fontSmall);
		menuButton.setText(L"^");

		buttonAndRowLayout.setAutoSize(&menuButton);
		buttonAndRowLayout += &menuButton;

		*this += &buttonAndRowLayout;
	}
}