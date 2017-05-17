/*
 * MotorControl.h
 *
 *  Created on: May 10, 2017
 *      Author: FRED
 */

#ifndef SRC_MOTORCONTROL_H_
#define SRC_MOTORCONTROL_H_
#include "WPILib.h"

namespace MEC {

class MotorControl {
public:
	MotorControl();
	virtual ~MotorControl();
	frc::Joystick stick{0}; //joystick cont

	bool flip;//flippy up
	bool flop;//flippy down
	bool harvy;//harvester on
	bool aim;//aiming mode?

	//speeds
	double harspe = 1;
	double flipspe = .7;

	//Conch support
	bool pressed;
	bool pressed1;
	//Talon Bank
	frc::Talon harvester{5};
	frc::Talon flippy{2};
	frc::Talon FrontRight{3};
	frc::Talon FrontLeft{4};
	frc::Talon BackRight{0};
	frc::Talon BackLeft{1};

	frc::RobotDrive Drive{ FrontLeft, BackLeft, FrontRight, BackRight};



	void Reset()
	{
	aim = false;
	harvy = false;
	flip = false;
	flop = false;
	Drive.SetInvertedMotor(Drive.kFrontLeftMotor, true); //this is inverting front left motor
	Drive.SetInvertedMotor(Drive.kFrontRightMotor, true); //this is inverting front right motor
	Drive.SetInvertedMotor(Drive.kRearLeftMotor, true); //this is inverting back left motor
	Drive.SetInvertedMotor(Drive.kRearRightMotor, true); //this is inverting rear right motor
	}

	void Motorcontrols()
	{

		//a toggle for harvester
		if(stick.GetRawButton(4)&& pressed == false)
		{
			pressed = true;
			harvy = !harvy;
		}
		else if(stick.GetRawButton(4) == false)
			pressed = false;

		//a toggle for aiming
		if(stick.GetRawButton(8)&& pressed1 == false)
			{
				pressed1 = true;
				aim = !aim;
			}
			else if(stick.GetRawButton(8) == false)
				pressed1 = false;

		//flippy controls
		flip = stick.GetRawButton(5);
		flop = stick.GetRawButton(6);

	}

	void MotorSet()
	{
		if(flip)
		{
			flippy.Set(flipspe);
		}
		else if(flop)
		{
			flippy.Set(flipspe*-1);
		}
		else
			flippy.Set(0);

		if(harvy)
		{
			harvester.Set(harspe);
		}
		else
			harvester.Set(0);


		Drive.ArcadeDrive(stick);

	}



};

} /* namespace MEC */

#endif /* SRC_MOTORCONTROL_H_ */
