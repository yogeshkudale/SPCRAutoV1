void manualMode() {
  // Check Switch State
  if (leftSwitchState == LOW && rightSwitchState == HIGH && switchMoveRight == LOW && returnCall == LOW) {
    printMessages("Left Side Cleaning");


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
  if (rightSwitchState == LOW && leftSwitchState == HIGH && switchMoveLeft == LOW && returnCall == LOW) {
    //Right Side Cleaning
    printMessages("Right Side cleaning");


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
  if (leftSwitchState == LOW && rightSwitchState == HIGH && switchMoveRight == LOW && returnCall == HIGH) {
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
  if (rightSwitchState == LOW && leftSwitchState == HIGH && switchMoveLeft == LOW && returnCall == HIGH) {
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
  if (rightSwitchState == LOW && leftSwitchState == LOW) {
    //ERROR BOTH SWITCH TRIGGERRED
    stopState = HIGH;
    switchState = 1;
    printMessages("Error! Both Switch Triggered!");
    switchMoveLeft = 0;
    switchMoveRight = 0;
    sendSwitchFault = 1;
    sendData = 1;
  }

  if (switchMoveLeft == HIGH && leftSensorState == LOW) {  ////
    //Code to move forward
    stopState = 0;
    switchState = LOW;
    // printMessages("Clean Left Started");

    cleanLeft();
  }
  if (switchMoveLeft == HIGH && leftSensorState == HIGH) {  ////
    //Code to stop motors
    stopState = HIGH;
    switchMoveLeft = LOW;
    sendCleaningFinished = 1;
    sendData = 1;
    printMessages("Clean Left Finsished");
  }
  if (switchMoveRight == HIGH && rightSensorState == LOW) {  ////
    //Code to move reverse
    stopState = 0;
    switchState = LOW;
    printMessages("Clean Right Started");

    cleanRight();
  }
  if (switchMoveRight == HIGH && rightSensorState == HIGH) {  ////
    //Code to stop motors
    stopState = HIGH;
    switchMoveRight = LOW;
    sendCleaningFinished = 1;
    sendData = 1;
    printMessages("Clean Right Finished");
  }
  if (leftSensorState == HIGH && rightSensorState == HIGH) {  ////
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
