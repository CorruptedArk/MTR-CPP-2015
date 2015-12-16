#include "Robot.h"

/**
 * WARNING: While it may look like a good choice to use for your code if you're inexperienced,
 * don't. Unless you know what you are doing, complex code will be much more difficult under
 * this system. Use IterativeRobot or Command-Based instead if you're new.
 */

	RobotDrive *myDrive;
	Joystick *moveStick, *shootStick;

	ExecutiveOrder control;
	ExecutiveRelease release;


	Talon *liftMotor;
	LiftControl *lift;


	DriveState *orientationSwitcher;


	SendableChooser *autoChooser;
	int autonomousID = -1;
	SendableChooser *teleChooser;
	int teleID = -1;




	//Constants for Buttons
    static const unsigned int A_BUTTON = 1;
	static const unsigned int B_BUTTON = 2;
    static const unsigned int X_BUTTON = 3;
	static const unsigned int Y_BUTTON = 4;
	static const unsigned int LEFT_BUMPER = 5;
    static const unsigned int RIGHT_BUMPER = 6;
	static const unsigned int BACK_BUTTON = 7;
	static const unsigned int START_BUTTON = 8;
	static const unsigned int LEFT_JOYSTICK_CLICK = 9;
	static const unsigned int RIGHT_JOYSTICK_CLICK = 10;

	//Constants for Axes
	static const unsigned int LEFT_X_AXIS = 0;
	static const unsigned int LEFT_Y_AXIS = 1;
	static const unsigned int LEFT_TRIGGER_AXIS = 2;
	static const unsigned int RIGHT_TRIGGER_AXIS = 3;
	static const unsigned int RIGHT_X_AXIS = 4;
	static const unsigned int RIGHT_Y_AXIS = 5;
	static const unsigned int D_PAD = 6; // Buggy, not recommended



	Robot::Robot()
	{
		myDrive = new RobotDrive(0,1,3,2);
		moveStick = new Joystick(0);
		shootStick = new Joystick(1);

		control = new ExecutiveOrder(moveStick,shootStick,Y_BUTTON);
		release = new ExecutiveRelease(control);

		liftMotor = new Talon(4);

		autoChooser = new SendableChooser();
		autoChooser->AddDefault("Auto Forward", new int(1));
		autoChooser->AddObject("Auto Sideways", new int(2));
		autoChooser->AddObject("Auto Twist", new int(3));

		teleChooser = new SendableChooser();
		teleChooser->AddDefault("Default", new int(0));
		teleChooser->AddObject("Secondary", new int(1));
		teleChooser->AddObject("Guest Driver", new int(2));

		SmartDashboard::PutData("Autonomous Chooser", autoChooser);
		SmartDashboard::PutData("TeleOp Chooser", teleChooser);
		SmartDashboard::PutNumber("Scale Down Factor", 1);

		myDrive.SetInvertedMotor(RobotDrive::MotorType::kFrontLeftMotor, true);
		myDrive.SetInvertedMotor(RobotDrive::MotorType::kRearLeftMotor, true);
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous()
	{
				autonomousID = (int)autoChooser->GetSelected();

		    	double scale = SmartDashboard::GetNumber("Scale Down Factor", 1);

		    	if(scale <= 1){
		            scale = 1;
		        }

		        switch(autonomousID) {
		            case 1:
		                Autonomous1(scale);
		                break;
		            case 2:
		                Autonomous2(scale);
		                break;
		            case 3:
		                Autonomous3(scale);
		                break;
		        }
	}

	/**
	 * Runs the motors with arcade steering.
	 */
	void OperatorControl()
	{
		myDrive.SetSafetyEnabled(true);
		        teleID = (int)teleChooser->GetSelected();

		        switch(teleID) {
		            case 0:
		                TeleOpLoop0();
		                break;
		            case 1:
		                TeleOpLoop1();
		                break;
		            case 2:
		                TeleOpLoop2();
		                break;
		        }
	}

		/**
	     * Forward driving.
	     * @param scale The amount to divide the speed by.
	     */
	void Autonomous1(double scale){
	        myDrive.SetSafetyEnabled(false);

	        myDrive.MecanumDrive_Cartesian(0.0,1.0/scale,0.0,0.0);
	        Wait(1.5);
	        myDrive.MecanumDrive_Cartesian(0.0,0.0,0.0,0.0);

	    }

	    /**
	     * Sideways driving.
	     * @param scale The amount to divide the speed by.
	     */
	 void Autonomous2(double scale){
	        myDrive.SetSafetyEnabled(false);

	        myDrive.MecanumDrive_Cartesian(1.0/scale,0.0,0.0,0.0);
	        Wait(1.5);
	        myDrive.MecanumDrive_Cartesian(0.0,0.0,0.0,0.0);

	    }

	    /**
	     * Rotation in place.
	     * @param scale The amount to divide the speed by.
	     */
	  void Autonomous3(double scale){
	        myDrive.SetSafetyEnabled(false);

	        myDrive.MecanumDrive_Cartesian(0.0,0.0,1.0/scale,0.0);
	        Wait(1.5);
	        myDrive.MecanumDrive_Cartesian(0.0,0.0,0.0,0.0);

	    }



	    /**
	     * Normal teleOp, doesn't use an ExecutiveOrder.
	     */
	  void TeleOpLoop0(){
	    	orientationSwitcher = new DriveState(true,moveStick,RIGHT_BUMPER);
	        orientationSwitcher.start();

	        lift = new LiftControl(shootStick,A_BUTTON,X_BUTTON,0.5,liftMotor);
	        lift.start();

	        double scale = SmartDashboard::GetNumber("Scale Down Factor", 1);

	        while (Robot::IsOperatorControl() && Robot::IsEnabled()) {
	            myDrive.SetSafetyEnabled(true);
	            bool inverted = orientationSwitcher.getOrientation();
	            double yMovement = Buffer(LEFT_Y_AXIS,moveStick,inverted,0.18,-0.18,scale);
	            double xMovement = Buffer(LEFT_X_AXIS,moveStick,inverted,0.18,-0.18,scale);
	            double twist = Buffer(RIGHT_X_AXIS,moveStick,inverted,0.18,-0.18,scale);
	            myDrive.MecanumDrive_Cartesian(xMovement,yMovement,twist,0.0);



	            Wait(0.01);
	        }
	        orientationSwitcher.stop();
	        lift.stop();
	    }

	    /**
	     * Restricted teleOp, only uses ExecutiveOrder to override accessories.
	     */
	  void TeleOpLoop1() {
	        release.start();

	        orientationSwitcher = new DriveState(true,moveStick,RIGHT_BUMPER);
	        orientationSwitcher.start();

	        lift = new LiftControl(control,A_BUTTON,X_BUTTON,0.5,liftMotor);
	        lift.start();

	        double scale = SmartDashboard::GetNumber("Scale Down Factor", 1);

	        while (Robot::IsOperatorControl() && Robot::IsEnabled()) {
	            myDrive.SetSafetyEnabled(true);
	            if(control.president.GetRawButton(B_BUTTON)){
	               control.trap();
	            }
	            bool inverted = orientationSwitcher.getOrientation();
	            double yMovement = Buffer(LEFT_Y_AXIS,moveStick,inverted,0.18,-0.18,scale);
	            double xMovement = Buffer(LEFT_X_AXIS,moveStick,inverted,0.18,-0.18,scale);
	            double twist = Buffer(RIGHT_X_AXIS,moveStick,inverted,0.18,-0.18,scale);
	            myDrive.MecanumDrive_Cartesian(xMovement,yMovement,twist,0.0);



	            Wait(0.01);
	        }

	        release.stop();
	        orientationSwitcher.stop();
	        lift.stop();
	    }

	    /**
	     * Guest teleOp, uses ExecutiveOrder for full system.
	     */
	  void TeleOpLoop2() {
	        release.start();

	        orientationSwitcher = new DriveState(true,control,RIGHT_BUMPER);
	        orientationSwitcher.start();

	        lift = new LiftControl(control,A_BUTTON,X_BUTTON,0.5,liftMotor);
	        lift.start();

	        double scale = SmartDashboard::GetNumber("Scale Down Factor", 1);

	        while (Robot::IsOperatorControl() && Robot::IsEnabled()) {
	        	myDrive.SetSafetyEnabled(true);
	            Joystick &currentDriver;
	            if(control.president.GetRawButton(B_BUTTON)){
	               control.trap();
	            }
	            if(control.getReleaseState()){
	                &currentDriver = control.congress;
	            }
	            else {
	                &currentDriver = control.president;
	            }
	            myDrive.SetSafetyEnabled(true);
	            bool inverted = orientationSwitcher.getOrientation();
	            double yMovement = Buffer(LEFT_Y_AXIS,currentDriver,inverted,0.18,-0.18,scale);
	            double xMovement = Buffer(LEFT_X_AXIS,currentDriver,inverted,0.18,-0.18,scale);
	            double twist = Buffer(RIGHT_X_AXIS,currentDriver,inverted,0.18,-0.18,scale);
	            myDrive.MecanumDrive_Cartesian(xMovement,yMovement,twist,0.0);



	            Wait(0.01);
	        }

	        release.stop();
	        orientationSwitcher.stop();
	        lift.stop();
	    }



	    /**
	     * This function Buffers Joystick.GetRawAxis() input.
	     * @param axisNum The ID for the axis of a Joystick.
	     * @param joystickName The Joystick that input is coming from.
	     * @param inverted Is it flipped?
	     * @param highMargin The high margin of the Buffer.
	     * @param lowMargin The low margin of the Buffer.
	     * @return moveOut - The Buffered axis data from joystickName.getRawAxis().
	     **/
	  double Buffer(int axisNum, Joystick &joystickName, bool inverted, double highMargin, double lowMargin) {
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
	     * This function Buffers Joystick.getRawAxis() input.
	     * @param axisNum The ID for the axis of a Joystick.
	     * @param joystickName The Joystick that input is coming from.
	     * @param inverted Is it flipped?
	     * @param highMargin The high margin of the Buffer.
	     * @param lowMargin The low margin of the Buffer.
	     * @param scale The amount you want to divide the output by.
	     * @return moveOut - The Buffered axis data from joystickName.getRawAxis().
	     **/
	  double Buffer(int axisNum, Joystick &joystickName, bool inverted, double highMargin, double lowMargin, double scale) {
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
	     * This function toggles the solenoids with two buttons.
	     * @param offButton ID of button to deactivate
	     * @param onButton ID of button to activate
	     * @param joystickName Name of Joystick input is coming from
	     * @param solenoid1 The first solenoid
	     * @param solenoid2 The second solenoid
	     */

	void solenoidToggle(int offButton, int onButton, Joystick &joystickName, Solenoid &solenoid1, Solenoid &solenoid2 ) {
	       bool pressedOn = joystickName.GetRawButton(onButton);
	       bool pressedOff = joystickName.GetRawButton(offButton);

	       if (pressedOn) {
	        solenoid1.Set(true);
	        solenoid2.Set(false);
	       }
	       else if (pressedOff) {
	        solenoid1.Set(false);
	        solenoid2.Set(true);
	       }

	    }

	    /**
	     * This function controls operation of a relay with a switch.
	     * @param relayName The Relay object.
	     * @param switchName1 The switch for forward motion.
	     * @param switchName2 The switch for backward motion.
	     */

	void RelayControl(Relay &relayName, DigitalInput &switchName1, DigitalInput &switchName2){

	        if(switchName1.Get() && !switchName2.Get()) {
	            relayName.Set(Relay::Value::kForward);
	        }
	        else if(!switchName1.Get() && switchName2.Get()) {
	            relayName.Set(Relay::Value::kReverse);
	        }
	        else{
	            relayName.Set(Relay::Value::kOff);
	        }
	    }

	    /**
	     * This runs the winch with an AnalogChannel senor.
	     * @param relayName The relay spike.
	     * @param sonicPing The ultrasonic sensor.
	     * @param pullBack The distance to pull back.
	     */
	  void RelayControl(Relay &relayName, AnalogInput &sonicPing, double pullBack) {


	        double pulledBack = (sonicPing.GetVoltage()/0.0048828125);

	        if(pulledBack != pullBack){
	            relayName.Set(Relay::Value::kForward);
	        }
	        else if(pulledBack == pullBack){
	            relayName.Set(Relay::Value::kOff);
	        }
	    }

	    /**
	     * This controls a relay with either axis input or two buttons.
	     * When using an axis, forward and back should be the same value.
	     * @param relayName The Relay that is being controlled.
	     * @param joystickName The joystick for input.
	     * @param forward The id for the forward button or one half of an axis.
	     * @param back The id for the back button or one half of an axis.
	     * @param type Is the input from a button or axis?
	     * @exception IllegalArgumentException() If type is invalid.
	     */
	  void RelayControl(Relay &relayName, Joystick &joystickName, int forward, int back, string type) {
	        bool pressedForward = false;
	        bool pressedBack = false;

	        if(type.compare("button") == 0) {
	           pressedForward = joystickName.GetRawButton(forward);
	           pressedBack = joystickName.GetRawButton(back);
	        }
	        else if(type.compare("axis") == 0) {
	           pressedForward = joystickName.GetRawAxis(forward) <= -0.40;
	           pressedBack = joystickName.GetRawAxis(back) >= 0.40;
	        }
	        else {

	        }

	        if(pressedForward && !pressedBack) {
	            relayName.Set(Relay::Value::kForward);
	        }
	        else if(!pressedForward && pressedBack) {
	            relayName.Set(Relay::Value::kReverse);
	        }
	        else {
	            relayName.Set(Relay::Value::kOff);
	        }
	    }


	     /**
	     * Relay control with buttons and limit switches.
	     * @param relayName The relay under control.
	     * @param joystickName The joystick controlling it.
	     * @param forward The id for the forward button or one half of an axis.
	     * @param back The id for the back button or one half of an axis.
	     * @param type Is the input from a button or axis?
	     * @param inside The switch at the inside limit.
	     * @param outside The switch on the outside limit.
	     * @exception IllegalArgumentException If type is invalid.
	     */
	   void RelayControl(Relay &relayName, Joystick &joystickName, int forward,
	             int back, string type , DigitalInput &inside, DigitalInput &outside) {
	        bool pressedForward = false;
	        bool pressedBack = false;

	        if(type.compare("button") == 0) {
	           pressedForward = joystickName.GetRawButton(forward);
	           pressedBack = joystickName.GetRawButton(back);
	        }
	        else if(type.compare("axis") == 0) {
	           pressedForward = joystickName.GetRawAxis(forward) <= -0.40;
	           pressedBack = joystickName.GetRawAxis(back) >= 0.40;
	        }
	        else {

	        }

	        if(pressedForward && !pressedBack && !outside.Get()) {
	            relayName.Set(Relay::Value::kForward);
	        }
	        else if(pressedForward && !pressedBack && outside.Get()) {
	            relayName.Set(Relay::Value::kOff);
	        }
	        else if(!pressedForward && pressedBack && !inside.Get()) {
	            relayName.Set(Relay::Value::kReverse);
	        }
	        else if(!pressedForward && pressedBack && inside.Get()) {
	            relayName.Set(Relay::Value::kOff);
	        }
	        else {
	            relayName.Set(Relay::Value::kOff);
	        }
	    }

	/**
	 * Runs during test mode
	 */
	void Test()
	{
	}


START_ROBOT_CLASS(Robot);
