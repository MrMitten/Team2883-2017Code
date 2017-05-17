/*
 * Vision.h
 *
 *  Created on: May 11, 2017
 *      Author: FRED
 */

#ifndef SRC_VISION_H_
#define SRC_VISION_H_
#include <unistd.h>
#include <stdio.h>
#include "WPILib.h"
#include <iostream>

namespace MEC {

class Vision {
public:
	Vision();
	virtual ~Vision();

	std::shared_ptr<NetworkTable> table;
	cs::UsbCamera cam1 = frc::CameraServer::GetInstance()->StartAutomaticCapture(0);
	cs::MjpegServer server = frc::CameraServer::GetInstance()->AddServer("gripStream", 1180);

	float rotation;
	float move;
	bool Shot = false;

	void startup()
	{

		//sets multiple camera values as well as setting up the camera server.
		server.SetSource(cam1);
		cam1.SetBrightness(0);
		cam1.SetResolution(160,160);
		cam1.SetExposureManual(30);
		cam1.SetExposureHoldCurrent();
		cam1.SetWhiteBalanceManual(50);
		cam1.SetWhiteBalanceHoldCurrent();
	}

	//vision proccessing used to track the GearPeg. needs to use two array values because there are two tapes, and then it sets integer "a"
	//as the point averaged between.
	void UpdateVision()
			{
		move = 0;
		rotation = 0;


					std::cout << "Center Left Right";

					std::vector<double> arr = table->GetNumberArray("centerX", llvm::ArrayRef<double>());

						//int a = arr[0];

						if(arr[0] > 0)
						{
							int a = arr[0];

						if(a > 80)
						{
							//myRobot.Drive(0.15*((arr[0] - 80)/80) + .05,-1.0);
							rotation = 0.25*((a - 80)/80) + .4;
							std::cout << "right";
						}
						else if(a < 80)
						{
							//myRobot.Drive(0.15*((80 - arr[0])/80) +.05, 1.0);
							rotation = -0.25*((80 - (a))/80) -.4;
							std::cout << "left";
						}
						if(arr.size() > 3)
						{
							rotation = 0;
						}
					}
						else
							rotation = 0;


					std::cout << "Center Up Down";
					std::vector<double> ary = table->GetNumberArray("centerY", llvm::ArrayRef<double>());
								std::cout << ary[0] << "  ";
										if(ary[0] > 0)
										{
										if(ary[0] > 80)
										{
											//myRobot.Drive(0.35*((ary[0] - 40)/120) + .15,0);
											move = +0.5*((ary[0] - 80)/80) + .35;
											//std::cout << move;
										}
										else if(ary[0] < 80)
										{
											//myRobot.Drive(-0.35*((40 - ary[0])/40) -.15, 0);
											move = -0.5*((80 - ary[0])/80) -.35;
											//std::cout << move;
										}
										}
										else
											move = 0;

					//std::cout << " " << timer.Get();
					std::cout << std::endl;
			}




};

} /* namespace MEC */

#endif /* SRC_VISION_H_ */
