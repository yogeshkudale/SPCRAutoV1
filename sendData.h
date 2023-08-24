void batteryVoltageCheck()
{
  char cmd[128];
  int batteryVoltageValue = batteryVoltageInt();
  //    int batteryVoltageValue = random(40, 69);
  //  int batteryVoltageValue = 54;

  sprintf(cmd, "AT+MSG=B\%d\r\n", batteryVoltageValue);
  int ret = at_send_check_response("+MSG: Done", 10000, cmd);
  if (ret) {
    printMessages("Sent");
  }
  else {
    printMessages("Send failed!\r\n\r\n");
  }


  //Battery Voltage Check
  if (automaticMode == 1 && batteryLowState == 0) {
    if (batteryVoltageValue < 10)
    {
      Serial.print(battery.level());
      printMessages("Battery Low! Returning to dock and entering Sleep Mode. Please charge the battery...");
      //Cancel Everyhing
      testMode = 0;
      executeCycle = 0;
      switchMoveLeft = 0;
      switchMoveRight = 0;
      cleanRightState = 0;
      cleanLeftState = 0;
      returnToDockState = 1;
      sendBatteryLow = 1;
      batteryLowState = 1;
    }
  }
  if (automaticMode == 0) {
    if (batteryVoltageValue < 1 )
    {
      printMessages("Battery Low! Stopping motors to avoid battery failure. Please charge the battery...");
      //Cancel Everyhing
      testMode = 0;
      switchMoveLeft = 0;
      switchMoveRight = 0;
      stopState = 1;
      sendWheelMotorFault = 0;
      sendReturnToDock = 0;
      sendBatteryLow = 1;
    }
  }

}


void LinkCheckReq()
{
  int ret = at_send_check_response("+MSG: Done", 10000, "AT+MSG");
      if (ret) {
        printMessages("Connected");
      } else {
        printMessages("Connection Lost!");
        is_join == true;
      }
}


void sendMotorCurrent()
{
  char cmd[128];

  sprintf(cmd, "AT+MSG=MD\%dMB%d\r\n", driveMotorCurrentValue, brushMotorCurrentValue);
  int ret = at_send_check_response("+MSG: Done", 10000, cmd);
  if (ret) {
    printMessages("Sent");
  }
  else {
    printMessages("Send failed!\r\n\r\n");
  }
  delay(500);

  if (at_send_check_response("+TEMP:", 1000, "AT+TEMP\r\n"))
  {
    String tempString = String(recv_buf + 6);
    String actualtemp = tempString.substring(0, 4);
    temp = actualtemp.toFloat() * 1000;
    Serial.println(temp);
  }

}
void sendMessagestoServer() {

  if (sendData == 1)
  {
    //////Time interval to send commands- Send Messages to server
    unsigned long currentTimeCheckCommands = millis();

    if (currentTimeCheckCommands - previousTimeCheckCommands >= timeIntervalCheckCommands) {
      previousTimeCheckCommands = currentTimeCheckCommands;
      sendMessage = 1;
      sendData = 0;

    }
  }

  if (sendMessage == 1)
  {
    if (sendBatteryLow == 1)
    {
      int ret = at_send_check_response("+CMSG: Done", 10000, "AT+CMSG=F5\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendBatteryLow = 0;
      sendMessage = 0;

    }

    if (sendWheelMotorFault == 1)
    {
      int ret = at_send_check_response("+CMSG: Done", 10000, "AT+CMSG=F1\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendWheelMotorFault = 0;

      sendMessage = 0;


    }

    ///Temperature Limit Reached
    if (sendOvertemperature == 1)
    {
      int ret = at_send_check_response("+CMSG: Done", 10000, "AT+CMSG=F6\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendOvertemperature = 0;


      sendMessage = 0;

    }

    if (sendBrushMotorFault == 1)
    {
      int ret = at_send_check_response("+CMSG: Done", 10000, "AT+CMSG=F2\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendBrushMotorFault = 0;

      sendMessage = 0;


    }

    if (sendSwitchFault == 1)
    {
      int ret = at_send_check_response("+MSG: Done", 10000, "AT+MSG=F3\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendSwitchFault = 0;


      sendMessage = 0;

    }

    if (sendSensorFault == 1)
    {
      int ret = at_send_check_response("+MSG: Done", 10000, "AT+MSG=F4\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendSensorFault = 0;


      sendMessage = 0;

    }

    if (sendExecuteCycle == 1)
    {
      int ret = at_send_check_response("+CMSG: Done", 10000, "AT+CMSG=C1\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendExecuteCycle = 0;

      sendMessage = 0;

    }


    if (sendCancelCleaning == 1)
    {

      int ret = at_send_check_response("+CMSG: Done", 10000, "AT+CMSG=CC\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendCancelCleaning = 0;

      sendMessage = 0;


    }

    if (updateSpeedSend == 1)
    {

      int ret = at_send_check_response("+MSG: Done", 10000, "AT+MSG=US\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      updateSpeedSend = 0;


      sendMessage = 0;

    }

    if (sendMoveLeft == 1)
    {

      int ret = at_send_check_response("+MSG: Done", 10000, "AT+MSG=C4\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendMoveLeft = 0;

      sendMessage = 0;


    }

    if (sendMoveRight == 1)
    {

      int ret = at_send_check_response("+MSG: Done", 10000, "AT+MSG=C5\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendMoveRight = 0;

      sendMessage = 0;


    }

    if (sendCleanLeft == 1)
    {

      int ret = at_send_check_response("+CMSG: Done", 10000, "AT+CMSG=C2\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendCleanLeft = 0;


      sendMessage = 0;

    }

    if (sendCleanRight == 1)
    {

      int ret = at_send_check_response("+CMSG: Done", 10000, "AT+CMSG=C3\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendCleanRight = 0;

      sendMessage = 0;


    }

    if (sendReturnToDock == 1)
    {

      int ret = at_send_check_response("+CMSG: Done", 10000, "AT+CMSG=RD\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendReturnToDock = 0;

      sendMessage = 0;


    }
    if (sendAtDock == 1)
    {

      int ret = at_send_check_response("+CMSG: Done", 10000, "AT+CMSG=AD\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendAtDock = 0;

      sendMessage = 0;


    }


    if (sendLeftDockReached == 1)
    {

      int ret = at_send_check_response("+CMSG: Done", 10000, "AT+CMSG=DR\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendLeftDockReached = 0;

      sendMessage = 0;


    }

    if (sendRightDockReached == 1)
    {

      int ret = at_send_check_response("+CMSG: Done", 10000, "AT+CMSG=DR\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendRightDockReached = 0;

      sendMessage = 0;


    }
    if (sendCleaningFinished == 1)
    {

      int ret = at_send_check_response("+CMSG: Done", 10000, "AT+CMSG=CF\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendCleaningFinished = 0;


      sendMessage = 0;

    }
    if (sendWeatherLockSuccessful == 1)
    {

      int ret = at_send_check_response("+MSG: Done", 10000, "AT+MSG=WL\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendWeatherLockSuccessful = 0;


      sendMessage = 0;

    }
    sendMessage = 0;

  }

}
