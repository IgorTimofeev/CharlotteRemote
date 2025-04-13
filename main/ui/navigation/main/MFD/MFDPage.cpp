#include "MFDPage.h"

#include "../../../theme.h"
#include "../../../../rc.h"

namespace pizda {
	MFDPageEbanina::MFDPageEbanina(MFDModeButton* button, Route* route, bool autoSize) : button(button), route(route), autoSize(autoSize) {

	}

	MFDPage::MFDPage() {
		_rows += &_PFD;

		// Controls
		for (auto& ebanina : _ebaninas) {
			ebanina.button->gotEvent += [this, &ebanina](Event* event) {
				if (event->getTypeID() != TouchDownEvent::typeID)
					return;

				getApplication()->enqueueOnTick([this, &ebanina]() {
					setEbanina(&ebanina);
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
		_rows.setAutoSize(&_buttonsRow);
		_rows += &_buttonsRow;

		*this += &_rows;

		// Initialization
		setEbanina(&_ebaninas[0]);
	}

	void MFDPage::setEbanina(MFDPageEbanina* ebanina) {
		if (ebanina == _selectedEbanina)
			return;

		constexpr static const uint8_t ebaninaIndex = 1;

		if (_selectedEbanina != nullptr) {
			auto selectedElement = _rows[ebaninaIndex];

			// Deleting old element
			if (selectedElement != nullptr) {
				_rows.removeChildAt(ebaninaIndex);
				delete selectedElement;
			}
		}

		// Building new element
		auto newElement = ebanina->route->buildElement();

		if (ebanina->autoSize)
			_rows.setAutoSize(newElement);

		_rows.insertChild(ebaninaIndex, newElement);

		// Updating buttons selection
		for (auto& ebanina2 : _ebaninas) {
			ebanina2.button->setSelected(&ebanina2 == ebanina);
		}

		_selectedEbanina = ebanina;

		invalidate();
	}
}