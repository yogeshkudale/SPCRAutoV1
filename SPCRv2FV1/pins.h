#include <SoftwareSerial.h>
SoftwareSerial SerialLora(20, 10);//20,10

void(* resetFunc) (void) = 0;


// Pin Number Starts TAMSV1.1
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
const uint8_t driveMotorCurrent = PA0; // Pin to check wheel Motor Current via INA181 ic- Input - PA0
const uint8_t brushMotorCurrent = PA1; //Pin to check brush motor current via INA181 IC - Input - PA1
const uint8_t loraReset = PC3; //Pin to reset seed lora E5 IC - Output - PC3
const uint8_t ExtraLED = PC2; //PC2 //Additional LED. - PC2
//Pin Number Ends
