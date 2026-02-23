#pragma once

#include <functional>

#include <YOBA/UI.h>
#include <YOBA/main.h>

#include "UI/elements/dialogs/scrollViewDialog.h"
#include "UI/elements/referencer.h"

namespace pizda {
	using namespace YOBA;

	class SelectorDialog;

	class SelectorDialogItem : public Button {
		public:
			SelectorDialogItem(SelectorDialog* comboBoxDialog, const uint8_t index);

		protected:
			void onClick() override;

		private:
			SelectorDialog* _comboBoxDialog;
			uint8_t _index;
	};

	class SelectorDialog : public ScrollViewDialog {
		public:
			SelectorDialog(
				const std::wstring_view& title,
				const std::wstring_view* items,
				uint8_t itemCount,
				uint8_t selectedIndex,
				const std::function<void(uint8_t)>& onItemSelected
			);

			~SelectorDialog() override;

			const std::wstring_view* getItems() const;
			const std::function<void(uint8_t)>& getOnItemSelected();

		protected:
			void onAddedToParent(Layout* parent) override;

		private:
			const std::wstring_view* _items;
			uint8_t _itemCount;
			uint8_t _selectedIndex;
			std::function<void(uint8_t)> _onItemSelected;

			RelativeStackLayout _itemRows {};
	};
}