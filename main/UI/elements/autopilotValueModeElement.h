#pragma once

#include <cstdint>

#include <YOBA/main.h>
#include <YOBA/UI.h>

namespace pizda {
	using namespace YOBA;
	
	enum class AutopilotValueMode : uint8_t {
		none,
		selected,
		acknowledged
	};
	
	class AutopilotValueModeElement : public virtual Element {
		public:
			AutopilotValueMode getMode() const {
				return _mode;
			}
			
			void setMode(AutopilotValueMode mode) {
				if (mode == _mode)
					return;
				
				_mode = mode;
				
				invalidate();
			}
		
		private:
			AutopilotValueMode _mode {};
	};
}
