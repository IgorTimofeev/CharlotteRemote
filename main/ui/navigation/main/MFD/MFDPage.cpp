#include "MFDPage.h"

#include "../../../theme.h"
#include "../../../../rc.h"

namespace pizda {
	MFDPage::MFDPage() {
		_rows += &_PFD;

		// Controls
		for (const auto& pair : _buttonsAndRoutes) {
			pair.first->gotEvent += [this, pair](Event* event) {
				if (event->getTypeID() == TouchDownEvent::typeID)
					selectControls(pair);
			};

			_buttonsRow += pair.first;
		}

		// Menu
		_menuButton.gotEvent += [this](Event* event) {
			if (event->getTypeID() == TouchDownEvent::typeID) {
				_menuButton.setSelected(true);

			}
			else if (event->getTypeID() == TouchUpEvent::typeID) {
				_menuButton.setSelected(false);

				RC::getInstance().setMenuVisibility(true);
			}
		};

		_buttonsRow += &_menuButton;

		// Buttons
		_buttonsRow.setOrientation(Orientation::horizontal);
		_buttonsRow.setHeight(22);
		_rows.setFit(&_buttonsRow, true);
		_rows += &_buttonsRow;

		*this += &_rows;

		// Initialization
		selectControls(_buttonsAndRoutes[0]);
	}

	Element* MFDPage::getSelectedControls() {
		return _rows.getChildrenCount() == 3 ? _rows[1] : nullptr;
	}

	void MFDPage::selectControls(const std::pair<MFDModeButton*, Route*>& pair) {
		auto oldControls = getSelectedControls();

		if (oldControls != nullptr) {
			_rows.removeChildAt(1);
			delete oldControls;
		}

		for (auto& [otherButton, otherBuilder] : _buttonsAndRoutes)
			otherButton->setSelected(otherButton == pair.first);

		auto newControls = pair.second->buildElement();
		_rows.setFit(newControls, true);
		_rows.insertChild(1, newControls);

		invalidate();
	}
}