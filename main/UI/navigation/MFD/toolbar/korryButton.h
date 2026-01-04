#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "UI/theme.h"

#include "UI/navigation/MFD/toolbar/toolbarSection.h"

namespace pizda {
	using namespace YOBA;
	
	class KorryButton : public ToolbarSection {
		public:
			KorryButton(std::wstring_view title, const Image* image) : ToolbarSection(title) {
				_imageView.setMargin(Margin(Toolbar::contentHorizontalMargin, 2, Toolbar::contentHorizontalMargin, 0));
				_imageView.setAlignment(Alignment::center);
				_imageView.setImage(image);
				*this += &_imageView;
			}
			
		private:
			ImageView _imageView {};
	};
}