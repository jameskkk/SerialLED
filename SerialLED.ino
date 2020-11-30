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

//-----------------------------------------------------------------------------
// Global Variable Define
//-----------------------------------------------------------------------------
int g_lightMode = 0;
bool isGetStartByte = false;
extern Adafruit_NeoPixel g_strip;

//-----------------------------------------------------------------------------
// Sub Functions declare
//-----------------------------------------------------------------------------
int checkMode();

//-----------------------------------------------------------------------------
void setup() {
  // initialize serial:
  Serial.begin(115200);

  initStrip(); // Initialize WS2812B LED
}

void loop() {
  // print the string when a newline arrives:
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
      Serial.print(inChar);
      Serial.println();
      
      g_lightMode = checkMode();
      Serial.print(F("checkMode() = "));
      Serial.println(g_lightMode);
  
      digitalWrite(MODE_LED_PIN, HIGH); // Set PIN 13 to HIGH, LED on
      delay(500);                       // delay 500ms
      digitalWrite(MODE_LED_PIN, LOW);  // Set PIN 13 to LOW, LED off
      delay(500);                       // delay 500ms
    }
    else if (isGetStartByte && inChar == STOP_BYTE) {
      isGetStartByte = false;
      g_lightMode = 0;
      Serial.println(F("Receive STOP_BYTE..."));
    }
    else {
      Serial.print(F("Receive Error: "));
      Serial.print(inChar);
      Serial.println();
      continue;
    }

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
      newKITT(0xff, 0, 0, 8, 50, 250);
      break;
    case 3:
      cylonBounce(0xff, 0, 0, 4, 50, 250);
      break;
    case 4:
      runningLights(0xff, 0xff, 0x00, 150);
      break;
    case 5:
      meteorRain(0xff, 0xff, 0xff, 10, 64, true, 30);
      break;
    case 6:
      fadeInOut(0xff, 0x77, 0x00);
      fadeInOut(0xff, 0x77, 0x77);
      break;
    case 7:
      // Some example procedures showing how to display to the pixels:
      colorWipe(g_strip.Color(255, 0, 0), 0); // Red，parameters of colorWipe (RGB color, delay time).
      break;
    case 8:
      // Some example procedures showing how to display to the pixels:
      colorWipe(g_strip.Color(0, 255, 0), 0); // Green
      break;
    case 9:
      colorWipe(g_strip.Color(0, 0, 255), 0); // Blue
      break;
    case 10:
      colorWipe(g_strip.Color(255, 255, 0), 10); // Yellow
      break;
    case 11:
      colorWipe(g_strip.Color(139, 0, 255), 20); // Purple
      break;
    case 12:
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
