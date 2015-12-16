/*
 * LiftControl.h
 *
 *  Created on: Feb 19, 2015
 *      Author: Noah
 */

#ifndef SRC_LIFTCONTROL_H_
#define SRC_LIFTCONTROL_H_

#include <WPILib.h>


using namespace std;

class LiftControl{
private:
	bool running = true;
    Joystick driver;
	int upID, downID;
	bool upAxisPositive, downAxisPositive;
	SpeedController &motor;
	string inputType;
	double defaultSpeed;
	ExecutiveOrder order;

public:

	LiftControl(Joystick driver, int upButtonID, int downButtonID, double defaultSpeed, SpeedController &motor);
	LiftControl(Joystick driver, int upAxisID, int downAxisID, bool upAxisPositive, bool downAxisPositive, SpeedController &motor);
	LiftControl(ExecutiveOrder order, int upButtonID, int downButtonID, double defaultSpeed, SpeedController &motor);
	LiftControl(ExecutiveOrder order, int upAxisID, int downAxisID, bool upAxisPositive, bool downAxisPositive, SpeedController &motor);
	void start();
	void run();
	void executiveButtonControl();
	void executiveAxisControl();
	void buttonControl();
	void axisControl();
	void stop();
	bool getExecutiveButtonPressed(int toggler);
	bool getAxisPressed(int id, Joystick currentDriver, bool axisPositive);
	double buffer(int axisNum, Joystick joystickName, bool inverted, double highMargin, double lowMargin);
	double buffer(int axisNum, Joystick joystickName, bool inverted, double highMargin, double lowMargin, double scale);
	double buffer(int axisNum, Joystick joystickName, double highMargin, double lowMargin, bool isPositive);
	bool iequals(const string& a, const string& b);

};

#endif /* SRC_LIFTCONTROL_H_ */
