/*
 * SolenoidClick.cpp
 *
 *  Created on: Feb 22, 2015
 *      Author: Noah
 */

#include "SolenoidClick.h"

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
    /**
     * This constructor passes the needed objects to control the solenoid.
     * It uses a default margin if an axis is used.
     * @param toggler The ID of the button to toggle with
     * @param joystickName The joystick object used for input
     * @param solenoid1 The first solenoid
     * @param solenoid2 The second solenoid
     * @param inputType Axis or Button?
     */
    SolenoidClick(int toggler, Joystick joystickName, Solenoid solenoid1, Solenoid solenoid2, string inputType) {
        this->running = true;
        this->toggler = toggler;
        this->joystickName = joystickName;
        this->solenoid1 = solenoid1;
        this->solenoid2 = solenoid2;
        this->inputType = inputType;
        this->highMargin = 0.4;
        this->lowMargin = -0.4;
        this->switch1 = NULL;
    }

    /**
     * This constructor passes the needed objects to control the solenoid.
     * This constructor uses a custom margin.
     * @param toggler The ID of the button to toggle with
     * @param joystickName The joystick object used for input
     * @param solenoid1 The first solenoid
     * @param solenoid2 The second solenoid
     * @param inputType Axis or Button?
     * @param highMargin The high margin for the axis
     * @param lowMargin The low margin for the axis
     */
    SolenoidClick(int toggler, Joystick joystickName, Solenoid solenoid1, Solenoid solenoid2, string inputType, double highMargin, double lowMargin) {
        this->running = true;
        this->toggler = toggler;
        this->joystickName = joystickName;
        this->solenoid1 = solenoid1;
        this->solenoid2 = solenoid2;
        this->inputType = inputType;
        this->highMargin = highMargin;
        this->lowMargin = lowMargin;
        this->switch1 = NULL;
    }

    /**
     * Constructor.
     * Uses a switch to toggle the solenoid.
     * @param switch1 The DigitalInput switch.
     * @param solenoid1 The first solenoid.
     * @param solenoid2 The second solenoid.
     */
   SolenoidClick(DigitalInput switch1, Solenoid solenoid1, Solenoid solenoid2) {
        this->running = true;
        this->switch1 = switch1;
        this->solenoid1 = solenoid1;
        this->solenoid2 = solenoid2;
        this->inputType = "switch";
        this->toggler = 1;
        this->joystickName = new Joystick(10);
        this->highMargin = 0.4;
        this->lowMargin = -0.4;
    }

    /**
     * Constructor. Creates an instance of SolenoidClick that uses an ExecutiveOrder object for input.
     * @param toggler The ID of the button that will toggle the solenoids.
     * @param control The ExecutiveOrder object
     * @param solenoid1 The first solenoid
     * @param solenoid2 The second solenoid
     * @param inputType Axis or Button?
     */
    SolenoidClick(int toggler, ExecutiveOrder control, Solenoid solenoid1, Solenoid solenoid2, string inputType){
        this->running = true;
        this->toggler = toggler;
        this->control = control;
        this->joystickName = NULL;
        this->solenoid1 = solenoid1;
        this->solenoid2 = solenoid2;
        this->inputType = inputType;
        this->highMargin = 0.4;
        this->lowMargin = -0.4;
        this->switch1 = NULL;
    }


    void start(){
    	std::thread solenoidClick(run);
    }


    void run() {
        running = true;
        if(control != NULL && iequals(inputType,"button")){
            executiveButtonToggle();
        }
        else if(control != NULL && iequals(inputType,"axis")){
            executiveAxisToggle();
        }
        else if(iequals(inputType,"button")) {
            buttonToggle();
        }
        else if(iequals(inputType,"axis")) {
            axisToggle();
        }
        else if(iequals(inputType,"switch") && switch1 != NULL) {
            switchToggle();
        }
        else {
            cout << inputType + " is not a valid type of input.";
        }
    }

    /**
     * Toggles solenoids with a button.
     */
    void buttonToggle() {
        while(running) {
            bool pressed = joystickName.GetRawButton(toggler);

            if(pressed) {
                solenoid1.Set(!solenoid1.Get());
                solenoid2.Set(!solenoid2.Get());
                while(pressed) {
                    pressed = joystickName.GetRawButton(toggler);


                }
            }

        }
    }

    /**
     * Toggles solenoids with an axis.
     */
    void axisToggle() {
        while(running) {
            bool pressed;
            double axisVal = joystickName.GetRawAxis(toggler);
            pressed = axisVal >= highMargin || axisVal <= lowMargin;


            if(pressed) {
                solenoid1.Set(!solenoid1.Get());
                solenoid2.Set(!solenoid2.Get());
                while(pressed) {
                    axisVal = joystickName.GetRawAxis(toggler);
                    pressed = axisVal >= highMargin || axisVal <= lowMargin;


                }
            }

        }
    }

    /**
     * Toggles solenoids with a switch.
     */
    void switchToggle() {
        while(running) {
            bool pressed = switch1.Get();

            if(pressed) {
                solenoid1.Set(!solenoid1.Get());
                solenoid2.Set(!solenoid2.Get());
                while(pressed) {
                    pressed = switch1.Get();

                }
            }

        }


    }

    /**
     *Uses an ExecutiveOrder object and a button to toggle solenoids.
     */
    void executiveButtonToggle() {
      while(running) {
            bool pressed = getExecutiveButtonPressed();


            if(pressed) {
                solenoid1.Set(!solenoid1.Get());
                solenoid2.Set(!solenoid2.Get());
                while(pressed) {
                    pressed = getExecutiveButtonPressed();


                }
            }

        }
    }

    /**
     *Uses an ExecutiveOrder object and an axis to toggle solenoids.
     */
    void executiveAxisToggle() {
      while(running) {
        bool pressed = getExecutiveAxisPressed();


            if(pressed) {
                solenoid1.Set(!solenoid1.Get());
                solenoid2.Set(!solenoid2.Get());
                while(pressed) {
                    pressed = getExecutiveAxisPressed();


                }
            }

        }
    }

    /**
     * Uses an ExecutiveOrder object to check if the button is pressed.
     * @return pressed
     */
    bool getExecutiveButtonPressed() {
        bool pressed = false;

        if(control.president.GetRawButton(toggler)){
            control.trap();
            pressed = true;
        }
        else if(control.congress.GetRawButton(toggler) && control.getReleaseState()){
            pressed = true;
        }


        return pressed;
    }

    /**
     *Uses an ExecutiveOrder object to check if the axis is in the defined margin.
     * @return pressed
     */
    bool getExecutiveAxisPressed() {
        bool pressed = false;
        double presidentAxis = control.president.GetRawAxis(toggler);
        double congressAxis = control.congress.GetRawAxis(toggler);

        if(presidentAxis >= highMargin || presidentAxis  <= lowMargin){
            control.trap();
            pressed = true;
        }
        else if((congressAxis >= highMargin || congressAxis  <= lowMargin) && control.getReleaseState()){
            pressed = true;
        }


        return pressed;

    }

    void stop() {
        running = false;
    }

    bool iequals(const string& a, const string& b){
           unsigned int sz = a.size();
           if (b.size() != sz)
               return false;
           for (unsigned int i = 0; i < sz; ++i)
               if (tolower(a[i]) != tolower(b[i]))
                    return false;
           return true;
       }

};
