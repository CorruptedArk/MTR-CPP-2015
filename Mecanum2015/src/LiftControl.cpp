/*
 * LiftControl.cpp
 *
 *  Created on: Feb 19, 2015
 *      Author: Noah
 */

#include <LiftControl.h>




	bool running = true;
    Joystick driver;
	int upID, downID;
	bool upAxisPositive, downAxisPositive;
	SpeedController &motor;
	string inputType;
	double defaultSpeed;
	ExecutiveOrder order;



	/**
	 * Constructor. Uses buttons and a single operator.
	 * @param driver The operator's joystick.
	 * @param upButtonID The ID of the up button.
	 * @param downButtonID The ID of the down button.
	 * @param defaultSpeed The speed of movement from 0 to 1.
	 * @param motor The speed controller object.
	 */
	LiftControl::LiftControl(Joystick driver, int upButtonID, int downButtonID, double defaultSpeed, SpeedController &motor){
		this->driver = driver;
		this->order = NULL;
		this->upID = upButtonID;
		this->downID = downButtonID;
		this->motor = motor;
		this->inputType = "button";
		if(defaultSpeed < -1){
			defaultSpeed = -1;
		}else if(defaultSpeed > 1){
			defaultSpeed = 1;
		}
		this->defaultSpeed = abs(defaultSpeed);
		this->upAxisPositive = false;
		this->downAxisPositive = false;
	}

	/**
	 * Constructor. Uses axes and a single operator.
	 * @param driver The operator's joystick.
	 * @param upAxisID The ID of the up axis.
	 * @param downAxisID The ID of the down axis.
	 * @param upAxisPostive Is it using the positive end of the up axis?
	 * @param downAxisPositive Is it using the positive end of the up axis?
	 * @param motor The speed controller object.
	 */
	LiftControl::LiftControl(Joystick driver, int upAxisID, int downAxisID, bool upAxisPositive, bool downAxisPositive, SpeedController &motor){
		this->driver = driver;
		this->order = NULL;
		this->upID = upAxisID;
		this->downID = downAxisID;
		this->upAxisPositive = upAxisPositive;
		this->downAxisPositive = downAxisPositive;
		this->motor = motor;
		this->inputType = "axis";
		this->defaultSpeed = 0.0;
	}

	/**
	 * Constructor. Uses buttons and an ExecutiveOrder.
	 * @param order The ExecutiveOrder of two operators.
	 * @param upButtonID The ID of the up button.
	 * @param downButtonID The ID of the down button.
	 * @param defaultSpeed The speed of movement from 0 to 1.
	 * @param motor The speed controller object.
	 */
	LiftControl::LiftControl(ExecutiveOrder order, int upButtonID, int downButtonID, double defaultSpeed, SpeedController &motor){
		this->driver = NULL;
		this->order = order;
		this->upID = upButtonID;
		this->downID = downButtonID;
		this->motor = motor;
		this->inputType = "button";
		if(defaultSpeed < -1){
			defaultSpeed = -1;
		}else if(defaultSpeed > 1){
			defaultSpeed = 1;
		}
		this->defaultSpeed = abs(defaultSpeed);
		this->upAxisPositive = false;
		this->downAxisPositive = false;

	}

	/**
	 * Constructor. Uses axes and an ExecutiveOrder.
	 * @param order The ExecutiveOrder of two operators.
	 * @param upAxisID The ID of the up axis.
	 * @param downAxisID The ID of the down axis.
	 * @param upAxisPostive Is it using the positive end of the up axis?
	 * @param downAxisPositive Is it using the positive end of the up axis?
	 * @param motor The speed controller object.
	 */
	LiftControl::LiftControl(ExecutiveOrder order, int upAxisID, int downAxisID, bool upAxisPositive, bool downAxisPositive, SpeedController &motor){
		this->driver = NULL;
		this->order = order;
		this->upID = upAxisID;
		this->downID = downAxisID;
		this->upAxisPositive = upAxisPositive;
		this->downAxisPositive = downAxisPositive;
		this->motor = motor;
		this->inputType = "axis";
		this->defaultSpeed = 0.0;
	}


	void start() {
		std::thread lift(run);
	}


	void run() {
		running = true;
		if(order != NULL && iequals(inputType,"button")){
			executiveButtonControl();
		}else if(order != NULL && iequals(inputType,"axis")){
			executiveAxisControl();
		}else if(driver != NULL && iequals(inputType,"button")){
			buttonControl();
		}else if(driver != NULL && iequals(inputType,"axis")){
			axisControl();
		}else{
			cout << "Something was invalid.";
		}

	}

	/**
	 * Controls the lift with buttons and an ExecutiveOrder.
	 */
	void executiveButtonControl(){
		while(running){
			if(getExecutiveButtonPressed(upID) && !getExecutiveButtonPressed(downID)){
				motor.Set(defaultSpeed);
			}else if(!getExecutiveButtonPressed(upID) && getExecutiveButtonPressed(downID)){
				motor.Set(-defaultSpeed);

			}else{
				motor.Set(0.0);
			}


			Wait(0.005);
		}
	}

	/**
	 * Controls the lift with axes and an ExecutiveOrder.
	 */
	void executiveAxisControl(){

		while(running){
			Joystick currentDriver;

			if(order.getReleaseState()){
				currentDriver = order.congress;
			}else{
				currentDriver = order.president;
			}

			if(getAxisPressed(upID,currentDriver,upAxisPositive) && !getAxisPressed(downID,currentDriver,downAxisPositive)){
				motor.Set(buffer(upID,currentDriver,0.18,-0.18,true));
			}else if(!getAxisPressed(upID,currentDriver,upAxisPositive) && getAxisPressed(downID,currentDriver,downAxisPositive)){
				motor.Set(buffer(downID,currentDriver,0.18,-0.18,false));
			}else{
				motor.Set(0.0);
			}
			Wait(0.005);
		}
	}


	/**
	 * Controls the lift with buttons and a single driver.
	 */
	void buttonControl(){
		while(running){
			if(driver.GetRawButton(upID) && !driver.GetRawButton(downID)){
				motor.Set(defaultSpeed);
			}else if(!driver.GetRawButton(upID) && driver.GetRawButton(downID)){
				motor.Set(-defaultSpeed);
			}else{
				motor.Set(0.0);
			}


			Wait(0.005);
		}
	}

	/**
	 * Controls lift with axes and a single driver.
	 */
	void axisControl(){
		while(running){
			if(getAxisPressed(upID,driver,upAxisPositive) && !getAxisPressed(downID,driver,downAxisPositive)){
				motor.Set(buffer(upID,driver,0.18,-0.18,true));
			}else if(!getAxisPressed(upID,driver,upAxisPositive) && getAxisPressed(downID,driver,downAxisPositive)){
				motor.Set(buffer(downID,driver,0.18,-0.18,false));
			}else{
				motor.Set(0.0);
			}
			Wait(0.005);
		}
	}

	/**
	 * Stops the control loop.
	 */
	void stop(){
		running = false;
		motor.Set(0.0);
	}

	 /**
     * Uses an ExecutiveOrder object to check if the button is pressed.
     * @param toggler The toggler button ID
     * @return Whether or not the button is pressed.
     */
    bool getExecutiveButtonPressed(int toggler) {
        bool pressed = false;

        if(order.president.GetRawButton(toggler)){
            order.trap();
            pressed = true;
        }
        else if(order.congress.GetRawButton(toggler) && order.getReleaseState()){
            pressed = true;
        }


        return pressed;
    }
    /**
     * Checks if axis is pressed.
     * @param id ID of the axis
     * @param currentDriver The current driver.
     * @param axisPositive Does the axis use the positive end?
     * @return Whether or not the axis is pressed.
     */
    bool getAxisPressed(int id, Joystick currentDriver, bool axisPositive){
    	bool pressed;
    	if(buffer(id,driver,true,0.18,-0.18) > 0.0 == axisPositive){
    		pressed = true;
    	}else if(buffer(id,driver,true,0.18,-0.18) < 0.0 == !axisPositive){
    		pressed = true;
    	}else{
    		pressed = false;
    	}

    	return pressed;
    }

	/**
     * This function buffers Joystick.getRawAxis() input.
     * @param axisNum The ID for the axis of a Joystick.
     * @param joystickName The Joystick that input is coming from.
     * @param inverted Is it flipped?
     * @param highMargin The high margin of the buffer.
     * @param lowMargin The low margin of the buffer.
     * @return moveOut - The buffered axis data from joystickName.getRawAxis().
     **/
    double buffer(int axisNum, Joystick joystickName, bool inverted, double highMargin, double lowMargin) {
        double moveIn = joystickName.GetRawAxis(axisNum);
        double moveOut;
        moveOut = 0.0;

        if(moveIn >= lowMargin && moveIn <= highMargin ) {
            moveOut = 0.0;
        }
        else{
            if(inverted){
                moveOut = -moveIn;
            }
            else if(!inverted){
                moveOut = moveIn;
            }
        }

	return moveOut;
   }


    /**
     * This function buffers Joystick.getRawAxis() input.
     * @param axisNum The ID for the axis of a Joystick.
     * @param joystickName The Joystick that input is coming from.
     * @param inverted Is it flipped?
     * @param highMargin The high margin of the buffer.
     * @param lowMargin The low margin of the buffer.
     * @param scale The amount you want to divide the output by.
     * @return moveOut - The buffered axis data from joystickName.getRawAxis().
     **/
    double buffer(int axisNum, Joystick joystickName, bool inverted, double highMargin, double lowMargin, double scale) {
        double moveIn = joystickName.GetRawAxis(axisNum);
        double moveOut;
        moveOut = 0.0;

        if(moveIn >= lowMargin && moveIn <= highMargin ) {
            moveOut = 0.0;
        }
        else{
            if(inverted){
                moveOut = -moveIn;
            }
            else if(!inverted){
                moveOut = moveIn;
            }
        }

        if(scale <= 1){
            scale = 1;
        }

        moveOut = moveOut/scale;

	return moveOut;
   }

    /**
     * This function buffers Joystick.getRawAxis() input.
     * @param axisNum The ID for the axis of a Joystick.
     * @param joystickName The Joystick that input is coming from.
     * @param isPositive Is it positive?
     * @param highMargin The high margin of the buffer.
     * @param lowMargin The low margin of the buffer.
     * @return moveOut - The buffered axis data from joystickName.getRawAxis().
     **/
    double buffer(int axisNum, Joystick joystickName, double highMargin, double lowMargin, bool isPositive) {
        double moveIn = joystickName.GetRawAxis(axisNum);
        double moveOut;
        moveOut = 0.0;

        if(moveIn >= lowMargin && moveIn <= highMargin ) {
            moveOut = 0.0;
        }
        else{
            if(isPositive){
                moveOut = abs(moveIn);
            }
            else{
                moveOut = -abs(moveIn);
            }
        }

	return moveOut;
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





