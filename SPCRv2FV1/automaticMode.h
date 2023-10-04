
void remoteMovements() {
  if (moveRightState == HIGH && rightSensorState == LOW && moveToDock == 0 && switchState == 0 && executeCycle == 0) {  ////
    //Code to move right
    weatherLockState = LOW;
    stopState = 0;
    moveRight();
  }
  if (moveRightState == HIGH && rightSensorState == HIGH && moveToDock == 0 && switchState == 0 && executeCycle == 0) {  ////
    stopState = 1;
    moveRightState = LOW;
    sendCleaningFinished = 1;
    sendData = 1;

    //Send Error Right Hall Sensor Trigerred
  }
  if (moveLeftState == HIGH && leftSensorState == LOW && moveToDock == 0 && switchState == 0 && executeCycle == 0) {  ////
    //Code to move left
    weatherLockState = LOW;
    stopState = 0;
    moveLeft();
  }
  if (moveLeftState == HIGH && leftSensorState == HIGH && moveToDock == 0 && switchState == 0 && executeCycle == 0) {  ////

    stopState = 1;
    moveLeftState = LOW;
    sendCleaningFinished = 1;
    sendData = 1;


    //Send Error Left Hall Sensor Trigerred
  }
  if (cleanRightState == HIGH && rightSensorState == LOW && moveToDock == 0 && switchState == 0 && executeCycle == 0) {  ////
    //Code to move right
    weatherLockState = LOW;
    stopState = 0;
    cleanRight();
  }
  if (cleanRightState == HIGH && rightSensorState == HIGH && moveToDock == 0 && switchState == 0 && executeCycle == 0) {  ////
    stopState = 1;
    //Send Error Right Hall Sensor Trigerred
    cleanRightState = LOW;
    sendCleaningFinished = 1;
    sendData = 1;
  }
  if (cleanLeftState == HIGH && leftSensorState == LOW && moveToDock == 0 && switchState == 0 && executeCycle == 0) {  ////
    //Code to move left
    weatherLockState = LOW;
    stopState = 0;
    cleanLeft();
  }
  if (cleanLeftState == HIGH && leftSensorState == HIGH && moveToDock == 0 && switchState == 0 && executeCycle == 0) {  ////

    stopState = 1;
    cleanLeftState = LOW;
    //Send Error Left Hall Sensor Trigerred
    sendCleaningFinished = 1;
    sendData = 1;
  }
}



void returnToDock() {
  if (returnToDockState == 1 && switchState == 0) {
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
    if (topic_left_eeprom_value == "1" && topic_right_eeprom_value == "0") {
      //docking at left
      stopState = 0;
      // printMessages("Dock is at left side");
      leftDockInCycleLeft = HIGH;
      rightDockInCycleRight = LOW;
      returnToDockState = LOW;
    }
    if (topic_left_eeprom_value == "0" && topic_right_eeprom_value == "1") {
      //docking at right
      // printMessages("Dock is at right side");
      stopState = 0;
      leftDockInCycleLeft = LOW;
      rightDockInCycleRight = HIGH;
      returnToDockState = LOW;
    }
  }
  if (leftDockInCycleLeft == HIGH && leftSensorState == LOW && moveToDock == 1) {
    stopState = LOW;
    printMessages("Docking at Left");
    moveLeft();
  }
  if (leftDockInCycleLeft == HIGH && leftSensorState == HIGH && moveToDock == 1) {
    printMessages("Stopping Motors");
    stopState = HIGH;
    leftDockInCycleLeft = LOW;
    moveToDock = LOW;
    sendLeftDockReached = 1;
    sendData = 1;
    weatherLockState = 1;
  }
  if (rightDockInCycleRight == HIGH && rightSensorState == LOW && moveToDock == 1) {
    stopState = LOW;
    printMessages("Docking at Right");
    moveRight();
  }
  if (rightDockInCycleRight == HIGH && rightSensorState == HIGH && moveToDock == 1) {
    printMessages("Stopping Motors");
    stopState = HIGH;
    rightDockInCycleRight = LOW;
    moveToDock = LOW;
    sendRightDockReached = 1;
    sendData = 1;
    weatherLockState = 1;
  }
}


void executeCleaningCycle() {
  if (executeCleaning == 1 && executeCycle == 1) {
    Serial.println("Cleaning already in progress");
    executeCleaning = 0;
  }

  if (executeCleaning == 1 && switchState == 0 && moveToDock == 0 && executeCycle == 0) {
    weatherLockState = LOW;
    readStringFromEEPROM(topic_left_offset_address, &topic_left_eeprom_value);
    readStringFromEEPROM(topic_right_offset_address, &topic_right_eeprom_value);
    if (topic_left_eeprom_value == "1" && topic_right_eeprom_value == "0") {
      //docking at left
      // printMessages(F("Dock is at left side"));

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
    if (topic_left_eeprom_value == "0" && topic_right_eeprom_value == "1") {
      //docking at right
      // printMessages(F("Dock is at right side"));

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
  if (leftDockInCycle == HIGH && rightDockInCycleRight == LOW && rightDockInCycleLeft == LOW && executeCycle == 1) {
    leftDockInCycleRight = HIGH;
    leftDockInCycleLeft = LOW;
    leftDockInCycle = LOW;
  }
  if (rightDockInCycle == HIGH && leftDockInCycleRight == LOW && leftDockInCycleLeft == LOW && executeCycle == 1) {
    rightDockInCycleRight = LOW;
    rightDockInCycleLeft = HIGH;
    rightDockInCycle = LOW;
  }
  if (leftDockInCycleRight == HIGH && rightSensorState == LOW && executeCycle == 1) {
    rightDockInCycleRight = LOW;
    rightDockInCycleLeft = LOW;
    //    printMessages("Cleaning Cycle Right");
    cleanRight();
  }
  if (leftDockInCycleRight == HIGH && rightSensorState == HIGH && executeCycle == 1) {
    printMessages("Stopping Motors");
    stopMotors();
    leftDockInCycleRight = LOW;
    leftDockInCycleLeft = HIGH;
  }
  if (leftDockInCycleLeft == HIGH && leftSensorState == LOW && executeCycle == 1) {
    stopState = LOW;
    //    printMessages("Cleaning Cycle Left");
    cleanLeft();
  }
  if (leftDockInCycleLeft == HIGH && leftSensorState == HIGH && executeCycle == 1) {
    printMessages("Stopping Motors");
    stopState = HIGH;
    leftDockInCycleLeft = LOW;
    executeCycle = LOW;
    leftDockInCycle = LOW;
    rightDockInCycle = LOW;
    sendCleaningFinished = 1;
    sendData = 1;
  }
  if (rightDockInCycleLeft == HIGH && leftSensorState == LOW && executeCycle == 1) {
    leftDockInCycleRight = LOW;
    leftDockInCycleLeft = LOW;
    stopState = LOW;
    //    printMessages("Cleaning Cycle Left");
    cleanLeft();
  }
  if (rightDockInCycleLeft == HIGH && leftSensorState == HIGH && executeCycle == 1) {
    stopMotors();
    printMessages("Stopping Motors");
    rightDockInCycleRight = HIGH;
    rightDockInCycleLeft = LOW;
  }
  if (rightDockInCycleRight == HIGH && rightSensorState == LOW && executeCycle == 1) {
    stopState = LOW;
    //    printMessages("Cleaning Cycle Right");
    cleanRight();
  }
  if (rightDockInCycleRight == HIGH && rightSensorState == HIGH && executeCycle == 1) {
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

void weatherLockEnable() {
  if (weatherLockState == 1 && moveToDock == 0 && switchState == 0 && executeCycle == 0) {
    readStringFromEEPROM(topic_left_offset_address, &topic_left_eeprom_value);
    readStringFromEEPROM(topic_right_offset_address, &topic_right_eeprom_value);
    if (topic_left_eeprom_value == "1" && topic_right_eeprom_value == "0") {
      //docking at left
      // printMessages(F("Dock is at left side"));
      leftDockInCycleLeft = HIGH;
      rightDockInCycleRight = LOW;
    }
    if (topic_left_eeprom_value == "0" && topic_right_eeprom_value == "1") {
      //docking at right
      // printMessages(F("Dock is at right side"));
      leftDockInCycleLeft = LOW;
      rightDockInCycleRight = HIGH;
    }
  }
  if (leftDockInCycleLeft == HIGH && leftSensorState == LOW && weatherLockState == 1 && moveToDock == 0 && switchState == 0 && executeCycle == 0) {
    stopState = LOW;
    printMessages("Weather Lock Enabled");
    moveLeft();
  }
  if (leftDockInCycleLeft == HIGH && leftSensorState == HIGH && weatherLockState == 1 && moveToDock == 0 && switchState == 0 && executeCycle == 0) {
    // printMessages("Weather Lock Successful");
    stopState = HIGH;
    leftDockInCycleLeft = LOW;
    returnToDockState = LOW;
  }
  if (rightDockInCycleRight == HIGH && rightSensorState == LOW && weatherLockState == 1 && moveToDock == 0 && switchState == 0 && executeCycle == 0) {
    stopState = LOW;
    printMessages("Weather Lock Enabled");
    moveRight();
  }
  if (rightDockInCycleRight == HIGH && rightSensorState == HIGH && weatherLockState == 1 && moveToDock == 0 && switchState == 0 && executeCycle == 0) {
    // printMessages("Weather Lock Successful");
    stopState = HIGH;
    rightDockInCycleRight = LOW;
    returnToDockState = LOW;
    sendWeatherLockSuccessful = 1;
    sendData = 1;
  }
}
