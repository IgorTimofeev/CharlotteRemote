#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/elements/titler.h>
#include <UI/elements/dialogs/dialog.h>

#include <types/navigationData.h>

namespace pizda {
	using namespace YOBA;

	class SelectWaypointDialog : public Dialog {
		public:
			SelectWaypointDialog(
				std::wstring_view titleText,
				const std::function<bool(const NavigationWaypointData&)>& filter,
				const std::function<void(uint16_t)>& onConfirm
			);

		private:
			TextField _name {};
			Titler _nameTitle { L"Name", &_name };

			Button _confirmButton {};
	};
}
