#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/elements/dialogs/dialog.h"

namespace pizda {
	using namespace YOBA;

	class ScrollViewDialog : public Dialog {
		public:
			ScrollViewDialog();

			ScrollView scrollView {};
			StackLayout rows {};
			TextView title {};
	};
}