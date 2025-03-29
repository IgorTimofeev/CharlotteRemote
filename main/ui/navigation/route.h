#pragma once

#include "../../../components/yoba/src/ui.h"

namespace pizda {
	using namespace yoba::ui;

	class Route {
		public:
			virtual Element* buildElement() const = 0;
	};

	template<typename T>
	class ElementRoute : public Route {
		public:
			Element* buildElement() const override;
	};

	template<typename T>
	Element* ElementRoute<T>::buildElement() const {
		return new T();
	}
}