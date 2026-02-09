#include "comboBox.h"

#include "UI/theme.h"

namespace pizda {
	ComboBoxDialogItem::ComboBoxDialogItem(ComboBoxDialog* comboBoxDialog, const uint8_t index) : _comboBoxDialog(comboBoxDialog), _index(index) {
		Theme::applySecondary(this);

		setToggle(true);
		setActiveBorderColor(&Theme::magenta1);
		setText(_comboBoxDialog->getComboBox()->getItems()[index]);
	}

	void ComboBoxDialogItem::onClick() {
		Button::onClick();

		Application::getCurrent()->invokeOnNextTick([this] {
			_comboBoxDialog->getComboBox()->setSelectedIndex(_index);
			_comboBoxDialog->hide();
			delete _comboBoxDialog;
		});
	}

	ComboBoxDialog::ComboBoxDialog(ComboBox* comboBox) : _comboBox(comboBox) {
		title.setText(_comboBox->getDialogTitle());

		_itemRows.setGap(Theme::gap);
		rows += &_itemRows;

		for (uint16_t i = 0; i < _comboBox->getItemCount(); ++i) {
			const auto item = new ComboBoxDialogItem(this, i);
			item->setActive(i == _comboBox->getSelectedIndex());

			_itemRows += item;
		}
	}

	ComboBoxDialog::~ComboBoxDialog() {
		for (const auto child : _itemRows)
			delete child;
	}

	ComboBox* ComboBoxDialog::getComboBox() const {
		return _comboBox;
	}

	void ComboBoxDialog::onAddedToParent(Layout* parent) {
		ScrollViewDialog::onAddedToParent(parent);

		_itemRows[_comboBox->getSelectedIndex()]->scrollIntoView();
	}

	ComboBox::ComboBox() {
		setHeight(Theme::elementHeight);
	}

	std::wstring_view* ComboBox::getItems() const {
		return _items;
	}

	void ComboBox::setItems(std::wstring_view* const items) {
		_items = items;
	}

	uint8_t ComboBox::getSelectedIndex() const {
		return _selectedIndex;
	}

	void ComboBox::setSelectedIndex(const uint8_t value) {
		_selectedIndex = value;
	}

	uint8_t ComboBox::getItemCount() const {
		return _itemCount;
	}

	void ComboBox::setItemCount(const uint8_t itemCount) {
		_itemCount = itemCount;
	}

	const std::wstring& ComboBox::getDialogTitle() {
		return _dialogTitle;
	}

	void ComboBox::setDialogTitle(const std::wstring_view& title) {
		_dialogTitle = title;
	}

	void ComboBox::onClick() {
		Referencer::onClick();

		(new ComboBoxDialog(this))->show();
	}

	void ComboBox::onRender(Renderer* renderer, const Bounds& bounds) {
		Referencer::onRender(renderer, bounds);

		if (!_items || _itemCount == 0)
			return;

		renderer->renderString(
			Point(
				bounds.getX() + 10,
				bounds.getYCenter() - Theme::fontNormal.getHeight() / 2
			),
			&Theme::fontNormal,
			&Theme::fg1,
			_items[_selectedIndex]
		);
	}
}
