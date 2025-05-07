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
		menuButton.setSize(Size(12, Size::computed));
		menuButton.setCornerRadius(0);

		menuButton.setPressedBackgroundColor(&Theme::fg1);
		menuButton.setPressedTextColor(&Theme::bg1);

		buttonAndRowLayout.setAutoSize(&menuButton);
		buttonAndRowLayout += &menuButton;

		*this += &buttonAndRowLayout;
	}
}