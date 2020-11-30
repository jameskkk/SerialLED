#ifndef LED_UTIL_H
#define LED_UTIL_H

#include <Adafruit_NeoPixel.h>
#include <FastLED.h>

// WS2812B LED control functions
void initStrip();
void showStrip();
void setPixel(int Pixel, byte red, byte green, byte blue);
void setAllLED(byte red, byte green, byte blue);
void colorWipe(uint32_t color, uint8_t wait);

// fadeInOut(0xff, 0x00, 0x00); // red
void fadeInOut(byte red, byte green, byte blue);

// Slower:
// strobe(0xff, 0x77, 0x00, 10, 100, 1000);
// Fast:
// strobe(0xff, 0xff, 0xff, 10, 50, 1000);
void strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause, bool OpenOnEnd);

// cylonBounce(0xff, 0, 0, 4, 10, 50);
void cylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay);

// runningLights(0xff,0xff,0x00, 50);
void runningLights(byte red, byte green, byte blue, int WaveDelay);

// meteorRain(0xff,0xff,0xff,10, 64, true, 30);
void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay);
void fadeToBlack(int ledNo, byte fadeValue);

// NewKITT(0xff, 0, 0, 8, 10, 50);
void newKITT(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay);

// Rainbow light functions
void rainbowSingle();
void rainbowCycle(uint8_t wait);

// Unusing --------------------------------------------------
// halloweenEyes(0xff, 0x00, 0x00, 1,4, true, 10, 80, 3000);
void halloweenEyes(byte red, byte green, byte blue, int EyeWidth, int EyeSpace, boolean Fade, int Steps, int FadeDelay, int EndPause);

// twinkle(0xff, 0, 0, 10, 100, false);
void twinkle(byte red, byte green, byte blue, int Count, int SpeedDelay, boolean OnlyOne);

// sparkle(0xff, 0xff, 0xff, 0);
void sparkle(byte red, byte green, byte blue, int SpeedDelay);

// theaterChase(0xff, 0, 0, 50);
void theaterChase(byte red, byte green, byte blue, int SpeedDelay);

// fire(55, 120, 15);
void fire(int Cooling, int Sparking, int SpeedDelay);

// Meteor Light functions
void initMeteorLight();
void startMeteorLight();

#endif