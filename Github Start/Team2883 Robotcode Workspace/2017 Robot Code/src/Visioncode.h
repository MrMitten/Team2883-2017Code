/*
 * Visioncode.h
 *
 *  Created on: Apr 20, 2017
 *      Author: FRED
 */

#ifndef SRC_VISIONCODE_H_
#define SRC_VISIONCODE_H_
#include <unistd.h>
#include <stdio.h>
#include "WPILib.h"
#include <iostream>
namespace AUT {

/*
 *
Probably the most complex code you will have to learn. This is the vision processing that runs off of the grip values recieved
from the computer. In order to understand fully you must know a few things:
1. "server" is the virtual server that reads our camera values and publishes them to grip. It does so by streaming one camera at a time
and can be switched between "cam1" and "cam2".
2. "cam1" and "cam2" are the USB cameras plugged into the roborio.
3. "table" is simply a vectored list that grabs the values from the network tables.(NetworkTables is what GRIP publishes our tracking values to,
in the form of vector arrays or "arr[1]" would equal (value x, value y) and "arr[2]" would equal (value x2, value y2) as published by GRIP in the
form of pixel values. x represents number of pixels from the left, y represents number of pixels from the top.
4. From here we use the pixel values to declare the speed of "rotation" or "move", which is a double value that is exponentially lower the closer we get to the pixel
values we wish to center on.
5. "Shot" is a simple boolean that declares when it is safe to shoot accuratly
*
*/
class Visioncode {
public:
	Visioncode();
	virtual ~Visioncode();
	std::shared_ptr<NetworkTable> table;
	cs::UsbCamera cam1 = frc::CameraServer::GetInstance()->StartAutomaticCapture(0);
	cs::UsbCamera cam2 = frc::CameraServer::GetInstance()->StartAutomaticCapture(1);
	cs::MjpegServer server = frc::CameraServer::GetInstance()->AddServer("gripStream", 1180);
	float rotation;
	float move;
	bool Shot = false;

	void startup()
	{

		//sets multiple camera values as well as setting up the camera server.
		server.SetSource(cam2);
		cam1.SetBrightness(0);
		cam1.SetResolution(160,160);
		cam1.SetExposureManual(30);
		cam1.SetExposureHoldCurrent();
		cam1.SetWhiteBalanceManual(50);
		cam1.SetWhiteBalanceHoldCurrent();
		cam2.SetBrightness(0);
		cam2.SetResolution(160,160);
		cam2.SetExposureManual(30);
		cam2.SetExposureHoldCurrent();
		cam2.SetWhiteBalanceManual(50);
		cam2.SetWhiteBalanceHoldCurrent();
	}

	//vision proccessing used to track the GearPeg. needs to use two array values because there are two tapes, and then it sets integer "a"
	//as the point averaged between.
	void UpdateVisionGearbox()
			{
				std::cout << "Center Left Right";

					std::vector<double> arr = table->GetNumberArray("centerX", llvm::ArrayRef<double>());// grabs arrays for NetworkTables

					for(unsigned int i = 0; i < arr.size(); i++)//for loop to control robot
					{
						int a = (arr[0] + arr[1])/2;//used this to find the center of two seperate values.
							std::cout << arr[0] << "  ";
							std::cout << arr[1] << "  ";
							std::cout << a;
						if(arr[0] > 0)
						{
						if(a > 85)
						{
							//myRobot.Drive(0.15*((arr[0] - 80)/80) + .05,-1.0);
							rotation = 0.25*((a - 85)/73) + .4;//math to tune speed
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
					//std::cout << " " << timer.Get();
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
							//std::cout << " " << timer.Get();
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
											Shot = true;
									}

					}
				//std::cout << " " << timer.Get();
				std::cout << std::endl;
		}



};

} /* namespace AUT */

#endif /* SRC_VISIONCODE_H_ */
