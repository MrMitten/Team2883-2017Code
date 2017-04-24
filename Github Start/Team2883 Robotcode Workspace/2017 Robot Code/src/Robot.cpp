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
	std::shared_ptr<NetworkTable> table;
	frc::PowerDistributionPanel power{0};

	//motor conrols
	double motorspeed = .95;
	bool gear = false;
	bool  motorswitch = false;
	bool  motorswitch2 = false;
	bool  motorswitch3 = false;
	bool  motorswitch4 = false;
	bool pressed = false;
	bool pressed2 = false;
	bool pressed3 = false;
	bool pressed4 = false;
	bool pressed5 = false;

	bool drive;
	double rotation;
	double move;


	//testing purposes
	double motorspeedtest = 1;


	//pnumatics controls

	//auto stuff
	bool isTeleop = false;
	std::shared_ptr<NetworkTable> Auto;
	std::string autostring;
	bool autoswitch = false;
	bool autoswitch2 = false;
	bool pressed6 = false;
	cs::UsbCamera cam1 = frc::CameraServer::GetInstance()->StartAutomaticCapture(0);
	cs::UsbCamera cam2 = frc::CameraServer::GetInstance()->StartAutomaticCapture(1);
	cs::MjpegServer server = frc::CameraServer::GetInstance()->AddServer("gripStream", 1180);

	Robot() {
		myRobot.SetExpiration(0.1);
		timer.Start();
		table = NetworkTable::GetTable("GRIP/myContoursReport"); //this is setting up a ref too our vision values
		Auto = NetworkTable::GetTable("Preferences");

	}

private:
	frc::RobotDrive myRobot { FrontLeft, RearLeft, FrontRight, RearRight };
	//frc::RobotDrive myRobot { 3, 1, 0, 2 }; // Robot drive system (0)rearleft, (1)rearrite, (2)frontlef, (3)frontright
	frc::Joystick stick { 0 };         // Only joystick
	frc::LiveWindow* lw = frc::LiveWindow::GetInstance();
	frc::Timer timer;
	void RobotInit() {
		//camera setup
		server.SetSource(cam2);
		cam1.SetBrightness(0);
		//cam1.SetResolution(160,160);
		cam1.SetExposureManual(30);
		cam1.SetExposureHoldCurrent();
		cam1.SetWhiteBalanceManual(50);
		cam1.SetWhiteBalanceHoldCurrent();
		cam2.SetBrightness(0);
		//cam2.SetResolution(160,160);
		cam2.SetExposureManual(30);
		cam2.SetExposureHoldCurrent();
		cam2.SetWhiteBalanceManual(50);
		cam2.SetWhiteBalanceHoldCurrent();

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
		timer.Reset();
		timer.Start();
		autostring = Auto->GetString("Auto Selector", "VisionShot");
	}

	void AutonomousPeriodic() override {

		if(autostring == "VisionShot")
		{
			//VisionShot();
			shooter.Set(.93);
			move = 0;
			rotation = 0;
			UpdateVision();
			myRobot.ArcadeDrive(move*1.05, rotation*.5);
		}
		if(autostring == "VisionGear")
		{
			VisionGear();
		}
		if(autostring == "Blue")
			{
				FullTryLeft();
			}
		if(autostring == "Red")
			{
				FullTryRight();
			}
		if(autostring == "BlueMid")
			{
				FullTryMiddleLeft();
			}
		if(autostring == "RedMid")
			{
				FullTryMiddleRight();
			}
		if(autostring == "BlueGear")
			{
				FullTryGearLeft();
			}
		if(autostring == "RedGear")
			{
				FullTryGearRight();
			}
		if(autostring == "Hoppa")
			{
				HoppaDrop();
			}
		if(autostring == "HoppaRight")
			{
				HoppaDropRight();
			}
		std::cout << autostring;
		/*isTeleop = false;
		if(Auto == 0)
		{
		if(timer.Get() < 2.5)
		{
			myRobot.ArcadeDrive(-.5, 0);
			server.SetSource(cam1);
			std::cout << "drive";
		}
		else if(timer.Get() < 3.7 && timer.Get() > 2.5)
		{
			myRobot.ArcadeDrive(0, .5);
			std::cout << "drive";
		}
		else if(timer.Get() < 6.0 && timer.Get() > 3.7)
			{
				move = 0;
				rotation = 0;
				UpdateVisionGearbox();
				myRobot.ArcadeDrive(move, rotation);
			}
		else if(timer.Get() > 7.5 && timer.Get() < 9.3)
		{
			myRobot.ArcadeDrive(.5, .65);
			server.SetSource(cam2);
		}
		else if(timer.Get() > 9.3)
		{
		move = 0;
		rotation = 0;
		std::cout << timer.Get();
		UpdateVision();
		myRobot.ArcadeDrive(move, rotation);
		//std::cout << move;
		//std::cout << rotation;
		//std::cout << std::endl;
		}

		//}
		//send to log... this is the vision code that sends the roborio grip values
		/std::vector<double> arr = table->GetNumberArray("centerX", llvm::ArrayRef<double>());
		if(arr[0] > 0)
			{
			if(arr[0] > 80)
			{
				myRobot.Drive(0.2*((80 - (arr[0] - 80))/80),-1.0);
				//std::cout << "right";
			}
			else if(arr[0] < 80)
			{
				myRobot.Drive(0.2*((80 - arr[0])/80), 1.0);
				//std::cout << "left";
			}
			//}

			if(arr[0] > 73 && arr[0] < 87)
			{
				shooter.Set(1);
			}
			else
			{
				shooter.Set(0);
			}
		}*/

		/*if(Auto == 0)
		{
		UpdateVision();
		}
		if(Auto == 1)
		{
		if (timer.Get() < 2.0) {
		myRobot.Drive(-0.3, 0.0);  // Drive sideways 3/4 speed
		} else {
		myRobot.Drive(0.0, 0.0);  // Stop robot
		}
		}*/
	}

	void VisionGear()
	{
		if(timer.Get() < 7)
		{
		server.SetSource(cam2);
		move = 0;
		rotation = 0;
		UpdateVisionGearbox();
		myRobot.ArcadeDrive(.5, rotation*1.25);
		}
		if(timer.Get() > 7 && timer.Get() < 8)
		{
			myRobot.ArcadeDrive(-.5, 0);
		}
		if(timer.Get() > 8)
			{
			server.SetSource(cam2);
			move = 0;
			rotation = 0;
			UpdateVisionGearbox();
			myRobot.ArcadeDrive(.5, rotation*1.25);
			}
	}


	void HoppaDrop()
	{
		if(timer.Get() < .75)
		{
		server.SetSource(cam2);
		myRobot.ArcadeDrive(-.79/(power.GetVoltage()/12), 0);
		}
		if(timer.Get() > .75 && timer.Get() < 1.125)
			{
			server.SetSource(cam2);
			myRobot.ArcadeDrive(0.0, -0.65/(power.GetVoltage()/12));
			}
		if(timer.Get() < 3.5 && timer.Get() > 1.125)
			{
			server.SetSource(cam2);
			move = 0;
			rotation = 0;
			UpdateVisionHoppa();
			myRobot.ArcadeDrive(-.55/(power.GetVoltage()/12), rotation*1.35/(power.GetVoltage()/12));
			}
		if(timer.Get() > 3.75 && timer.Get() < 4.35)
			{
			server.SetSource(cam1);
			myRobot.ArcadeDrive(0.90/(power.GetVoltage()/12), -0.625/(power.GetVoltage()/12));
			harvester.Set(motorspeed);
			//hoppercycle.Set(1);
			hooper.Set(1);
			}
		if(timer.Get() > 4.35 && timer.Get() < 5.25)
			{
			hoppercycle.Set(0);
			hooper.Set(0);
				server.SetSource(cam1);
				move = 0;
				rotation = 0;
				UpdateVision();

				if(move != 0)
				{
				myRobot.ArcadeDrive(move*1.4, rotation*.8);
				}
				else
				{
					myRobot.ArcadeDrive(0, -.55/(power.GetVoltage()/12));
				}

			}
			if(timer.Get() > 5.25 && timer.Get() < 10)
				{
				if(hoppercycle.Get() == 0)
				{
					server.SetSource(cam1);
					move = 0;
					rotation = 0;
					UpdateVision();
					myRobot.ArcadeDrive(move*1.2, rotation*.6);
					shooter.Set((motorspeed-.065)/(power.GetVoltage()/12));
				}
				}
			if(timer.Get() > 10 && timer.Get() < 10.5)
			{
				//hoppercycle.Set(1);
				hooper.Set(1);
			}
			if(timer.Get() > 11)
			{
				//hoppercycle.Set(-1);
				hooper.Set(-1);
			}

	}
	void HoppaDropRight()
	{
	if(timer.Get() < .75)
	{
	server.SetSource(cam2);
	myRobot.ArcadeDrive(-.79/(power.GetVoltage()/12), 0);
	}
	if(timer.Get() > .75 && timer.Get() < 1.125)
		{
		server.SetSource(cam2);
		myRobot.ArcadeDrive(0.0, 0.65/(power.GetVoltage()/12));
		}
	if(timer.Get() < 3.5 && timer.Get() > 1.125)
		{
		server.SetSource(cam2);
		move = 0;
		rotation = 0;
		UpdateVisionHoppa();
		myRobot.ArcadeDrive(-.55/(power.GetVoltage()/12), rotation*1.35/(power.GetVoltage()/12));
		}
	if(timer.Get() > 3.75 && timer.Get() < 4.35)
		{
		server.SetSource(cam1);
		myRobot.ArcadeDrive(0.90/(power.GetVoltage()/12), 0.625/(power.GetVoltage()/12));
		harvester.Set(motorspeed);
		//hoppercycle.Set(1);
		hooper.Set(1);
		}
	if(timer.Get() > 4.35 && timer.Get() < 5.25)
		{
		hoppercycle.Set(0);
		hooper.Set(0);
			server.SetSource(cam1);
			move = 0;
			rotation = 0;
			UpdateVision();

			if(move != 0)
			{
			myRobot.ArcadeDrive(move*1.4, rotation*.8);
			}
			else
			{
				myRobot.ArcadeDrive(0, .55/(power.GetVoltage()/12));
			}

		}
		if(timer.Get() > 5.25 && timer.Get() < 10)
			{
			if(hoppercycle.Get() == 0)
			{
				server.SetSource(cam1);
				move = 0;
				rotation = 0;
				UpdateVision();
				myRobot.ArcadeDrive(move*1.2, rotation*.6);
				shooter.Set((motorspeed-.065)/(power.GetVoltage()/12));
			}
			}
		if(timer.Get() > 10 && timer.Get() < 10.5)
		{
			//hoppercycle.Set(1);
			hooper.Set(1);
		}
		if(timer.Get() > 11)
		{
			//hoppercycle.Set(-1);
			hooper.Set(-1);
		}


		}



	//Boiler on Left, Left Peg//getting her workin
	void FullTryLeft()
	{

		if(timer.Get() < .75)
		{
		server.SetSource(cam2);
		myRobot.ArcadeDrive(.755/(power.GetVoltage()/12), 0);
		}
		if(timer.Get() > .8 && timer.Get() < 1.1)
		{
			myRobot.ArcadeDrive(0, .55);

		}
		if(timer.Get() > 1.2 && timer.Get() < 4.9)
		{
			server.SetSource(cam2);
			move = 0;
			rotation = 0;
			UpdateVisionGearbox();
			myRobot.ArcadeDrive(.55, rotation*.9/(power.GetVoltage()/12));
		}
		if(timer.Get() < 5.25 && timer.Get() > 4.9)
			{
			server.SetSource(cam1);
			myRobot.ArcadeDrive(-.82/(power.GetVoltage()/12), -0.35);
			}

			if(timer.Get() > 5.25 && timer.Get() < 5.5)
				{
				server.SetSource(cam2);
				myRobot.ArcadeDrive(0.5, 0/(power.GetVoltage()/12));
				}
			if(timer.Get() < 7.5 && timer.Get() > 5.5)
				{
				/*server.SetSource(cam1);
				myRobot.ArcadeDrive(-.635/(power.GetVoltage()/12),0);
				}
			if(timer.Get() < 6.95 && timer.Get() > 6.65)
			{
				myRobot.ArcadeDrive(-.3/(power.GetVoltage()/12), 0.0);
				shooter.Set(.89);
			}*/
				server.SetSource(cam2);
				move = 0;
				rotation = 0;
				UpdateVisionHoppa();
				myRobot.ArcadeDrive(-.7/(power.GetVoltage()/12), rotation*1.35/(power.GetVoltage()/12));
				}
			if(timer.Get() > 7.5 && timer.Get() < 8.25)
			{
				myRobot.ArcadeDrive(0.0, 0.0);
				server.SetSource(cam1);
			}
			if(timer.Get() > 8.25 && timer.Get() < 8.85)
			{
			server.SetSource(cam1);
			myRobot.ArcadeDrive(1, -0.8);
			}
			if(timer.Get() > 8.85 && timer.Get()< 9)
			{
				myRobot.ArcadeDrive(0.9, 0.0);
				harvester.Set(motorspeed);
			}
		if(timer.Get() > 9 && timer.Get() < 10)
		{
			server.SetSource(cam1);
			move = 0;
			rotation = 0;
			UpdateVision();
			if(move != 0)
			{
			myRobot.ArcadeDrive(move*1.4, rotation*.6);
			}
			else
			{
				myRobot.ArcadeDrive(0, -.55);
			}

		}
		if(timer.Get() > 10)
			{
			if(hoppercycle.Get() == 0)
			{
				server.SetSource(cam1);
				move = 0;
				rotation = 0;
				UpdateVision();
				myRobot.ArcadeDrive(move*1.1, rotation*.5);
				shooter.Set((motorspeed - .115)/(power.GetVoltage()/12.4));
			}
				else
				{
					harvester.Set(0);
				}
			}
	}


	//Boiler on Right, Right Peg
	void FullTryRight()
		{
		if(timer.Get() < .75)
				{
				server.SetSource(cam2);
				myRobot.ArcadeDrive(.755/(power.GetVoltage()/12), 0);
				}
				if(timer.Get() > .8 && timer.Get() < 1.1)
				{
					myRobot.ArcadeDrive(0, -.6);

				}
				if(timer.Get() > 1.2 && timer.Get() < 4.9)
				{
					server.SetSource(cam2);
					move = 0;
					rotation = 0;
					UpdateVisionGearbox();
					myRobot.ArcadeDrive(.55, rotation*.8/(power.GetVoltage()/12));
				}
				if(timer.Get() < 5.25 && timer.Get() > 4.9)
					{
					server.SetSource(cam1);
					myRobot.ArcadeDrive(-.92/(power.GetVoltage()/12), -0.25);
					shooter.Set((motorspeed - .115));
					}

					/*if(timer.Get() > 5.25 && timer.Get() < 5.5)
						{
						server.SetSource(cam2);
						myRobot.ArcadeDrive(0.5, 0/(power.GetVoltage()/12));
						}
					if(timer.Get() < 7.5 && timer.Get() > 5.5)
						{
						server.SetSource(cam1);
						myRobot.ArcadeDrive(-.635/(power.GetVoltage()/12),0);
						}
					if(timer.Get() < 6.95 && timer.Get() > 6.65)
					{
						myRobot.ArcadeDrive(-.3/(power.GetVoltage()/12), 0.0);
						shooter.Set(.89);
					}
						server.SetSource(cam2);
						move = 0;
						rotation = 0;
						UpdateVisionHoppa();
						myRobot.ArcadeDrive(-.7/(power.GetVoltage()/12), rotation*1.35/(power.GetVoltage()/12));
						}
					if(timer.Get() > 7.5 && timer.Get() < 8.25)
					{
						myRobot.ArcadeDrive(0.0, 0.0);
						server.SetSource(cam1);
					}
					if(timer.Get() > 8.25 && timer.Get() < 8.85)
					{
					server.SetSource(cam1);
					myRobot.ArcadeDrive(1, 0.8);
					}
					if(timer.Get() > 8.85 && timer.Get()< 9)
					{
						myRobot.ArcadeDrive(0.9, 0.0);
						harvester.Set(motorspeed);
					}*/
				if(timer.Get() > 5.25 && timer.Get() < 10)
				{
					server.SetSource(cam1);
					move = 0;
					rotation = 0;
					UpdateVision();
					if(move != 0)
					{
					myRobot.ArcadeDrive(move*1.4, rotation*.6);
					}
					else
					{
						myRobot.ArcadeDrive(0, .55);
					}

				}
				if(timer.Get() > 10)
					{
					if(hoppercycle.Get() == 0)
					{
						server.SetSource(cam1);
						move = 0;
						rotation = 0;
						UpdateVision();
						myRobot.ArcadeDrive(move*1.1, rotation*.5);
						shooter.Set((motorspeed - .09));
					}
						else
						{
							harvester.Set(0);
						}
					}
		}


	//Boiler on Left, Middle Peg
	void FullTryMiddleLeft()
		{
			if(timer.Get() < 4.9)
			{
				server.SetSource(cam2);
				move = 0;
				rotation = 0;
				UpdateVisionGearbox();
				myRobot.ArcadeDrive(.55, rotation*.9/(power.GetVoltage()/12));
			}
			if(timer.Get() < 5.25 && timer.Get() > 4.9)
				{
				server.SetSource(cam1);
				myRobot.ArcadeDrive(-.70/(power.GetVoltage()/12), 0);
				}
				if(timer.Get() > 5.25 && timer.Get() < 5.5)
					{
					server.SetSource(cam1);
					myRobot.ArcadeDrive(0.0, 0.65/(power.GetVoltage()/12));
					}
				if(timer.Get() > 5.75 && timer.Get() < 6.5)
					{
					server.SetSource(cam1);
					shooter.Set(.83);
					myRobot.ArcadeDrive(-.935/(power.GetVoltage()/12), 0);
					}
			if(timer.Get() > 6.5 && timer.Get() < 8.5)
			{
				harvester.Set(motorspeed);
				server.SetSource(cam1);
				move = 0;
				rotation = 0;
				UpdateVision();
				if(move != 0)
				{
				myRobot.ArcadeDrive(move*1.4, rotation*.8);
				}
				else
				{
					myRobot.ArcadeDrive(0, -.55);
				}

			}
			if(timer.Get() > 8.5)
				{
					server.SetSource(cam1);
					move = 0;
					rotation = 0;
					UpdateVision();
					myRobot.ArcadeDrive(move*1.08, rotation*.7);
					shooter.Set((motorspeed - .08)/(power.GetVoltage()/12.4));
				}
		}

	//Boiler on Right, Middle Peg
	void FullTryMiddleRight()
			{
		if(timer.Get() < 4.9)
					{
						server.SetSource(cam2);
						move = 0;
						rotation = 0;
						UpdateVisionGearbox();
						myRobot.ArcadeDrive(.55, rotation*.9/(power.GetVoltage()/12));
					}
					if(timer.Get() < 5.25 && timer.Get() > 4.9)
						{
						server.SetSource(cam1);
						myRobot.ArcadeDrive(-.70/(power.GetVoltage()/12), 0);
						}
						if(timer.Get() > 5.25 && timer.Get() < 5.5)
							{
							server.SetSource(cam1);
							myRobot.ArcadeDrive(0.0, -0.65/(power.GetVoltage()/12));
							}
						if(timer.Get() > 5.75 && timer.Get() < 6.5)
							{
							server.SetSource(cam1);
							shooter.Set(.83);
							myRobot.ArcadeDrive(-.935/(power.GetVoltage()/12), 0);
							}
					if(timer.Get() > 6.5 && timer.Get() < 8.5)
					{
						harvester.Set(motorspeed);
						server.SetSource(cam1);
						move = 0;
						rotation = 0;
						UpdateVision();
						if(move != 0)
						{
						myRobot.ArcadeDrive(move*1.4, rotation*.8);
						}
						else
						{
							myRobot.ArcadeDrive(0, .55);
						}

					}
					if(timer.Get() > 8.5)
						{
							server.SetSource(cam1);
							move = 0;
							rotation = 0;
							UpdateVision();
							myRobot.ArcadeDrive(move*1.08, rotation*.7);
							shooter.Set((motorspeed - .08)/(power.GetVoltage()/12.4));
						}
			}



	//Left Peg, Non Boiler side, Not Shots, just a gear run
	void FullTryGearLeft()
		{

		if(timer.Get() < .75)
				{
				server.SetSource(cam2);
				myRobot.ArcadeDrive(.755/(power.GetVoltage()/12), 0);
				}
				if(timer.Get() > .8 && timer.Get() < 1.1)
				{
					myRobot.ArcadeDrive(0, .7);

				}
				if(timer.Get() > 1.2 && timer.Get() < 4.9)
				{
					server.SetSource(cam2);
					move = 0;
					rotation = 0;
					UpdateVisionGearbox();
					myRobot.ArcadeDrive(.55, rotation*1/(power.GetVoltage()/12));
				}
				if(timer.Get() < 6.25 && timer.Get() > 5.9)
					{
					server.SetSource(cam1);
					myRobot.ArcadeDrive(-.92/(power.GetVoltage()/12), 0);
					}
				if(timer.Get() < 7.25 && timer.Get() > 6.25)
								{
								server.SetSource(cam1);
								myRobot.ArcadeDrive(-.52/(power.GetVoltage()/12), 0.65);
								}
				if(timer.Get() < 10.25 && timer.Get() > 7.25)
								{
								server.SetSource(cam1);
								myRobot.ArcadeDrive(-.52/(power.GetVoltage()/12), 0);
								}
		}

	void FullTryGearRight()
			{

				if(timer.Get() < .65)
				{
				server.SetSource(cam2);
				myRobot.ArcadeDrive(.75/(power.GetVoltage()/12), 0);
				}
				if(timer.Get() > .65 && timer.Get() < .75)
				{
					myRobot.ArcadeDrive(0, -.7);

				}
				if(timer.Get() > 1.4 && timer.Get() < 3.9)
				{
					server.SetSource(cam2);
				move = 0;
				rotation = 0;
				UpdateVisionGearbox();
				if(move == 0)
				{
					myRobot.ArcadeDrive(0, -.55);
				}
				else
				myRobot.ArcadeDrive(.55, rotation*1.2/(power.GetVoltage()/12));
				}
				if(timer.Get() < 5.25 && timer.Get() > 4.9)
					{
					server.SetSource(cam1);
					myRobot.ArcadeDrive(-.90/(power.GetVoltage()/12), 0);
					}
					if(timer.Get() > 5.25 && timer.Get() < 5.55)
						{
						server.SetSource(cam1);
						myRobot.ArcadeDrive(0.0, -0.75/(power.GetVoltage()/12));
						}
					if(timer.Get() > 5.55 && timer.Get() < 6.25)
						{
								server.SetSource(cam1);
								myRobot.ArcadeDrive(-.94/(power.GetVoltage()/12), 0);
						}
					if(timer.Get() > 6.25 && timer.Get() < 6.75)
						{
									server.SetSource(cam1);
									myRobot.ArcadeDrive(0.25, 0.55/(power.GetVoltage()/12));
						}
						if(timer.Get() > 6.75 && timer.Get() < 9.5)
							{
								server.SetSource(cam1);
								myRobot.ArcadeDrive(-.50/(power.GetVoltage()/12), 0);
							}
						if(timer.Get() > 9.5 && timer.Get() < 10)
								{
									server.SetSource(cam1);
									myRobot.ArcadeDrive(.750/(power.GetVoltage()/12), -.80);
								}
						if(timer.Get() > 11)
							{
								server.SetSource(cam1);
								myRobot.ArcadeDrive(-.30/(power.GetVoltage()/12), 0);
							}

			}




	void TeleopInit() override {
		timer.Reset();
		timer.Start();
	}

	void TeleopPeriodic() override {

		UpdateControlsMotors();
		UpdateControlsSpeed();
		UpdateCamera();
		//UpdateVision();
		//isTeleop = true;
		// Drive with arcade style (use right stick)
		if(drive == false)
		{
			myRobot.SetInvertedMotor(myRobot.kFrontLeftMotor, false); //this is inverting front left motor
			myRobot.SetInvertedMotor(myRobot.kFrontRightMotor, false); //this is inverting front right motor
			myRobot.SetInvertedMotor(myRobot.kRearLeftMotor, false); //this is inverting back left motor
			myRobot.SetInvertedMotor(myRobot.kRearRightMotor, false); //this is inverting rear right motor*/
			if(gear == false)
			{
			myRobot.ArcadeDrive(stick);
			}
			else
			myRobot.ArcadeDrive(stick.GetRawAxis(1)*.65, stick.GetRawAxis(0)*.65);

		}
		else
		{
			server.SetSource(cam2);
			move = 0;
			rotation = 0;
			UpdateVisionGearbox();
			myRobot.ArcadeDrive(.55, rotation*.8);

		}
	}


	// motorcontols
	void UpdateControlsMotors()
	{
		//Wait(1);

		//enable shooter with latch and bool
		if(stick.GetRawButton(2) && pressed == false)
		{
			pressed = true;
		if(pressed == true)
		{
			motorswitch = !motorswitch;
		}
		}
		else if(stick.GetRawButton(2) == false)
			pressed = false;

		//enable climber with latch
		if(stick.GetRawButton(3) && pressed2 == false)
			{
				pressed2 = true;
			if(pressed2 == true)
			{
				motorswitch2 = !motorswitch2;
				//Auto++;
				std::cout << Auto;
			}
			}
			else if(stick.GetRawButton(3) == false)
			pressed2 = false;

		//enable harvester
		if(stick.GetRawButton(4) && pressed3 == false)
			{
				pressed3 = true;
			if(pressed3 == true)
			{
				motorswitch3 = !motorswitch3;
				//Auto--;
				std::cout << Auto;
			}
			}
			else if(stick.GetRawButton(4) == false)
			pressed3 = false;

		if(stick.GetRawButton(6) && pressed4 == false)
				{
					pressed4 = true;
				if(pressed4 == true)
				{
					motorswitch4 = !motorswitch4;
				}
				}
			else if(stick.GetRawButton(6) == false)
				pressed4 = false;

		//determine drive direction
		if(stick.GetRawButton(1))
		{
			drive = true;
			//motorspeed += .05;
		}
		else
		{
			drive = false;
		}

		//set talon speeds
		if(motorswitch)
		{
			shooter.Set((motorspeed - .085)/(power.GetVoltage()/12));
		}
		else
		{
			shooter.Set(0);
			Auto->SetDefaultNumber("Auto", power.GetVoltage());
			//harvester.Set(motorspeed);
		}
		if(motorswitch2)
			climber.Set(motorspeed);
		else
			climber.Set(0);
		if(motorswitch3)
			harvester.Set(motorspeed);
			else
			harvester.Set(0);
		if(motorswitch4)
		{
			hoppercycle.Set(motorspeedtest);
			hooper.Set(motorspeedtest);
		}
			else
			{
			hoppercycle.Set(0);
			hooper.Set(0);
			}
	}


	//Auto Switcher
	/*void AutoControls()
	{

		if(stick.GetRawButton(2) && autoswitch == false)
		{
			 autoswitch = true;
		if( autoswitch == true)
		{
			Auto++;
		}
		}
		else if(stick.GetRawButton(2) == false)
			autoswitch = false;

		if(stick.GetRawButton(1) && autoswitch2 == false)
			{
				 autoswitch2 = true;
			if( autoswitch2 == true)
			{
				Auto--;
			}
			}
		else if(stick.GetRawButton(1) == false)
			autoswitch2 = false;

	}*/


	//pnumatics controls
	void UpdateCamera()
	{
		if(stick.GetRawButton(10) )
					{
						server.SetSource(cam2);
					}


		if(stick.GetRawButton(9))
					{
						server.SetSource(cam1);
					}
	}

	void UpdateControlsSpeed()
	{
		if(stick.GetRawButton(5) && pressed6 == false)
					{
						pressed6 = true;
					if(pressed6 == true)
					{
						motorspeedtest = 1;

					}
					}
		else if(stick.GetRawButton(5) == false)
		{
					pressed6 = false;
					motorspeedtest = -  1;
		}


		if(stick.GetRawButton(8) && pressed5 == false)
					{
						pressed5 = true;
					if(pressed5 == true)
					{
						gear = !gear;
					}
					}
		else if(stick.GetRawButton(8) == false)
					pressed5 = false;
	}


	void UpdateVisionGearbox()
		{
			std::cout << "Center Left Right";

				std::vector<double> arr = table->GetNumberArray("centerX", llvm::ArrayRef<double>());

				for(unsigned int i = 0; i < arr.size(); i++)
				{
					int a = (arr[0] + arr[1])/2;
						std::cout << arr[0] << "  ";
						std::cout << arr[1] << "  ";
						std::cout << a;
					if(arr[0] > 0)
					{
					if(a > 85)
					{
						//myRobot.Drive(0.15*((arr[0] - 80)/80) + .05,-1.0);
						rotation = 0.25*((a - 85)/73) + .4;
						std::cout << "right";
					}
					else if(a < 85)
					{
						//myRobot.Drive(0.15*((80 - arr[0])/80) +.05, 1.0);
						rotation = -0.25*((85 - (a))/85) -.4;
						std::cout << "left";
					}
					if(arr.size() > 3)
					{
						rotation = 0;
					}
				}
				/*}
				std::cout << "Center Up Down";
				std::vector<double> ary = table->GetNumberArray("centerY", llvm::ArrayRef<double>());
					for(unsigned int i = 0; i < ary.size(); i++)
					{
							std::cout << ary[0] << "  ";
									if(ary[0] > 0)
									{
									if(ary[0] > 36)
									{
										//myRobot.Drive(0.35*((ary[0] - 40)/120) + .15,0);
										move = +0.5*((ary[0] - 35)/125) + .35;
										//std::cout << move;
									}
									else if(ary[0] < 34)
									{
										//myRobot.Drive(-0.35*((40 - ary[0])/40) -.15, 0);
										move = -0.5*((35 - ary[0])/35) -.35;
										//std::cout << move;
									}
									}
									else
										move = 0;

					}*/
				std::cout << " " << timer.Get();
				std::cout << std::endl;
		}
		}




	void UpdateVisionHoppa()
			{
				std::cout << "Center Left Right";

					std::vector<double> arr = table->GetNumberArray("centerX", llvm::ArrayRef<double>());
					for(unsigned int i = 0; i < arr.size(); i++)
					{
						int a = (arr[0] + arr[1])/2;
							std::cout << arr[0] << "  ";
							std::cout << arr[1] << "  ";
							std::cout << a;
						if(arr[0] > 0)
						{
						if(a > 42)
						{
							//myRobot.Drive(0.15*((arr[0] - 80)/80) + .05,-1.0);
							rotation = 0.35*((a - 41)/119) + .35;
							std::cout << "right";
						}
						else if(a < 40)
						{
							//myRobot.Drive(0.15*((80 - arr[0])/80) +.05, 1.0);
							rotation = -0.35*((41 - (a))/41) -.35;
							std::cout << "left";
						}

					}
					}
					std::cout << "Center Up Down";
					std::vector<double> ary = table->GetNumberArray("centerY", llvm::ArrayRef<double>());
						for(unsigned int i = 0; i < ary.size(); i++)
						{
								std::cout << ary[0] << "  ";
										if(ary[0] > 0)
										{
										if(ary[0] > 100)
										{
											//myRobot.Drive(0.35*((ary[0] - 40)/120) + .15,0);
											move = 0.5*((ary[0] - 99)/61) + .35;
											//std::cout << move;
										}
										else if(ary[0] < 98)
										{
											//myRobot.Drive(-0.35*((40 - ary[0])/40) -.15, 0);
											move = -0.5*(( - ary[0])/99) -.35;
											//std::cout << move;
										}
										}
										else
											move = 0;

						}
						std::cout << " " << timer.Get();
					std::cout << std::endl;
			}





	//vision code here
	void UpdateVision()
	{
		std::cout << "Center Left Right";

			std::vector<double> ary = table->GetNumberArray("centerY", llvm::ArrayRef<double>());
			std::vector<double> arr = table->GetNumberArray("centerX", llvm::ArrayRef<double>());
			for(unsigned int i = 0; i < arr.size(); i++)
			{
				if(ary[0] < ary[1])
				{
					ary[0] = ary[1];
				}
					std::cout << arr[0] << "  ";
				if(arr[0] > 0)
				{
				if(arr[0] > 96)
				{
					//myRobot.Drive(0.15*((arr[0] - 80)/80) + .05,-1.0);
					rotation = +0.32*((arr[0] - 95)/65) + .6;
					std::cout << "right";
				}
				else if(arr[0] < 94)
				{
					//myRobot.Drive(0.15*((80 - arr[0])/80) +.05, 1.0);
					rotation = -0.32*((95 - arr[0])/95) -.6;
					std::cout << "left";
				}
				}

			}
			std::cout << "Center Up Down";
				for(unsigned int i = 0; i < ary.size(); i++)
				{
						std::cout << ary[0] << "  ";
								if(ary[0] > 0)
								{
								if(ary[0] > 19)
								{
									//myRobot.Drive(0.35*((ary[0] - 40)/120) + .15,0);
									move = -0.55*((ary[0] - 18)/142) - .3;
									//std::cout << move;
								}
								else if(ary[0] < 17)
								{
									//myRobot.Drive(-0.35*((40 - ary[0])/40) -.15, 0);
									move = +0.55*((18 - ary[0])/18) +.3;
									//std::cout << move;
								}
								}
								else
									move = 0;

					if((ary[0] - 18) < 10 && (ary[0] - 18) > -5 && (arr[0] - 95) < 8 && (arr[0] - 95) > -8)
								{
										hoppercycle.Set(-1);
										hooper.Set(-1);
								}

				}
			std::cout << " " << timer.Get();
			std::cout << std::endl;
	}


	void TestPeriodic() override {
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
