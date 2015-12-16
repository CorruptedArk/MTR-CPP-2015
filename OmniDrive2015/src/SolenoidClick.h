/*
 * SolenoidClick.h
 *
 *  Created on: Feb 22, 2015
 *      Author: Noah
 */

#ifndef SRC_SOLENOIDCLICK_H_
#define SRC_SOLENOIDCLICK_H_

#include "WPILib.h"
#include "ExecutiveOrder.h"

using namespace std;

class SolenoidClick{

private:

	int toggler;
    Joystick joystickName;
    Solenoid solenoid1;
    Solenoid solenoid2;
    string inputType;
    double highMargin;
    double lowMargin;
    DigitalInput switch1;
    ExecutiveOrder control = NULL;

    volatile bool running;

public:

    SolenoidClick(int toggler, Joystick joystickName, Solenoid solenoid1, Solenoid solenoid2, string inputType);
    SolenoidClick(int toggler, Joystick joystickName, Solenoid solenoid1, Solenoid solenoid2, string inputType, double highMargin, double lowMargin);
    SolenoidClick(DigitalInput switch1, Solenoid solenoid1, Solenoid solenoid2);
    SolenoidClick(int toggler, ExecutiveOrder control, Solenoid solenoid1, Solenoid solenoid2, string inputType);
    void start();
    void run();
    void buttonToggle();
    void axisToggle();
    void switchToggle();
    void executiveButtonToggle();
    void executiveAxisToggle();
    bool getExecutiveButtonPressed();
    bool getExecutiveAxisPressed();
    void stop();
    bool iequals(const string& a, const string& b);
};

#endif /* SRC_SOLENOIDCLICK_H_ */
