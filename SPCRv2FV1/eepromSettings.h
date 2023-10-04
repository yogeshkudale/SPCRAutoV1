#include <EEPROM.h>  // include EEPROM library

#define topic_speed_wheel_offset_address 100
#define topic_speed_brush_offset_address 130
#define topic_brushMotorDockingSpeed_address 160
#define topic_speed_offset_address 190
#define topic_left_offset_address 210
#define topic_right_offset_address 240
#define topic_brushMotorLimit_address 270
#define topic_wheelMotorLimit_address 300
#define FirstTimeUpload_address 330
#define deviceSerialNumber_address 330

String topic_speed_wheel_eeprom_value;
String topic_speed_brush_eeprom_value;
String topic_BrushMotorDockingSpeed_value;
String topic_left_eeprom_value;
String topic_right_eeprom_value;
String topic_brushMotorLimit_value;
String topic_wheelMotorLimit_value;
String FirstTimeUpload;
String deviceSerialNumber;


//FirstTime Settings
String topic_speed_wheel_eeprom_value_default = "255";
String topic_speed_brush_eeprom_value_default = "255";
String topic_BrushMotorDockingSpeed_value_default = "255";
String topic_left_eeprom_value_default = "1";  //Docking at left
String topic_right_eeprom_value_default = "0";
String topic_brushMotorLimit_value_default = "230";
String topic_wheelMotorLimit_value_default = "210";
String FirstTimeUpload_value_default = "1";
String deviceSerialNumber_default = "2223001";


int writeStringToEEPROM(int addrOffset, const String &strToWrite) {
  byte len = strToWrite.length();
  // EEPROM.write(addrOffset, len);
  EEPROM.update(addrOffset, len);
  for (int i = 0; i < len; i++) {
    // EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
    //  EEPROM.update(address, val);
    EEPROM.update(addrOffset + 1 + i, strToWrite[i]);
  }
  return addrOffset + 1 + len;
}
int readStringFromEEPROM(int addrOffset, String *strToRead) {
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++) {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0';  // !!! NOTE !!! Remove the space between the slash "/" and "0" (I've added a space because otherwise there is a display bug)
  *strToRead = String(data);
  return addrOffset + 1 + newStrLen;
}

void firstTimeSettings() {
  readStringFromEEPROM(FirstTimeUpload_address, &FirstTimeUpload);
  int FirstTimeUploadCheck = FirstTimeUpload.toInt();

  if (FirstTimeUploadCheck != 1) {

    writeStringToEEPROM(topic_speed_wheel_offset_address, topic_speed_wheel_eeprom_value_default);
    writeStringToEEPROM(topic_speed_brush_offset_address, topic_speed_brush_eeprom_value_default);
    writeStringToEEPROM(topic_brushMotorDockingSpeed_address, topic_BrushMotorDockingSpeed_value_default);
    writeStringToEEPROM(topic_left_offset_address, topic_left_eeprom_value_default);
    writeStringToEEPROM(topic_right_offset_address, topic_right_eeprom_value_default);
    writeStringToEEPROM(topic_brushMotorLimit_address, topic_brushMotorLimit_value_default);
    writeStringToEEPROM(topic_wheelMotorLimit_address, topic_wheelMotorLimit_value_default);
    writeStringToEEPROM(FirstTimeUpload_address, FirstTimeUpload_value_default);
    writeStringToEEPROM(deviceSerialNumber_address, deviceSerialNumber_default);

    FirstTimeUploadCheck = 1;
    Serial.println("Settings Updated Successfully!");

  } else {
    Serial.println("Settings Already Up to Date!");
  }

  Serial.println("Checking Settings...");

  readStringFromEEPROM(topic_speed_wheel_offset_address, &topic_speed_wheel_eeprom_value);
  readStringFromEEPROM(topic_speed_brush_offset_address, &topic_speed_brush_eeprom_value);
  readStringFromEEPROM(topic_brushMotorDockingSpeed_address, &topic_BrushMotorDockingSpeed_value);
  readStringFromEEPROM(topic_left_offset_address, &topic_left_eeprom_value);
  readStringFromEEPROM(topic_right_offset_address, &topic_right_eeprom_value);
  readStringFromEEPROM(topic_brushMotorLimit_address, &topic_brushMotorLimit_value);
  readStringFromEEPROM(topic_wheelMotorLimit_address, &topic_wheelMotorLimit_value);
  readStringFromEEPROM(FirstTimeUpload_address, &FirstTimeUpload);
  readStringFromEEPROM(deviceSerialNumber_address, &deviceSerialNumber);

  Serial.println("topic_speed_wheel_eeprom_value: " + topic_speed_wheel_eeprom_value);
  Serial.println("topic_speed_brush_eeprom_value: " + topic_speed_brush_eeprom_value);
  Serial.println("topic_BrushMotorDockingSpeed_value: " + topic_BrushMotorDockingSpeed_value);
  Serial.println("topic_left_eeprom_value: " + topic_left_eeprom_value);
  Serial.println("topic_right_eeprom_value: " + topic_right_eeprom_value);
  Serial.println("topic_brushMotorLimit_value: " + topic_brushMotorLimit_value);
  Serial.println("topic_wheelMotorLimit_value: " + topic_wheelMotorLimit_value);
  Serial.println("FirstTimeUpload_value: " + FirstTimeUpload);
  Serial.println("deviceSerialNumber_value: " + deviceSerialNumber);
}