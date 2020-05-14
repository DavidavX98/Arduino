#include <SPI.h>
#include "Ucglib.h"

Ucglib_ILI9341_18x240x320_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);
uint8_t button1 = 4;
uint8_t button2 = 2;
uint8_t leds = 3;
uint8_t grayLevel = 120;
uint8_t martorXPozition = 120;
////////////////////drawCar/////////////////////////////////////////////////
void drawCar(uint8_t x, uint8_t y) {
  ucg.setColor(0, 150, 130);
  ucg.drawRBox(x, y + 5, 80, 160, 20);
  ucg.setColor(0, 180, 160);
  ucg.drawRBox(x, y, 80, 160, 20);
  ucg.setColor(130, 235, 255);
  ucg.drawRBox(x + 3, y + 40, 74, 117, 18);
  ucg.setColor(0, 180, 160);
  ucg.drawLine(x + 6, y + 40, x + 13, y + 75);
  ucg.drawLine(x + 73, y + 40, x + 66, y + 75);
  ucg.drawLine(x + 6, y + 153, x + 13, y + 134);
  ucg.drawLine(x + 73, y + 153, x + 66, y + 134);
  ucg.drawRBox(x + 10, y + 70, 60, 70, 8);
}
/////////////////drawLight/////////////////////////////////////
void drawLight(uint8_t x, uint8_t y, uint8_t color) {
  switch (color) {
    case 0:
      ucg.setColor(grayLevel, grayLevel, grayLevel);
      break;
    case 1:
      ucg.setColor(255, 255, 0);
      break;
    default:
      ucg.setColor(255, 0, 0);
  }
  ucg.drawDisc(x, y, 7, 12);
  ucg.drawLine(x - 3, y - 3, x - 5, y - 15);
  ucg.drawLine(x, y - 3, x, y - 15);
  ucg.drawLine(x + 3, y - 3, x + 5, y - 15);
}
/////////////////drawIconLightAuto/////////////////////////////////////
void drawIconLightAuto(uint8_t x, uint8_t y, uint8_t color) {
  switch (color) {
    case 1:
      ucg.setColor(0, 0, 255);
      break;
    default:
      ucg.setColor(grayLevel, grayLevel, grayLevel);
  }
  ucg.drawDisc(x, y, 7, 12);
  ucg.drawLine(x - 4, y - 3, x - 5, y - 15);
  ucg.drawLine(x, y - 11, x, y - 15);
  ucg.drawLine(x + 4, y - 3, x + 5, y - 15);
  ucg.setFont(ucg_font_6x10_tr);
  ucg.setPrintPos(x - 2, y - 2);
  ucg.print("A");
}
/////////////////drawIconHorn/////////////////////////////////////
void drawIconHorn(uint8_t x, uint8_t y, uint8_t color) {
  switch (color) {
    case 1:
      ucg.setColor(0, 255, 255);
      break;
    default:
      ucg.setColor(grayLevel, grayLevel, grayLevel);
  }
  ucg.drawTriangle(x - 5, y - 13, x + 5, y - 13, x, y - 7);
  ucg.drawLine(x, y - 7, x, y + 5);
  ucg.drawLine(x - 1, y + 5, x + 1, y + 5);
  ucg.drawRFrame(x - 5, y - 7, 5, 10, 1);
}
/////////////////drawEmergencyLightIcon/////////////////////////////////////
void drawEmergencyLightIcon(uint8_t x, uint8_t y, uint8_t color) {
  switch (color) {
    case 1:
      ucg.setColor(255, 0, 0);
      break;
    default:
      ucg.setColor(grayLevel, grayLevel, grayLevel);
  }
  ucg.drawLine(x - 10, y + 5, x + 10, y + 5);
  ucg.drawLine(x + 10, y + 5, x, y - 15);
  ucg.drawLine(x, y - 15, x - 10, y + 5);
  ucg.drawTriangle(x - 6, y + 3, x + 6, y + 3, x, y - 10);
}

///////////////////////lightsEnable///////////////////////////////////////
bool lastLeftLed = 0;
bool lastRightLed = 0;
bool LeftLed = 0;
bool RightLed = 0;
bool lastEnable = 0;
void lightsEnable(bool enable) {
  digitalWrite(leds, enable);
  LeftLed = (analogRead(A1) > 550 && analogRead(A1) < 700);
  RightLed = (analogRead(A2) > 550 && analogRead(A2) < 700);
  if (enable != lastEnable || LeftLed != lastLeftLed || RightLed != lastRightLed) {
    if (enable) {
      if (LeftLed != lastLeftLed || enable != lastEnable) {
        if (LeftLed)
          drawLight(95, 68, 1);
        else
          drawLight(95, 68, 2);
      }
      if (RightLed != lastRightLed || enable != lastEnable) {
        if (RightLed)
          drawLight(144, 68, 1);
        else
          drawLight(144, 68, 2);
      }
    } else {
      drawLight(144, 68, 0);
      drawLight(95, 68, 0);
    }
  }
  lastEnable = enable;
  lastLeftLed = LeftLed;
  lastRightLed = RightLed;
}
////////////////lightsEnableAuto/////////////////////////////////////////
bool autoEnable = 0;
bool lastAutoEnable = 0;
bool lastEnable1 = 0;
void lightsEnableAuto(bool enable) {
  if (enable != lastEnable1) {
    if (enable) {
      if (!autoEnable && analogRead(A0) < 506)
        autoEnable = 1;
      else if (autoEnable && analogRead(A0) > 518)
        autoEnable = 0;
      if (autoEnable != lastAutoEnable || enable) {
        lightsEnable(autoEnable);
      }
      lastAutoEnable = autoEnable;
    }
  }
}
/////////////////emergencyLight/////////////////////////////////////
uint64_t timer = 0;
bool lightsOn1 = 0;
bool lastLightsOn1 = 0;
bool lastEnable2;
void emergencyLight(bool enable) {
  if (enable) {
    if (millis() - timer > 500) {
      lightsOn1 = !lightsOn1;
      timer = millis();
    }
    if (lastLightsOn1 != lightsOn1) {
      lightsEnable(lightsOn1);
      drawEmergencyLightIcon(martorXPozition+45, 20, lightsOn1);
    }
    lastLightsOn1 = lightsOn1;
  } else if (lastEnable2 != enable) {
    drawEmergencyLightIcon(martorXPozition+45, 20, 0);
  }
  lastEnable2 = enable;
}
//////////////drawSensor//////////////////////////////////////////////////
void drawLevel(uint8_t x, uint8_t y, uint8_t raza, uint8_t rotire) {
  ucg.drawCircle(x, y, (raza * 10) + 9, rotire);
  ucg.drawCircle(x, y, (raza * 10) + 10, rotire);
}
void drawInitialSensor(uint8_t x, uint8_t y, bool parte) {
  uint8_t rotire = parte ? 8 : 4;
  ucg.setColor(grayLevel, grayLevel, grayLevel);
  drawLevel(x, y, 0, rotire);
  drawLevel(x, y, 1, rotire);
  drawLevel(x, y, 2, rotire);
  drawLevel(x, y, 3, rotire);
}
uint8_t lastLevel[2];
void drawSensor(uint8_t x, uint8_t y, uint8_t level, bool parte) {
  uint8_t rotire = parte ? 8 : 4;
  ucg.setColor(grayLevel, grayLevel, grayLevel);
  if (lastLevel[parte] != level) {
    switch (lastLevel[parte]) {
      case 3:
        drawLevel(x, y, lastLevel[parte], rotire);
        break;
      case 2:
        drawLevel(x, y, lastLevel[parte], rotire);
        break;
      case 1:
        drawLevel(x, y, lastLevel[parte], rotire);
        break;
      case 0:
        drawLevel(x, y, lastLevel[parte], rotire);
        break;
    }
    switch (level) {
      case 3:
        ucg.setColor(0, 255, 0);
        drawLevel(x, y, level, rotire);
        break;
      case 2:
        ucg.setColor(255, 255, 0);
        drawLevel(x, y, level, rotire);
        break;
      case 1:
        ucg.setColor(255, 160, 0);
        drawLevel(x, y, level, rotire);
        break;
      case 0:
        ucg.setColor(255, 0, 0);
        drawLevel(x, y, level, rotire);
        break;
    }
  }
  lastLevel[parte] = level;
}

///////////////PushDebounce////////////////////////////////
boolean buttonState[2];
boolean lastButtonState[2];
unsigned long lastDebounceTime[2];

bool pushDebounce(uint8_t button, uint8_t instance) {
  bool returnValue = 0;
  bool reading = !digitalRead(button);
  if (reading && !lastButtonState[instance]) {
    lastDebounceTime[instance] = millis();
  }
  if (reading != buttonState[instance]) {
    if ((millis() - lastDebounceTime[instance]) > 20 && millis() - lastDebounceTime[instance] < 500)
    {
      buttonState[instance] = reading;
      if (buttonState[instance] == LOW) {
        returnValue = 1;
      }
    }
  }
  lastButtonState[instance] = reading;
  return returnValue;
}

//////////////holdDebounce//////////////////////////////////////////
boolean buttonState1[2];
boolean lastButtonState1[2];
unsigned long lastDebounceTime1[2];

bool holdDebounce(uint8_t button, uint8_t instance) {
  bool returnValue = 0;
  bool reading = !digitalRead(button);
  if (reading != lastButtonState1[instance]) {
    lastDebounceTime1[instance] = millis();
  }
  if (reading && !buttonState1[instance] && millis() - lastDebounceTime1[instance] >= 500) {
    buttonState1[instance] = reading;
    returnValue = 1;
  }
  if (!reading && buttonState1[instance] && millis() - lastDebounceTime1[instance] > 20)
  {
    buttonState1[instance] = reading;
  }
  lastButtonState1[instance] = reading;
  return returnValue;
}
//////////setup/////////////////////////////////////////////////////////

void setup(void) {
  Serial.begin(2000000);
  ucg.begin(UCG_FONT_MODE_TRANSPARENT);
  ucg.clearScreen();
  drawCar(80, 80);
  drawLight(95, 68, 0);
  drawLight(144, 68, 0);
  drawInitialSensor(84, 239, 0);
  drawInitialSensor(155, 239, 1);
  drawLight(martorXPozition-45, 20, 0);
  drawIconLightAuto(martorXPozition-15, 20, 0);
  drawIconHorn(martorXPozition+15, 20, 0);
  drawEmergencyLightIcon(martorXPozition+45, 20, 0);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(leds, OUTPUT);
}
//////////////////////////loop//////////////////////////
String sRead;
uint8_t sensor0 = 0;
uint8_t sensor1 = 0;
uint8_t lastSensor0 = 0;
uint8_t lastSensor1 = 0;

bool lightsOn = 0;
bool lightsAutoEnable = 0;
bool lastLightsOn = 0;
bool lastLightsAutoEnable = 0;
bool hornEnable = 0;
bool lastHornEnable = 0;
bool emergencyLightEnable = 0;
uint32_t timp = 0;
void loop(void)
{
  //Serial.println(analogRead(A0));
  //timp = millis();
  //////////horn//////////////////////////
  hornEnable = !digitalRead(button2);
  if (hornEnable != lastHornEnable) {
    Serial.println(hornEnable);
    drawIconHorn(martorXPozition+15, 20, hornEnable);
  }
  lastHornEnable = hornEnable;
  //////////lightOn///////////////////////
  if (pushDebounce(button1, 0))
  {
    if (lightsOn)
      lightsOn = false;
    else
      lightsOn = true;
  }
  //////////lightsAutoEnable///////////////////////
  if (holdDebounce(button1, 0))
  {
    if (lightsAutoEnable)
      lightsAutoEnable = false;
    else
      lightsAutoEnable = true;
  }
  ////////emergencyLightEnable/////////////////////
  if (holdDebounce(button2, 1))
  {
    emergencyLightEnable = !emergencyLightEnable;
  }
  if (emergencyLightEnable) {
    emergencyLight(1);
  } else {
    emergencyLight(0);
    if (lightsAutoEnable) {
      lightsEnableAuto(1);
    } else {
      lightsEnableAuto(0);
      lightsEnable(lightsOn);
    }
  }
  if (lastLightsOn != lightsOn) {
    if (lightsOn)
      drawLight(martorXPozition-45, 20, 1);
    else
      drawLight(martorXPozition-45, 20, 0);
  }
  if (lastLightsAutoEnable != lightsAutoEnable) {
    if (lightsAutoEnable)
      drawIconLightAuto(martorXPozition-15, 20, 1);
    else
      drawIconLightAuto(martorXPozition-15, 20, 0);
  }
  if (Serial.available() > 0) {
    sRead = Serial.readStringUntil('\n');
    sensor0 = sRead.toInt() / 10;
    sensor1 = sRead.toInt() % 10;
    if (sensor0 != lastSensor0)
      drawSensor(84, 239, sensor0, 0);
    if (sensor1 != lastSensor1)
      drawSensor(155, 239, sensor1, 1);
  }
  lastLightsOn = lightsOn;
  lastLightsAutoEnable = lightsAutoEnable;
  lastSensor0 = sensor0;
  lastSensor1 = sensor1;
}
