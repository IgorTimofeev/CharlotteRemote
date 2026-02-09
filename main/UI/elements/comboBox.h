#pragma once

#include <functional>

#include <YOBA/UI.h>
#include <YOBA/main.h>

#include "UI/elements/dialogs/scrollViewDialog.h"
#include "UI/elements/referencer.h"

namespace pizda {
	using namespace YOBA;

	class ComboBox;
	class ComboBoxDialog;

	class ComboBoxDialogItem : public Button {
		public:
			ComboBoxDialogItem(ComboBoxDialog* comboBoxDialog, const uint8_t index);

		protected:
			void onClick() override;

		private:
			ComboBoxDialog* _comboBoxDialog;
			uint8_t _index;
	};

	class ComboBoxDialog : public ScrollViewDialog {
		public:
			ComboBoxDialog(ComboBox* comboBox);
			~ComboBoxDialog() override;

			ComboBox* getComboBox() const;

		protected:
			void onAddedToParent(Layout* parent) override;

		private:
			ComboBox* _comboBox;
			RelativeStackLayout _itemRows {};
	};

	class ComboBox : public Referencer {
		public:
			ComboBox();

			std::wstring_view* getItems() const;
			void setItems(std::wstring_view* const items);

			uint8_t getItemCount() const;
			void setItemCount(const uint8_t itemCount);

			uint8_t getSelectedIndex() const;
			void setSelectedIndex(const uint8_t value);

			const std::wstring& getDialogTitle();
			void setDialogTitle(const std::wstring_view& title);

		protected:
			void onClick() override;

			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			std::wstring_view* _items = nullptr;
			uint8_t _itemCount = 0;
			uint8_t _selectedIndex = 0;
			std::wstring _dialogTitle;
	};
}