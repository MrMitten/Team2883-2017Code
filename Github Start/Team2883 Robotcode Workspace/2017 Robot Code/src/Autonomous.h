/*
 * Autonomous.h
 *
 *  Created on: Apr 20, 2017
 *      Author: FRED
 */

#ifndef SRC_AUTONOMOUS_H_
#define SRC_AUTONOMOUS_H_
#include <unistd.h>
#include <stdio.h>
#include "WPILib.h"
#include <iostream>
#include "Visioncode.h"


namespace AUT {

class Autonomous {
private:
	frc::Timer timer;
public:
	Autonomous();
	virtual ~Autonomous();

	//a vector that controls the drive chain. drive[0] is forward, drive[1] is back
	std::vector<double> drive;
	Visioncode vis;

	//The following doubles are all fairly self explanatory, and set motorspeed values for the main through doubles.
	//names represent what they control.
	double hoppercycle;
	double hooper;
	double shooter;
	double harvester;

	//value to measure current battery voltage and scale motorspeeds through current voltage.
	float volts;

	//basic function that sets the voltage through a float value that is set using the main
	void UpdateVolts(float f)
	{
		volts = f;
	}

	//starts a timer
	void Update()
	{
		timer.Reset();
		timer.Start();
	}


	//Receives a string from main, Runs autonomous
	void RUNAUTO(std::string autostring)
	{
		if(autostring == "VisionShot")
			{
				shooter=(.93);
				vis.move = 0;
				vis.rotation = 0;
				vis.UpdateVision();
				drive.assign(vis.move*1.05, vis.rotation*.5);
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
	}


	//basic function simply aims the robot and then shoots once on.
	void VisionGear()
		{

			if(vis.Shot)
			{
				hoppercycle = (-1);
				hooper = (-1);
			}

			if(timer.Get() < 7)
			{
			vis.server.SetSource(vis.cam2);
			vis.move = 0;
			vis.rotation = 0;
			vis.UpdateVisionGearbox();
			drive.assign(.5, vis.rotation*1.25);
			}
			if(timer.Get() > 7 && timer.Get() < 8)
			{
				drive.assign(-.5, 0);
			}
			if(timer.Get() > 8)
				{
				vis.server.SetSource(vis.cam2);
				vis.move = 0;
				vis.rotation = 0;
				vis.UpdateVisionGearbox();
				drive.assign(.5, vis.rotation*1.25);
				}
		}

		//drops a hopper, lines up and shoots. HoppaDropRight is this but flipped
		void HoppaDrop()
		{
			if(vis.Shot)
			{
				hoppercycle = (-1);
				hooper = (-1);
			}

			if(timer.Get() < .75)
			{
				vis.server.SetSource(vis.cam2);
				drive.assign(-.79/(volts/12), 0);
			}
			if(timer.Get() > .75 && timer.Get() < 1.125)
				{
				drive.assign(0.0, -0.65/(volts/12));
				}
			if(timer.Get() < 3.5 && timer.Get() > 1.125)
				{
				vis.move = 0;
				vis.rotation = 0;
				vis.UpdateVisionHoppa();
				drive.assign(-.55/(volts/12), vis.rotation*1.35/(volts/12));
				}
			if(timer.Get() > 3.75 && timer.Get() < 4.35)
				{
				vis.server.SetSource(vis.cam1);
				drive.assign(0.90/(volts/12), -0.625/(volts/12));
				harvester = (.95);
				//hoppercycle.Set(1);
				hooper=(1);
				}
			if(timer.Get() > 4.35 && timer.Get() < 5.25)
				{
				hoppercycle=(0);
				hooper=(0);
					vis.move = 0;
					vis.rotation = 0;
					vis.UpdateVision();

					if(vis.move != 0)
					{
						drive.assign(vis.move*1.4, vis.rotation*.8);
					}
					else
					{
						drive.assign(0, -.55/(volts/12));
					}

				}
				if(timer.Get() > 5.25 && timer.Get() < 10)
					{
					if(hoppercycle == 0)
					{
						vis.move = 0;
						vis.rotation = 0;
						vis.UpdateVision();
						drive.assign(vis.move*1.2, vis.rotation*.6);
						shooter= ((.95-.065)/(volts/12));
					}
					}
				if(timer.Get() > 10 && timer.Get() < 10.5)
				{
					//hoppercycle.Set(1);
					hooper=(1);
				}
				if(timer.Get() > 11)
				{
					//hoppercycle.Set(-1);
					hooper=(-1);
				}

		}

		//same as above but flipped
		void HoppaDropRight()
		{
			if(vis.Shot)
						{
							hoppercycle = (-1);
							hooper = (-1);
						}

						if(timer.Get() < .75)
						{
							vis.server.SetSource(vis.cam2);
							drive.assign(-.79/(volts/12), 0);
						}
						if(timer.Get() > .75 && timer.Get() < 1.125)
							{
							drive.assign(0.0, 0.65/(volts/12));
							}
						if(timer.Get() < 3.5 && timer.Get() > 1.125)
							{
							vis.move = 0;
							vis.rotation = 0;
							vis.UpdateVisionHoppa();
							drive.assign(-.55/(volts/12), vis.rotation*1.35/(volts/12));
							}
						if(timer.Get() > 3.75 && timer.Get() < 4.35)
							{
							vis.server.SetSource(vis.cam1);
							drive.assign(0.90/(volts/12), 0.625/(volts/12));
							harvester = (.95);
							//hoppercycle.Set(1);
							hooper=(1);
							}
						if(timer.Get() > 4.35 && timer.Get() < 5.25)
							{
							hoppercycle=(0);
							hooper=(0);
								vis.move = 0;
								vis.rotation = 0;
								vis.UpdateVision();

								if(vis.move != 0)
								{
									drive.assign(vis.move*1.4, vis.rotation*.8);
								}
								else
								{
									drive.assign(0, .55/(volts/12));
								}

							}
							if(timer.Get() > 5.25 && timer.Get() < 10)
								{
								if(hoppercycle == 0)
								{
									vis.move = 0;
									vis.rotation = 0;
									vis.UpdateVision();
									drive.assign(vis.move*1.2, vis.rotation*.6);
									shooter= ((.95-.065)/(volts/12));
								}
								}
							if(timer.Get() > 10 && timer.Get() < 10.5)
							{
								//hoppercycle.Set(1);
								hooper=(1);
							}
							if(timer.Get() > 11)
							{
								//hoppercycle.Set(-1);
								hooper=(-1);
							}

			}



		//Boiler left(blue side) drops a gear, drops a hopper, lines up and shoots.
		void FullTryLeft()
		{
			if(vis.Shot)
				{
					hoppercycle = (-1);
					hooper = (-1);
				}

			if(timer.Get() < .75)
			{
			vis.server.SetSource(vis.cam2);
			drive.assign(.755/(volts/12), 0);
			}
			if(timer.Get() > .8 && timer.Get() < 1.1)
			{
				drive.assign(0, .55);

			}
			if(timer.Get() > 1.2 && timer.Get() < 4.9)
			{

				vis.move = 0;
				vis.rotation = 0;
				vis.UpdateVisionGearbox();
				drive.assign(.55, vis.rotation*.9/(volts/12));
			}
			if(timer.Get() < 5.25 && timer.Get() > 4.9)
				{
				drive.assign(-.82/(volts/12), -0.35);
				}

				if(timer.Get() > 5.25 && timer.Get() < 5.5)
					{
					drive.assign(0.5, 0);
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
					vis.move = 0;
					vis.rotation = 0;
					vis.UpdateVisionHoppa();
					drive.assign(-.7/(volts/12), vis.rotation*1.35/(volts/12));
					}
				if(timer.Get() > 7.5 && timer.Get() < 8.25)
				{
					drive.assign(0.0, 0.0);
					vis.server.SetSource(vis.cam1);
				}
				if(timer.Get() > 8.25 && timer.Get() < 8.85)
				{
					drive.assign(1, -0.8);
				}
				if(timer.Get() > 8.85 && timer.Get()< 9)
				{
					drive.assign(0.9, 0.0);
					harvester=(.95);
				}
			if(timer.Get() > 9 && timer.Get() < 10)
			{
				vis.move = 0;
				vis.rotation = 0;
				vis.UpdateVision();
				if(vis.move != 0)
				{
					drive.assign(vis.move*1.4, vis.rotation*.6);
				}
				else
				{
					drive.assign(0, -.55);
				}

			}
			if(timer.Get() > 10)
				{
				if(hoppercycle == 0)
				{
					vis.move = 0;
					vis.rotation = 0;
					vis.UpdateVision();
					drive.assign(vis.move*1.1, vis.rotation*.5);
					shooter=((.95 - .115)/(volts/12.4));
				}
					else
					{
						harvester=(0);
					}
				}
		}


		//Boiler on Right, Right Peg, same as above
		void FullTryRight()
			{
			if(vis.Shot)
				{
					hoppercycle = (-1);
					hooper = (-1);
				}

					if(timer.Get() < .75)
					{
					vis.server.SetSource(vis.cam2);
					drive.assign(.755/(volts/12), 0);
					}
					if(timer.Get() > .8 && timer.Get() < 1.1)
					{
						drive.assign(0, -.55);

					}
					if(timer.Get() > 1.2 && timer.Get() < 4.9)
					{

						vis.move = 0;
						vis.rotation = 0;
						vis.UpdateVisionGearbox();
						drive.assign(.55, vis.rotation*.9/(volts/12));
					}
					if(timer.Get() < 5.25 && timer.Get() > 4.9)
						{
						drive.assign(-.82/(volts/12), 0.35);
						}

						if(timer.Get() > 5.25 && timer.Get() < 5.5)
							{
							drive.assign(0.5, 0);
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
							vis.move = 0;
							vis.rotation = 0;
							vis.UpdateVisionHoppa();
							drive.assign(-.7/(volts/12), vis.rotation*1.35/(volts/12));
							}
						if(timer.Get() > 7.5 && timer.Get() < 8.25)
						{
							drive.assign(0.0, 0.0);
							vis.server.SetSource(vis.cam1);
						}
						if(timer.Get() > 8.25 && timer.Get() < 8.85)
						{
							drive.assign(1, 0.8);
						}
						if(timer.Get() > 8.85 && timer.Get()< 9)
						{
							drive.assign(0.9, 0.0);
							harvester=(.95);
						}
					if(timer.Get() > 9 && timer.Get() < 10)
					{
						vis.move = 0;
						vis.rotation = 0;
						vis.UpdateVision();
						if(vis.move != 0)
						{
							drive.assign(vis.move*1.4, vis.rotation*.6);
						}
						else
						{
							drive.assign(0, .55);
						}

					}
					if(timer.Get() > 10)
						{
						if(hoppercycle == 0)
						{
							vis.move = 0;
							vis.rotation = 0;
							vis.UpdateVision();
							drive.assign(vis.move*1.1, vis.rotation*.5);
							shooter=((.95 - .115)/(volts/12.4));
						}
							else
							{
								harvester=(0);
							}
						}
			}


		//Boiler on Left, Middle Peg, lines up, shoots.
		void FullTryMiddleLeft()
			{

			if(vis.Shot)
						{
							hoppercycle = (-1);
							hooper = (-1);
						}

				if(timer.Get() < 4.9)
				{
					vis.server.SetSource(vis.cam2);
					vis.move = 0;
					vis.rotation = 0;
					vis.UpdateVisionGearbox();
					drive.assign(.55, vis.rotation*.9/(volts/12));
				}
				if(timer.Get() < 5.25 && timer.Get() > 4.9)
					{
					vis.server.SetSource(vis.cam1);
					drive.assign(-.70/(volts/12), 0);
					}
					if(timer.Get() > 5.25 && timer.Get() < 5.5)
						{
						drive.assign(0.0, 0.65/(volts/12));
						}
					if(timer.Get() > 5.75 && timer.Get() < 6.5)
						{
						shooter=(.83);
						drive.assign(-.935/(volts/12), 0);
						}
				if(timer.Get() > 6.5 && timer.Get() < 8.5)
				{
					harvester=(.95);
					vis.move = 0;
					vis.rotation = 0;
					vis.UpdateVision();
					if(vis.move != 0)
					{
						drive.assign(vis.move*1.4, vis.rotation*.8);
					}
					else
					{
						drive.assign(0, -.55);
					}

				}
				if(timer.Get() > 8.5)
					{
					vis.move = 0;
					vis.rotation = 0;
					vis.UpdateVision();
					drive.assign(vis.move*1.08, vis.rotation*.7);
						shooter=((.95 - .08)/(volts/12.4));
					}
			}

		//Boiler on Right, Middle Peg, same as above
		void FullTryMiddleRight()
				{


			if(vis.Shot)
						{
							hoppercycle = (-1);
							hooper = (-1);
						}

			if(timer.Get() < 4.9)
					{
						vis.server.SetSource(vis.cam2);
						vis.move = 0;
						vis.rotation = 0;
						vis.UpdateVisionGearbox();
						drive.assign(.55, vis.rotation*.9/(volts/12));
					}
					if(timer.Get() < 5.25 && timer.Get() > 4.9)
						{
						vis.server.SetSource(vis.cam1);
						drive.assign(-.70/(volts/12), 0);
						}
						if(timer.Get() > 5.25 && timer.Get() < 5.5)
							{
							drive.assign(0.0, -0.65/(volts/12));
							}
						if(timer.Get() > 5.75 && timer.Get() < 6.5)
							{
							shooter=(.83);
							drive.assign(-.935/(volts/12), 0);
							}
					if(timer.Get() > 6.5 && timer.Get() < 8.5)
					{
						harvester=(.95);
						vis.move = 0;
						vis.rotation = 0;
						vis.UpdateVision();
						if(vis.move != 0)
						{
							drive.assign(vis.move*1.4, vis.rotation*.8);
						}
						else
						{
							drive.assign(0, .55);
						}

					}
					if(timer.Get() > 8.5)
						{
						vis.move = 0;
						vis.rotation = 0;
						vis.UpdateVision();
						drive.assign(vis.move*1.08, vis.rotation*.7);
							shooter=((.95 - .08)/(volts/12.4));
						}
				}



		//Left Peg, Non Boiler side, No Shots, just a gear run
		void FullTryGearLeft()
			{

			if(timer.Get() < .75)
					{
				vis.server.SetSource(vis.cam2);
				drive.assign(.755/(volts/12), 0);
					}
					if(timer.Get() > .8 && timer.Get() < 1.1)
					{
						drive.assign(0, .7);

					}
					if(timer.Get() > 1.2 && timer.Get() < 4.9)
					{
						vis.move = 0;
						vis.rotation = 0;
						vis.UpdateVisionGearbox();
						drive.assign(.55, vis.rotation*1/(volts/12));
					}
					if(timer.Get() < 6.25 && timer.Get() > 5.9)
						{
						vis.server.SetSource(vis.cam1);
						drive.assign(-.92/(volts/12), 0);
						}
					if(timer.Get() < 7.25 && timer.Get() > 6.25)
									{
						drive.assign(-.52/(volts/12), 0.65);
									}
					if(timer.Get() < 10.25 && timer.Get() > 7.25)
									{
						drive.assign(-.52/(volts/12), 0);
									}
			}

		//right side peg, non boiler side, no shots, just a gear run
		void FullTryGearRight()
				{

			if(timer.Get() < .75)
							{
						vis.server.SetSource(vis.cam2);
						drive.assign(.755/(volts/12), 0);
							}
							if(timer.Get() > .8 && timer.Get() < 1.1)
							{
								drive.assign(0, -.7);

							}
							if(timer.Get() > 1.2 && timer.Get() < 4.9)
							{
								vis.move = 0;
								vis.rotation = 0;
								vis.UpdateVisionGearbox();
								drive.assign(.55, vis.rotation*1/(volts/12));
							}
							if(timer.Get() < 6.25 && timer.Get() > 5.9)
								{
								vis.server.SetSource(vis.cam1);
								drive.assign(-.92/(volts/12), 0);
								}
							if(timer.Get() < 7.25 && timer.Get() > 6.25)
											{
								drive.assign(-.52/(volts/12), -0.65);
											}
							if(timer.Get() < 10.25 && timer.Get() > 7.25)
											{
								drive.assign(-.52/(volts/12), 0);
											}
				}




};

} /* namespace AUT */

#endif /* SRC_AUTONOMOUS_H_ */
