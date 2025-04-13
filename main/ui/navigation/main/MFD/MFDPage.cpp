#include "MFDPage.h"

#include "../../../theme.h"
#include "../../../../rc.h"

namespace pizda {
	MFDPageEbanina::MFDPageEbanina(MFDModeButton* button, Route* route, bool autoSize) : button(button), route(route), autoSize(autoSize) {

	}

	MFDPage::MFDPage() {
		// Controls
		for (auto& ebanina : _ebaninas) {
			ebanina.button->gotEvent += [this, &ebanina](Event* event) {
				if (event->getTypeID() != TouchDownEvent::typeID)
					return;

				getApplication()->enqueueOnTick([this, &ebanina]() {
					toggleEbanina(&ebanina);
				});

				event->setHandled(true);
			};

			_buttonsRow += ebanina.button;
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

		*this += &_rows;

		// Initialization
		toggleEbanina(&_ebaninas[0]);
	}

	void MFDPage::toggleEbanina(MFDPageEbanina* ebanina) {
		// Re-placing children from scratch
		_rows.removeChildren();

		_rows += &_PFD;

		// Deleting old element
		if (ebanina->button->isSelected()) {
			if (ebanina->element != nullptr) {
				delete ebanina->element;
				ebanina->element = nullptr;
			}
		}
		// Building new element
		else {
			ebanina->element = ebanina->route->buildElement();
		}

		// Adding elements in order
		for (auto& ebanina2 : _ebaninas) {
			if (ebanina2.element != nullptr) {
				if (ebanina2.autoSize)
					_rows.setAutoSize(ebanina2.element);

				_rows.insertChild(1, ebanina2.element);
			}
		}

		// Filling children
		_rows.setAutoSize(&_buttonsRow);
		_rows += &_buttonsRow;

		// Toggling button selection
		ebanina->button->setSelected(!ebanina->button->isSelected());

		invalidate();
	}
}