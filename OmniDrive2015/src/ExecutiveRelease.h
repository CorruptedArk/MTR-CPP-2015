/*
 * ExecutiveRelease.h
 *
 *  Created on: Feb 19, 2015
 *      Author: Noah
 */

#ifndef SRC_EXECUTIVERELEASE_H_
#define SRC_EXECUTIVERELEASE_H_

#include "WPILib.h"
#include "ExecutiveOrder.h"
#include <thread>

using namespace std;

class ExecutiveRelease {
	ExecutiveOrder control;
	volatile bool running;

public:
	ExecutiveRelease(ExecutiveOrder &control);
	void start();
	void run();
	void stop();

};

#endif /* SRC_EXECUTIVERELEASE_H_ */
