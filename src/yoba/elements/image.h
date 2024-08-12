#pragma once

#include "element.h"
#include "yoba/screen.h"
#include "yoba/bounds.h"
#include "yoba/image_source.h"

namespace yoba {
	class Image : public Element {
		public:
			void onRender(Screen& display) override {
				if (!getSource() || !getSource()->getPixels())
					return;

				display.renderImage(
					Bounds(
						getBounds().getPosition(),
						getSource()->getSize()
					),
					getSource()->getPixels()
				);
			}

			// -------------------------------- Getters & setters --------------------------------

			const ImageSource* getSource() const {
				return _source;
			}

			void setSource(ImageSource* source) {
				_source = source;

				invalidateRender();
			}

		private:
			ImageSource* _source = nullptr;
	};
}
