#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/UI/spatial.h>

namespace pizda {
	using namespace YOBA;
	using namespace YOBA::spatial;

	class PFDScene : public Scene {
		public:
			PFDScene();

		protected:
			void onRender(Renderer& renderer, const Bounds& bounds) override;

		private:
			static void renderPitchOverlay(
				Renderer& renderer,
				const Bounds& bounds,
				float pitchPixelOffsetProjected,
				float projectionPlaneDistance,
				const Point& horizonLeft,
				const Point& horizonRight,

				const Vector2F& horizonVec,
				const Vector2F& horizonVecNorm,
				const Vector2F& horizonVecPerp,
				const Vector2F& horizonCenter
			);

			static void renderTurnCoordinatorOverlay(Renderer& renderer, const Bounds& bounds);
			static void renderFlightModeAnnunciatorOverlay(Renderer& renderer, const Bounds& bounds);
			static void renderYawOverlay(Renderer& renderer, const Bounds& bounds);
	};
}