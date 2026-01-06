#pragma once

#include "resources/sounds/bootSound.h"
#include "resources/sounds/transceiverConnectSound.h"
#include "resources/sounds/transceiverDisconnectSound.h"
#include "resources/sounds/engagedSound.h"
#include "resources/sounds/disengagedSound.h"
#include "resources/sounds/feedbackSound.h"

namespace pizda::resources {
	class sounds {
		public:
			constexpr static BootSound boot {};
			constexpr static FeedbackSound feedback {};
			constexpr static TransceiverConnectSound transceiverConnect {};
			constexpr static TransceiverDisconnectSound transceiverDisconnect {};
			constexpr static EngagedSound engaged {};
			constexpr static DisengagedSound disengaged {};
	};
}