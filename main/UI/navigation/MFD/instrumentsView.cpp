#include "instrumentsView.h"
#include "../menu/menuRoutes.h"

namespace pizda {
	InstrumentsView::InstrumentsView() : menuButton(OpenMenuButton(&MenuRoutes::MFD)) {
		const uint16_t titleHeight = Theme::fontSmall.getHeight() + titleVerticalOffset * 2;

		// Titles
		titlesRect.setFillColor(&Theme::bg2);
		titlesRect.setHeight(titleHeight);
		titlesRect.setVerticalAlignment(Alignment::start);
		*this += &titlesRect;

		buttonAndRowLayout.setOrientation(Orientation::horizontal);

		// Row
		row.setOrientation(Orientation::horizontal);
		row.setSpacing(10);
		row.setHorizontalAlignment(Alignment::center);
		row.setMargin(Margin(5, titleVerticalOffset, 5, 0));
		buttonAndRowLayout += &row;

		// Menu button
		menuButton.setVerticalAlignment(Alignment::center);
		menuButton.setMargin(Margin(0, 5, 5, 0));
		buttonAndRowLayout.setAutoSize(&menuButton);
		buttonAndRowLayout += &menuButton;

		*this += &buttonAndRowLayout;
	}

	void InstrumentsView::applyTitlerStyle(Titler& titler) {
		titler.setSpacing(titleSpacing);
		titler.getTitle().setFont(&Theme::fontSmall);
	}
}