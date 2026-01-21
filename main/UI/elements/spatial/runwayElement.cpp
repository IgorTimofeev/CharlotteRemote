#include "runwayElement.h"

#include "rc.h"

namespace pizda {
	RunwayElement::RunwayElement(const uint16_t runwayIndex, const Color* color): _runwayIndex(runwayIndex) {
		const auto& runwayData = RC::getInstance().getNavigationData().runways[_runwayIndex];

		setVertices(runwayData.spatialVertices);
		setVertexCount(5);

		setLineVertexIndices(_lineVertexIndices);
		setLineVertexIndicesCount(8);

		setColor(color);
	}

	uint16_t RunwayElement::getRunwayIndex() const {
		return _runwayIndex;
	}
}
