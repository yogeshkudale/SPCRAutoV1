void setMotorCurrentLimits() {
  readStringFromEEPROM(topic_brushMotorLimit_address, &topic_brushMotorLimit_value);
  burshMotorCurrentLimit = topic_brushMotorLimit_value.toInt();
  readStringFromEEPROM(topic_wheelMotorLimit_address, &topic_wheelMotorLimit_value);
  wheelMotorCurrentLimit = topic_wheelMotorLimit_value.toInt();
  printMessages("Motor Current Limits Fetched");

}



void cleanRight()
{
  printMessages(String(brushMotorCurrentValue));
  printMessages("Clean Right");
  readStringFromEEPROM(topic_speed_wheel_offset_address, &topic_speed_wheel_eeprom_value);
  wheelPWMvalue = topic_speed_wheel_eeprom_value.toInt();
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


}

void cleanLeft()
{

    float driveMotorCurrentAmps = adc_to_current(driveMotorCurrentValue);
  float brushMotorCurrentAmps = adc_to_current(brushMotorCurrentValue);
      printMessages("D");
  printMessages(String(driveMotorCurrentAmps));
  printMessages("A");

    printMessages("B");
    printMessages(String(brushMotorCurrentAmps));
  printMessages("A");


  printMessages("Clean Left");
  readStringFromEEPROM(topic_speed_wheel_offset_address, &topic_speed_wheel_eeprom_value);
  wheelPWMvalue = topic_speed_wheel_eeprom_value.toInt();
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



}

void stopMotors()
{

  curr_state = 'S';


  analogWrite(wheel_pwm, 0);
  delay(100);
  readStringFromEEPROM(topic_speed_brush_offset_address, &topic_speed_brush_eeprom_value);
  brushPWMvalue = topic_speed_brush_eeprom_value.toInt();
  // printMessages("Both Motors Stopped");
  
  // Stop Motor 2 from 100% to 0% PWM
  for (int i = currentBrushPWM; i > 0; i--) {
    analogWrite(brush_pwm, i);
    Serial.println(i);
    delay(20);
  }
  currentBrushPWM = 0;
  currentWheelPWM = 0; 
  motorRunningState  = LOW;

}

void moveRight()
{
  printMessages("Move Right");
  readStringFromEEPROM(topic_speed_wheel_offset_address, &topic_speed_wheel_eeprom_value);
  wheelPWMvalue = topic_speed_wheel_eeprom_value.toInt();
  readStringFromEEPROM(topic_brushMotorDockingSpeed_address, &topic_BrushMotorDockingSpeed_value);
  antiStuckBrushPWM = topic_BrushMotorDockingSpeed_value.toInt();
  motorRunningState  = HIGH;

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


}

void moveLeft()
{
  printMessages("Move Left");
  readStringFromEEPROM(topic_speed_wheel_offset_address, &topic_speed_wheel_eeprom_value);
  wheelPWMvalue = topic_speed_wheel_eeprom_value.toInt();
  readStringFromEEPROM(topic_brushMotorDockingSpeed_address, &topic_BrushMotorDockingSpeed_value);
  antiStuckBrushPWM = topic_BrushMotorDockingSpeed_value.toInt();  
  motorRunningState  = HIGH;

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
}




void getMotorSettings(){
  if (wheelPWMGetState == HIGH) {

    //Code to get PWM
    wheelPWMGet();
    wheelPWMGetState = LOW;
      printMessages("Wheel PWM sent");

  }
  if (burshPWMGetState == HIGH) {

    //Code to get PWM
    burshPWMGet();
    burshPWMGetState = LOW;
      printMessages("Brush PWM Sent");


  }

    if (wheelSpeedUpdate == HIGH) {

    //Code to SET PWM3
    updateWheelSpeed();
    wheelSpeedUpdate = LOW;
    // printMessages("Wheel Speed set to HIGH!");


  }


  // if (wheelPWM1State == HIGH) {

  //   //Code to SET PWM1
  //   wheelPWM1();
  //   wheelPWM1State = LOW;
  //     printMessages("Wheel Speed set to LOW!");

  // }
  // if (wheelPWM2State == HIGH) {

  //   //Code to SET PWM2
  //   wheelPWM2();
  //   wheelPWM2State = LOW;
  //         printMessages("Wheel Speed set to MEDIUM!");


  // }
  // if (wheelPWM3State == HIGH) {

  //   //Code to SET PWM3
  //   wheelPWM3();
  //   wheelPWM3State = LOW;
  //             printMessages("Wheel Speed set to HIGH!");

  // }
  // if (brushPWM1State == HIGH) {

  //   //Code to SET PWM1
  //   brushPWM1();
  //   brushPWM1State = LOW;
  //                 printMessages("BRUSH Speed set to LOW!");

  // }
  // if (brushPWM2State == HIGH) {

  //   //Code to SET PWM2
  //   brushPWM2();
  //   brushPWM2State = LOW;
  //                     printMessages("BRUSH Speed set to MEDIUM!");

  // }
  // if (brushPWM3State == HIGH) {

  //   //Code to SET PWM3
  //   brushPWM3();
  //   brushPWM3State = LOW;
  //                         printMessages("BRUSH Speed set to HIGH!");


  // }

    if (brushSpeedUpdate == HIGH) {

    //Code to SET PWM3
    updateBrushSpeed();
    brushSpeedUpdate = LOW;
    // printMessages("BRUSH Speed set to HIGH!");


  }

  if (set40WattMode == 1) {
    writeStringToEEPROM(topic_brushMotorLimit_address, "230"); //Update to EPROM
    printMessages("40 Watt Brush Motor Mode Success");
    writeStringToEEPROM(topic_wheelMotorLimit_address, "180"); //Update to EPROM
    printMessages("40 Watt Brush Motor Mode for Wheel Success");
    set40WattMode = 0;
  }

  if (set60WattMode == 1) {
    writeStringToEEPROM(topic_brushMotorLimit_address, "230"); //Update to EPROM
    printMessages("60 Watt Brush Motor Mode Success");
    writeStringToEEPROM(topic_wheelMotorLimit_address, "210"); //Update to EPROM
    printMessages("60 Watt Brush Motor Mode for Wheel Success");
    set60WattMode = 0;

  }

}