#include "MFDPage.h"

#include "UI/theme.h"
#include "rc.h"

namespace pizda {
	MFDPage* MFDPage::_instance = nullptr;

	MFDPageSplitter::MFDPageSplitter() {
		setSize(Size(PFD::speedWidth, PFD::miniHeight));
		setVerticalAlignment(Alignment::end);
	}

	void MFDPageSplitter::onEvent(Event* event) {
		if (event->getTypeID() == PointerDownEvent::typeID) {
			_pointerY = reinterpret_cast<PointerDownEvent*>(event)->getPosition().getY();

			setCaptured(true);
			setActive(true);

			event->setHandled(true);
		}
		else if (event->getTypeID() == PointerDragEvent::typeID) {
			const auto pointerY = reinterpret_cast<PointerDragEvent*>(event)->getPosition().getY();

			if (!_draggedDohuya && std::abs(pointerY - _pointerY) >= 10) {
				_draggedDohuya = true;
			}

			if (_draggedDohuya) {
				_pointerY = pointerY;

				auto& rc = RC::getInstance();

				const auto parent = dynamic_cast<MFDLayout*>(getParent()->getParent());
				const auto& bounds = parent->getBounds();

				rc.getSettings().personalization.MFD.splitPercent = static_cast<uint8_t>(std::clamp<int32_t>(
					(_pointerY - bounds.getY()) * 100 / bounds.getHeight(),
					10,
					90
				));

				parent->deleteShit();
				parent->createShit();

				invalidate();
			}

			event->setHandled(true);

		}
		else if (event->getTypeID() == PointerUpEvent::typeID) {
			auto& rc = RC::getInstance();

			if (!_draggedDohuya) {
				rc.getSettings().personalization.MFD.ND.visible = !rc.getSettings().personalization.MFD.ND.visible;

				const auto parent = dynamic_cast<MFDLayout*>(getParent()->getParent());
				parent->deleteShit();
				parent->createShit();
			}

			rc.getSettings().personalization.scheduleWrite();

			_pointerY = -1;
			_draggedDohuya = false;

			setCaptured(false);
			setActive(false);

			event->setHandled(true);
		}
	}

	void MFDPageSplitter::onRender(Renderer* renderer, const Bounds& bounds) {
		renderer->renderFilledRectangle(bounds, isActive() ? &Theme::fg1 : &Theme::bg2);

		const auto text = RC::getInstance().getSettings().personalization.MFD.ND.visible ? L"HIDE" : L"SPLT";

		renderer->renderString(
			Point(
				bounds.getXCenter() - Theme::fontSmall.getWidth(text) / 2,
				bounds.getYCenter() - Theme::fontSmall.getHeight() / 2
			),
			&Theme::fontSmall,
			isActive() ? &Theme::bg1 : &Theme::fg1,
			text
		);
	}

	PFDAndSplitterLayout::PFDAndSplitterLayout() {
		*this += &_PFD;
		*this += &_splitter;
	}

	void MFDLayout::deleteShit() {
		removeChildren();

		auto& rc = RC::getInstance();

		if (_PFDAndSplitter.get() && !rc.getSettings().personalization.MFD.PFD.visible)
			_PFDAndSplitter.reset();

		if (_ND.get() && !rc.getSettings().personalization.MFD.ND.visible)
			_ND.reset();
	}

	void MFDLayout::createShit() {
		auto& rc = RC::getInstance();

		if (rc.getSettings().personalization.MFD.PFD.visible) {
			if (!_PFDAndSplitter)
				_PFDAndSplitter = std::make_unique<PFDAndSplitterLayout>();

			*this += _PFDAndSplitter.get();
		}

		if (rc.getSettings().personalization.MFD.ND.visible) {
			if (!_ND)
				_ND = std::make_unique<ND>();

			*this += _ND.get();
		}

		if (rc.getSettings().personalization.MFD.PFD.visible && rc.getSettings().personalization.MFD.ND.visible) {
			setRelativeSize(_PFDAndSplitter.get(), static_cast<float>(rc.getSettings().personalization.MFD.splitPercent) / 100.f);
			setRelativeSize(_ND.get(), 1.f - static_cast<float>(rc.getSettings().personalization.MFD.splitPercent) / 100.f);
		}
		else {
			setRelativeSize(_PFDAndSplitter.get(), 1);
		}
	}

	MFDPage::MFDPage() {
		_instance = this;

		*this += &_rows;

		// Initialization
		fromSettingsInstance();
	}

	MFDPage::~MFDPage() {
		_instance = nullptr;
	}

	void MFDPage::fromSettings() {
		if (_instance)
			_instance->fromSettingsInstance();
	}

	void MFDPage::fromSettingsInstance() {
		_rows.removeChildren();

		auto& rc = RC::getInstance();

		// Deleting
		_MFDLayout.deleteShit();

		if (_autopilotToolbar.get() && rc.getSettings().personalization.MFD.toolbar.mode != PersonalizationSettingsMFDToolbarMode::autopilot)
			_autopilotToolbar.reset();

		if (_baroToolbar.get() && rc.getSettings().personalization.MFD.toolbar.mode != PersonalizationSettingsMFDToolbarMode::baro)
			_baroToolbar.reset();
		
		if (_trimToolbar.get() && rc.getSettings().personalization.MFD.toolbar.mode != PersonalizationSettingsMFDToolbarMode::trim)
			_trimToolbar.reset();
		
		if (_lightsToolbar.get() && rc.getSettings().personalization.MFD.toolbar.mode != PersonalizationSettingsMFDToolbarMode::lights)
			_lightsToolbar.reset();
		
		// Creating
		switch (rc.getSettings().personalization.MFD.toolbar.mode) {
			case PersonalizationSettingsMFDToolbarMode::autopilot: {
				if (!_autopilotToolbar)
					_autopilotToolbar = std::make_unique<AutopilotToolbar>();
				
				_rows.setAutoSize(_autopilotToolbar.get(), true);
				_rows += _autopilotToolbar.get();
				
				break;
			}
			case PersonalizationSettingsMFDToolbarMode::baro: {
				if (!_baroToolbar)
					_baroToolbar = std::make_unique<BaroToolbar>();
				
				_rows.setAutoSize(_baroToolbar.get(), true);
				_rows += _baroToolbar.get();
				
				break;
			}
			case PersonalizationSettingsMFDToolbarMode::trim: {
				if (!_trimToolbar)
					_trimToolbar = std::make_unique<TrimToolbar>();
				
				_rows.setAutoSize(_trimToolbar.get(), true);
				_rows += _trimToolbar.get();
				
				break;
			}
			case PersonalizationSettingsMFDToolbarMode::lights: {
				if (!_lightsToolbar)
					_lightsToolbar = std::make_unique<LightsToolbar>();
				
				_rows.setAutoSize(_lightsToolbar.get(), true);
				_rows += _lightsToolbar.get();
				
				break;
			}
			default:
				break;
		}

		_MFDLayout.createShit();
		_rows += &_MFDLayout;
		
		// Main
		_rows.setAutoSize(&_mainToolbar, true);
		_rows += &_mainToolbar;
		
		invalidate();
	}
}
