#include "trimToolbar.h"

#include "rc.h"

namespace pizda {
	TrimToolbar::TrimToolbar() :
		_ailerons(TrimControl { L"Ailerons", &RC::getInstance().getSettings().controls.aileronsTrim }),
		_elevator(TrimControl { L"Elevator", &RC::getInstance().getSettings().controls.elevatorTrim }),
		_rudder(TrimControl { L"Rudder", &RC::getInstance().getSettings().controls.rudderTrim })
	{
		row += _ailerons;
		row += _elevator;
		row += _rudder;
	}
}