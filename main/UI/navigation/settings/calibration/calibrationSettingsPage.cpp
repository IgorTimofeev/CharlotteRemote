#include "calibrationSettingsPage.h"

#include <format>

#include "rc.h"
#include "UI/theme.h"

namespace pizda {
	CalibrationSettingsPage::CalibrationSettingsPage() {
		// Page title
		title.setText(L"Calibration");
		
		// Description
		Theme::applyDescription(&_descriptionText);
		
		_descriptionText.setWrappingEnabled(true);
		
		_descriptionText.setText(
			LR"(It is advisable to calibrate the accelerometer and gyroscope only after changing the location of the MCU in the fuselage

The magnetometer should be calibrated each time before first takeoff in new geolocation to take into account the influence of parasitic magnetic fields from nearby metal structures)"
		);
		
		rows += &_descriptionText;
		
		// Progress bar
		Theme::apply(&_progressBar);
		rows += &_progressBar;
		
		// Progress text
		Theme::applyDescription(&_progressText);
		_progressText.setHorizontalAlignment(Alignment::center);
		rows += &_progressText;
		
		// Accel & gyro
		Theme::applyPrimary(&_accelAndGyroButton);
		_accelAndGyroButton.setText(L"Accel & gyro");
		
		_accelAndGyroButton.setOnClick([] {
			RC::getInstance().getRemoteData().calibrationSystem = AircraftCalibrationSystem::accelAndGyro;
			RC::getInstance().getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::calibrate);
		});
		
		rows += &_accelAndGyroButton;
		
		// Mag
		Theme::applyPrimary(&_magButton);
		_magButton.setText(L"Mag");
		
		_magButton.setOnClick([] {
			RC::getInstance().getRemoteData().calibrationSystem = AircraftCalibrationSystem::mag;
			RC::getInstance().getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::calibrate);
		});
		
		rows += &_magButton;
	}
	
	void CalibrationSettingsPage::onTick() {
		Layout::onTick();
		
		auto& rc = RC::getInstance();
		
		const auto calibrating = rc.getAircraftData().raw.calibration.isCalibrating();
		
		if (calibrating) {
			_progressBar.setValue(static_cast<float>(rc.getAircraftData().raw.calibration.progress) / 0xFF);
			
			_progressText.setText(std::format(
				L"Calibrating {}: {}%",
				rc.getAircraftData().raw.calibration.system == AircraftCalibrationSystem::accelAndGyro
					 ? L"accel & gyro"
					 : L"mag",
				rc.getAircraftData().raw.calibration.progress * 100 / 0xFF
			));
		}
		
		_progressBar.setVisible(calibrating);
		_progressText.setVisible(calibrating);
		
		_accelAndGyroButton.setVisible(!calibrating);
		_magButton.setVisible(!calibrating);
	}
}