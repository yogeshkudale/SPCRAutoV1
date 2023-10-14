void safetyFeatures()
{
  unsigned long currentTimeJoinLNS = millis();

  //Connect to LoraWAN Network Server or check if still connected
  if (currentTimeJoinLNS - previousJoinLora > timeIntervalJoinLora) {
    previousJoinLora = currentTimeJoinLNS;
    // printMessages("Checking Network Status!");
    // LinkCheckReq();
    connectLNS();
  }

  // set initial counter values to 0
  unsigned long driveMotorCurrentOverLimitTime = 0;
  unsigned long brushMotorCurrentOverLimitTime = 0;
  unsigned long currentTimeSendCurrent = millis();
  // read motor current values
  driveMotorCurrentValue = analogRead(driveMotorCurrent);
  brushMotorCurrentValue = analogRead(brushMotorCurrent);

  float driveMotorCurrentAmps = adc_to_current(driveMotorCurrentValue);
  float brushMotorCurrentAmps = adc_to_current(brushMotorCurrentValue);

  const unsigned long intervalCurrentCheckMillis = 1;

  // check if drive motor current is over the limit
  if (driveMotorCurrentValue > wheelMotorCurrentLimit)
  {
        printMessages("Wheel OverCurrent Detected!");
if(enableMotorCurrentCheck  == 1){
    // increment drive motor current over limit counter
    driveMotorCurrentOverLimitTime += intervalCurrentCheckMillis;

    // check if the counter has reached 1000ms (1 second)
    if (driveMotorCurrentOverLimitTime >= 1000)
    {

      // perform desired function here
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
      printMessages("Drive Motor Current Over Limit. Stopping Motors...");
      printMessages(String(driveMotorCurrentValue));
    }
  }
  else
  {
    // reset drive motor current over limit counter if current is below the limit
    driveMotorCurrentOverLimitTime = 0;
  }
}
  // repeat the same logic for brush motor current if needed
  // check if drive motor current is over the limit
  if (brushMotorCurrentValue > burshMotorCurrentLimit)
  {
            printMessages("Brush OverCurrent Detected!");

if (enableMotorCurrentCheck == 1){
    // increment drive motor current over limit counter
    brushMotorCurrentOverLimitTime += intervalCurrentCheckMillis;

    // check if the counter has reached 1000ms (1 second)
    if (brushMotorCurrentOverLimitTime >= 1000)
    {
      // perform desired function here
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
      printMessages("Brush Motor Current Over Limit. Stopping Motors...");
      printMessages(String(brushMotorCurrentValue));

    }
  }
  else
  {
    // reset drive motor current over limit counter if current is below the limit
    brushMotorCurrentOverLimitTime = 0;
  }
}

  if (testMode == 1 && executeCycle == 0) {
    executeCleaning = 1; // Testbed Test Mode
  }


  //   SendMotorCurrent to Server
  if (is_exist)
  {
    if (motorRunningState == HIGH) {
      if (currentTimeSendCurrent - previousTimeSendCurrent > timeIntervalSendCurrent) {
        previousTimeSendCurrent = currentTimeSendCurrent;
        if (is_join == false)
        {
          sendMotorCurrentValues();

        }
      }
    }
  }


  //   Send Board Temperature to Server

  unsigned long currentTimeSendTemp = millis();

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
                  printMessages("Overtemperature Detected!! Stopping Motors!");

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

}