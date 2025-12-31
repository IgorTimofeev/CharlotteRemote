#pragma once

#include "resources/sounds/bootSound.h"
#include "resources/sounds/transceiverConnectSound.h"
#include "resources/sounds/transceiverDisconnectSound.h"
#include "resources/sounds/autopilotEngagedSound.h"
#include "resources/sounds/autopilotDisengagedSound.h"
#include "resources/sounds/feedbackSound.h"

namespace pizda::resources {
	class sounds {
		public:
			constexpr static BootSound boot {};
			constexpr static FeedbackSound feedback {};
			constexpr static TransceiverConnectSound transceiverConnect {};
			constexpr static TransceiverDisconnectSound transceiverDisconnect {};
			constexpr static AutopilotEngagedSound autopilotEngaged {};
			constexpr static AutopilotDisengagedSound autopilotDisengaged {};
	};
}