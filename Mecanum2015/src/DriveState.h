/*
 * DriveState.h
 *
 *  Created on: Feb 22, 2015
 *      Author: Noah
 */

#ifndef SRC_DRIVESTATE_H_
#define SRC_DRIVESTATE_H_

#include "WPILib.h"
#include "ExecutiveOrder.h"

using namespace std;

class DriveState {

private:
	volatile bool orientation;
    bool defaultState;
    Joystick controller;
    int buttonID;
    volatile bool running = true;
    ExecutiveOrder *control = NULL;

    void setOrientation(Joystick &currentController);

public:
    DriveState(bool defaultState,Joystick &controller,int buttonID);
    DriveState(bool defaultState,ExecutiveOrder &control,int buttonID);
    bool getOrientation();
    void start();
    void run();
    void stop();
};


#endif /* SRC_DRIVESTATE_H_ */
