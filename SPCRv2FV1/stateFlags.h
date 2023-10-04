
// motor function state
char curr_state = 'S'; // S for Stop, L for moving left, R for moving right
char last_state = 'S';

//Hall Sensor Flags
int leftSensorState = 0;
int rightSensorState = 0;

//Switches Flags
int leftSwitchState = 0;
int rightSwitchState = 0;
int cleanRightFlag = 0;
int cleanLeftFlag = 0;
int resetFirmwareValue = 0;


//Remote Monitoring and Operation Flags
int returnCall = 0; //Switch to 1 if want to call rebot back to initial position after pressing manual cleaning switch
int automaticMode = 0;
int batteryVoltageCheckState = 0;
int executeCycle = 0;
int stopState = 0;
int switchState = 0;
int switchMoveRight = 0;
int switchMoveLeft = 0;
int switchCleanLeftReturn = 0;
int switchCleanRightReturn = 0;
int wheelPWMGetState = 0;
int burshPWMGetState = 0;
int wheelPWM1State = 0;
int wheelPWM2State = 0;
int wheelPWM3State = 0;
int brushPWM1State = 0;
int brushPWM2State = 0;
int brushPWM3State = 0;
int moveLeftState = 0;
int moveRightState = 0;
int cleanLeftState = 0;
int cleanRightState = 0;
int returnToDockState = 0;
int leftDockState = 0;
int rightDockState = 0;
int weatherLockState = 0;
int leftDockInCycle = 0;
int rightDockInCycle = 0;
int leftDockInCycleLeft = 0;
int rightDockInCycleLeft = 0;
int leftDockInCycleRight = 0;
int rightDockInCycleRight = 0;
int msgReceivedLEDState = 0;
int ledState = 0;             // ledState used to set the LED
int motorRunningState = 0;
int moveToDock = 0;
int executeCleaning = 0;
int cleanRightMotors = 0;
int cleanLeftMotors = 0;
int testMode = 0;
int executeManualReturnCycle = 0;
int brushSpeedUpdate = 0;
int wheelSpeedUpdate = 0;
int dockingSpeedUpdate = 0;
int wheelCurrentUpdate = 0;
int brushCurrentUpdate = 0;
int brushCurrentReceived;
int wheelCurrentReceived;
int getFirmwareVersion = 0;
int deviceSerialNumberUpdate = 0;
int sendDeviceSerialNumber = 0;
//sendMessageFlags
int sendData = 0;
int sendMessage = 0;
int sendExecuteCycle = 0;
int sendCancelCleaning = 0;
int updateSpeedSend = 0;
int sendMoveLeft = 0;
int sendMoveRight = 0;
int sendCleanLeft = 0;
int sendCleanRight = 0;
int sendReturnToDock = 0;
int sendLeftDock = 0;
int sendRightDock = 0;
int sendCleaningFinished = 0;
int sendLeftDockReached = 0;
int sendRightDockReached = 0;
int sendBatteryLow = 0;
int sendBrushMotorFault = 0;
int sendWheelMotorFault = 0;
int sendSwitchFault = 0;
int sendWeatherLockSuccessful = 0;
int sendSensorFault = 0;
int sendOvertemperature = 0;
int batteryLowState = 0;
int sendAtDock = 0;
//Motor Direction Parameters
/* Start of Motor Variables */
