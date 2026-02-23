#include "comboBox.h"

#include "UI/theme.h"
#include "UI/elements/dialogs/selectorDialog.h"

namespace pizda {
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

		(
			new SelectorDialog(
				getDialogTitle(),
				_items,
				_itemCount,
				_selectedIndex,
				[this](const uint8_t index) {
					setSelectedIndex(index);
				}
			)
		)->show();
	}

	void ComboBox::onRender(Renderer& renderer, const Bounds& bounds) {
		Referencer::onRender(renderer, bounds);

		if (!_items || _itemCount == 0)
			return;

		renderer.renderText(
			Point(
				bounds.getX() + 10,
				bounds.getYCenter() - Theme::fontNormal.getHeight() / 2
			),
			Theme::fontNormal,
			&Theme::fg1,
			_items[_selectedIndex]
		);
	}
}
