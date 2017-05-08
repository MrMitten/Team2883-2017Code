#include <IterativeRobot.h>
#include <Joystick.h>
#include <LiveWindow/LiveWindow.h>
#include <RobotDrive.h>
#include <Timer.h>
#include <TalonSRX.h>
#include <Talon.h>
#include <SpeedController.h>
#include <unistd.h>
#include <stdio.h>
#include "WPILib.h"
#include <iostream>
#include "CANTalon.h"
#include "Autonomous.cpp"
#include "Visioncode.cpp"
#include "Controls.cpp"
//all these above are refrences to codes and commands we will be using

//kTriggerbutton is A button
//still need to figure out buttons
class Robot: public frc::IterativeRobot {
public:
	//reference
	//depending on build, comment.
	CANTalon FrontLeft{10};
	CANTalon RearLeft{11};
	CANTalon FrontRight{12};
	CANTalon RearRight{13};
	CANTalon shooter{16};// motor
	CANTalon climber{14};// motor
	CANTalon harvester{15};// motor
	CANTalon hoppercycle{17};

	frc::Talon hooper{0};// motor
	/*frc::Talon climber{5};// motor
	frc::Talon harvester{6};// motor
	frc::Talon hoppercycle{7};*/


	frc::DigitalOutput spark{0};
	frc::Solenoid Gearbox{0};
	frc::PowerDistributionPanel power{0};


	//testing purposes


	AUT::Visioncode vis;
	AUT::Autonomous aut;
	AUT::Controls con;
	//pnumatics controls

	//auto stuff
	bool isTeleop = false;
	std::shared_ptr<NetworkTable> Auto;
	std::string autostring;


	frc::RobotDrive myRobot { FrontLeft, RearLeft, FrontRight, RearRight };

	Robot() {
		myRobot.SetExpiration(0.1);
		timer.Start();
		vis.table = NetworkTable::GetTable("GRIP/myContoursReport"); //this is setting up a ref too our vision values
		Auto = NetworkTable::GetTable("Preferences");

	}

private:
	//frc::RobotDrive myRobot { 3, 1, 0, 2 }; // Robot drive system (0)rearleft, (1)rearrite, (2)frontlef, (3)frontright
	frc::LiveWindow* lw = frc::LiveWindow::GetInstance();
	frc::Timer timer;
	void RobotInit() {
		//camera setup
		vis.startup();
		//Autonomous Selection

		//if(isTeleop == false)
		//{
			//AutoControls();
		//}
	//myRobot.SetInvertedMotor(myRobot.kFrontLeftMotor, true); //this is inverting front left motor
	//myRobot.SetInvertedMotor(myRobot.kFrontRightMotor, true); //this is inverting front right motor
	//myRobot.SetInvertedMotor(myRobot.kRearLeftMotor, true); //this is inverting back left motor
	//myRobot.SetInvertedMotor(myRobot.kRearRightMotor, true); //this is inverting rear right motor
	}

	void AutonomousInit() override {
		aut.Update();
		autostring = Auto->GetString("Auto Selector", "VisionShot");
	}

	void AutonomousPeriodic() override {

		aut.UpdateVolts(power.GetVoltage());
		aut.RUNAUTO(autostring);
		hoppercycle.Set(aut.hoppercycle);
		shooter.Set(aut.shooter);
		harvester.Set(aut.harvester);
		hooper.Set(aut.hooper);
		myRobot.ArcadeDrive(aut.drive[0], aut.drive[1]);

	}




	void TeleopInit() override {
		timer.Reset();
		timer.Start();
	}

	void TeleopPeriodic() override {

		con.UpdateControlsMotors();
		con.UpdateControlsSpeed();
		con.UpdateCamera();
		//UpdateVision();
		//isTeleop = true;
		// Drive with arcade style (use right stick)
		if(con.drive == false)
		{
			myRobot.SetInvertedMotor(myRobot.kFrontLeftMotor, false); //this is inverting front left motor
			myRobot.SetInvertedMotor(myRobot.kFrontRightMotor, false); //this is inverting front right motor
			myRobot.SetInvertedMotor(myRobot.kRearLeftMotor, false); //this is inverting back left motor
			myRobot.SetInvertedMotor(myRobot.kRearRightMotor, false); //this is inverting rear right motor*/
			if(con.gear == false)
			{
			myRobot.ArcadeDrive(con.stick);
			}
			else
			myRobot.ArcadeDrive(con.stick.GetRawAxis(1)*.65, con.stick.GetRawAxis(0)*.65);

		}
		else
		{
			vis.server.SetSource(vis.cam2);
			vis.move = 0;
			vis.rotation = 0;
			vis.UpdateVisionGearbox();
			myRobot.ArcadeDrive(.55, vis.rotation*.8);

		}
		//set talon speeds
			if(con.motorswitch)
			{
				shooter.Set((con.motorspeed - .085)/(power.GetVoltage()/12));
			}
			else
			{
				shooter.Set(0);
				//harvester.Set(motorspeed);
			}
			if(con.motorswitch2)
				climber.Set(con.motorspeed);
			else
				climber.Set(0);
			if(con.motorswitch3)
				harvester.Set(con.motorspeed);
				else
				harvester.Set(0);
			if(con.motorswitch4)
			{
				hoppercycle.Set(con.motorspeedtest);
				hooper.Set(con.motorspeedtest);
			}
				else
				{
				hoppercycle.Set(0);
				hooper.Set(0);
				}
	}


	// motorcontols





	void TestPeriodic() override {
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
