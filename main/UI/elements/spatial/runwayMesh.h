#pragma once

#include <numbers>
#include <esp_log.h>

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/UI/spatial.h>

#include "../../../rc.h"
#include "../../../settings/settings.h"
#include "../../../types/navigationData.h"

namespace pizda {
	using namespace YOBA;
	using namespace YOBA::spatial;

	class RunwayMesh : public LinearMesh {
		public:
			explicit RunwayMesh(const NavigationRunwayData* runwayData, const Color* color) : _runwayData(runwayData) {
				setVertices(_runwayData->vertices + 1);
				setVertexCount(4);

				setLineVertexIndices(_lineVertexIndices);
				setLineVertexIndicesCount(8);

				setColor(color);
			}

		private:
			constexpr static uint16_t _lineVertexIndices[] = {
				0, 1,
				1, 2,
				2, 3,
				0, 3
			};

			const NavigationRunwayData* _runwayData;
	};
}