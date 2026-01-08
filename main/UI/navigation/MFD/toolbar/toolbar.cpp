#include "toolbar.h"

#include "UI/theme.h"

namespace pizda {
	Toolbar::Toolbar() {
		setHeight(36);
		
		// Top panel
		topPanel.setHeight(topPanelHeight);
		topPanel.setVerticalAlignment(Alignment::start);
		topPanel.setFillColor(&Theme::bg2);
		*this += &topPanel;
	}
	
	RowToolbar::RowToolbar() {
		// Row
		row.setOrientation(Orientation::horizontal);
		row.setSpacing(1);
		row.setHorizontalAlignment(Alignment::center);
		*this += &row;
	}
}