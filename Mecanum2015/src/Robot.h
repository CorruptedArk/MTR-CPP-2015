/*
 * Robot.h
 *
 *  Created on: Jan 21, 2015
 *      Author: Noah
 */

#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

#include "WPILib.h"
#include "ExecutiveOrder.h"
#include "ExecutiveRelease.h"
#include "LiftControl.h"
#include "DriveState.h"
#include "SolenoidClick.h"

using namespace std;

class Robot: public SampleRobot
{
		RobotDrive myDrive;
		Talon centerDrive;
		Joystick moveStick, shootStick;

		ExecutiveOrder control;
		ExecutiveRelease release;

		Talon liftMotor;
		LiftControl lift;


		DriveState orientationSwitcher;


		SendableChooser *autoChooser;
		int autonomousID = -1;
		SendableChooser *teleChooser;
		int teleID = -1;

public:
		//Constants for Buttons
		static const unsigned int A_BUTTON;
		static const unsigned int B_BUTTON;
		static const unsigned int X_BUTTON;
		static const unsigned int Y_BUTTON;
		static const unsigned int LEFT_BUMPER;
		static const unsigned int RIGHT_BUMPER;
		static const unsigned int BACK_BUTTON;
		static const unsigned int START_BUTTON;
		static const unsigned int LEFT_JOYSTICK_CLICK;
		static const unsigned int RIGHT_JOYSTICK_CLICK;

		//Constants for Axes
		static const unsigned int LEFT_X_AXIS;
		static const unsigned int LEFT_Y_AXIS;
		static const unsigned int LEFT_TRIGGER_AXIS;
	    static const unsigned int RIGHT_TRIGGER_AXIS;
		static const unsigned int RIGHT_X_AXIS;
		static const unsigned int RIGHT_Y_AXIS;
		static const unsigned int D_PAD; // Buggy, not recommended
		Robot();
		void Autonomous();
		void OperatorControl();
		void Autonomous1(double scale);
		void Autonomous2(double scale);
		void Autonomous3(double scale);
		void TeleOpLoop0();
		void TeleOpLoop1();
		void TeleOpLoop2();
		double Buffer(int axisNum, Joystick &joystickName, bool inverted, double highMargin, double lowMargin);
		double Buffer(int axisNum, Joystick &joystickName, bool inverted, double highMargin, double lowMargin, double scale);
		void solenoidToggle(int offButton, int onButton, Joystick &joystickName, Solenoid &solenoid1, Solenoid &solenoid2 );
		void RelayControl(Relay &relayName, DigitalInput &switchName1, DigitalInput &switchName2);
		void RelayControl(Relay &relayName, AnalogInput &sonicPing, double pullBack);
		void RelayControl(Relay &relayName, Joystick &joystickName, int forward, int back, string type);
		void RelayControl(Relay &relayName, Joystick &joystickName, int forward,
			             int back, string type , DigitalInput &inside, DigitalInput &outside);
		void Test();
};

#endif /* SRC_ROBOT_H_ */
