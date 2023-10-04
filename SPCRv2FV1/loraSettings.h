
/* Start of Lora Variables */
#define RECV_BUF_SIZE 256  // Adjust this value based on your needs

static char recv_buf[RECV_BUF_SIZE];
static bool is_exist = false;
static bool is_join = false;
/* End of Lora Variables */


//Start of LOra COnversion
static int at_send_check_response(char *p_ack, int timeout_ms, char *p_cmd, ...) {
  int ch;
  int index = 0;
  unsigned long startMillis = millis();
  va_list args;
  char formatted_cmd[256];  // Adjust this size based on your needs

  memset(recv_buf, 0, sizeof(recv_buf));

  va_start(args, p_cmd);
  vsnprintf(formatted_cmd, sizeof(formatted_cmd), p_cmd, args);
  SerialLora.print(formatted_cmd);
  Serial.print(formatted_cmd);
  va_end(args);

  // delay(200);  // Delay after sending the command

  if (p_ack == NULL) {
    return 0;
  }

  do {
    while (SerialLora.available() > 0) {
      ch = SerialLora.read();
      if (index < RECV_BUF_SIZE - 1) {  // Protect against buffer overflow
        recv_buf[index++] = ch;
      }
      Serial.print((char)ch);
      delay(2);  // Delay during reading
    }

    if (strstr(recv_buf, p_ack) != NULL) {
      return 1;
    }

  } while (millis() - startMillis < timeout_ms);

  return 0;
}

static void recv_prase(char *p_msg) {
  msgReceivedLEDState = HIGH;
  Serial.print(p_msg);
  if (p_msg == NULL) {
    return;
  }
  char *p_start = NULL;
  char hex_data[17] = { 0 };  // Assuming the hex data will not be more than 9 characters long

  p_start = strstr(p_msg, "RX");
  if (p_start && (1 == sscanf(p_start, "RX: \"%16s\"\r\n", hex_data))) {
    // Convert hex data to string
    char data_string[9] = { 0 };
    int len = strlen(hex_data);
    for (int i = 0; i < len; i += 2) {
      char byte[3] = { hex_data[i], hex_data[i + 1], '\0' };
      data_string[i / 2] = (char)strtol(byte, NULL, 16);
    }

    Serial.print("Input Received : ");
    Serial.println(data_string);

    if (strcmp(data_string, "BV") == 0) {
      // Check battery Voltage
      batteryVoltageCheckState = 1;

    } else if (strcmp(data_string, "C1") == 0) {
      // Cleaning Cycle Start
      executeCleaning = 1;
    }

    else if (strcmp(data_string, "CC") == 0) {
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

    else if (strcmp(data_string, "PW") == 0) {
      //PWM WHEEL GET
      wheelPWMGetState = 1;
    }

    else if (strcmp(data_string, "PB") == 0) {
      //PWM Brush Get
      burshPWMGetState = 1;
    } else if (strncmp(data_string, "SPW", 3) == 0)  // Check if the string starts with "SPB"
    {
      if (sscanf(data_string, "SPW%d", &wheelSpeedReceived) == 1)  // Extract the number
      {
        // Print the extracted number
        Serial.print("Received Speed: ");
        Serial.println(String(wheelSpeedReceived));
        wheelSpeedUpdate = 1;
      }
    } else if (strcmp(data_string, "W1") == 0) {
      //Set Wheel PWM Value TO 150
      wheelPWM1State = 1;
      updateSpeedSend = 1;
      sendData = 1;

    }

    else if (strcmp(data_string, "W2") == 0) {
      //Set Wheel PWM Value TO 200
      wheelPWM2State = 1;
      updateSpeedSend = 1;
      sendData = 1;

    }

    else if (strcmp(data_string, "W3") == 0) {
      //Set Wheel PWM Value TO 250

      wheelPWM3State = 1;
      updateSpeedSend = 1;
      sendData = 1;

    } else if (strncmp(data_string, "SPB", 3) == 0)  // Check if the string starts with "SPB"
    {
      if (sscanf(data_string, "SPB%d", &brushSpeedReceived) == 1)  // Extract the number
      {
        // Print the extracted number
        Serial.print("Received Speed: ");
        Serial.println(String(brushSpeedReceived));
        brushSpeedUpdate = 1;
      }
    } else if (strncmp(data_string, "SPD", 3) == 0)  // Check if the string starts with "SPB"
    {
      if (sscanf(data_string, "SPD%d", &dockingSpeedReceived) == 1)  // Extract the number
      {
        // Print the extracted number
        Serial.print("Received Speed: ");
        Serial.println(String(dockingSpeedReceived));
        dockingSpeedUpdate = 1;
      }
    }



    else if (strcmp(data_string, "B1") == 0) {
      //Set Brush PWM Value TO 150
      brushPWM1State = 1;
      updateSpeedSend = 1;
      sendData = 1;

    }

    else if (strcmp(data_string, "B2") == 0) {
      //Set Brush PWM Value TO 200
      brushPWM2State = 1;
      updateSpeedSend = 1;
      sendData = 1;

    }

    else if (strcmp(data_string, "B3") == 0) {
      //Set Brush PWM Value TO 250
      brushPWM3State = 1;
      updateSpeedSend = 1;
      sendData = 1;

    }

    else if (strncmp(data_string, "SBC", 3) == 0)  // Check if the string starts with "SPB"
    {
      if (sscanf(data_string, "SBC%d", &brushCurrentReceived) == 1)  // Extract the number
      {
        // Print the extracted number
        Serial.print("Received Brush Current: ");
        Serial.println(String(brushCurrentReceived));
        brushCurrentUpdate = 1;
      }
    }

    else if (strncmp(data_string, "SWC", 3) == 0)  // Check if the string starts with "SPB"
    {
      if (sscanf(data_string, "SWC%d", &wheelCurrentReceived) == 1)  // Extract the number
      {
        // Print the extracted number
        Serial.print("Received Wheel Current: ");
        Serial.println(String(wheelCurrentReceived));
        wheelCurrentUpdate = 1;
      }
    }

else if (strncmp(data_string, "DSN", 3) == 0)  // Check if the string starts with "SPB"
    {
      if (sscanf(data_string, "DSN%d", &deviceSerialNumber) == 1)  // Extract the number
      {
        // Print the extracted number
        Serial.print("Received Serial Number: ");
        Serial.println(String(deviceSerialNumber));
        deviceSerialNumberUpdate = 1;
      }
    }

    else if (strcmp(data_string, "GSN") == 0) {
      //Set Brush PWM Value TO 250
      sendDeviceSerialNumber = 1;

    }


    else if (strcmp(data_string, "C4") == 0) {
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

    else if (strcmp(data_string, "C5") == 0) {
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

    else if (strcmp(data_string, "C2") == 0) {
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

    else if (strcmp(data_string, "C3") == 0) {
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

    else if (strcmp(data_string, "D1") == 0) {
      //back to docking station
      returnToDockState = 1;
      sendReturnToDock = 1;
      sendData = 1;

    }

    else if (strcmp(data_string, "D2") == 0) {
      //Set Left docking station
      leftDockState = 1;
      sendLeftDock = 1;
      sendData = 1;

    } else if (strcmp(data_string, "D3") == 0) {
      //Set right docking station
      rightDockState = 1;
      sendRightDock = 1;
      sendData = 1;

    } else if (strcmp(data_string, "TS") == 0) {
      //TURN ON TEST MODE
      testMode = 1;

    } else if (strcmp(data_string, "TC") == 0) {
      //TURN OFF TEST MODE/CANCEL TEST MODE
      testMode = 0;
    }


    else if (strcmp(data_string, "RE") == 0) {
      //reset the firmware
      resetFirmwareValue = 1;
    } else if (strcmp(data_string, "4W") == 0) {
      //Set the mode For 40 Watt Motors
      set40WattMode = 1;
    } else if (strcmp(data_string, "6W") == 0) {
      //Set the mode For 60 Watt Motors
      set60WattMode = 1;
    } else if (strcmp(data_string, "GF") == 0) {
      //Set the mode For 60 Watt Motors
      getFirmwareVersion = 1;
    }
     else {
      Serial.print("Incorrect Lora Input! Please Correct The Input...");
    }
  }
}
/* End of Lora Conversion */

void setupLora() {
  printMessages("Initializing LoraWAN Sequence...");
  if (at_send_check_response("+AT: OK", 100, "AT\r\n")) {
    is_exist = true;

    at_send_check_response("+FDEFAULT: OK", 1000, "AT+FDEFAULT\r\n");

    at_send_check_response("+RTC: ZONE, +05:30", 1000, "AT+RTC=ZONE, \"+05:30\"\r\n");

    if (at_send_check_response("+TEMP:", 1000, "AT+TEMP\r\n")) {
      String tempString = String(recv_buf + 7);
      String actualtemp = tempString.substring(0, 4);
      temp = actualtemp.toFloat();
      Serial.print("Board Temperature: ");
      Serial.print(temp);
      Serial.println("°C");
    }

    at_send_check_response("+MODE: LWOTAA", 1000, "AT+MODE=LWOTAA\r\n");
    //    at_send_check_response("+DR: DR5", 1000, "AT+DR=5\r\n");
    at_send_check_response("+DR: IN865", 1000, "AT+DR=IN865\r\n");  //Default Channels Hz - 865062500 / 865402500 / 865985000

    at_send_check_response("+CH:3", 1000, "AT+CH=3, 865.6025,0,5\r\n");   //DRAGINO GATEWAY 865602500
    at_send_check_response("+CH:4", 1000, "AT+CH=4, 866.1850,0,5\r\n");   //DRAGINO GATEWAY 866185000
    at_send_check_response("+CH:5", 1000, "AT+CH=5, 866.7850,0,5\r\n");   //DRAGINO GATEWAY 866785000
    at_send_check_response("+CH:6", 1000, "AT+CH=6, 865.2325,0,5\r\n");   //TAYPRO WISGATE GATEWAY 865232500
    at_send_check_response("+CH:7", 1000, "AT+CH=7, 865.5725,0,5\r\n");   //TAYPRO WISGATE GATEWAY 865572500
    at_send_check_response("+CH:8", 1000, "AT+CH=8, 865.7425,0,5\r\n");   //TAYPRO WISGATE GATEWAY 865742500
    at_send_check_response("+CH:9", 1000, "AT+CH=9, 865.9125,0,5\r\n");   //TAYPRO WISGATE GATEWAY 865912500
    at_send_check_response("+CH:10", 1000, "AT+CH=10, 865.5,0,5\r\n");    //TAYPRO WISGATE GATEWAY 865500000
    at_send_check_response("+CH:11", 1000, "AT+CH=11, 866.495,0,5\r\n");  //TAYPRO WISGATE GATEWAY 866495000
    at_send_check_response("+CH:12", 1000, "AT+CH=12, 866.665,0,5\r\n");  //TAYPRO WISGATE GATEWAY 866665000
    at_send_check_response("+CH:13", 1000, "AT+CH=13, 866.835,0,5\r\n");  //TAYPRO WISGATE GATEWAY 866835000
    at_send_check_response("+CH:14", 1000, "AT+CH=14, 867,0,5\r\n");      //TAYPRO WISGATE GATEWAY 867000000
    at_send_check_response("+CH:15", 1000, "AT+CH=15, 866.155,0,5\r\n");  //TAYPRO WISGATE GATEWAY 866155000
    at_send_check_response("+ADR: ON", 1000, "AT+ADR=ON\r\n");            //TURN ON ADAPTIVE DATA RATE

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

    if (at_send_check_response("+VDD:", 1000, "AT+VDD\r\n")) {
      String vddString = String(recv_buf + 6);
      String actualvdd = vddString.substring(0, 4);
      vdd = actualvdd.toFloat() * 1000;
      Serial.print(vdd);
      Serial.println(" mV");
    }

    is_join = true;
    PORTD |= (1 << PD6);  // Set bit PD6 to turn LED on
    automaticMode = 1;

  } else {
    is_exist = false;
    printMessages("Lora IC Not found.\r\n");
    intervalNetworkLEDMillis = 100;
    automaticMode = 0;
  }
}

void connectLNS() {
  if (is_exist) {
    int ret = 0;
    if (is_join) {
      printMessages("\r\nConnecting to LoraWAN Network Server\r\n");

      unsigned long startTimeJoin = millis();  // Record the start time

      ret = at_send_check_response("+JOIN: Network joined", 10000, "AT+JOIN\r\n");

      unsigned long elapsedTimeJoin = millis() - startTimeJoin;  // Calculate the elapsed time

      if (ret && elapsedTimeJoin <= 10000)  // Check if acknowledgment was received and it was within 12 seconds
      {
        is_join = false;
        printMessages("\r\n\Connected to LoraWAN Network Server\r\n\r\n");

        //SEND FIRST UPLINK
        delay(1000);

        unsigned long startTimeUplink = millis();  // Record the start time

        ret = at_send_check_response("+CMSG: Done", 10000, "AT+CMSG=1\r\n");
        unsigned long elapsedTimeUplink = millis() - startTimeUplink;  // Calculate the elapsed time

        if (ret && elapsedTimeUplink <= 10000)  // Check if acknowledgment was received and it was within 12 seconds
        {
          recv_prase(recv_buf);
          printMessages("First uplink send Success! Server Connection Verified.");
          at_send_check_response("+DTR", 5000, "AT+LW=DTR\r\n");
          at_send_check_response("+TIME", 5000, "AT+RTC\r\n");
        } else {
          printMessages("First uplink send failed! Retrying in 30 Seconds.");
          at_send_check_response("+TIME", 5000, "AT+RTC\r\n");
        }
      } else {
        at_send_check_response("+ID: AppEui", 1000, "AT+ID\r\n");
        printMessages("\r\n\r\nJOIN failed!\r\n\r\n");
      }
    }
  }
}



void LinkCheckReq() {
  // int ret = at_send_check_response("+MSG: Done", 10000, "AT+MSG");
  // if (ret) {
  //   printMessages("Connected");
  // } else {
  //   printMessages("Connection Lost!");
  //   is_join == true;
  // }
  printMessages("Checking Network Status");

  int ret = at_send_check_response("+LW: STATUS", 5000, "AT+LW=STATUS\r\n");
  if (ret) {
    // Parse the recv_buf to determine the status
    if (strstr(recv_buf, "JOINED") != NULL) {
      // The device is joined to the network
      printMessages("Connected");

    } else {
      // The device is not joined
      printMessages("Connection Lost!");
      is_join == true;
    }
  } else {
    // Failed to get status
  }
}



void wheelPWMGet() {
  printMessages("Get Wheel PWM");
  char cmd[128];
  readStringFromEEPROM(topic_speed_wheel_offset_address, &topic_speed_wheel_eeprom_value);
  wheelPWMvalue = topic_speed_wheel_eeprom_value.toInt();
  sprintf(cmd, "AT+CMSG=PW\%d\r\n", wheelPWMvalue);
  int ret = at_send_check_response("+CMSG: Done", 10000, cmd);
  if (ret)
    printMessages("Sent");
  else
    printMessages("Send failed!\r\n\r\n");
}

void burshPWMGet() {
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

void checkDownlink() {
  unsigned long currentTimeCheckMessages = millis();

  // Check Received Message
  if (is_exist) {
    if (currentTimeCheckMessages - previousTimeCheckMessage > timeIntervalCheckMessage) {
      previousTimeCheckMessage = currentTimeCheckMessages;
      if (is_join == false) {
        int ret = at_send_check_response("+MSG:", 500, "");
        if (ret) {
          recv_prase(recv_buf);
        } else {
          printMessages("No Message");
        }
      }
    }
  }
}

void updateMotorCurrentLimit() {

  if (wheelCurrentUpdate == 1) {

    writeStringToEEPROM(topic_wheelMotorLimit_address, String(wheelCurrentReceived));
    wheelCurrentUpdate = 0;
  }

  if (brushCurrentUpdate == 1) {
    writeStringToEEPROM(topic_brushMotorLimit_address, String(brushCurrentReceived));
    brushCurrentUpdate = 0;
  }
}
