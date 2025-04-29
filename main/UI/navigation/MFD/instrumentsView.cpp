#include "instrumentsView.h"
#include "../menu/menuRoutes.h"

namespace pizda {
	InstrumentsView::InstrumentsView() : menuButton(OpenMenuButton(&MenuRoutes::MFD)) {
		// Titles
		topPanel.setHeight(InstrumentsView::topPanelHeight);
		topPanel.setVerticalAlignment(Alignment::start);
		topPanel.setFillColor(&Theme::bg2);
		*this += &topPanel;

		buttonAndRowLayout.setOrientation(Orientation::horizontal);

		// Row
		row.setOrientation(Orientation::horizontal);
		row.setHorizontalAlignment(Alignment::center);
		buttonAndRowLayout += &row;

		// Menu button
		menuButton.setVerticalAlignment(Alignment::center);
		menuButton.setMargin(Margin(0, 5, 5, 0));
		buttonAndRowLayout.setAutoSize(&menuButton);
		buttonAndRowLayout += &menuButton;

		*this += &buttonAndRowLayout;
	}
}