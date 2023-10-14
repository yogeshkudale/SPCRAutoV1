#include <MemoryFree.h>


String inputString = "";         // a string to hold incoming data
bool stringComplete = false;  // whether the string is complete

int ret;
int enableMotorRampup = 1;
int enableMotorCurrentCheck = 1;


int last_brushPWMvalue;
int last_wheelPWMvalue;

int currentWheelPWM = 0;
int currentBrushPWM = 0;

int CW = 0;
int CCW = 1;

// //Motor delay variables
// int delay_rampup_wheel = 10;
// int delay_rampup_brush = 25;
// int delay_rampdown_brush = 5;
// int delay_rampdown_wheel = 5;


//Motor PWM Values Parameters
int brushPWMvalue; //make sure it is absolute divisor of 25 (brushPWMincrement) thus set to 250
int wheelPWMvalue;
int antiStuckBrushPWM; //Avoids Brush Getting Stucked In Wheels

// //Ramp up increment
// int brushPWMincrement = 25; //Steps at which speeds need to be reduced
// int wheelPWMincrement = 25;


//battery parameters
int vdd = 3300;
int temp;

String prevStr = "";

//Print Serial Statement Only Once
void printMessages(String str) {
if (enableSerialDebug) {
  if (str == prevStr) {}
  else {
    Serial.println(str);
    prevStr = str;
  }
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
int dockingSpeedReceived;

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
long timeIntervalJoinLora = 30000;

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


void wheelPWM1()
{
  writeStringToEEPROM(topic_speed_wheel_offset_address, "180"); //Update to EPROM
  printMessages("set wheel PWM to LOW Success");
}

void wheelPWM2()
{
  writeStringToEEPROM(topic_speed_wheel_offset_address, "220"); //Update to EPROM
  printMessages("set wheel PWM to MEDIUM Success");
}

void wheelPWM3()
{
  writeStringToEEPROM(topic_speed_wheel_offset_address, "250"); //Update to EPROM
  printMessages("set wheel PWM to HIGH Success");
}
void updateWheelSpeed()
{
  writeStringToEEPROM(topic_speed_wheel_offset_address, String(wheelSpeedReceived)); //Update to EPROM
  printMessages("Wheel Speed Setting Successful!");
  printMessages(String(wheelSpeedReceived));


}

void brushPWM1()
{
  writeStringToEEPROM(topic_speed_brush_offset_address, "180"); //Update to EPROM
  printMessages("set brush PWM to LOW Success");
}

void brushPWM2()
{
  writeStringToEEPROM(topic_speed_brush_offset_address, "220"); //Update to EPROM
  printMessages("set brush PWM to MEDIUM Success");
}

void brushPWM3()
{
  writeStringToEEPROM(topic_speed_brush_offset_address, "250"); //Update to EPROM
  printMessages("set brush PWM to HIGH Success");
}

void updateBrushSpeed()
{
  writeStringToEEPROM(topic_speed_brush_offset_address, String(brushSpeedReceived)); //Update to EPROM
  printMessages("Brush Speed Setting Successful!");
  printMessages(String(brushSpeedReceived));


}

void updateDockingSpeed()
{
  writeStringToEEPROM(topic_brushMotorDockingSpeed_address, String(dockingSpeedReceived)); //Update to EPROM
  printMessages("Brush Docking Speed Setting Successful!");
  printMessages(String(dockingSpeedReceived));


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

void saveTimestampToEEPROM(unsigned long timestamp) {
  EEPROM.put(firmwareUpdatedTimestamp, timestamp);
  // EEPROM.commit();  // Required for ESP8266/ESP32 boards
}

void updatFirmware(){

 // Get the compilation date and time as strings
  const char* compileDate = __DATE__;
  const char* compileTime = __TIME__;

  // Parse the date and time strings
  int compileYear, compileMonth, compileDay, compileHour, compileMinute, compileSecond;
  sscanf(compileDate, "%s %d %d", NULL, &compileDay, &compileYear);
  sscanf(compileTime, "%d:%d:%d", &compileHour, &compileMinute, &compileSecond);

  // Convert to Unix timestamp
  unsigned long currentTimestamp = compileSecond + compileMinute * 60UL +
                                    compileHour * 3600UL + compileDay * 86400UL +
                                    (compileMonth - 1) * 2629746UL +
                                    (compileYear - 1970) * 31536000UL;

  saveTimestampToEEPROM(currentTimestamp);
  delay(10000);  // Adjust the delay as needed

}

String unixTimestampToDateTime(unsigned long timestamp) {
  // Convert the Unix timestamp to a human-readable date and time format
  unsigned long year, month, day, hour, minute, second;

  second = timestamp % 60;
  timestamp /= 60;
  minute = timestamp % 60;
  timestamp /= 60;
  hour = timestamp % 24;
  timestamp /= 24;

  // Calculate the year, month, and day
  year = 1970;
  while (timestamp >= (365 * 4 + 1)) {
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
      if (timestamp >= 366) {
        timestamp -= 366;
        year++;
      }
    } else {
      timestamp -= 365;
      year++;
    }
  }
  month = 1;
  while (true) {
    int daysInMonth = 31;
    if (month == 4 || month == 6 || month == 9 || month == 11) {
      daysInMonth = 30;
    } else if (month == 2) {
      if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        daysInMonth = 29;
      } else {
        daysInMonth = 28;
      }
    }
    if (timestamp >= daysInMonth) {
      timestamp -= daysInMonth;
      month++;
    } else {
      break;
    }
  }
  day = timestamp + 1;  // Days are 1-based

  // Create a formatted date and time string
  char dateTimeBuffer[20];
  sprintf(dateTimeBuffer, "%02lu/%02lu/%04lu %02lu:%02lu:%02lu", month, day, year, hour, minute, second);

  return String(dateTimeBuffer);
}

void readFirmwareUpdatedDate(){

  // Retrieve the timestamp from EEPROM
  unsigned long savedTimestamp;
  EEPROM.get(firmwareUpdatedTimestamp, savedTimestamp);

  // Convert the timestamp to a human-readable format
  String formattedDateTime = unixTimestampToDateTime(savedTimestamp);

  // Display the formatted date and time
  Serial.println("Timestamp from EEPROM: " + formattedDateTime);

}



void setupWatchdog(int timerPrescaler) {
    if (timerPrescaler > 9 ) 
        timerPrescaler = 9; // limit to valid range
    
    byte bb = timerPrescaler & 7;  
    if (timerPrescaler > 7) 
        bb |= (1<<5);  // Set the special 5th bit if necessary
    
    MCUSR &= ~(1<<WDRF);
    
    // Start timed sequence
    WDTCSR |= (1<<WDCE) | (1<<WDE);
    
    // Set Watchdog settings
    WDTCSR = bb;
    WDTCSR |= _BV(WDIE);
}


void checkResetCause(){
  // Initialize Watchdog Timer
// wdt_enable(WDTO_2S);
    setupWatchdog(9); // Set the Watchdog Timer to its maximum value (about 8 seconds)


    if (MCUSR & _BV(PORF)) {
        Serial.println("Power-on Reset occurred.");
    }

    if (MCUSR & _BV(EXTRF)) {
        Serial.println("External Reset occurred.");
    }

    if (MCUSR & _BV(BORF)) {
        Serial.println("Brown-out Reset occurred.");
    }

    if (MCUSR & _BV(WDRF)) {
        Serial.println("Watchdog Reset occurred.");
    }

    MCUSR = 0;  // Clear all reset flags
}
