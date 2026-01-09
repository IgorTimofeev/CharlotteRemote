#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "UI/theme.h"

#include "UI/navigation/MFD/toolbar/toolbar.h"
#include "UI/navigation/MFD/toolbar/trim/trimControls.h"

namespace pizda {
	using namespace YOBA;

	class TrimToolbar : public RowToolbar {
		public:
			TrimToolbar();
			
		private:
			TrimControl _ailerons;
			TrimControl _elevator;
			TrimControl _rudder;
	};
}