#include "ADIRSSettingsPage.h"

#include <format>

#include "rc.h"
#include "UI/theme.h"
#include "utilities/string.h"

namespace pizda {
	ADIRSSettingsPage::ADIRSSettingsPage() {
		// ----------------------------- ADIRS -----------------------------

		title.setText(L"ADIRS");

		// Magnetic declination
		Theme::apply(&_ADIRSMagneticDeclinationTextField);

		_ADIRSMagneticDeclinationTextField.setText(std::to_wstring(RC::getInstance().getSettings().ADIRS.magneticDeclinationDeg));

		_ADIRSMagneticDeclinationTextField.setOnInput([this](const Key key, std::optional<std::wstring_view>) {
			if (key != Key::enter)
				return;

			auto& rc = RC::getInstance();
			rc.getSettings().ADIRS.magneticDeclinationDeg = StringUtils::tryParseInt32Or(_ADIRSMagneticDeclinationTextField.getText(), 0);
			rc.getSettings().ADIRS.scheduleWrite();

			rc.getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::ADIRS);
		});

		rows += &_ADIRSMagneticDeclinationTitle;

		// ----------------------------- Calibration -----------------------------

		// Separator
		rows += &_calSeparator;

		// Title
		Theme::applyPageTitle(&_calTitle);
		_calTitle.setText(L"Calibration");
		rows += &_calTitle;
		
		// Progress bar
		Theme::apply(&_calProgressBar);
		rows += &_calProgressBar;
		
		// Progress text
		Theme::applyDescription(&_calProgressText);
		_calProgressText.setHorizontalAlignment(Alignment::center);
		rows += &_calProgressText;
		
		// Accel & gyro
		Theme::applyPrimary(&_calAccelAndGyroButton);
		_calAccelAndGyroButton.setText(L"Accel & gyro");
		
		_calAccelAndGyroButton.setOnClick([] {
			RC::getInstance().getRemoteData().calibrationSystem = AircraftCalibrationSystem::accelAndGyro;
			RC::getInstance().getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::calibrate);
		});
		
		rows += &_calAccelAndGyroButton;
		
		// Mag
		Theme::applyPrimary(&_calMagButton);
		_calMagButton.setText(L"Mag");
		
		_calMagButton.setOnClick([] {
			RC::getInstance().getRemoteData().calibrationSystem = AircraftCalibrationSystem::mag;
			RC::getInstance().getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::calibrate);
		});

		rows += &_calMagButton;

		// Description
		Theme::applyDescription(&_calDescriptionText);

		_calDescriptionText.setWrappingEnabled(true);

		_calDescriptionText.setText(
			LR"(It is advisable to calibrate the accelerometer and gyroscope only after changing MCU location in the fuselage

The magnetometer should be calibrated each time before first takeoff in new geolocation to take into account the influence of parasitic magnetic fields from nearby metal structures)"
		);

		rows += &_calDescriptionText;
	}
	
	void ADIRSSettingsPage::onTick() {
		Layout::onTick();
		
		auto& rc = RC::getInstance();
		
		const auto calibrating = rc.getAircraftData().raw.calibration.isCalibrating();
		
		if (calibrating) {
			_calProgressBar.setValue(static_cast<float>(rc.getAircraftData().raw.calibration.progress) / 0xFF);
			
			_calProgressText.setText(std::format(
				L"Calibrating {}: {}%",
				rc.getAircraftData().raw.calibration.system == AircraftCalibrationSystem::accelAndGyro
					 ? L"accel & gyro"
					 : L"mag",
				rc.getAircraftData().raw.calibration.progress * 100 / 0xFF
			));
		}
		
		_calProgressBar.setVisible(calibrating);
		_calProgressText.setVisible(calibrating);
		
		_calAccelAndGyroButton.setVisible(!calibrating);
		_calMagButton.setVisible(!calibrating);
	}
}