/*
 * ExecutiveOrder.h
 *
 *  Created on: Jan 21, 2015
 *      Author: Noah
 */

#ifndef SRC_EXECUTIVEORDER_H_
#define SRC_EXECUTIVEORDER_H_

#include "WPILib.h"

using namespace std;

class ExecutiveOrder{
private:
	bool releaseState;
public:
    Joystick president;
    Joystick congress;
    unsigned int releaseID;

    ExecutiveOrder(Joystick &president, Joystick &congress,unsigned int releaseID);
    void trap();
    void release();
    bool getReleaseState();
};

#endif /* SRC_EXECUTIVEORDER_H_ */
