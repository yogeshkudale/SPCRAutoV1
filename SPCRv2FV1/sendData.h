void sendMotorCurrentValues()
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
  unsigned long currentTimeCheckDock = millis();

  //Check If Robot is At Dock if yes send at dock
  if (currentTimeCheckDock - previousTimeCheckDock > timeIntervalCheckDock) {
    previousTimeCheckDock = currentTimeCheckDock;
    readStringFromEEPROM(topic_left_offset_address, &topic_left_eeprom_value);
    readStringFromEEPROM(topic_right_offset_address, &topic_right_eeprom_value);
    if (topic_left_eeprom_value == "1" && topic_right_eeprom_value == "0")
    {
      //docking at left
      if (leftSensorState == HIGH) {
        sendMessage = 1;
        sendAtDock = 1;
        printMessages("At Dock");

      }
    }
    if (topic_left_eeprom_value == "0" && topic_right_eeprom_value == "1")
    {
      //docking at right
      if (rightSensorState == HIGH) {
        sendMessage = 1;
        sendAtDock = 1;
        printMessages("At Dock");

      }
    }
  }

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
            printMessages("Battery LOW!");

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
                  printMessages("Motor Fault!");

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
                  printMessages("High Board Temperature!");

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
                  printMessages("Brush motor fault!");

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
                  printMessages("Switch Fault!");

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
                        printMessages("Sensor Fault!");

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
                        printMessages("Executed Cleaning Cycle!");

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
                        printMessages("Cleaning Cycle Cancelled!");

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
                        printMessages("Speed Updated!");

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
                        printMessages("Returning to Dock!");

      int ret = at_send_check_response("+CMSG: Done", 10000, "AT+CMSG=RD\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendReturnToDock = 0;

      sendMessage = 0;


    }

    if (sendLeftDockReached == 1)
    {
                        printMessages("At Dock!");

      int ret = at_send_check_response("+CMSG: Done", 10000, "AT+CMSG=DR\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendLeftDockReached = 0;

      sendMessage = 0;


    }

    if (sendAtDock == 1)
    {
                        printMessages("At Dock!");

      int ret = at_send_check_response("+CMSG: Done", 10000, "AT+CMSG=AD\r\n");
      if (ret) {
        printMessages("Sent");
      } else {
        printMessages("Send failed!\r\n\r\n");
      }
      sendAtDock = 0;

      sendMessage = 0;


    }

    if (sendRightDockReached == 1)
    {
                        printMessages("At Dock!");

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
                        printMessages("Cleaning Cycle Completed!");

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
                        printMessages("Weather Lock Successful!");

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
