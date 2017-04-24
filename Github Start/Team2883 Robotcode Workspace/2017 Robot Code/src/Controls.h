/*
 * Controls.h
 *
 *  Created on: Apr 24, 2017
 *      Author: FRED
 */

#ifndef SRC_CONTROLS_H_
#define SRC_CONTROLS_H_
#include "WPILib.h"
#include "Visioncode.h"
namespace AUT {

class Controls {
public:
	Controls();
	virtual ~Controls();
	frc::Joystick stick { 0 };         // Only joystick
	//motor conrols
	double motorspeed = .95;
	double motorspeedtest = 1;
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
	bool drive = false;
	bool pressed6 = false;
	Visioncode vis;


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

		}


		//pnumatics controls
		void UpdateCamera()
		{
			if(stick.GetRawButton(10) )
						{
							vis.server.SetSource(vis.cam2);
						}


			if(stick.GetRawButton(9))
						{
							vis.server.SetSource(vis.cam1);
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



};

} /* namespace AUT */

#endif /* SRC_CONTROLS_H_ */
