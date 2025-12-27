#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "UI/theme.h"

#include "UI/navigation/MFD/toolbar/toolbar.h"
#include "UI/navigation/MFD/toolbar/toolbarSection.h"
#include "UI/navigation/MFD/toolbar/korryButton.h"
#include "UI/navigation/MFD/toolbar/rotaryControl.h"

namespace pizda {
	using namespace YOBA;

	class LogToolbar : public Toolbar {
		public:
			explicit LogToolbar();
			
			KorryButton button { L"TST" };
		
		private:

	};
}