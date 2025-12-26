#include "toolbar.h"

#include "UI/theme.h"

namespace pizda {
	Toolbar::Toolbar() {
		// Top panel
		topPanel.setHeight(topPanelHeight);
		topPanel.setVerticalAlignment(Alignment::start);
		topPanel.setFillColor(&Theme::bg2);
		*this += &topPanel;

		// Row
		row.setOrientation(Orientation::horizontal);
		row.setHorizontalAlignment(Alignment::center);
		*this += &row;
	}
}