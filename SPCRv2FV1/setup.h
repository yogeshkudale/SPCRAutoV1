

String  Hardware = "TAMS-V1.1";
String  Firmware = "LoraTAMSV11.2";


int last_brushPWMvalue;
int last_wheelPWMvalue;

int currentWheelPWM = 0;
int currentBrushPWM = 0;

int CW = 0;
int CCW = 1;

//Motor delay variables
int delay_rampup_wheel = 10;
int delay_rampup_brush = 25;
int delay_rampdown_brush = 5;
int delay_rampdown_wheel = 5;


//Motor PWM Values Parameters
int brushPWMvalue = 250; //make sure it is absolute divisor of 25 (brushPWMincrement) thus set to 250
int wheelPWMvalue = 250;
int antiStuckBrushPWM = 180; //Avoids Brush Getting Stucked In Wheels

//Ramp up increment
int brushPWMincrement = 25; //Steps at which speeds need to be reduced
int wheelPWMincrement = 25;


//battery parameters
int vdd = 3300;
int temp = 25;

String prevStr = "";

//Print Serial Statement Only Once
void printMessages(String str) {
  if (str == prevStr) {}
  else {
    Serial.println(str);
    prevStr = str;
  }
}


void resetFirmware()
{
  printMessages("Resetting Firmware!");

  resetFunc();
}
//read motor currunt values
int driveMotorCurrentValue = 0;
int brushMotorCurrentValue = 0;

//Motor Current Limits
//Value Range 0-255. Need to callibrate on actual reading. 0 = 0 Volts, 255 = 3.3 Volts
int burshMotorCurrentLimit;
int wheelMotorCurrentLimit;

int set40WattMode = 0;
int set60WattMode = 0;
/* End of Motor Variables */
int brushSpeedReceived;
int wheelSpeedReceived;


int overTempLimit = 45;

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

//Time inbetween online pings // Update Battery Voltage
unsigned long previousTimeCheckDock = millis();
long timeIntervalCheckDock = 300000;

//Time inbetween online pings //sendMotorCurrent
unsigned long previousTimeSendCurrent = millis();
long timeIntervalSendCurrent = 540000;

//Time interval for retry attempts to connect network
unsigned long previousJoinLora = millis();
long timeIntervalJoinLora = 10000;

//Frequency to blink after network is connected
unsigned long previousNetworkLEDMillis = millis();
long intervalNetworkLEDMillis = 2000;

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


// void wheelPWM1()
// {
//   writeStringToEEPROM(topic_speed_wheel_offset_address, "180"); //Update to EPROM
//   printMessages("set wheel PWM to LOW Success");
// }

// void wheelPWM2()
// {
//   writeStringToEEPROM(topic_speed_wheel_offset_address, "220"); //Update to EPROM
//   printMessages("set wheel PWM to MEDIUM Success");
// }

// void wheelPWM3()
// {
//   writeStringToEEPROM(topic_speed_wheel_offset_address, "250"); //Update to EPROM
//   printMessages("set wheel PWM to HIGH Success");
// }
void updateWheelSpeed()
{
  writeStringToEEPROM(topic_speed_wheel_offset_address, String(wheelSpeedReceived)); //Update to EPROM
  printMessages("Wheel Speed Setting Successful!");
  printMessages(String(wheelSpeedReceived));


}

// void brushPWM1()
// {
//   writeStringToEEPROM(topic_speed_brush_offset_address, "180"); //Update to EPROM
//   printMessages("set brush PWM to LOW Success");
// }

// void brushPWM2()
// {
//   writeStringToEEPROM(topic_speed_brush_offset_address, "220"); //Update to EPROM
//   printMessages("set brush PWM to MEDIUM Success");
// }

// void brushPWM3()
// {
//   writeStringToEEPROM(topic_speed_brush_offset_address, "250"); //Update to EPROM
//   printMessages("set brush PWM to HIGH Success");
// }

void updateBrushSpeed()
{
  writeStringToEEPROM(topic_speed_brush_offset_address, String(brushSpeedReceived)); //Update to EPROM
  printMessages("Brush Speed Setting Successful!");
  printMessages(String(brushSpeedReceived));


}

void dockLeft()
{
  if (leftDockState == 1)
  {
    //code to set left docking station
    writeStringToEEPROM(topic_left_offset_address, "1");
    writeStringToEEPROM(topic_right_offset_address, "0");
    leftDockState = 0;
    printMessages("LeftDockSuccessfullySet");
  }
}
void dockRight()
{
  if (rightDockState == 1)
  {
    //code to set right docking station
    writeStringToEEPROM(topic_left_offset_address, "0");
    writeStringToEEPROM(topic_right_offset_address, "1");
    rightDockState = 0;
    printMessages("RightDockSuccessfullySet");

  }
}

float adc_to_current(int adc_value) {
  float v_ref = 3.3;
  float gain = 20.0;
  float r_shunt = 0.005;

  // Calculate the voltage at the ADC input
  float v_adc = (adc_value / 1023.0) * v_ref;

  // Calculate the voltage across the shunt resistor
  float v_shunt = v_adc / gain;

  // Calculate the current flowing through the shunt resistor
  float current = v_shunt / r_shunt;

  return current;
}