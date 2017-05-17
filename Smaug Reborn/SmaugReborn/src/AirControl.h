/*
 * AirControl.h
 *
 *  Created on: May 10, 2017
 *      Author: FRED
 */

#ifndef SRC_AIRCONTROL_H_
#define SRC_AIRCONTROL_H_
#include "WPILib.h"

namespace MEC {

class AirControl {
public:
	AirControl();
	virtual ~AirControl();

	frc::Joystick stick{0};

	//bools for control
	bool shot;
	bool harvy;


	bool pressed;

	frc::Timer time;
	frc::Solenoid shooter {3};
	frc::Solenoid shooter2 {2};
	frc::DoubleSolenoid harvester{0, 1};

	frc::Relay compressor {0, frc::Relay::Direction::kForwardOnly};
	frc::Relay Lights {2, frc::Relay::Direction::kForwardOnly};
	frc::DigitalInput compcon{9};

	void Reset()
	{
	harvy = false;
	shot = false;
	time.Reset();
	time.Start();
	Lights.Set(frc::Relay::kOff);
	}

	void Aircontrols()
	{

		//a toggle for harvester
		if(stick.GetRawButton(1)&& pressed == false)
		{
			pressed = true;
			harvy = !harvy;
		}
		else if(stick.GetRawButton(1) == false)
		{
			pressed = false;
		}


		if(stick.GetRawButton(7)&& pressed == false)
		{
			pressed = true;
			//Lights.Set(frc::Relay::kOn);
		}
		else if(stick.GetRawButton(7) == false)
		{
			pressed = false;
			//Lights.Set(frc::Relay::kOff);
		}

		//a toggle for aiming
		if(stick.GetRawButton(3) && shot == false)
			{
				shot = true;
			}
		else if(time.Get() > .5 && shot == true)
		{
				shot = false;
				time.Reset();
		}
	}

	void AirSet()
	{

		if(harvy)
		{
		harvester.Set(frc::DoubleSolenoid::kForward);
		shooter.Set(shot);
		shooter2.Set(shot);
		}
		else
		{
		harvester.Set(frc::DoubleSolenoid::kReverse);
		}



		if(compcon.Get() == false)
		{
			compressor.Set(frc::Relay::kOn);
		}
		else
			compressor.Set(frc::Relay::kOff);
	}
};

} /* namespace MEC */

#endif /* SRC_AIRCONTROL_H_ */
