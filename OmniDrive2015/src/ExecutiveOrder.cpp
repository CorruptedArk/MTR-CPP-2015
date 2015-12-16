/*
 * ExecutiveOrder.cpp
 *
 *  Created on: Jan 21, 2015
 *      Author: Noah
 */
#include "ExecutiveOrder.h"


class ExecutiveOrder {
private:
	bool releaseState;
public:
    Joystick president;
    Joystick congress;
    unsigned int releaseID;


    /**
     * A constructor.
     * @param president The lead driver's controller.
     * @param congress The secondary driver's controller.
     * @param release The button deciding whether or not congress will be trapped.
     */
    ExecutiveOrder(Joystick &president, Joystick &congress, unsigned int releaseID){
        this->releaseState = true;
        this->president = president;
        this->congress = congress;
        this->releaseID = releaseID;
    }

    /**
     *Sets releaseState to false.
     */
    void trap(){
       releaseState = false;
    }

    /**
     *Sets releaseState to true.
     */
    void release(){
       releaseState = true;
    }

    /**
     *Returns the current releaseState value
     * @return releaseState
     */
    bool getReleaseState() {
       return releaseState;
    }

};



