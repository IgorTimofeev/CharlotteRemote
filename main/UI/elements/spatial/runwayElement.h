#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/UI/spatial.h>

#include "types/navigationData.h"

namespace pizda {
	using namespace YOBA;
	using namespace YOBA::spatial;

	class RunwayElement : public LinearMesh {
		public:
			explicit RunwayElement(uint16_t runwayIndex, const Color& color);

			uint16_t getRunwayIndex() const;

		private:
			constexpr static uint16_t _lineVertexIndices[] = {
				0, 1,
				1, 2,
				2, 3,
				0, 3
			};

			const uint16_t _runwayIndex;
	};
}