#pragma once

#include "yoba/application.h"

using namespace yoba;

namespace ui {
	class RCApplication : public Application {
		public:
			static RCApplication& getInstance();

			void onTick() override;
			void onRender() override;

			void begin() override;

		private:
			RCApplication();
	};
}