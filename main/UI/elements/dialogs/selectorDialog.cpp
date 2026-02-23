#include "selectorDialog.h"

#include "UI/theme.h"

namespace pizda {
	SelectorDialogItem::SelectorDialogItem(SelectorDialog* comboBoxDialog, const uint8_t index) : _comboBoxDialog(comboBoxDialog), _index(index) {
		Theme::applySecondary(this);

		setToggle(true);
		setActiveBorderColor(&Theme::accent1);
		setText(_comboBoxDialog->getItems()[index]);
	}

	void SelectorDialogItem::onClick() {
		Button::onClick();

		Application::getCurrent()->invokeOnNextTick([this] {
			_comboBoxDialog->getOnItemSelected()(_index);
			_comboBoxDialog->hide();

			delete _comboBoxDialog;
		});
	}

	SelectorDialog::SelectorDialog(
		const std::wstring_view& title,
		const std::wstring_view* items,
		const uint8_t itemCount,
		const uint8_t selectedIndex,
		const std::function<void(uint8_t)>& onItemSelected
	) :
		_items(items),
		_itemCount(itemCount),
		_selectedIndex(selectedIndex),
		_onItemSelected(onItemSelected)
	{
		this->title.setText(title);

		_itemRows.setGap(Theme::verticalGap);
		rows += &_itemRows;

		for (uint16_t i = 0; i < _itemCount; ++i) {
			const auto item = new SelectorDialogItem(this, i);
			item->setActive(i == _selectedIndex);

			_itemRows += item;
		}
	}

	SelectorDialog::~SelectorDialog() {
		for (const auto child : _itemRows)
			delete child;
	}

	const std::wstring_view* SelectorDialog::getItems() const {
		return _items;
	}

	const std::function<void(uint8_t)>& SelectorDialog::getOnItemSelected() {
		return _onItemSelected;
	}

	void SelectorDialog::onAddedToParent(Layout* parent) {
		ScrollViewDialog::onAddedToParent(parent);

		_itemRows[_selectedIndex]->scrollIntoView();
	}
}
