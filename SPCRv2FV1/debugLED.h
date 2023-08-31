void motorRunning() {
  if (motorRunningState == HIGH) {
    unsigned long currentmotorRunningStateMillis = millis();
    if (currentmotorRunningStateMillis - previousmotorRunningStateMillis >= intervalmotorRunningStateMillis) {
      previousmotorRunningStateMillis = currentmotorRunningStateMillis;
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
      if (ledState == HIGH) {
        PORTD |= (1 << ledDebug); // turn on LED
      } else {
        PORTD &= ~(1 << ledDebug); // turn off LED
      }
    }
  } else {
    unsigned long currentmotorRunningStateMillis = millis();
    if (currentmotorRunningStateMillis - previousmotorRunningStateMillis >= intervalmotorRunningStateMillis) {
      previousmotorRunningStateMillis = currentmotorRunningStateMillis;
      if (ledState == HIGH) {
        ledState = LOW;
        PORTD &= ~(1 << ledDebug); // turn off LED
      }
    }
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
      printMessages("Message Received");

      // if the LED is off turn it on and vice-versa:
      if (PORTD & (1 << ledLora)) {
        PORTD &= ~(1 << ledLora);
      } else {
        PORTD |= (1 << ledLora);
      }
    }
  }
}

void networkConnected()
{
  unsigned long currentLoraLEDMillis = millis();
  if (currentLoraLEDMillis - previousNetworkLEDMillis >= intervalNetworkLEDMillis && is_join == false) {
    previousNetworkLEDMillis = currentLoraLEDMillis;
          // printMessages("Network Connected");

    if (ledState == LOW) {
      ledState = HIGH;
      PORTD |= (1 << ledLora); // turn on the LED on the specified pin
    } else {
      ledState = LOW;
      PORTD &= ~(1 << ledLora); // turn off the LED on the specified pin
    }
  }
}