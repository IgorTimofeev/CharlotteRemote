#include "rc.h"
#include "UI/theme.h"
#include "UI/navigation/MFD/MFDPage.h"

namespace pizda {
	void PFDPageSplitter::onEvent(Event& event) {
		if (event.getTypeID() == PointerDownEvent::typeID) {
			_pointerY = reinterpret_cast<PointerDownEvent&>(event).getPosition().getY();

			setCaptured(true);
			setActive(true);

			event.setHandled(true);
		}
		else if (event.getTypeID() == PointerDragEvent::typeID) {
			auto& rc = RC::getInstance();

			const auto pointerY = reinterpret_cast<PointerDragEvent&>(event).getPosition().getY();

			if (!_draggedDohuya && std::abs(pointerY - _pointerY) >= 10) {
				_draggedDohuya = true;
			}

			if (_draggedDohuya && rc.getSettings().personalization.MFD.split.mode == PersonalizationSettingsMFDSplitMode::split) {
				_pointerY = pointerY;

				// this > PFD > MFD layout
				const auto layout = dynamic_cast<MFDLayout*>(getParent()->getParent());
				const auto& bounds = layout->getBounds();

				rc.getSettings().personalization.MFD.split.ratio = static_cast<uint8_t>(std::clamp<int32_t>(
					(_pointerY - bounds.getY()) * 100 / bounds.getHeight(),
					20,
					80
				));

				rc.getSettings().personalization.scheduleWrite();

				layout->updateSplitRatio();

				invalidate();
			}

			event.setHandled(true);

		}
		else if (event.getTypeID() == PointerUpEvent::typeID) {
			auto& rc = RC::getInstance();

			if (!_draggedDohuya) {
				rc.getSettings().personalization.MFD.split.mode =
					rc.getSettings().personalization.MFD.split.mode == PersonalizationSettingsMFDSplitMode::PFD
					? PersonalizationSettingsMFDSplitMode::split
					: PersonalizationSettingsMFDSplitMode::PFD;

				const auto parent = dynamic_cast<MFDLayout*>(getParent()->getParent());
				parent->deleteShit();
				parent->createShit();
			}

			rc.getSettings().personalization.scheduleWrite();

			_pointerY = -1;
			_draggedDohuya = false;

			setCaptured(false);
			setActive(false);

			event.setHandled(true);
		}
	}

	void PFDPageSplitter::onRender(Renderer& renderer, const Bounds& bounds) {
		renderer.renderFilledRectangle(bounds, isActive() ? Theme::fg1 : Theme::bg2);

		const auto text = RC::getInstance().getSettings().personalization.MFD.split.mode == PersonalizationSettingsMFDSplitMode::PFD ? L"< >" : L"> <";

		renderer.renderText(
			Point(
				bounds.getXCenter() - Theme::fontSmall.getWidth(text) / 2,
				bounds.getYCenter() - Theme::fontSmall.getHeight() / 2
			),
			Theme::fontSmall,
			isActive() ? Theme::bg1 : Theme::ocean,
			text
		);
	}
}
