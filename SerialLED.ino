/*
  Arduino Serial LED Sample
  Command:
  START_BYTE   0x1B
  TRIGGER_BYTE 0x31
  STOP_BYTE    0x30
*/
#include <Arduino.h>

#include "Configure.h"
#include "LEDUtil.h"
#include "MP3Util.h"

//-----------------------------------------------------------------------------
// Global Variable Define
//-----------------------------------------------------------------------------
int g_lightMode = 0;
int g_lightModeEx = 0;
int g_MP3Count = 0;
bool g_bRGB[3] = { false };
char g_cRGB[3] = { 0 };
bool isGetStartByte = false;
bool isLEDCustomer = false;
bool isSetVolume = false;
bool isPlayMP3 = false;
extern Adafruit_NeoPixel g_strip;

//-----------------------------------------------------------------------------
// Sub Functions declare
//-----------------------------------------------------------------------------
int checkMode();
void setLED();
void setLEDEx();
void disableLED();
 
//-----------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);   // Initialize serial

  initStrip(); // Initialize WS2812B LED
  initDFPlayer(); // Initialize DFPlayer
}

void loop() {
  serialEvent();
}

/*----------------------------------------------------------------------------
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
----------------------------------------------------------------------------*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();

    // add it to the inputString
    if (inChar == START_BYTE) {
      isGetStartByte = true;
      Serial.println(F("Receive START_BYTE..."));
      continue;
    }
    else if (isGetStartByte && inChar == TRIGGER_BYTE) {
      isGetStartByte = false;
      Serial.print(F("Receive TRIGGER_BYTE: "));
      Serial.print(inChar, HEX);
      Serial.println();
      
      g_lightMode = checkMode();
      Serial.print(F("checkMode() = "));
      Serial.println(g_lightMode);
  
      digitalWrite(MODE_LED_PIN, HIGH); // Set PIN 13 to HIGH, LED on
      delay(500);                       // delay 500ms
      digitalWrite(MODE_LED_PIN, LOW);  // Set PIN 13 to LOW, LED off
      delay(500);                       // delay 500ms

      setLED();
      isPlayMP3 = true;
    }
    else if (isGetStartByte && inChar == TRIGGER_EX_BYTE) {
      isGetStartByte = false;
      Serial.print(F("Receive TRIGGER_EX_BYTE: "));
      Serial.print(inChar, HEX);
      Serial.println();
      
      g_lightModeEx = checkModeEx();
      Serial.print(F("checkModeEx() = "));
      Serial.println(g_lightModeEx);
  
      digitalWrite(MODE_LED_PIN, HIGH); // Set PIN 13 to HIGH, LED on
      delay(500);                       // delay 500ms
      digitalWrite(MODE_LED_PIN, LOW);  // Set PIN 13 to LOW, LED off
      delay(500);                       // delay 500ms

      setLEDEx();
      isPlayMP3 = true;
    }
    else if (isGetStartByte && inChar == SET_VOLUME) {
      isSetVolume = true;
      Serial.print(F("Receive SET_VOLUME: "));
      Serial.print(inChar, HEX);
      Serial.println();
    }
    else if (isSetVolume) {
      int volume = (int)inChar;
      if (volume > 30)
        volume = 30;
      setVolume(volume);
      isSetVolume = false;
    }
    else if (isGetStartByte && inChar == TRIGGER_CUS_BYTE) {
      isLEDCustomer = true;
      Serial.print(F("Receive TRIGGER_CUS_BYTE: "));
      Serial.print(inChar, HEX);
      Serial.println();
    }
    else if (isLEDCustomer) {
      if (!g_bRGB[0])
      {
        g_bRGB[0] = true;
        g_cRGB[0] = inChar;
      }
      else if (!g_bRGB[1])
      {
        g_bRGB[1] = true;
        g_cRGB[1] = inChar;
      }
      else if (!g_bRGB[2])
      {
        g_bRGB[2] = true;
        g_cRGB[2] = inChar;

        digitalWrite(MODE_LED_PIN, HIGH); // Set PIN 13 to HIGH, LED on
        delay(500);                       // delay 500ms
        digitalWrite(MODE_LED_PIN, LOW);  // Set PIN 13 to LOW, LED off
        delay(500);                       // delay 500ms

        Serial.print(F("CustomerLED("));
        Serial.print(g_cRGB[0], HEX);
        Serial.print(F(", "));
        Serial.print(g_cRGB[1], HEX);
        Serial.print(F(", "));
        Serial.print(g_cRGB[2], HEX);
        Serial.println(F(")"));
        setCustomerLED(g_cRGB[0], g_cRGB[1], g_cRGB[2]);
        isLEDCustomer = false;
        for (int i = 0; i < 3; i++) {
            g_bRGB[i] = false;
            g_cRGB[i] = 0;
        }
        isPlayMP3 = true;
      } else {
        Serial.println(F("Receive Error, disable LED..."));
        disableLED();
      }
    }
    else if (isGetStartByte && inChar == STOP_BYTE) {
      isGetStartByte = false;
      g_lightMode = 0;
      g_lightModeEx = 0;

      disableLED();
      Serial.println(F("Receive STOP_BYTE..."));
    }
    else {
      Serial.print(F("Receive Error: "));
      Serial.print(inChar, HEX);
      Serial.println();
      continue;
    }

#ifdef MP3_PLAYER
    if (isPlayMP3)
    {
      isPlayMP3 = false;
      //----Play the first mp3----
      Serial.println(F("Play mp3..."));
      g_MP3Count++;
      if (g_MP3Count > 6)
        g_MP3Count = 0;
      switch (g_MP3Count)
      {
      case 0:
      case 1:
        playWithID(1);
        break;
      case 2:
        playWithID(2);
        break;
      case 3:
        playWithID(3);
        break;
      case 4:
        playWithID(4);
        break;
      case 5:
        playWithID(5);
        break;
      case 6:
        playWithID(6);
        break;
      default:
        playWithID(1);
        break;
      }
    }
#endif
  }
}

//-----------------------------------------------------------------------------
int checkMode()
{
  int mode = g_lightMode;
  if (mode < MODE_NUMS)
  {
    mode++;
  }
  else
  {
    mode = 0;
  }

  return mode;
}

int checkModeEx()
{
  int mode = g_lightModeEx;
  if (mode < MODEEX_NUMS)
  {
    mode++;
  }
  else
  {
    mode = 0;
  }

  return mode;
}

void setLED()
{
  #ifdef LED_LIGHT
    switch (g_lightMode)
    {
    case 0:
      colorWipe(0, 100); // Disable All LED
      break;
    case 1:
      strobe(0xff, 0xff, 0xff, 10, 50, 1000, true);
      break;
    case 2:
      // Some example procedures showing how to display to the pixels:
      colorWipe(g_strip.Color(255, 0, 0), 0); // Red，parameters of colorWipe (RGB color, delay time).
      break;
    case 3:
      // Some example procedures showing how to display to the pixels:
      colorWipe(g_strip.Color(0, 255, 0), 0); // Green
      break;
    case 4:
      colorWipe(g_strip.Color(0, 0, 255), 0); // Blue
      break;
    case 5:
      colorWipe(g_strip.Color(255, 255, 0), 10); // Yellow
      break;
    case 6:
      colorWipe(g_strip.Color(139, 0, 255), 20); // Purple
      break;
    default: // For testing
      // Some example procedures showing how to display to the pixels:
      colorWipe(g_strip.Color(255, 0, 0), 500); // Red，parameters of colorWipe (RGB color, delay time).
      colorWipe(g_strip.Color(0, 255, 0), 500); // Green
      colorWipe(g_strip.Color(0, 0, 255), 500); // Blue
      rainbowCycle(20);
      break;
    }
#endif
}

void setLEDEx()
{
  #ifdef LED_LIGHT
    switch (g_lightModeEx)
    {
    case 0:
      colorWipe(0, 100); // Disable All LED
      break;
    case 1:
      newKITT(0xff, 0, 0, 8, 50, 250);
      break;
    case 2:
      cylonBounce(0xff, 0, 0, 4, 50, 250);
      break;
    case 3:
      runningLights(0xff, 0xff, 0x00, 150);
      break;
    case 4:
      meteorRain(0xff, 0xff, 0xff, 10, 64, true, 30);
      break;
    case 5:
      fadeInOut(0xff, 0x77, 0x00);
      fadeInOut(0xff, 0x77, 0x77);
      break;
    case 6:
      rainbowSingle();
      //rainbowCycle(20);
      break;
    default: // For testing
      // Some example procedures showing how to display to the pixels:
      colorWipe(g_strip.Color(255, 0, 0), 500); // Red，parameters of colorWipe (RGB color, delay time).
      colorWipe(g_strip.Color(0, 255, 0), 500); // Green
      colorWipe(g_strip.Color(0, 0, 255), 500); // Blue
      rainbowCycle(20);
      break;
    }
#endif

}

void setCustomerLED(char r, char g, char b)
{
    colorWipe(g_strip.Color(r, g, b), 0); // Red，parameters of colorWipe (RGB color, delay time).
}

void disableLED()
{
  colorWipe(0, 100); // Disable All LED
}



