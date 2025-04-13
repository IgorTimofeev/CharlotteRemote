#include "MFDPage.h"

#include "../../../theme.h"
#include "../../../../rc.h"

namespace pizda {
	MFDPage::MFDPage() {
		_rows += &_PFD;

		// Controls
		for (const auto& pair : _buttonsAndRoutes) {
			pair.first->gotEvent += [this, &pair](Event* event) {
				if (event->getTypeID() != TouchDownEvent::typeID)
					return;

				getApplication()->enqueueOnTick([this, &pair]() {
					selectControls(pair.first->isSelected() ? nullptr : &pair);
				});

				event->setHandled(true);
			};

			_buttonsRow += pair.first;
		}

		// Menu
		_menuButton.gotEvent += [this](Event* event) {
			if (event->getTypeID() == TouchDownEvent::typeID) {
				_menuButton.setCaptured(true);
				_menuButton.setSelected(true);

				event->setHandled(true);
			}
			else if (event->getTypeID() == TouchUpEvent::typeID) {
				_menuButton.setCaptured(false);
				_menuButton.setSelected(false);

				RC::getInstance().setMenuVisibility(true);

				event->setHandled(true);
			}
		};

		_buttonsRow += &_menuButton;

		// Buttons
		_buttonsRow.setOrientation(Orientation::horizontal);
		_buttonsRow.setHeight(19);
		_rows.setAutoSize(&_buttonsRow);
		_rows += &_buttonsRow;

		*this += &_rows;

		// Initialization
		selectControls(&_buttonsAndRoutes[0]);
	}

	Element* MFDPage::getSelectedControls() {
		return _rows.getChildrenCount() == 3 ? _rows[1] : nullptr;
	}

	void MFDPage::selectControls(const std::pair<MFDModeButton*, Route*>* buttonAndRoute) {
		auto oldControls = getSelectedControls();

		// Removing old
		if (oldControls != nullptr) {
			_rows.removeChildAt(1);
			delete oldControls;
		}

		// Updating buttons selection
		for (auto& otherButtonAndRoute : _buttonsAndRoutes)
			otherButtonAndRoute.first->setSelected(buttonAndRoute != nullptr && otherButtonAndRoute.first == buttonAndRoute->first);

		// Building new control
		if (buttonAndRoute != nullptr) {
			auto newControls = buttonAndRoute->second->buildElement();
			_rows.setAutoSize(newControls);
			_rows.insertChild(1, newControls);
		}

		invalidate();
	}
}