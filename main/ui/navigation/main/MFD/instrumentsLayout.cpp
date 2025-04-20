#include "instrumentsLayout.h"

namespace pizda {
	InstrumentsLayout::InstrumentsLayout() {
		const uint16_t titleHeight = Theme::fontSmall.getHeight() + titleVerticalOffset * 2;

		// Titles
		_titlesRect.setFillColor(&Theme::bg2);
		_titlesRect.setHeight(titleHeight);
		_titlesRect.setVerticalAlignment(Alignment::start);
		*this += &_titlesRect;

		// Row
		_row.setOrientation(Orientation::horizontal);
		_row.setSpacing(15);
		_row.setHorizontalAlignment(Alignment::center);
		_row.setMargin(Margin(5, titleVerticalOffset, 5, 0));
		*this += &_row;

		// Menu button
		_openMenuButton.setAlignment(Alignment::start, Alignment::center);
		_openMenuButton.setMargin(Margin(8, 5, 0, 0));
		*this += &_openMenuButton;
	}

	void InstrumentsLayout::applyTitlerStyle(Titler& titler) {
		titler.setSpacing(titleSpacing);
		titler.getTitle().setFont(&Theme::fontSmall);
	}
}