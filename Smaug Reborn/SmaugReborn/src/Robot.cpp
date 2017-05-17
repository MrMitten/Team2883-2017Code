#include <iostream>
#include <memory>
#include <string>

#include "WPILib.h"
#include "Joystick.h"
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include "MotorControl.h"
#include "AirControl.h"
#include "Vision.h"

class Robot: public frc::IterativeRobot {
private:
	frc::LiveWindow* lw = LiveWindow::GetInstance();
	frc::SendableChooser<std::string> chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "Vision";
	std::string autoSelected;

	MEC::MotorControl mot;
	MEC::AirControl air;
	MEC::Vision vis;

public:

	void RobotInit() {
		chooser.AddDefault(autoNameDefault, autoNameDefault);
		chooser.AddObject(autoNameCustom, autoNameCustom);
		frc::SmartDashboard::PutData("Auto Modes", &chooser);
		mot.Reset();
		air.Reset();
		vis.startup();
	}
	Robot()
	{
		mot.Drive.SetExpiration(0.1);
		vis.table = NetworkTable::GetTable("GRIP/myContoursReport");
	}
	/*
	 * This autonomous (along with the chooser code above) shows how to select
	 * between different autonomous modes using the dashboard. The sendable
	 * chooser code works with the Java SmartDashboard. If you prefer the
	 * LabVIEW Dashboard, remove all of the chooser code and uncomment the
	 * GetString line to get the auto name from the text box below the Gyro.
	 *
	 * You can add additional auto modes by adding additional comparisons to the
	 * if-else structure below with additional strings. If using the
	 * SendableChooser make sure to add them to the chooser code above as well.
	 */
	void AutonomousInit() override {
		autoSelected = chooser.GetSelected();
		// std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if (autoSelected == autoNameCustom) {
			// Custom Auto goes here
			mot.Reset();
						air.Reset();
		} else {

			// Default Auto goes here
		}
	}

	void AutonomousPeriodic() {
		if (autoSelected == autoNameCustom) {
			// Custom Auto goes here
			vis.UpdateVision();
					air.Lights.Set(frc::Relay::kOn);
					mot.Drive.ArcadeDrive(vis.move, vis.rotation);
		} else {
			// Default Auto goes here

		}
	}

	void TeleopInit() {
			mot.Reset();
			air.Reset();
			vis.startup();
	}

	void TeleopPeriodic() {
			mot.Motorcontrols();
			mot.MotorSet();
			air.Aircontrols();
			air.AirSet();
	}

	void TestPeriodic() {
		lw->Run();
	}

};

START_ROBOT_CLASS(Robot)
