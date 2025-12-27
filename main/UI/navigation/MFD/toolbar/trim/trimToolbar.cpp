#include "trimToolbar.h"
#include "rc.h"

namespace pizda {
	using namespace YOBA;

	TrimToolbar::TrimToolbar() {
		setHeight(36);
		
		auto& settings = RC::getInstance().getSettings();
		
		button.setMargin(Margin(8, 0, 0, 0));
		button.setVerticalAlignment(Alignment::center);
		button.setToggle(true);
		button.setActive(false);
		
		row += &button;
	}
}