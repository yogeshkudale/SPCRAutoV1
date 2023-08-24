/*
  base64 to TEXT to Hex - Function

**Commands Sent to the robots
  QlY= - BV - 4256 - Check Battery Voltage
  QzE= - C1 - 4331 - Cleaning Cycle Start
  Q0M= - CC - 4343 - Cancel Cleaning
  UFc= - PW - 5057 - PWM WHEEL GET
  UEI= - PB - 5042 - PWM Brush Get
  QjE= - B1 - 4231 - Set Wheel PWM Value TO 150
  QjI= - B2 - 4232 - Set Wheel PWM Value TO 200
  QjM= - B3 - 4233 - Set Wheel PWM Value TO 250
  VzE= - W1 - 5731 - Set Brush PWM Value TO 150
  VzI= - W2 - 5732 - Set Brush PWM Value TO 200
  VzM= - W3 - 5733 - Set Brush PWM Value TO 250
  QzQ= - C4 - 4334 - Move Left
  QzU= - C5 - 4335 - Move Right
  QzI= - C2 - 4332 - Clean Left
  QzM= - C3 - 4333 - Clean Right
  RDE= - D1 - 4431 - Back to Docking Station
  RDI= - D2 - 4432 - Set Left Docking Station
  RDM= - D3 - 4433 - Set Right Docking Station
  VFM= - TS - 5453 - Turn ON Test Mode
  VEM= - TC - 5443 - Turn OFF Test Mode
  QVc= - AW - 4157 - Weather Lock
  UkU= - RE - 5245 - Reset Board
  
**Commands Sent From The Robots
  MD"driveCurrent"MB"brushCurrent" - Motor Current Values
  B"VALUE" - Battery Capacity Value
  C1 - Cleaning Started
  CC - Cleaning Cancelled
  DR - Docking Station Reached
  C3 -  - Right cleaning start
  C2 -  - Left cleaning start
  RS -  - Right cleaning stop
  LS -  - Left cleaning stop
  CF - Cleaning Finished
  US -  - Updated Data Sucessfully
  F1 - Fault Cause 1 - Wheel Motor Fault
  F2 - Fault Cause 2 - Brush Motor Fault
  F3 -  Fault Cause 3 - Switch Fault (Both Switch Triggerred)
  F4 - Hall effect sensor error
  F5 - Battery Low
  RD - Return to dock
  C4 - 4334 - Move Left
  C5 - 4335 - Move Right
  PW"Value" = Wheel PWM Value
  PB"Value" = Brush PWM Value
  T"Value" = Board Temperature
*/
