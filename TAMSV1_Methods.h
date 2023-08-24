#include "TAMSV1.h"

void readEEPROM()
{
  // SerialMon.println("Reading from EEPROM");
  readStringFromEEPROM(topic_timer1_offset_address, &topic_timer1_eeprom_value);
  delay(100);
  readStringFromEEPROM(topic_timer2_offset_address, &topic_timer2_eeprom_value);
  delay(100);
  readStringFromEEPROM(topic_timer3_offset_address, &topic_timer3_eeprom_value);
  delay(100);
  readStringFromEEPROM(topic_speed_offset_address, &topic_speed_eeprom_value);
  delay(100);
  readStringFromEEPROM(topic_left_offset_address, &topic_left_eeprom_value);
  delay(100);
  readStringFromEEPROM(topic_right_offset_address, &topic_right_eeprom_value);
  delay(100);
  readStringFromEEPROM(topic_speed_brush_offset_address, &topic_speed_brush_eeprom_value);
  delay(100);
  brushPWMvalue = topic_speed_brush_eeprom_value.toInt();
  wheelPWMvalue = topic_speed_eeprom_value.toInt();
  Serial.print(F("topic_timer1_eeprom_value: ")); Serial.println(topic_timer1_eeprom_value);
  Serial.print(F("topic_timer2_eeprom_value: ")); Serial.println(topic_timer2_eeprom_value);
  Serial.print(F("topic_timer3_eeprom_value: ")); Serial.println(topic_timer3_eeprom_value);
  Serial.print(F("topic_speed_eeprom_value: ")); Serial.println(topic_speed_eeprom_value);
  Serial.print(F("topic_left_eeprom_value: ")); Serial.println(topic_left_eeprom_value);
  Serial.print(F("topic_right_eeprom_value: ")); Serial.println(topic_right_eeprom_value);
  Serial.print(F("topic_speed_brush_eeprom_value: ")); Serial.println(topic_speed_brush_eeprom_value);
  //  Serial.println("Machine Time : " + getRTCtimenow(1));
  Serial.print("Battery voltage is ");
  Serial.print(battery.voltage());
  Serial.print(" (");
  Serial.print(battery.level());
  Serial.println("%)");
}

//Print Serial Statement Only Once
void printMessages(String str) {
  if (str == prevStr) {}
  else {
    Serial.println(str);
    prevStr = str;
  }
}

void setupLora()
{
  printMessages("Initializing LoraWAN Sequence...");
  if (at_send_check_response("+AT: OK", 100, "AT\r\n"))
  {
    is_exist = true;

    at_send_check_response("+FDEFAULT: OK", 1000, "AT+FDEFAULT\r\n");

    at_send_check_response("+RTC: ZONE, +05:30", 1000, "AT+RTC=ZONE, \"+05:30\"\r\n");

    if (at_send_check_response("+TEMP:", 1000, "AT+TEMP\r\n"))
    {
      String tempString = String(recv_buf + 7);
      String actualtemp = tempString.substring(0, 4);
      temp = actualtemp.toFloat();
      Serial.print("Board Temperature: ");
      Serial.print(temp);
      Serial.println("°C");
    }

    at_send_check_response("+MODE: LWOTAA", 1000, "AT+MODE=LWOTAA\r\n");
    //    at_send_check_response("+DR: DR5", 1000, "AT+DR=5\r\n");
    at_send_check_response("+DR: IN865", 1000, "AT+DR=IN865\r\n"); //Default Channels Hz - 865062500 / 865402500 / 865985000

    at_send_check_response("+CH:3", 1000, "AT+CH=3, 865.6025,0,5\r\n");  //DRAGINO GATEWAY 865602500
    at_send_check_response("+CH:4", 1000, "AT+CH=4, 866.1850,0,5\r\n");  //DRAGINO GATEWAY 866185000
    at_send_check_response("+CH:5", 1000, "AT+CH=5, 866.7850,0,5\r\n");  //DRAGINO GATEWAY 866785000
    at_send_check_response("+CH:6", 1000, "AT+CH=6, 865.2325,0,5\r\n");    //TAYPRO WISGATE GATEWAY 865232500
    at_send_check_response("+CH:7", 1000, "AT+CH=7, 865.5725,0,5\r\n");    //TAYPRO WISGATE GATEWAY 865572500
    at_send_check_response("+CH:8", 1000, "AT+CH=8, 865.7425,0,5\r\n");    //TAYPRO WISGATE GATEWAY 865742500
    at_send_check_response("+CH:9", 1000, "AT+CH=9, 865.9125,0,5\r\n");    //TAYPRO WISGATE GATEWAY 865912500
    at_send_check_response("+CH:10", 1000, "AT+CH=10, 865.5,0,5\r\n");    //TAYPRO WISGATE GATEWAY 865500000
    at_send_check_response("+CH:11", 1000, "AT+CH=11, 866.495,0,5\r\n");    //TAYPRO WISGATE GATEWAY 866495000
    at_send_check_response("+CH:12", 1000, "AT+CH=12, 866.665,0,5\r\n");    //TAYPRO WISGATE GATEWAY 866665000
    at_send_check_response("+CH:13", 1000, "AT+CH=13, 866.835,0,5\r\n");    //TAYPRO WISGATE GATEWAY 866835000
    at_send_check_response("+CH:14", 1000, "AT+CH=14, 867,0,5\r\n");    //TAYPRO WISGATE GATEWAY 867000000
    at_send_check_response("+CH:15", 1000, "AT+CH=15, 866.155,0,5\r\n");    //TAYPRO WISGATE GATEWAY 866155000
    at_send_check_response("+ADR: ON", 1000, "AT+ADR=ON\r\n");    //TURN ON ADAPTIVE DATA RATE
    
    at_send_check_response("+CH: NUM", 1000, "AT+CH=NUM,0-15\r\n");
    at_send_check_response("+POWER: 30", 1000, "AT+POWER=30\r\n");
    at_send_check_response("+KEY: APPKEY", 1000, "AT+KEY=APPKEY,\"996f7fbd44cb2fa22678817fdbf29883\"\r\n");
    at_send_check_response("+CLASS: C", 1000, "AT+CLASS=C,SAVE\r\n");
    at_send_check_response("+PORT: 8", 1000, "AT+PORT=8\r\n");
    at_send_check_response("+RXWIN2: 865.55,DR0", 1000, "AT+RXWIN2=865.55,DR2\r\n");
      
    at_send_check_response("+RETRY: 3", 1000, "AT+RETRY=3\r\n");
    at_send_check_response("+REPT: 2", 1000, "AT+REPT=2\r\n");
    at_send_check_response("+LW: MC, OFF", 1000, "AT+LW=MC,OFF\r\n");
    at_send_check_response("+LW: SCR, ON", 1000, "AT+LW=SCR,ON\r\n");
    at_send_check_response("+LW: LDRO, AUTO", 1000, "AT+LW=LDRO,AUTO\r\n");
    printMessages("LoRaWAN Initialized.\r\n");
    at_send_check_response("+ID, AUTO", 1000, "AT+ID\r\n");
    printMessages(" \r\n");

    if (at_send_check_response("+VDD:", 1000, "AT+VDD\r\n"))
    {
      String vddString = String(recv_buf + 6);
      String actualvdd = vddString.substring(0, 4);
      vdd = actualvdd.toFloat() * 1000;
      Serial.print(vdd);
      Serial.println(" mV");
    }

    is_join = true;
    digitalWrite(ledLora, HIGH);
    automaticMode = 1;

  }
  else
  {
    is_exist = false;
    printMessages("Lora IC Not found.\r\n");
    intervalNetworkLEDMillis = 100;
    automaticMode = 0;
  }
}

void connectLNS()
{
  if (is_exist)
  {
    int ret = 0;
    if (is_join)
    {
      printMessages("\r\nConnecting to LoraWAN Network Server\r\n");
      ret = at_send_check_response("+JOIN: Network joined", 12000, "AT+JOIN\r\n");
      if (ret)
      {
        is_join = false;
        printMessages("\r\n\Connected to LoraWAN Network Server\r\n\r\n");

        //SEND FIRST UPLINK
        delay(5000);
        ret = at_send_check_response("+CMSG: Done", 12000, "AT+CMSG=1\r\n");
        if (ret)
        {
          recv_prase(recv_buf);
          printMessages("First uplink send Success! Server Connection Verified.");
          at_send_check_response("+DTR", 10000, "AT+LW=DTR\r\n");
          at_send_check_response("+TIME", 10000, "AT+RTC\r\n");
        }
        else
        {
          printMessages("First uplink send failed! Retrying in 30 Seconds.");
          at_send_check_response("+TIME", 10000, "AT+RTC\r\n");
        }
      }
      else
      {
        at_send_check_response("+ID: AppEui", 1000, "AT+ID\r\n");
        printMessages("\r\n\r\nJOIN failed!\r\n\r\n");
      }
    }
  }
}

void networkConnected()
{
  unsigned long currentLoraLEDMillis = millis();
  if (currentLoraLEDMillis - previousNetworkLEDMillis >= intervalNetworkLEDMillis && is_join == false) {
    // save the last time you blinked the LED
    previousNetworkLEDMillis = currentLoraLEDMillis;
    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    // set the LED with the ledState of the variable:
    digitalWrite(ledLora, ledState);
  }
}

void msgReceived()
{
  unsigned long currentmsgReceivedStateMillis = millis();
  if (currentmsgReceivedStateMillis - previousNmsgReceivedStateMillis >= intervalmsgReceivedStateMillis) {
    // save the last time you blinked the LED
    previousNmsgReceivedStateMillis = currentmsgReceivedStateMillis;
    msgReceivedLEDState = 0;
  }
  if (msgReceivedLEDState == HIGH) {
    unsigned long currentmsgReceivedMillis = millis();
    if (currentmsgReceivedMillis - previousMsgLEDMillis >= intervalMsgLEDMillis) {
      // save the last time you blinked the LED
      previousMsgLEDMillis = currentmsgReceivedMillis;
      // if the LED is off turn it on and vice-versa:
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
      // set the LED with the ledState of the variable:
      digitalWrite(ledLora, ledState);
    }
  }
}

void motorRunning()
{
  if (motorRunningState  == HIGH) {
    unsigned long currentmotorRunningStateMillis = millis();
    if (currentmotorRunningStateMillis - previousmotorRunningStateMillis >= intervalmotorRunningStateMillis) {
      // save the last time you blinked the LED
      previousmotorRunningStateMillis = currentmotorRunningStateMillis;
      // if the LED is off turn it on and vice-versa:
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
      // set the LED with the ledState of the variable:
      digitalWrite(ledDebug, ledState);
    }
  }


  if (motorRunningState  == LOW) {
    unsigned long currentmotorRunningStateMillis = millis();
    if (currentmotorRunningStateMillis - previousmotorRunningStateMillis >= intervalmotorRunningStateMillis) {
      // save the last time you blinked the LED
      previousmotorRunningStateMillis = currentmotorRunningStateMillis;
      // if the LED is off turn it on and vice-versa:
      if (ledState == HIGH) {
        ledState = LOW;
      } else {
        //Do Nothing
      }
      // set the LED with the ledState of the variable:
      digitalWrite(ledDebug, ledState);
    }
  }
}

void setMotorCurrentLimits() {
  readStringFromEEPROM(topic_brushMotorLimit_address, &topic_brushMotorLimit_value);
  burshMotorCurrentLimit = topic_brushMotorLimit_value.toInt();
  readStringFromEEPROM(topic_wheelMotorLimit_address, &topic_wheelMotorLimit_value);
  wheelMotorCurrentLimit = topic_wheelMotorLimit_value.toInt();
}

void cleanRight()
{
  printMessages(String(brushMotorCurrentValue));
  printMessages("Clean Right");
  readStringFromEEPROM(topic_speed_offset_address, &topic_speed_eeprom_value);
  wheelPWMvalue = topic_speed_eeprom_value.toInt();
  readStringFromEEPROM(topic_speed_brush_offset_address, &topic_speed_brush_eeprom_value);
  brushPWMvalue = topic_speed_brush_eeprom_value.toInt();
  motorRunningState  = HIGH;
  cleanRightFlag = HIGH;

  //SerialMon.println("Moving Right");
  curr_state = 'R';
  PORTB &= ~(1 << wheel_direction); // Set wheel direction to LOW (cleaning right side)
  PORTB &= ~(1 << brush_direction); // Set brush direction to LOW (cleaning right side)
  // Start Motor 2 from 0% to 100% PWM

  for (int i = currentBrushPWM; i < brushPWMvalue; i++) {
    analogWrite(brush_pwm, i);
    Serial.println(i);
    delay(15);
  }
  currentBrushPWM = brushPWMvalue;

  delay(100);
  // Start Motor 1 from 0% to 100% PWM
  for (int i = currentWheelPWM; i < wheelPWMvalue; i++) {
    analogWrite(wheel_pwm, i);
    Serial.println(i);
    delay(10);
  }
  currentWheelPWM = wheelPWMvalue;
//  Serial.println("PWM SET TO - ");
//  Serial.print(currentBrushPWM);
//  Serial.print(currentWheelPWM);

}

void cleanLeft()
{
  
  printMessages(String(brushMotorCurrentValue));
  printMessages("Clean Left");
  readStringFromEEPROM(topic_speed_offset_address, &topic_speed_eeprom_value);
  wheelPWMvalue = topic_speed_eeprom_value.toInt();
  readStringFromEEPROM(topic_speed_brush_offset_address, &topic_speed_brush_eeprom_value);
  brushPWMvalue = topic_speed_brush_eeprom_value.toInt();
  motorRunningState  = HIGH;
  cleanLeftFlag = HIGH;
  //SerialMon.println("Moving Left");
  curr_state = 'L';

  PORTB |= (1 << wheel_direction); // Set wheel direction to HIGH (cleaning left side)
  PORTB |= (1 << brush_direction); // Set brush direction to HIGH (cleaning left side)

  // Start Motor 2 from 0% to 100% PWM
  for (int i = currentBrushPWM; i < brushPWMvalue; i++) {
    analogWrite(brush_pwm, i);
    Serial.println(i);
    delay(15);
  }
  currentBrushPWM = brushPWMvalue;
  delay(100);
  // Start Motor 1 from 0% to 100% PWM
  for (int i = currentWheelPWM; i < wheelPWMvalue; i++) {
    analogWrite(wheel_pwm, i);
    Serial.println(i);
    delay(10);
  }
  currentWheelPWM = wheelPWMvalue;
//  Serial.println("PWM SET TO - ");
//  Serial.print(currentBrushPWM);
//  Serial.print(currentWheelPWM);


}

void stopMotors()
{
  readStringFromEEPROM(topic_speed_offset_address, &topic_speed_eeprom_value);
  wheelPWMvalue = topic_speed_eeprom_value.toInt();
  readStringFromEEPROM(topic_speed_brush_offset_address, &topic_speed_brush_eeprom_value);
  brushPWMvalue = topic_speed_brush_eeprom_value.toInt();
  //SerialMon.println("Both Motors Stopped");
//  printMessages("Both Motors Stopped");

  curr_state = 'S';
  // Stop Motor 1 from 100% to 0% PWM
//  for (int i = currentWheelPWM; i > 0; i--) {
//    analogWrite(wheel_pwm, i);
//    delay(1);
//    Serial.println(i);
//  }
    analogWrite(wheel_pwm, 0);

  currentWheelPWM = 0;
  delay(100);
  // Stop Motor 2 from 100% to 0% PWM
  for (int i = currentBrushPWM; i > 0; i--) {
    analogWrite(brush_pwm, i);
    Serial.println(i);
    delay(20);
  }
  currentBrushPWM = 0;
//  printMessages("PWM SET TO - ");
//  printMessages(currentBrushPWM);
//  printMessages(currentWheelPWM);

  //
  //  currentWheelPWM = 0;
  //  currentBrushPWM = 0;
  //  analogWrite(wheel_pwm, currentWheelPWM); //Stop Motors
  //  analogWrite(brush_pwm, currentBrushPWM); //Stop Motors
  //    Serial.println("Same State: Brush_PWM: " + String(last_brushPWMvalue) + "Wheel_PWM: " + String(last_wheelPWMvalue));
  motorRunningState  = LOW;
  //  printMessages("Both Motors Stopped");
  //  delay(3000);
}

void moveRight()
{
  printMessages("Move Right");
  readStringFromEEPROM(topic_speed_offset_address, &topic_speed_eeprom_value);
  wheelPWMvalue = topic_speed_eeprom_value.toInt();
  brushPWMvalue = antiStuckBrushPWM;
  motorRunningState  = HIGH;

  //SerialMon.println("Moving Left");
  curr_state = 'R';

  PORTB &= ~(1 << wheel_direction); // Set wheel direction to LOW (cleaning right side)
  PORTB &= ~(1 << brush_direction); // Set brush direction to LOW (cleaning right side)
  // Start Motor 2 from 0% to 100% PWM
  for (int i = currentBrushPWM; i < antiStuckBrushPWM; i++) {
    analogWrite(brush_pwm, i);
    Serial.println(i);
    delay(10);
  }
  currentBrushPWM = antiStuckBrushPWM;
  delay(100);
  // Start Motor 1 from 0% to 100% PWM
  for (int i = currentWheelPWM; i < wheelPWMvalue; i++) {
    analogWrite(wheel_pwm, i);
    Serial.println(i);
    delay(10);
  }
  currentWheelPWM = wheelPWMvalue;
//  Serial.println("PWM SET TO - ");
//  Serial.print(currentBrushPWM);
//  Serial.print(currentWheelPWM);

}

void moveLeft()
{
  printMessages("Move Left");
  readStringFromEEPROM(topic_speed_offset_address, &topic_speed_eeprom_value);
  wheelPWMvalue = topic_speed_eeprom_value.toInt();
  brushPWMvalue = antiStuckBrushPWM;
  motorRunningState  = HIGH;

  //SerialMon.println("Moving Left");
  curr_state = 'L';

  PORTB |= (1 << wheel_direction); // Set wheel direction to HIGH (cleaning left side)
  PORTB |= (1 << brush_direction); // Set brush direction to HIGH (cleaning left side)

  // Start Motor 2 from 0% to 100% PWM
  for (int i = currentBrushPWM; i < antiStuckBrushPWM; i++) {
    analogWrite(brush_pwm, i);
    Serial.println(i);
    delay(10);
  }

  currentBrushPWM = antiStuckBrushPWM;
  delay(100);
  // Start Motor 1 from 0% to 100% PWM
  for (int i = currentWheelPWM; i < wheelPWMvalue; i++) {
    analogWrite(wheel_pwm, i);
    Serial.println(i);
    delay(10);

  }
  currentWheelPWM = wheelPWMvalue;
//  Serial.println("PWM SET TO - ");
//  Serial.print(currentBrushPWM);
//  Serial.print(currentWheelPWM);

}

void wheelPWMGet()
{
  printMessages("Get Wheel PWM");
  char cmd[128];
  readStringFromEEPROM(topic_speed_offset_address, &topic_speed_eeprom_value);
  wheelPWMvalue = topic_speed_eeprom_value.toInt();
  sprintf(cmd, "AT+CMSG=PW\%d\r\n", wheelPWMvalue);
  int ret = at_send_check_response("+CMSG: Done", 10000, cmd);
  if (ret)
    printMessages("Sent");
  else
    printMessages("Send failed!\r\n\r\n");
}

void burshPWMGet()
{
  printMessages("Get brush PWM");
  char cmd[128];
  readStringFromEEPROM(topic_speed_brush_offset_address, &topic_speed_brush_eeprom_value);
  brushPWMvalue = topic_speed_brush_eeprom_value.toInt();
  sprintf(cmd, "AT+CMSG=PB\%d\r\n", brushPWMvalue);
  int ret = at_send_check_response("+CMSG: Done", 10000, cmd);
  if (ret)
    printMessages("Sent");
  else
    printMessages("Send failed!\r\n\r\n");
}

void wheelPWM1()
{
  writeStringToEEPROM(topic_speed_offset_address, "180"); //Update to EPROM
  printMessages("set wheel PWM to LOW Success");
}

void wheelPWM2()
{
  writeStringToEEPROM(topic_speed_offset_address, "220"); //Update to EPROM
  printMessages("set wheel PWM to MEDIUM Success");
}

void wheelPWM3()
{
  writeStringToEEPROM(topic_speed_offset_address, "250"); //Update to EPROM
  printMessages("set wheel PWM to HIGH Success");
}

void brushPWM1()
{
  writeStringToEEPROM(topic_speed_brush_offset_address, "200"); //Update to EPROM
  printMessages("set brush PWM to LOW Success");
}

void brushPWM2()
{
  writeStringToEEPROM(topic_speed_brush_offset_address, "230"); //Update to EPROM
  printMessages("set brush PWM to MEDIUM Success");
}

void brushPWM3()
{
  writeStringToEEPROM(topic_speed_brush_offset_address, "255"); //Update to EPROM
  printMessages("set brush PWM to HIGH Success");
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

void manualMode()
{
  // Check Switch State
  if (leftSwitchState == LOW && rightSwitchState == HIGH && switchMoveRight == LOW && returnCall == LOW)
  {
    //Left Side Cleaning
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
    weatherLockState = LOW;


    switchState = 1;
    switchMoveLeft = 1;
    sendCleanLeft = 1;
    sendData = 1;
  }
  if (rightSwitchState == LOW && leftSwitchState == HIGH && switchMoveLeft == LOW && returnCall == LOW)
  {
    //Right Side Cleaning
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
    weatherLockState = LOW;

    switchState = 1;
    switchMoveRight = 1;
    sendCleanRight = 1;
    sendData = 1;
  }
  if (leftSwitchState == LOW && rightSwitchState == HIGH && switchMoveRight == LOW && returnCall == HIGH)
  {
    //Left Side Cleaning With Return to Left After Cleaning
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
    weatherLockState = LOW;

    switchState = 1;
    switchCleanLeftReturn = 1;
    sendCleanLeft = 1;
    sendData = 1;
  }
  if (rightSwitchState == LOW && leftSwitchState == HIGH && switchMoveLeft == LOW && returnCall == HIGH)
  {
    //Right Side Cleaning with Return to Right After Cleaning
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
    weatherLockState = LOW;

    switchState = 1;
    switchCleanRightReturn = 1;
    sendCleanRight = 1;
    sendData = 1;

  }
  if (rightSwitchState == LOW && leftSwitchState == LOW)
  {
    //ERROR BOTH SWITCH TRIGGERRED
    stopState = HIGH;
    switchState = 1;
    printMessages("Error! Both Switch Triggered!");
    switchMoveLeft = 0;
    switchMoveRight = 0;
    sendSwitchFault = 1;
    sendData = 1;
  }


  if (switchMoveLeft == HIGH && leftSensorState == LOW) { ////
    //Code to move forward
    stopState = 0;
    switchState = LOW;
    cleanLeft();
  }
  if (switchMoveLeft == HIGH && leftSensorState == HIGH) { ////
    //Code to stop motors
    stopMotors();
    stopState = HIGH;
    switchMoveLeft = LOW;
    sendCleaningFinished = 1;
    sendData = 1;
  }
  if (switchMoveRight == HIGH && rightSensorState == LOW) { ////
    //Code to move reverse
    stopState = 0;
    switchState = LOW;
    cleanRight();
  }
  if (switchMoveRight == HIGH && rightSensorState == HIGH) { ////
    //Code to stop motors
    stopMotors();
    stopState = HIGH;
    switchMoveRight = LOW;
    sendCleaningFinished = 1;
    sendData = 1;

  }
  if (leftSensorState == HIGH && rightSensorState == HIGH) { ////
    //Code to stop motors
    stopState = HIGH;
    switchState = LOW;
    switchMoveLeft = 0;
    switchMoveRight = 0;
    sendSensorFault = 1;
    printMessages("Error! Both Stop Sensors Triggered!");
    sendData = 1;
  }
}

void remoteMovements()
{
  if (moveRightState == HIGH && rightSensorState == LOW && moveToDock == 0 && switchState == 0 && executeCycle == 0) { ////
    //Code to move right
    weatherLockState = LOW;
    stopState = 0;
    moveRight();
  }
  if (moveRightState == HIGH && rightSensorState == HIGH && moveToDock == 0 && switchState == 0 && executeCycle == 0) { ////
    stopState = 1;
    moveRightState = LOW;
    sendCleaningFinished = 1;
    sendData = 1;

    //Send Error Right Hall Sensor Trigerred
  }
  if (moveLeftState == HIGH && leftSensorState == LOW && moveToDock == 0 && switchState == 0 && executeCycle == 0) { ////
    //Code to move left
    weatherLockState = LOW;
    stopState = 0;
    moveLeft();
  }
  if (moveLeftState == HIGH && leftSensorState == HIGH && moveToDock == 0 && switchState == 0 && executeCycle == 0) { ////

    stopState = 1;
    moveLeftState = LOW;
    sendCleaningFinished = 1;
    sendData = 1;


    //Send Error Left Hall Sensor Trigerred
  }
  if (cleanRightState == HIGH && rightSensorState == LOW && moveToDock == 0 && switchState == 0 && executeCycle == 0) { ////
    //Code to move right
    weatherLockState = LOW;
    stopState = 0;
    cleanRight();
  }
  if (cleanRightState == HIGH && rightSensorState == HIGH && moveToDock == 0 && switchState == 0 && executeCycle == 0) { ////
    stopState = 1;
    //Send Error Right Hall Sensor Trigerred
    cleanRightState = LOW;
    sendCleaningFinished = 1;
    sendData = 1;

  }
  if (cleanLeftState == HIGH && leftSensorState == LOW && moveToDock == 0 && switchState == 0 && executeCycle == 0) { ////
    //Code to move left
    weatherLockState = LOW;
    stopState = 0;
    cleanLeft();
  }
  if (cleanLeftState == HIGH && leftSensorState == HIGH && moveToDock == 0 && switchState == 0 && executeCycle == 0) { ////

    stopState = 1;
    cleanLeftState = LOW;
    //Send Error Left Hall Sensor Trigerred
    sendCleaningFinished = 1;
    sendData = 1;

  }
}

void returnToDock()
{
  if (returnToDockState == 1 && switchState == 0)
  {
    stopState = 1;
    stopMotors();
    moveToDock = 1;
    weatherLockState = LOW;
    executeCycle = 0;
    cleanLeftState = 0;
    cleanRightState = 0;
    moveLeftState = 0;
    moveRightState = 0;
    readStringFromEEPROM(topic_left_offset_address, &topic_left_eeprom_value);
    readStringFromEEPROM(topic_right_offset_address, &topic_right_eeprom_value);
    if (topic_left_eeprom_value == "1" && topic_right_eeprom_value == "0")
    {
      //docking at left
      stopState = 0;
      printMessages("Dock is at left side");
      leftDockInCycleLeft = HIGH;
      rightDockInCycleRight = LOW;
      returnToDockState = LOW;
    }
    if (topic_left_eeprom_value == "0" && topic_right_eeprom_value == "1")
    {
      //docking at right
      printMessages("Dock is at right side");
      stopState = 0;
      leftDockInCycleLeft = LOW;
      rightDockInCycleRight = HIGH;
      returnToDockState = LOW;
    }
  }
  if (leftDockInCycleLeft == HIGH && leftSensorState == LOW && moveToDock == 1)
  {
    stopState = LOW;
    printMessages("Docking at Left");
    moveLeft();
  }
  if (leftDockInCycleLeft == HIGH && leftSensorState == HIGH && moveToDock == 1)
  {
    printMessages("Stopping Motors");
    stopState = HIGH;
    leftDockInCycleLeft = LOW;
    moveToDock = LOW;
    sendLeftDockReached = 1;
    sendData = 1;
    weatherLockState = 1;
  }
  if (rightDockInCycleRight == HIGH && rightSensorState == LOW && moveToDock == 1)
  {
    stopState = LOW;
    printMessages("Docking at Right");
    moveRight();
  }
  if (rightDockInCycleRight == HIGH && rightSensorState == HIGH && moveToDock == 1)
  {
    printMessages("Stopping Motors");
    stopState = HIGH;
    rightDockInCycleRight = LOW;
    moveToDock = LOW;
    sendRightDockReached = 1;
    sendData = 1;
    weatherLockState = 1;
  }
}

void executeCleaningCycle()
{
  if (executeCleaning == 1 && executeCycle == 1)
  {
    Serial.println("Cleaning already in progress");
    executeCleaning = 0;
  }

  if (executeCleaning == 1 && switchState == 0 && moveToDock == 0 && executeCycle == 0)
  {
    weatherLockState = LOW;
    readStringFromEEPROM(topic_left_offset_address, &topic_left_eeprom_value);
    readStringFromEEPROM(topic_right_offset_address, &topic_right_eeprom_value);
    if (topic_left_eeprom_value == "1" && topic_right_eeprom_value == "0")
    {
      //docking at left
      printMessages(F("Dock is at left side"));

      moveRightState = 0;
      moveLeftState = 0;
      cleanRightState = 0;
      cleanLeftState = 0;
      leftDockInCycleLeft = 0;
      rightDockInCycleRight = 0;
      leftDockInCycleRight = 0;
      rightDockInCycleLeft = 0;
      switchMoveLeft = 0;
      switchMoveRight = 0;
      leftDockInCycle = HIGH;
      rightDockInCycle = LOW;
      executeCleaning = 0;
      stopState = 0;
      executeCycle = 1;

      sendExecuteCycle = 1;
      sendData = 1;
    }
    if (topic_left_eeprom_value == "0" && topic_right_eeprom_value == "1")
    {
      //docking at right
      printMessages(F("Dock is at right side"));

      moveRightState = 0;
      moveLeftState = 0;
      cleanRightState = 0;
      cleanLeftState = 0;
      leftDockInCycleLeft = 0;
      rightDockInCycleRight = 0;
      leftDockInCycleRight = 0;
      rightDockInCycleLeft = 0;
      switchMoveLeft = 0;
      switchMoveRight = 0;

      leftDockInCycle = LOW;
      rightDockInCycle = HIGH;
      stopState = 0;
      executeCleaning = 0;
      executeCycle = 1;

      sendExecuteCycle = 1;
      sendData = 1;
    }
  }
  if (leftDockInCycle == HIGH && rightDockInCycleRight == LOW &&  rightDockInCycleLeft == LOW && executeCycle == 1)
  {
    leftDockInCycleRight = HIGH;
    leftDockInCycleLeft = LOW;
    leftDockInCycle = LOW;
  }
  if (rightDockInCycle == HIGH && leftDockInCycleRight == LOW && leftDockInCycleLeft == LOW && executeCycle == 1)
  {
    rightDockInCycleRight = LOW;
    rightDockInCycleLeft = HIGH;
    rightDockInCycle = LOW;

  }
  if (leftDockInCycleRight == HIGH && rightSensorState == LOW && executeCycle == 1)
  {
    rightDockInCycleRight = LOW;
    rightDockInCycleLeft = LOW;
    //    printMessages("Cleaning Cycle Right");
    cleanRight();
  }
  if (leftDockInCycleRight == HIGH && rightSensorState == HIGH && executeCycle == 1)
  {
    printMessages("Stopping Motors");
    stopMotors();
    leftDockInCycleRight = LOW;
    leftDockInCycleLeft = HIGH;

  }
  if (leftDockInCycleLeft == HIGH && leftSensorState == LOW && executeCycle == 1 )
  {
    stopState = LOW;
    //    printMessages("Cleaning Cycle Left");
    cleanLeft();
  }
  if (leftDockInCycleLeft == HIGH && leftSensorState == HIGH && executeCycle == 1)
  {
    printMessages("Stopping Motors");
    stopState = HIGH;
    leftDockInCycleLeft = LOW;
    executeCycle = LOW;
    leftDockInCycle = LOW;
    rightDockInCycle = LOW;
    sendCleaningFinished = 1;
    sendData = 1;
  }
  if (rightDockInCycleLeft == HIGH && leftSensorState == LOW && executeCycle == 1)
  {
    leftDockInCycleRight = LOW;
    leftDockInCycleLeft = LOW;
    stopState = LOW;
    //    printMessages("Cleaning Cycle Left");
    cleanLeft();
  }
  if (rightDockInCycleLeft == HIGH && leftSensorState == HIGH && executeCycle == 1)
  {
    stopMotors();
    printMessages("Stopping Motors");
    rightDockInCycleRight = HIGH;
    rightDockInCycleLeft = LOW;

  }
  if (rightDockInCycleRight == HIGH && rightSensorState == LOW && executeCycle == 1)
  {
    stopState = LOW;
    //    printMessages("Cleaning Cycle Right");
    cleanRight();
  }
  if (rightDockInCycleRight == HIGH && rightSensorState == HIGH && executeCycle == 1)
  {
    printMessages("Stopping Motors");
    stopState = HIGH;
    rightDockInCycleRight = LOW;
    executeCycle = LOW;
    leftDockInCycle = LOW;
    rightDockInCycle = LOW;
    sendCleaningFinished = 1;
    sendData = 1;
  }
}

void weatherLockEnable()
{
  if (weatherLockState == 1 && moveToDock == 0 && switchState == 0 && executeCycle == 0)
  {
    readStringFromEEPROM(topic_left_offset_address, &topic_left_eeprom_value);
    readStringFromEEPROM(topic_right_offset_address, &topic_right_eeprom_value);
    if (topic_left_eeprom_value == "1" && topic_right_eeprom_value == "0")
    {
      //docking at left
//      printMessages(F("Dock is at left side"));
      leftDockInCycleLeft = HIGH;
      rightDockInCycleRight = LOW;
    }
    if (topic_left_eeprom_value == "0" && topic_right_eeprom_value == "1")
    {
      //docking at right
//      printMessages(F("Dock is at right side"));
      leftDockInCycleLeft = LOW;
      rightDockInCycleRight = HIGH;
    }
  }
  if (leftDockInCycleLeft == HIGH && leftSensorState == LOW && weatherLockState == 1 && moveToDock == 0 && switchState == 0 && executeCycle == 0)
  {
    stopState = LOW;
    printMessages("Weather Lock Enabled");
    moveLeft();
  }
  if (leftDockInCycleLeft == HIGH && leftSensorState == HIGH && weatherLockState == 1 && moveToDock == 0 && switchState == 0 && executeCycle == 0)
  {
//    printMessages("Weather Lock Successful");
    stopState = HIGH;
    leftDockInCycleLeft = LOW;
    returnToDockState = LOW;
  }
  if (rightDockInCycleRight == HIGH && rightSensorState == LOW && weatherLockState == 1 && moveToDock == 0 && switchState == 0 && executeCycle == 0)
  {
    stopState = LOW;
    printMessages("Weather Lock Enabled");
    moveRight();
  }
  if (rightDockInCycleRight == HIGH && rightSensorState == HIGH && weatherLockState == 1 && moveToDock == 0 && switchState == 0 && executeCycle == 0)
  {
//    printMessages("Weather Lock Successful");
    stopState = HIGH;
    rightDockInCycleRight = LOW;
    returnToDockState = LOW;
    sendWeatherLockSuccessful = 1;
    sendData = 1;
  }
}

void executeManualReturnCleaning()
{
  if ( switchCleanLeftReturn == 1 || switchCleanRightReturn == 1) {
    if (switchCleanLeftReturn == 1 && switchCleanRightReturn == 0)
    {
      //docking at left
      printMessages(F("Executing Left Side Return Cleaning"));

      moveRightState = 0;
      moveLeftState = 0;
      cleanRightState = 0;
      cleanLeftState = 0;
      leftDockInCycleLeft = 0;
      rightDockInCycleRight = 0;
      leftDockInCycleRight = 0;
      rightDockInCycleLeft = 0;
      switchMoveLeft = 0;
      switchMoveRight = 0;
      leftDockInCycle = HIGH;
      rightDockInCycle = LOW;
      executeCleaning = 0;
      stopState = 0;
      executeCycle = 1;
      switchCleanLeftReturn = 0;

    }
    if (switchCleanLeftReturn == 0 && switchCleanRightReturn == 1)
    {
      //docking at right
      printMessages(F("Executing Right Side Return Cleaning"));

      moveRightState = 0;
      moveLeftState = 0;
      cleanRightState = 0;
      cleanLeftState = 0;
      leftDockInCycleLeft = 0;
      rightDockInCycleRight = 0;
      leftDockInCycleRight = 0;
      rightDockInCycleLeft = 0;
      switchMoveLeft = 0;
      switchMoveRight = 0;

      leftDockInCycle = LOW;
      rightDockInCycle = HIGH;
      stopState = 0;
      executeCleaning = 0;
      executeCycle = 1;
      switchCleanRightReturn = 0;

    }

    if (leftDockInCycle == HIGH && rightDockInCycleRight == LOW &&  rightDockInCycleLeft == LOW && executeCycle == 1)
    {
      leftDockInCycleRight = HIGH;
      leftDockInCycleLeft = LOW;
      leftDockInCycle = LOW;
    }
    if (rightDockInCycle == HIGH && leftDockInCycleRight == LOW && leftDockInCycleLeft == LOW && executeCycle == 1)
    {
      rightDockInCycleRight = LOW;
      rightDockInCycleLeft = HIGH;
      rightDockInCycle = LOW;

    }
    if (leftDockInCycleRight == HIGH && rightSensorState == LOW && executeCycle == 1)
    {
      rightDockInCycleRight = LOW;
      rightDockInCycleLeft = LOW;
      //    printMessages("Cleaning Cycle Right");
      cleanRight();
    }
    if (leftDockInCycleRight == HIGH && rightSensorState == HIGH && executeCycle == 1)
    {
      printMessages("Stopping Motors");
      stopMotors();
      leftDockInCycleRight = LOW;
      leftDockInCycleLeft = HIGH;

    }
    if (leftDockInCycleLeft == HIGH && leftSensorState == LOW && executeCycle == 1 )
    {
      stopState = LOW;
      //    printMessages("Cleaning Cycle Left");
      cleanLeft();
    }
    if (leftDockInCycleLeft == HIGH && leftSensorState == HIGH && executeCycle == 1)
    {
      printMessages("Stopping Motors");
      stopState = HIGH;
      leftDockInCycleLeft = LOW;
      executeCycle = LOW;
      leftDockInCycle = LOW;
      rightDockInCycle = LOW;
      sendCleaningFinished = 1;
      sendData = 1;
    }
    if (rightDockInCycleLeft == HIGH && leftSensorState == LOW && executeCycle == 1)
    {
      leftDockInCycleRight = LOW;
      leftDockInCycleLeft = LOW;
      stopState = LOW;
      //    printMessages("Cleaning Cycle Left");
      cleanLeft();
    }
    if (rightDockInCycleLeft == HIGH && leftSensorState == HIGH && executeCycle == 1)
    {
      stopMotors();
      printMessages("Stopping Motors");
      rightDockInCycleRight = HIGH;
      rightDockInCycleLeft = LOW;

    }
    if (rightDockInCycleRight == HIGH && rightSensorState == LOW && executeCycle == 1)
    {
      stopState = LOW;
      //    printMessages("Cleaning Cycle Right");
      cleanRight();
    }
    if (rightDockInCycleRight == HIGH && rightSensorState == HIGH && executeCycle == 1)
    {
      printMessages("Stopping Motors");
      stopState = HIGH;
      rightDockInCycleRight = LOW;
      executeCycle = LOW;
      leftDockInCycle = LOW;
      rightDockInCycle = LOW;
      sendCleaningFinished = 1;
      sendData = 1;
    }

  }
}

void resetFirmware()
{
    resetFunc();
}
