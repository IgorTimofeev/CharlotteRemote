#include "pfdPage.h"

namespace pizdanc {
	PFDPage::PFDPage() {
		_pfd.setMargin(Margin(0, 0, 0, InstrumentsLayout::panelSize));
		*this += &_pfd;

		_instrumentsLayout.setVerticalAlignment(Alignment::end);
		_instrumentsLayout.setHeight(InstrumentsLayout::panelSize);
		*this += &_instrumentsLayout;
	}
}