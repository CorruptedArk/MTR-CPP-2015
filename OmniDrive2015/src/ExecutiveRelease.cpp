/*
 * ExecutiveRelease.cpp
 *
 *  Created on: Feb 19, 2015
 *      Author: Noah
 */

#include "ExecutiveRelease.h"


class ExecutiveRelease{



	ExecutiveOrder control;
    volatile bool running;



public:
    /**
     *A constructor. This passes the ExecutiveOrder object that will be managed.
     * @param control The ExecutiveOrder object.
     */
    ExecutiveRelease(ExecutiveOrder &control){
        this->control = control;
    }

    void start(){
    	std::thread release (run);
    }

    void run() {
       running = true;
       while(running) {
          bool pressed = control.president.GetRawButton(control.releaseID);

          if(pressed){
              control.release();
              while(pressed){
                  pressed = control.president.GetRawButton(control.releaseID);
                  Wait(0.005);
              }
          }
         Wait(0.005);
       }
    }

    void stop() {
        running = false;
        control.release();
    }


};


