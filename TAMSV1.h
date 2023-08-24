#include <Battery.h>
#include <MemoryFree.h>;
#include <avr/wdt.h>

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

//State Variables
// State Read Variables:

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

int last_brushPWMvalue = 0;
int last_wheelPWMvalue = 0;

int currentWheelPWM = 0;
int currentBrushPWM = 0;

int CW = 0;
int CCW = 1;

// motor function state
char curr_state = 'S'; // S for Stop, L for moving left, R for moving right
char last_state = 'S';

//Motor delay variables
int delay_rampup_wheel = 10;
int delay_rampup_brush = 25;
int delay_rampdown_brush = 5;
int delay_rampdown_wheel = 5;


//Motor PWM Values Parameters
int brushPWMvalue = 255; //make sure it is absolute divisor of 25 (brushPWMincrement) thus set to 250
int wheelPWMvalue = 250;
int antiStuckBrushPWM = 210; //Avoids Brush Getting Stucked In Wheels

//Ramp up increment
int brushPWMincrement = 25; //Steps at which speeds need to be reduced
int wheelPWMincrement = 25;


//Motor Delay State
int motorDelayState = 0;

//battery parameters
int vdd = 3300;
int temp = 25;

//Motor Current Limits
//read motor currunt values

int driveMotorCurrentValue = 0;
int brushMotorCurrentValue = 0;

//Value Range 0-255. Need to callibrate on actual reading. 0 = 0 Volts, 255 = 3.3 Volts
int burshMotorCurrentLimit = 230;
int wheelMotorCurrentLimit = 210;
/* End of Motor Variables */


int overTempLimit = 48;

//Time interval to check new messages- LinkCheckReq
unsigned long previousTimeCheckMessage = millis();
long timeIntervalCheckMessage = 500;
//
////Time interval to send commands- Send Messages to server
unsigned long previousTimeCheckCommands = millis();
long timeIntervalCheckCommands = 120000;

//Time inbetween online pings // Update Battery Voltage
unsigned long previousTimeSendMessage = millis();
long timeIntervalSendMessage = 300000;

//Time inbetween online pings //sendMotorCurrent
unsigned long previousTimeSendCurrent = millis();
long timeIntervalSendCurrent = 540000;

//Time interval for retry attempts to connect network
unsigned long previousJoinLora = millis();
long timeIntervalJoinLora = 60000;

//Frequency to blink after network is connected
unsigned long previousNetworkLEDMillis = millis();
long intervalNetworkLEDMillis = 2000;

//Time inbetween online pings // Update Battery Voltage
unsigned long previousTimeCheckDock = millis();
long timeIntervalCheckDock = 300000;

//Frequency to blink after message is received
unsigned long previousMsgLEDMillis = millis();
long intervalMsgLEDMillis = 300;

//Time to blink after message is received
unsigned long previousNmsgReceivedStateMillis = millis();
long intervalmsgReceivedStateMillis = 2100;

//Frequency to blink after motor starts
unsigned long previousmotorRunningStateMillis = millis();
long intervalmotorRunningStateMillis = 500;

//Inteval to check board temperature
unsigned long previousTempMillis = millis();
long intervalTempMillis = 600000;

/* Start of Lora Variables */
static char recv_buf[512];
static bool is_exist = false;
static bool is_join = false;
/* End of Lora Variables */

String prevStr = "";

/* start of EEPROM Code */

#include<EEPROM.h> // include EEPROM library
boolean first_time_upload = false;
String default_date_string = "2021-11-28";

#define topic_timer1_offset_address 100
#define topic_timer2_offset_address 130
#define topic_timer3_offset_address 160
#define topic_speed_brush_offset_address 190
#define topic_speed_offset_address 210
#define topic_left_offset_address 240
#define topic_right_offset_address 270
#define topic_brushMotorLimit_address 300
#define topic_wheelMotorLimit_address 330

String topic_timer1_eeprom_value ;
String topic_timer2_eeprom_value ;
String topic_timer3_eeprom_value ;
String topic_left_eeprom_value ;
String topic_right_eeprom_value ;
String topic_speed_eeprom_value ;
String topic_speed_brush_eeprom_value ;
String topic_brushMotorLimit_value ;
String topic_wheelMotorLimit_value ;

int writeStringToEEPROM(int addrOffset, const String &strToWrite) {
  byte len = strToWrite.length();
  // EEPROM.write(addrOffset, len);
  EEPROM.update(addrOffset, len);
  for (int i = 0; i < len; i++) {
    // EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
    //  EEPROM.update(address, val);
    EEPROM.update(addrOffset + 1 + i, strToWrite[i]);
  }
  return addrOffset + 1 + len;
}
int readStringFromEEPROM(int addrOffset, String *strToRead) {
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0'; // !!! NOTE !!! Remove the space between the slash "/" and "0" (I've added a space because otherwise there is a display bug)
  *strToRead = String(data);
  return addrOffset + 1 + newStrLen;
}
/* end of EEPROM Code */

//Battery Voltage Check

Battery battery(10500, 13340, batteryVoltage);

String batteryVoltageRead() {

  //  Serial.println(battery.level());
  //  SerialMon.println(vbat);
  return String(battery.level());
}
int batteryVoltageInt() {

  //  Serial.println(battery.level());
  //  SerialMon.println(vbat);
  return int(battery.level());
}

/* Start of Lora Conversion */
static int at_send_check_response(char *p_ack, int timeout_ms, char *p_cmd, ...)
{
  int ch;
  int num = 0;
  int index = 0;
  int startMillis = 0;
  va_list args;
  memset(recv_buf, 0, sizeof(recv_buf));
  va_start(args, p_cmd);
  SerialLora.printf(p_cmd, args);
  Serial.printf(p_cmd, args);
  va_end(args);
  delay(200);
  startMillis = millis();

  if (p_ack == NULL)
  {
    return 0;
  }

  do
  {
    while (SerialLora.available() > 0)
    {
      ch = SerialLora.read();
      recv_buf[index++] = ch;
      Serial.print((char)ch);
      delay(2);
    }

    if (strstr(recv_buf, p_ack) != NULL)
    {
      return 1;
    }

  } while (millis() - startMillis < timeout_ms);
  return 0;
}

static void recv_prase(char *p_msg)
{
  msgReceivedLEDState = HIGH;
  //  Serial.println("In recv_prase");
  Serial.print(p_msg);
  if (p_msg == NULL)
  {
    return;
  }
  char *p_start = NULL;
  int data = 0;


  p_start = strstr(p_msg, "RX");
  if (p_start && (1 == sscanf(p_start, "RX: \"%d\"\r\n", &data)))
  {
    Serial.print("Input Received : ");
    Serial.println(data);
    if (data == 4256)
    {
      //Check battery Voltage
      batteryVoltageCheckState = 1;

    }

    else if (data == 4331)
    {
      //Cleaning Cycle Start
      executeCleaning = 1;

    }

    else if (data == 4343)
    {
      //Cancel Cleaning
      executeCycle = 0;
      switchMoveLeft = 0;
      switchMoveRight = 0;
      weatherLockState = 0;
      moveRightState = 0;
      moveLeftState = 0;
      cleanRightState = 0;
      cleanLeftState = 0;
      moveToDock = 0;
      leftDockInCycleLeft = 0;
      rightDockInCycleRight = 0;
      leftDockInCycleRight = 0;
      rightDockInCycleLeft = 0;
      stopState = 1;
      sendCancelCleaning = 1;
      sendData = 1;

    }

    else if (data == 5057)
    {
      //PWM WHEEL GET
      wheelPWMGetState = 1;
    }

    else if (data == 5042)
    {
      //PWM Brush Get
      burshPWMGetState = 1;
    }

    else if (data == 4231)
    {
      //Set Brush PWM Value TO 150
      wheelPWM1State = 1;
      updateSpeedSend = 1;
      sendData = 1;

    }

    else if (data == 4232)
    {
      //Set Brush PWM Value TO 200
      wheelPWM2State = 1;
      updateSpeedSend = 1;
      sendData = 1;

    }

    else if (data == 4233)
    {
      //Set Brush PWM Value TO 250

      wheelPWM3State = 1;
      updateSpeedSend = 1;
      sendData = 1;

    }

    else if (data == 5731)
    {
      //Set Brush PWM Value TO 150
      brushPWM1State = 1;
      updateSpeedSend = 1;
      sendData = 1;

    }

    else if (data == 5732)
    {
      //Set Brush PWM Value TO 200
      brushPWM2State = 1;
      updateSpeedSend = 1;
      sendData = 1;

    }

    else if (data == 5733)
    {
      //Set Brush PWM Value TO 250
      brushPWM3State = 1;
      updateSpeedSend = 1;
      sendData = 1;

    }

    else if (data == 4334)
    {
      //Move Left
      executeCycle = 0;
      switchMoveLeft = 0;
      switchMoveRight = 0;
      weatherLockState = 0;
      moveToDock = 0;
      stopState = 1;

      moveLeftState = 1;
      sendMoveLeft = 1;
      sendData = 1;

    }

    else if (data == 4335)
    {
      //Move Right
      executeCycle = 0;
      switchMoveLeft = 0;
      switchMoveRight = 0;
      weatherLockState = 0;
      moveToDock = 0;
      stopState = 1;

      moveRightState = 1;
      sendMoveRight = 1;
      sendData = 1;

    }

    else if (data == 4332)
    {
      //Clean Left
      executeCycle = 0;
      switchMoveLeft = 0;
      switchMoveRight = 0;
      weatherLockState = 0;
      moveToDock = 0;
      stopState = 1;

      cleanLeftState = 1;
      sendCleanLeft = 1;
      sendData = 1;

    }

    else if (data == 4333)
    {
      //Clean Right
      executeCycle = 0;
      switchMoveLeft = 0;
      switchMoveRight = 0;
      weatherLockState = 0;
      moveToDock = 0;
      stopState = 1;

      cleanRightState = 1;
      sendCleanRight = 1;
      sendData = 1;

    }

    else if (data == 4431)
    {
      //back to docking station
      returnToDockState = 1;
      sendReturnToDock = 1;
      sendData = 1;

    }

    else if (data == 4432)
    {
      //Set Left docking station
      leftDockState = 1;
      sendLeftDock = 1;
      sendData = 1;

    }

    else if (data == 5453)
    {
      //TURN ON TEST MODE
      testMode = 1;

    }
    else if (data == 5443)
    {
      //TURN OFF TEST MODE/CANCEL TEST MODE
      testMode = 0;
    }

    else if (data == 4433)
    {
      //Set right docking station
      rightDockState = 1;
      sendRightDock = 1;
      sendData = 1;

    }
    else if (data == 5245)
    {
      //Reset Firmware
    resetFirmwareValue = 1;
    }
    else if (data == 4157)
    {
      //Reset Firmware
    weatherLockState = 1;
    Serial.println("Weather Lock Successful");
    }
    else
    {
      Serial.print("Incorrect Lora Input! Please Correct The Input...");
    }
  }


}
/* End of Lora Conversion */
