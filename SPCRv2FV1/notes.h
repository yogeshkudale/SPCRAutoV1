/*

/*
  base64 to TEXT to Hex - Function

**Commands Sent to the robots
  QlY= - BV - 4256 - Check Battery Voltage
  QzE= - C1 - 4331 - Cleaning Cycle Start
  Q0M= - CC - 4343 - Cancel Cleaning
  UFc= - PW - 5057 - PWM WHEEL GET
  UEI= - PB - 5042 - PWM Brush Get
  // QjE= - B1 - 4231 - Set Wheel PWM Value TO 150 //Deleted
  // QjI= - B2 - 4232 - Set Wheel PWM Value TO 200 //Deleted
  // QjM= - B3 - 4233 - Set Wheel PWM Value TO 250 //Deleted
  // VzE= - W1 - 5731 - Set Brush PWM Value TO 150 //Deleted
  // VzI= - W2 - 5732 - Set Brush PWM Value TO 200 //Deleted
  // VzM= - W3 - 5733 - Set Brush PWM Value TO 250 //Deleted
  SPW255 - Set WHeel Speed to 255 MAX where 255 is the speed received- Can be anything between 150 to 255
  SPB255 - Set Brush SPeed to 255 MAX where 255 is the speed received- Can be anything between 150 to 255
  QzQ= - C4 - 4334 - Move Left
  QzU= - C5 - 4335 - Move Right
  QzI= - C2 - 4332 - Clean Left
  QzM= - C3 - 4333 - Clean Right
  RDE= - D1 - 4431 - Back to Docking Station
  RDI= - D2 - 4432 - Set Left Docking Station
  RDM= - D3 - 4433 - Set Right Docking Station
  VFM= - TS - 5453 - Turn ON Test Mode
  VEM= - TC - 5443 - Turn OFF Test Mode
  VEM= - WL - 5443 - Weather Lock
  UkU= - RE - 5245 - Reset Board
  NFc= - 4W - 3457 Set the mode For 40 Watt Motors
  Nlc= - 6W - 3657 Set the mode For 60 Watt Motors
**Commands Sent From The Robots
  MD"driveCurrent"MB"brushCurrent" - Motor Current Values
  B"VALUE" - Battery Capacity Value
  C1 - Cleaning Started
  CC - Cleaning Cancelled
  DR - Docking Station Reached
  C3 -  - Right cleaning start
  C2 -  - Left cleaning start
  RS -  - Right cleaning stop
  LS -  - Left cleaning stop
  CF - Cleaning Finished
  US -  - Updated Data Sucessfully
  F1 - Fault Cause 1 - Wheel Motor Fault
  F2 - Fault Cause 2 - Brush Motor Fault
  F3 -  Fault Cause 3 - Switch Fault (Both Switch Triggerred)
  F4 - Hall effect sensor error
  F5 - Battery Low
  RD - Return to dock
  C4 - 4334 - Move Left
  C5 - 4335 - Move Right
  PW"Value" = Wheel PWM Value
  PB"Value" = Brush PWM Value
  T"Value" = Board Temperature
  AD -  At Dock


--
This is a device which performs solar panel clenaing. We are writing a firmware for a solar panel cleaning robot.
// I have a seed lora E5 connected to atmega1284 via software serial.
The motors used are DC motors which are operated with only PWM and direction signal.
Use LED's for indication and debugging purpose. You can suggest any additional features to be added.
Add Comments wherever necessary for understanding purpose.

//Pin no starts

#define SW_RX2_PIN 20
#define SW_TX2_PIN 10
const uint8_t wheel_direction = PB2; //pin to set wheel motor direction- Output - PB2
const uint8_t brush_direction = PB1; //Pin to set Brush Motor Direction- Output - PB1
const uint8_t brush_pwm = PB3; //Pin to set Brush Motor Speed - Output - PB3
const uint8_t wheel_pwm = PB4; //Pin to set Wheel Motor Speed - Output - PB4
const uint8_t leftSensor = PD4; // Pin to check left side dead end to stop motors - Input Pullup - PD4
const uint8_t rightSensor = PD5; // Pin to check right side dead end - Input Pullup - PD5
const uint8_t ledLora = PD6; //Led to debug seed lora e5 IC connected via software serial - Output - PD6
const uint8_t ledDebug = PD7; //Led to debug motor running state - Output - PD7
const uint8_t leftSwitch = PA2; //Switch to move motors to Left - Input Pullup - PA2
const uint8_t rightSwitch = PB0; // Switch to move motors to right - Input Pullup - PB0
const uint8_t batteryVoltage = PA7; // Pin to check battery voltage -Input - PA7
const uint8_t wheelMotorCurrent = PA0; // Pin to check wheel Motor Current via INA181 ic- Input - PA0
const uint8_t brushMotorCurrent = PA1; //Pin to check brush motor current via INA181 IC - Input - PA1
const uint8_t loraReset = PC3; //Pin to reset seed lora E5 IC - Output - PC3
const uint8_t ExtraLED = PC2; //PC2 //Additional LED. - PC2

//Pin Number Ends

//Communication Method
I want you to write a code based on above library in previous message to send uplink and downlink to Serial2 using class C with IN865 band.
Do not use any delay to avoid blocking of code.
please get the default device EUI from the seed lora E5 Itself and use that for setup. Use max power for radio for better range.
And do not use frame counter validation. Accept the downlink even if the frame count is mismatched. I do not want any downlink to be missed.
If the specific Received data is "ON" then turn the ledLora ON for 2 seconds.
Use port mapping directly instead of digital read/digital write to improve the speed of the firmware.
//Communication Method End

Add function
////Manual Mode
I have 2 switches and 2 hall effect sensors connected to specific pins as mentioned previously in pin numbers.
I have two motors, one for brush and one for wheel. I want to set the default speed pwm values for both the motors in the eeprom.
The PWM value is a value between 0 to 255 and for the direction pin, high is clockwise and low is anticlockwise.
Now if the the left switch is pressed, the device will start moving to left by rotating brush and wheel motors in anti-clockwise direction. First the brush motor will start with increasing its speed from 0 to the default pwm value. once the speed of the brush motor is full, then it will start the wheel motor from 0 to default pwm value.
both the motors will keep on rotating and the device will keep on moving to left untill the left sensor is triggered.
Now if the the right switch is pressed, the device will start moving to right by rotating brush and wheel motors in clockwise direction. First the brush motor will start with increasing its speed from 0 to the default pwm value. once the speed of the brush motor is full, then it will start the wheel motor from 0 to default pwm value.
both the motors will keep on rotating and the device will keep on moving to right untill the right sensor is triggered.
If both the switches are pressed at a time, then this is an error as this condition is not possible so the motors should stop and display error message in serial.
Even if both the sensors are triggered at a time then this is an error as well as this is not possible so should display an error.
This uplink should be sent to lora as well if the Lora is available on software serial.
When the motors are rotating, the current of the motors needs to be monitored. an INA181A1IDBVR is used for monitoring the current for the motors.
If the current of wheel motor goes above 1 Amps then both motors should stop displaying the error over current of wheel motor.
If the current of Brush motor goes above 2 Amps then both motors should stop displaying the error over current of brush motor.
if any of the motor is rotating, then it should keep on monitoring the motor current and send the motor current to Serial2 after each 180 Seconds.

Add function
////Automatic Mode
//Docking station Position
The docking station is the location where the robot docks in idle condition. this maybe on the left side or on the right side.
Docking station position should be defined initially and should be stored in the EEprom. The default docking station position is left.
That means if the forward motors command is given then the device should move the right rotating in clockwise direction.
For the reverse motors command, it is vice a versa.
If the docking staion is defined as to the right, forward  should move device to the left and reverse should move device to the right.

//Forward Cleaning
if the FWDC downlink is received, forward cleaning command should be executed.
in this command first check the docking station location and then depending on the location move the device in the forward direction until the forward sensor is triggered.
Speed for the motors will be the default speed. First the brush motor will ramp-up from 0 to default pwm and once that is at full speed then wheel motor will rampup from 0 to default pwm.

//Reverse Cleaning
if the RVSC downlink is received, reverse cleaning command should be executed.
in this command first check the docking station location and then depending on the location move the device in the reverse direction until the reverse sensor is triggered.
Speed for the motors will be the default speed. First the brush motor will ramp-up from 0 to default pwm and once that is at full speed then wheel motor will rampup from 0 to default pwm.

//Forward Movement
if the FWDM downlink is received, forward movement command should be executed.
in this command first check the docking station location and then depending on the location move the device in the forward direction until the forward sensor is triggered.
Speed for the motors will be different thist time. First the brush motor will ramp-up from 0 to the 70% of the default pwm and once that is at the defined speed then wheel motor will rampup from 0 to full speed or the default speed set in the eeprom.

//Reverse Movement
if the RVSM downlink is received, reverse movement command should be executed.
in this command first check the docking station location and then depending on the location move the device in the reverse direction until the reverse sensor is triggered.
Speed for the motors will be different thist time. First the brush motor will ramp-up from 0 to the 70% of the default pwm and once that is at the defined speed then wheel motor will rampup from 0 to full speed or the default speed set in the eeprom.

//Stop Motors
if "SM" command is received, both the motors will be stopped even if the sensors are not trigerred.

Add function
//Automatic Cleaning Cycle One Direction
If a Downlink "ACOD" is received then Automatic Cleaning Cycle One Direction function should be performed.
First the device will perform function Forward Cleaning untill the sensor is triggerred based on the docking station position.
Then it will wait for 5 seconds and then it will perform the function Reverse Movement. 
This means the device will perform the cleaning only in the forward direction and in the reverse direction the brush speed will be reduced to 70%.

//Automatic Cleaning Cycle Both Directions
If a Downlink "ACBD" is received then Automatic Cleaning Cycle Both Direction function should be performed.
First the device will perform function Forward Cleaning untill the sensor is triggerred based on the docking station position.
Then it will wait for 5 seconds and then it will perform the function Reverse Cleaning. 
This means the device will perform the cleaning in both the direction with the default speed.

Add function
//Disable Device
if the "DDFE" command is received, the device will be disabled and it will not operate even with the manual switches until "EDFE" downlink is received to enable the device.
When the device is disabled, it will ignore all the other downlinks except "EDFE" which is the downlink to enable the devices.
The switches input will be ignored by the device when it is disabled.

Add function
//Return to Dock
If the "RETD" command is received, a return to dock function will be called.
in this function, first if the motors are rotating then it will be stopped and then the docking station position will be checked and then robot will be moved to the docking station side untill the sensors are triggered.

//Lock Robots to Docking
If "LRTD" command is received, the robots will keep on moving towards the docking station until sensor is triggered. If in some case someone pushes the robots and moves away from docking then it will again move to docking automatically till the sensors is triggered.
This "LRTD" command can be disabled when the unlock robots from Docking station command is given which is "ULRD". Once unlocked, the robots will not return to docking station when pushed by someone unless a return to dock command is given.

Add function
//Set Default Speed for Brush Motors
If a specific number is followed by alphabet "BMS" for eg. if the downlink is "200BMS" then this should save 200 in the eeprom and set the value for default bursh PWM as 200.

//Set Default Speed for Wheel Motors
If a specific number is followed by alphabet "WMS" for eg. if the downlink is "220WMS" then this should save 220 in the eeprom and set the value for default bursh PWM as 220.

//Set Default Current Limit For Brush Motor
If a specific number is followed by alphabet "BMC" for eg. if the downlink is "1.2BMC" then this should save 1.2 in the eeprom and set the value for default bursh motor current limit as 1.2 Amps. So the device will stop moving if in any case the brush motor current goes above 1.2 Amperes

//Set Default Current Limit For Wheel Motor
If a specific number is followed by alphabet "WMC" for eg. if the downlink is "0.5WMS" then this should save 0.5 in the eeprom and set the value for default wheel motor current limit as 0.5 Amps. So the device will stop moving if in any case the wheel motor current goes above 0.5 Amperes

Add function
//Battery Voltage Measurement
A LM358-N is used for the battery voltage measurement with voltage divider of  910K resistor and 100K resistor respectively.
The battery is 12.8v LiFePO4 battery. I also want to monitor the battery state and capacity and it should send the battery percentage from 0 to 100% after each 5 minutes to Serial2.
LoraE5 also has a default voltage measurement system. Maybe you can check the supply voltage of LoraE5 to set the arduino reference voltage for accurate battery voltage measurement. Or you can use any other method if you know.


//Board Temperature measurement
The seed Lora E5 has default temprature sensor, which senses the board temprature. The temprature values of the board should be sent to Serial2 after each 4 Minutes. If the temprature goes above 50 degree Celcius then the device should stop with sending a fault altert to Serial2.

Add function
//Emergency Power OFF
If in any case, if the device power is turned off, the atmega should retain the last state should send the status of the last state to Serial2.
It should also send the cause of the last turning OFF.


Add function
//Uplinks and Updates
Robot should send updates after specific intervals.
If the sensor at dock is triggered, then it should send at dock after each 3 minutes.
if the battery voltage is increased then it should send a command charging started
If the battery is full, it should send command battery is full.
If a specific command is received by the device and a particular function is started then it should send uplink with a specific function response

*/




