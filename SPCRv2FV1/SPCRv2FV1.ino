#include <EEPROM.h>
#include "pins.h";
#include "eepromSettings.h";
#include "stateFlags.h";
#include "setup.h";
#include "loraSettings.h";

#include "batterySettings.h";

#include "sendData.h";
#include "safetyFeatures.h";

#include "debugLED.h";

#include "motorFunctions.h";
#include "manualMode.h";
#include "automaticMode.h";

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
  firstTimeSettings(); //eepromSettings.h

  //Setup to LoraWAN Server Parameters
  setupLora(); //loraSettings.h
  connectLNS(); //loraSettings.h


  //Battery Setup
  battery.begin(vdd, 10.1); //batterySense-master Github 3090, 10.1 //Do not change

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
  setMotorCurrentLimits(); //motorFunctions.h


}

void loop() {

  // read the state of the sensor value:
  leftSensorState = (PIND & (1 << leftSensor)) >> leftSensor;
  rightSensorState = (PIND & (1 << rightSensor)) >> rightSensor;

  // read the state of the sensor value:
  leftSwitchState  = (PINA & (1 << leftSwitch)) >> leftSwitch;
  rightSwitchState  = (PINB & (1 << rightSwitch)) >> rightSwitch;



  manualMode(); //manualMode.h
  executeManualReturnCleaning(); //manualMode.h
  executeCleaningCycle(); //automaticMode.h
  returnToDock(); //automaticMode.h
  weatherLockEnable(); //automaticMode.h
  dockRight(); //automaticMode.h
  dockLeft(); //automaticMode.h
  remoteMovements(); //automaticMode.h
  networkConnected(); //debugLED.h
  msgReceived(); //debugLED.h
  motorRunning(); //debugLED.h
  sendMessagestoServer(); //sendData.h
  safetyFeatures(); //safetyFeatures.h
  checkDownlink();  //loraSettings.h
  checkBatteryVoltage(); //loraSettings.h
  getMotorSettings(); //motorFunctions.h

  //Stop Motors/Any Current Ongoin Operation
  if (stopState == HIGH) {

    //Code to Stop Motors
    stopMotors();

  }

//  Reset The Board
  if (resetFirmwareValue == 1) {
    resetFirmware();
  }
  
}
