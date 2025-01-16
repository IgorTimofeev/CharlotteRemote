#include "pfdPage.h"

namespace pizdanc {
	PFDPage::PFDPage() {
		_pfd.setMargin(Margin(0, 0, 54, 0));
		*this += &_pfd;

		_instrumentsLayout.setHorizontalAlignment(Alignment::end);
		_instrumentsLayout.setWidth(_pfd.getMargin().getRight());
		*this += &_instrumentsLayout;
	}
}