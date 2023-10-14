
//Battery Voltage Check
#include <Battery.h>
#include <avr/wdt.h>

int minBatteryVoltage = 11000;
int maxBatteryVoltage = 14200;

Battery battery(minBatteryVoltage, maxBatteryVoltage, batteryVoltage);

#define NUM_SAMPLES 10
#define DELAY_BETWEEN_SAMPLES 10  // in milliseconds

int getAverageBatteryLevel() {
    int total = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        total += battery.level();
        delay(DELAY_BETWEEN_SAMPLES);
    }
    return total / NUM_SAMPLES;
}

int getAverageBatteryVoltage() {
    int total = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        total += battery.voltage();
        delay(DELAY_BETWEEN_SAMPLES);
    }
    return total / NUM_SAMPLES;
}



String batteryVoltageRead() {

  //  Serial.println(battery.level());
  //  SerialMon.println(vbat);
    return String(getAverageBatteryLevel());
}
int batteryVoltageInt() {

  //  Serial.println(battery.level());
  //  SerialMon.println(vbat);
    return getAverageBatteryLevel();
}

void batteryVoltageCheck() {
  char cmd[128];
  int batteryVoltageValue = batteryVoltageInt();
  //    int batteryVoltageValue = random(40, 69);
  //  int batteryVoltageValue = 54;
  printMessages("BatteryVoltage");
  printMessages(String(batteryVoltageValue));


  sprintf(cmd, "AT+MSG=B\%d\r\n", batteryVoltageValue);
  int ret = at_send_check_response("+MSG: Done", 10000, cmd);
  if (ret) {
    printMessages("Sent");
  } else {
    printMessages("Send failed!\r\n\r\n");
  }




  //Battery Voltage Check
  if (automaticMode == 1 && batteryLowState == 0) {
    if (batteryVoltageValue < 10) {
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
    if (batteryVoltageValue < 1) {
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
void checkBatteryVoltage() {

  unsigned long currentTimeSendMessages = millis();

  //   Sync with server Message
  if (is_exist) {
    if (currentTimeSendMessages - previousTimeSendMessage > timeIntervalSendMessage) {
      previousTimeSendMessage = currentTimeSendMessages;

      batteryVoltageCheck();
      LinkCheckReq();
      batteryLowState = 0;
    }
  }
}