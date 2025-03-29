#pragma once

#include "src/main.h"
#include "src/ui.h"
#include "../../../theme.h"
#include "../../../../hardware/encoder.h"
#include "../../../../rc.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	template<uint8_t digitCount, uint32_t minimum, uint32_t maximum, uint16_t smallChange, uint16_t bigChange>
	template<typename TValue>
	void LeftButtonRotaryControl<digitCount, minimum, maximum, smallChange, bigChange>::addSettingsCallbacks(TValue* numericValue, bool* boolValue) {
		this->seven.setValue((uint32_t) *numericValue);
		this->button.setChecked(*boolValue);

		this->rotated += [numericValue, this]() {
			*numericValue = (TValue) this->seven.getValue();
			RC::getInstance().getSettings().enqueueWrite();
		};

		this->button.isCheckedChanged += [boolValue, this]() {
			*boolValue = this->button.isChecked();
			RC::getInstance().getSettings().enqueueWrite();
		};
	}
}