#include "LEDUtil.h"
#include "Configure.h"

//Adafruit_NeoPixel g_strip = Adafruit_NeoPixel(1, OUT_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel g_strip  = Adafruit_NeoPixel(NUM_LEDS, WS2812B_PIN, NEO_GRB + NEO_KHZ800); // NEO_GRB(顯示顏色是RGB)
  // Adafruit_NeoPixel就是類別，其建構子有3個參數，分別為LED的數量、硬體連接的腳位、以及LED的型別。
  // 以上例來說，只有1個LED，連接在第6支腳，採用GRB，800KHZ通訊訊號的速率。
  // 為何LED跟通訊訊號的速率有關，因為它採用串接方式，當送出10顆LED命令時，接收到的第一顆LED會取下其命令，然後把9個命令再往下送，以此類推。
  // Parameter 1 = number of pixels in strip
  // Parameter 2 = Arduino pin number (most are valid)
  // Parameter 3 = pixel type flags, add together as needed:
  //   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
  //   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
  //   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
  //   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
  //   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

// Define the array of leds for FastLED
CRGB g_ledsArray[NUM_LEDS];
CRGB g_meteors[NUM_METEORS];

//-----------------------------------------------------------------------------
// Sub Functions declare
//-----------------------------------------------------------------------------
uint32_t setWheel(byte wheelPos);
void centerToOutside(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay);
void outsideToCenter(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay);
void leftToRight(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay);
void rightToLeft(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay);
void setPixelHeatColor(int Pixel, byte temperature);

void initStrip()
{
#ifdef ADAFRUIT_NEOPIXEL_H
  g_strip.begin(); // Initialize Adafruit NeoPixel 
  g_strip.show();  // Initialize all pixels to 'off'. 0 is the default setting of all color pixel.
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  //FastLED.addLeds<WS2812B, WS2812B_PIN>(g_ledsArray, NUM_LEDS);
  FastLED.addLeds<WS2812B, WS2812B_PIN, GRB>(g_ledsArray, NUM_LEDS).setCorrection( TypicalLEDStrip );
#endif
}

//-----------------------------------------------------------------------------
void showStrip() 
{
  #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    g_strip.show();
  #endif
  #ifndef ADAFRUIT_NEOPIXEL_H
    // FastLED
    FastLED.show();
  #endif
}

//-----------------------------------------------------------------------------
void setPixel(int Pixel, byte red, byte green, byte blue) 
{
  #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    g_strip.setPixelColor(Pixel, g_strip.Color(red, green, blue));
  #endif
  #ifndef ADAFRUIT_NEOPIXEL_H
    // FastLED
    g_ledsArray[Pixel].r = red;
    g_ledsArray[Pixel].g = green;
    g_ledsArray[Pixel].b = blue;
  #endif
}

//-----------------------------------------------------------------------------
void setAllLED(byte red, byte green, byte blue) 
{
  for (int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

//-----------------------------------------------------------------------------
// Fill the dots one after the other with a color
//-----------------------------------------------------------------------------
void colorWipe(uint32_t color, uint8_t wait)
{
  //  uint32_t color = strip.Color(val, val, val);  // set LED Array，parameters is (R, G, B)，range is 0-255
  for (uint16_t i = 0; i < g_strip.numPixels(); i++)
  { // strip.numPixels() LED numbers - 1
    g_strip.setPixelColor(i, color);
    g_strip.show();
    delay(wait);
  }

  // From the above, if you want to display the red light on the first LED, the programs are as follows
  // strip.setPixelColor(0, strip.Color(255, 0, 0));
  // strip.show();
  // delay(5000);
}

//-----------------------------------------------------------------------------
void fadeInOut(byte red, byte green, byte blue)
{
  float r, g, b;
     
  for (int k = 0; k < 256; k=k+1) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAllLED(r, g, b);
    showStrip();
  }
     
  for (int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAllLED(r,g,b);
    showStrip();
  }
}

//-----------------------------------------------------------------------------
void strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause, bool OpenOnEnd)
{
  for (int j = 0; j < StrobeCount; j++) {
    setAllLED(red, green, blue);
    showStrip();
    delay(FlashDelay);
    setAllLED(0, 0, 0);
    showStrip();
    delay(FlashDelay);
  }
 
  delay(EndPause);

  if (OpenOnEnd) {
    setAllLED(red, green, blue);
    showStrip();
  }
}

//-----------------------------------------------------------------------------
void cylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay)
{
  for (int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAllLED(0, 0, 0);
    setPixel(i, red/10, green/10, blue/10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for (int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    setAllLED(0, 0, 0);
    setPixel(i, red/10, green/10, blue/10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
 
  delay(ReturnDelay);
}

//-----------------------------------------------------------------------------
void runningLights(byte red, byte green, byte blue, int WaveDelay) 
{
  int Position = 0;
 
  for (int j = 0; j < NUM_LEDS*2; j++)
  {
      Position++; // = 0; //Position + Rate;
      for (int i = 0; i < NUM_LEDS; i++) {
        // sine wave, 3 offset waves make a rainbow!
        //float level = sin(i+Position) * 127 + 128;
        //setPixel(i,level,0,0);
        //float level = sin(i+Position) * 127 + 128;
        setPixel(i,((sin(i+Position) * 127 + 128)/255)*red,
                   ((sin(i+Position) * 127 + 128)/255)*green,
                   ((sin(i+Position) * 127 + 128)/255)*blue);
      }
     
      showStrip();
      delay(WaveDelay);
  }
}

//-----------------------------------------------------------------------------
void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) 
{  
  setAllLED(0, 0, 0);
 
  for (int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
    // fade brightness all LEDs one step
    for (int j=0; j<NUM_LEDS; j++) {
      if ((!meteorRandomDecay) || (random(10) > 5)) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
   
    // draw meteor
    for (int j = 0; j < meteorSize; j++) {
      if (( i-j < NUM_LEDS) && (i-j >= 0)) {
        setPixel(i-j, red, green, blue);
      }
    }
   
    showStrip();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) 
{
 #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
   
    oldColor = g_strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
   
    g_strip.setPixelColor(ledNo, r, g, b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   g_ledsArray[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}

//-----------------------------------------------------------------------------
// Slightly different, this makes the rainbow equally distributed throughout
//-----------------------------------------------------------------------------
void rainbowSingle()
{
  uint16_t i, j;

  char sMessage[100] = {0};
  sprintf(sMessage, "g_strip.numPixels() = %d", g_strip.numPixels());
  Serial.println(sMessage);
  for (i = 0; i < g_strip.numPixels(); i++)
  {
    g_strip.setPixelColor(i, setWheel(((i * 256 / g_strip.numPixels()) + j) & 255));
  }
  g_strip.show();
}

//-----------------------------------------------------------------------------
// Slightly different, this makes the rainbow equally distributed throughout cycle
//-----------------------------------------------------------------------------
void rainbowCycle(uint8_t wait)
{
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++)
  { // 5 cycles of all colors on wheel
    for (i = 0; i < g_strip.numPixels(); i++)
    {
      g_strip.setPixelColor(i, setWheel(((i * 256 / g_strip.numPixels()) + j) & 255));
    }
    g_strip.show();
    delay(wait);
  }
}

//-----------------------------------------------------------------------------
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
//-----------------------------------------------------------------------------
uint32_t setWheel(byte wheelPos)
{
  wheelPos = 255 - wheelPos;
  if (wheelPos < 85)
  {
    return g_strip.Color(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  else if (wheelPos < 170)
  {
    wheelPos -= 85;
    return g_strip.Color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  else
  {
    wheelPos -= 170;
    return g_strip.Color(wheelPos * 3, 255 - wheelPos * 3, 0);
  }
}

//-----------------------------------------------------------------------------
void twinkle(byte red, byte green, byte blue, int Count, int SpeedDelay, boolean OnlyOne) 
{
  setAllLED(0, 0, 0);
 
  for (int i = 0; i < Count; i++) {
     setPixel(random(NUM_LEDS), red, green, blue);
     showStrip();
     delay(SpeedDelay);
     if (OnlyOne) {
       setAllLED(0,0,0);
     }
   }
 
  delay(SpeedDelay);
}

//-----------------------------------------------------------------------------
void sparkle(byte red, byte green, byte blue, int SpeedDelay) 
{
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel, red, green, blue);
  showStrip();
  delay(SpeedDelay);
  setPixel(Pixel, 0, 0, 0);
}

//-----------------------------------------------------------------------------
void theaterChase(byte red, byte green, byte blue, int SpeedDelay) 
{
  for (int j = 0; j<10; j++) { // do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < NUM_LEDS; i=i+3) {
        setPixel(i + q, red, green, blue); // turn every third pixel on
      }
      showStrip();
     
      delay(SpeedDelay);
     
      for (int i = 0; i < NUM_LEDS; i = i+3) {
        setPixel(i+q, 0, 0, 0); // turn every third pixel off
      }
    }
  }
}

//-----------------------------------------------------------------------------
void halloweenEyes(byte red, byte green, byte blue, int EyeWidth, int EyeSpace, boolean Fade, int Steps, int FadeDelay, int EndPause)
{
  randomSeed(analogRead(0));
 
  int i;
  int StartPoint  = random( 0, NUM_LEDS - (2*EyeWidth) - EyeSpace );
  int Start2ndEye = StartPoint + EyeWidth + EyeSpace;
 
  for(i = 0; i < EyeWidth; i++) {
    setPixel(StartPoint + i, red, green, blue);
    setPixel(Start2ndEye + i, red, green, blue);
  }
 
  showStrip();
 
  if(Fade==true) {
    float r, g, b;
 
    for(int j = Steps; j >= 0; j--) {
      r = j*(red/Steps);
      g = j*(green/Steps);
      b = j*(blue/Steps);
     
      for(i = 0; i < EyeWidth; i++) {
        setPixel(StartPoint + i, r, g, b);
        setPixel(Start2ndEye + i, r, g, b);
      }
     
      showStrip();
      delay(FadeDelay);
    }
  }
 
  setAllLED(0, 0, 0); // Set all black
 
  delay(EndPause);
}

//-----------------------------------------------------------------------------
void newKITT(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay)
{
  rightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  leftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  outsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  centerToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  leftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  rightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  outsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  centerToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
}

//-----------------------------------------------------------------------------
void centerToOutside(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay)
{
  for (int i = ((NUM_LEDS-EyeSize)/2); i >= 0; i--) {
    setAllLED(0, 0, 0);
   
    setPixel(i, red/10, green/10, blue/10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
   
    setPixel(NUM_LEDS-i, red/10, green/10, blue/10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(NUM_LEDS-i-j, red, green, blue);
    }
    setPixel(NUM_LEDS-i-EyeSize-1, red/10, green/10, blue/10);
   
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

//-----------------------------------------------------------------------------
void outsideToCenter(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) 
{
  for (int i = 0; i <= ((NUM_LEDS-EyeSize)/2); i++) {
    setAllLED(0, 0, 0);
   
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
   
    setPixel(NUM_LEDS-i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(NUM_LEDS-i-j, red, green, blue);
    }
    setPixel(NUM_LEDS-i-EyeSize-1, red/10, green/10, blue/10);
   
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

//-----------------------------------------------------------------------------
void leftToRight(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) 
{
  for (int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAllLED(0, 0, 0);
    setPixel(i, red/10, green/10, blue/10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

//-----------------------------------------------------------------------------
void rightToLeft(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    setAllLED(0, 0, 0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

//-----------------------------------------------------------------------------
void fire(int Cooling, int Sparking, int SpeedDelay) 
{
  static byte heat[NUM_LEDS];
  int cooldown;
 
  // Step 1.  Cool down every cell a little
  for (int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
   
    if (cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
 
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for (int k= NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
   
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if (random(255) < Sparking) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for (int j = 0; j < NUM_LEDS; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  showStrip();
  delay(SpeedDelay);
}

//-----------------------------------------------------------------------------
void setPixelHeatColor(int Pixel, byte temperature) 
{
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if ( t192 > 0x80) {                   // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if (t192 > 0x40) {             // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                              // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}

//-----------------------------------------------------------------------------
void initMeteorLight()
{
  FastLED.addLeds<WS2812B, WS2812B_PIN>(g_ledsArray, NUM_LEDS);

#ifdef DEBUG
  Serial.print(F("NUM_LEDS")); Serial.print("\t");
  Serial.print(F("NUM_METEORS")); Serial.print("\t");
  Serial.print(F("INCREASE_BRIGHTNESS")); Serial.print("\t");
  Serial.print(F("STAY_TIME"));
  Serial.println("");  
  Serial.print(NUM_LEDS); Serial.print("\t");  
  Serial.print(NUM_METEORS); Serial.print("\t");
  Serial.print(INCREASE_BRIGHTNESS); Serial.print("\t");
  Serial.print(STAY_TIME);
  Serial.print(INCREASE_BRIGHTNESS);
  Serial.println("");
#endif

  int i = 0, j = 0;
// Display meteor light length and brightness，from 0 .. 7 brightness
#ifdef DEBUG
  Serial.println(F("LED RGB value in ledsArray:"));
#endif
  for (i = INCREASE_BRIGHTNESS - 1; i < 256; i += INCREASE_BRIGHTNESS)
  {
#ifdef DEBUG
    Serial.print(i);
    Serial.print("\t");
    Serial.println(j);
#endif

    g_ledsArray[j++].setHSV(228, 255, i);
  }
#ifdef DEBUG
  Serial.println(F("-------------------------"));
#endif
}

//-----------------------------------------------------------------------------
void startMeteorLight()
{
  int i, ea, eb; // 流星現在在燈串的位置；ea: 低位元組，eb: 高位元組
  int ma, mb;    // 流星陣列複製的起始位置；ma: 低位元組，mb: 高位元組

  //--------------------------------------------------------------------------
  //   流星由近端到遠端移動
  //--------------------------------------------------------------------------
  for (i = 0; i < (NUM_LEDS + NUM_METEORS); i++) // ea: 流星起頭的位置
  {
    // 設定 leds 全部為 black
    for (ea = 0; ea < NUM_LEDS; ea++)
      g_ledsArray[ea] = CRGB::Black;

    // 找到複製 meteors 到 leds 的起頭與結束位置
    if (i < NUM_METEORS) // 整個流星還未完全進入到燈串當中 ( i = 0 ... 7 )
    {
      ma = 0;
      mb = i + 1;
      ea = NUM_METEORS - i - 1;
      eb = NUM_METEORS;
    }
    else if (i >= NUM_LEDS) // 流星頭部已經開始超出燈串 ( i = 11 ... 18 )
    {
      ma = i - NUM_METEORS + 1;
      mb = NUM_LEDS;
      ea = 0;
      eb = NUM_LEDS + NUM_METEORS - 1 - i;
    }
    else // 流星在燈串中 ( i = 8 ... 10 )
    {
      ma = i - NUM_METEORS + 1;
      mb = i + 1;
      ea = 0;
      eb = NUM_METEORS;
    }

#ifdef DEBUG
    Serial.print(F("i")); Serial.print(i); Serial.print("\t");
    Serial.print(F("ma")); Serial.print(ma); Serial.print("\t");
    Serial.print(F("mb")); Serial.print(mb); Serial.print("\t");
    Serial.print(F("ea")); Serial.print(ea); Serial.print("\t");
    Serial.print(F("eb")); Serial.print(eb); Serial.println("");
#endif

    for (ma; ma < mb; ma++) // Copy meteors array to ledsArray
    {
      g_ledsArray[ma] = g_meteors[ea++];
    }

    FastLED.show();
    delay(STAY_TIME); // default 45 ms
  }
  delay(1500);
}
