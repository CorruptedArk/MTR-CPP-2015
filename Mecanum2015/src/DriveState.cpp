/*
 * DriveState.cpp
 *
 *  Created on: Feb 22, 2015
 *      Author: Noah
 */

#include "DriveState.h"




	volatile bool orientation;
    bool defaultState;
    Joystick *controller;
    int buttonID;
    volatile bool running = true;
    ExecutiveOrder *control = NULL;

    void setOrientation(Joystick &currentController){
            bool pressed = currentController.GetRawButton(buttonID);

                if(pressed){
                    orientation = !orientation;
                    while(pressed){
                        pressed = currentController.GetRawButton(buttonID);
                        Wait(0.005);
                    }
                }
        }



    /**
     * A constructor using a single Joystick.
     * @param defaultState What state will the robot start out with?
     * @param controller the Joystick that the button is on
     * @param buttonID the ID of the button
     */
    DriveState::DriveState(bool defaultState,Joystick &controller,int buttonID){
        this->orientation = defaultState;
        this->defaultState = defaultState;
        this->controller = controller;
        this->buttonID = buttonID;

    }

    /**
     * A constructor using an ExecutiveOrder.
     * @param defaultState What state will the robot start out with?
     * @param control the ExecutiveOrder controlling the switching
     * @param buttonID the ID of the button
     */
    DriveState::DriveState(bool defaultState,ExecutiveOrder &control,int buttonID){
        this->orientation = defaultState;
        this->defaultState = defaultState;
        this->control = control;
        this->buttonID = buttonID;
        this->controller = NULL;
    }



    /**
     * Returns the orientation value.
     * @return orientation
     */

    bool getOrientation(){
        return orientation;
    }

    void start(){
    	std::thread orientationSwitcher(run);
    }

    void run(){
        running = true;
        Joystick &currentDriver;
        while(running){
            if(controller == NULL){
                if(control.getReleaseState()){
                    currentDriver = control.congress;
                }
                else {
                    currentDriver = control.president;
                }
            }else{
                currentDriver = controller;
            }
            setOrientation(currentDriver);
            Wait(0.005);
        }

    }



    void stop(){
        running = false;
        orientation = defaultState;
    }
