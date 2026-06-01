#pragma once

#include <functional>

#include <YOBA/UI.h>
#include <YOBA/main.h>

#include "UI/elements/referencer.h"

namespace pizda {
	using namespace YOBA;

	class ComboBox : public Referencer {
		public:
			ComboBox();

			std::string_view* getItems() const;
			void setItems(std::string_view* const items);

			uint8_t getItemCount() const;
			void setItemCount(const uint8_t itemCount);

			uint8_t getSelectedIndex() const;
			void setSelectedIndex(const uint8_t value);

			const std::string& getDialogTitle();
			void setDialogTitle(const std::string_view& title);

		protected:
			void onClick() override;

			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			std::string_view* _items = nullptr;
			uint8_t _itemCount = 0;
			uint8_t _selectedIndex = 0;
			std::string _dialogTitle;
	};
}