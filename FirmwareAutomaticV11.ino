
/*
  --Hardware: TAMSV1.1
  --Start Date 1 JUNE 2022
  --Updated by Yogesh Kudale On 8 AUGUST 2022 12:38
  --Refer TAMSV1_notes.h for commands and signals
  -Callibration of Motor Current Values based on actual conditions remaining.
  -It will help to stop the motors in case robot gets stucked somewhere.
  --LinkCheckReq Added to reconnect the device if the connection is lost

  996f7fbd44cb2fa22678817fdbf29883
*/

String  clientName = "";
String  deviceSerialNumber = "";
String  Hardware = "TAMS-V1.1";
String  Firmware = "LoraTAMSV10.5";

#include "TAMSV1_Methods.h"
#include "sendData.h"

void setup() {

  Serial.begin(9600);

  // Set Lora baud rate
  SerialLora.begin(9600);

  // Set your reset, enable, power pins here
  // !!!!!!!!!!!
  printMessages("TAMSV1: Initializing...");

  printMessages("Wait...");
  /* $ PinModes $ */

  DDRB |= (1 << wheel_direction) | (1 << brush_direction) | (1 << brush_pwm) | (1 << wheel_pwm); // set PB1, PB2, PB3, PB4 as output pins
  DDRA &= ~(1 << leftSwitch) & ~(1 << rightSwitch) & ~(1 << driveMotorCurrent) & ~(1 << brushMotorCurrent) & ~(1 << batteryVoltage); // set PA0, PA1, PA2, PA7 as input pins
  DDRD |= (1 << ledLora) | (1 << ledDebug); // set PD6, PD7 as output pins
  DDRD &= ~(1 << leftSensor) & ~(1 << rightSensor); // set PD4, PD5 as input pins
  DDRC |= (1 << loraReset) | (1 << ExtraLED); // set PC2, PC3 as output pins

  //  PORTA |= (1 << leftSwitch) | (1 << rightSwitch); // set PA2 and PB0 as input with pull-up
  PORTD |= (1 << leftSensor) | (1 << rightSensor); // set PD4 and PD5 as input with pull-up

  // Toggle the LED on and off three times with a 100ms delay between each transition
  for (int i = 0; i < 3; i++) {
    PORTD |= (1 << ledLora);
    _delay_ms(100);
    PORTD &= ~(1 << ledLora);
    _delay_ms(100);
  }

  printMessages("Initialized Successfully!");

  Serial.println("Hardware: " + Hardware);
  //  Serial.println(Hardware);
  Serial.println("Firmware: " + Firmware);
  //  Serial.println(Firmware);


  readEEPROM();
  //Setup to LoraWAN Server Parameters
  setupLora();
  connectLNS();


  //Battery Setup
  battery.begin(vdd, 10.1); //batterySense-master Github 3090, 10.1

  delay(10);
  // !!!!!!!!!!!
  Serial.println(vdd);
  //  batteryVoltageRead();
  Serial.print("Battery voltage is ");
  Serial.print(battery.voltage());
  Serial.print("mV (");
  Serial.print(battery.level());
  Serial.println("%)");

  //  testMode = 1; //Turn On TEST Mode
  setMotorCurrentLimits();
}

void loop() {



  // read the state of the sensor value:
  leftSensorState = (PIND & (1 << leftSensor)) >> leftSensor;
  rightSensorState = (PIND & (1 << rightSensor)) >> rightSensor;

  // read the state of the sensor value:
  leftSwitchState  = (PINA & (1 << leftSwitch)) >> leftSwitch;
  rightSwitchState  = (PINB & (1 << rightSwitch)) >> rightSwitch;

  unsigned long currentTimeJoinLNS = millis();
  unsigned long currentTimeCheckMessages = millis();
  unsigned long currentTimeSendMessages = millis();
  unsigned long currentTimeSendCurrent = millis();
  unsigned long currentTimeSendTemp = millis();
  unsigned long currentTimeCheckDock = millis();

  manualMode();
  executeManualReturnCleaning();
  executeCleaningCycle();
  returnToDock();
  weatherLockEnable();
  dockRight();
  dockLeft();
  remoteMovements();
  networkConnected();
  msgReceived();
  motorRunning();
  sendMessagestoServer();
  //Connect to LoraWAN Network Server
  if (currentTimeJoinLNS - previousJoinLora > timeIntervalJoinLora) {
    previousJoinLora = currentTimeJoinLNS;
    connectLNS();
  }

  // Check Received Message
  if (is_exist)
  {
    if (currentTimeCheckMessages - previousTimeCheckMessage > timeIntervalCheckMessage) {
      previousTimeCheckMessage = currentTimeCheckMessages;
      if (is_join == false)
      {
        int ret = at_send_check_response("+MSG:", 500, "");
        if (ret)
        {
          recv_prase(recv_buf);
        }
        else
        {
          printMessages("No Message");
        }
      }
    }
  }
  //   Sync with server Message
  if (is_exist)
  {
    if (currentTimeSendMessages - previousTimeSendMessage > timeIntervalSendMessage) {
      previousTimeSendMessage = currentTimeSendMessages;

      batteryVoltageCheck();
      int ret = at_send_check_response("+JOIN: Network joined", 12000, "AT+JOIN\r\n");
      if (ret)
      {
        is_join = false;
        printMessages("\r\n\Connected to LoraWAN Network Server\r\n\r\n");

        //SEND FIRST UPLINK
        ret = at_send_check_response("+CMSG: Done", 12000, "AT+CMSG=1\r\n");
        if (ret)
        {
          recv_prase(recv_buf);
          printMessages("First uplink send Success! Server Connection Verified.");
        }
        else
        {
          printMessages("First uplink send failed! Retrying in 30 Seconds.");
        }
      }
    }
    batteryLowState = 0;
  }


  //   SendMotorCurrent to Server
  if (is_exist)
  {
    if (motorRunningState == HIGH) {
      if (currentTimeSendCurrent - previousTimeSendCurrent > timeIntervalSendCurrent) {
        previousTimeSendCurrent = currentTimeSendCurrent;
        if (is_join == false)
        {
          sendMotorCurrent();

        }
      }
    }
  }

  //   Send Board Temperature to Server
  if (is_exist)
  {
    if (currentTimeSendTemp - previousTempMillis > intervalTempMillis) {
      previousTempMillis = currentTimeSendTemp;
      if (is_join == false)
      {
        if (at_send_check_response("+TEMP:", 1000, "AT+TEMP\r\n"))
        {
          String tempString = String(recv_buf + 7);
          String actualtemp = tempString.substring(0, 4);
          temp = actualtemp.toFloat();
          Serial.print("Board Temperature: ");
          Serial.print(temp);
          Serial.println("°C");
          char cmd[128];

          sprintf(cmd, "AT+MSG=T\%d\r\n", temp);
          at_send_check_response("+MSG: Done", 10000, cmd);
        }

        if (temp > overTempLimit)
        {
          Serial.print("Overtemperature Detected!! Stopping Motors!");
          executeCycle = 0;
          switchMoveLeft = 0;
          switchMoveRight = 0;
          weatherLockState = 0;
          moveRightState = 0;
          moveLeftState = 0;
          cleanRightState = 0;
          cleanLeftState = 0;
          moveToDock = 0;
          returnToDockState = 0;
          leftDockInCycleLeft = 0;
          rightDockInCycleRight = 0;
          leftDockInCycleRight = 0;
          rightDockInCycleLeft = 0;
          stopState = 1;
          sendOvertemperature = 1;
          sendData = 1;

        }

      }

    }
  }


  //Stop Motors/Any Current Ongoin Operation
  if (stopState == HIGH) {

    //Code to Stop Motors
    stopMotors();

  }

  //  if (batteryVoltageCheckState == HIGH) {
  //
  //    //Code to Check Battery Capacity
  //    batteryVoltageCheck();
  //    batteryVoltageCheckState = LOW;
  //
  //  }

  if (wheelPWMGetState == HIGH) {

    //Code to get PWM
    wheelPWMGet();
    wheelPWMGetState = LOW;
  }
  if (burshPWMGetState == HIGH) {

    //Code to get PWM
    burshPWMGet();
    burshPWMGetState = LOW;

  }
  if (wheelPWM1State == HIGH) {

    //Code to SET PWM1
    wheelPWM1();
    wheelPWM1State = LOW;
  }
  if (wheelPWM2State == HIGH) {

    //Code to SET PWM2
    wheelPWM2();
    wheelPWM2State = LOW;

  }
  if (wheelPWM3State == HIGH) {

    //Code to SET PWM3
    wheelPWM3();
    wheelPWM3State = LOW;
  }
  if (brushPWM1State == HIGH) {

    //Code to SET PWM1
    brushPWM1();
    brushPWM1State = LOW;
  }
  if (brushPWM2State == HIGH) {

    //Code to SET PWM2
    brushPWM2();
    brushPWM2State = LOW;
  }
  if (brushPWM3State == HIGH) {

    //Code to SET PWM3
    brushPWM3();
    brushPWM3State = LOW;

  }

  ////Boards without Motors

  //read motor currunt values
  driveMotorCurrentValue = analogRead(driveMotorCurrent);
  brushMotorCurrentValue = analogRead(brushMotorCurrent);

  if (driveMotorCurrentValue > wheelMotorCurrentLimit)
  {
    printMessages("Drive Motor Fault. Check the robot position before starting... ");
    //Cancel Everyhing
    //    testMode = 0;
    executeCycle = 0;
    switchMoveLeft = 0;
    switchMoveRight = 0;
    weatherLockState = 0;
    moveRightState = 0;
    moveLeftState = 0;
    cleanRightState = 0;
    cleanLeftState = 0;
    moveToDock = 0;
    returnToDockState = 0;
    leftDockInCycleLeft = 0;
    rightDockInCycleRight = 0;
    leftDockInCycleRight = 0;
    rightDockInCycleLeft = 0;
    stopState = 1;
    sendWheelMotorFault = 1;
    sendData = 1;

    if (cleanLeftFlag == 1 && cleanRightFlag == 0) {
      moveRightState = 1;
    }

    else if (cleanRightFlag == 1 && cleanLeftFlag == 0) {
      moveLeftState = 1;
    }
  }



  if (brushMotorCurrentValue > burshMotorCurrentLimit) //Value Range 0-1023. Need to callibrate on actual reading. 0 = 0 Volts, 1023 = 3.3 Volts
  {
    printMessages("Brush Motor Fault. Check the motors before starting...");
    //Cancel Everyhing
    //    testMode = 0;
    executeCycle = 0;
    switchMoveLeft = 0;
    switchMoveRight = 0;
    weatherLockState = 0;
    moveRightState = 0;
    moveLeftState = 0;
    cleanRightState = 0;
    cleanLeftState = 0;
    moveToDock = 0;
    returnToDockState = 0;
    leftDockInCycleLeft = 0;
    rightDockInCycleRight = 0;
    leftDockInCycleRight = 0;
    rightDockInCycleLeft = 0;
    stopState = 1;
    sendBrushMotorFault = 1;
    sendData = 1;

  }

  if (testMode == 1 && executeCycle == 0) {
    executeCleaning = 1; // Testbed Test Mode
  }


  //Check If Robot is At Dock
  if (currentTimeCheckDock - previousTimeCheckDock > timeIntervalCheckDock) {
    previousTimeCheckDock = currentTimeCheckDock;
    readStringFromEEPROM(topic_left_offset_address, &topic_left_eeprom_value);
    readStringFromEEPROM(topic_right_offset_address, &topic_right_eeprom_value);
    if (topic_left_eeprom_value == "1" && topic_right_eeprom_value == "0")
    {
      //docking at left
      if (leftSensorState == HIGH) {

      }
    }
    if (topic_left_eeprom_value == "0" && topic_right_eeprom_value == "1")
    {
      //docking at right
      if (rightSensorState == HIGH) {
        sendMessage = 1;
        sendAtDock = 1;
      }
    }
  }
  //Reset The Board
  if (resetFirmwareValue == 1) {
    resetFirmware();
  }


}
