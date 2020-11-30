#ifndef CONFIGURE_H
#define CONFIGURE_H

//-----------------------------------------------------------------------------
// Constant Configure Define
//-----------------------------------------------------------------------------
#define DEBUG
#define LED_LIGHT // LED display on/off

//-----------------------------------------------------------------------------
// Constant Variable Define
//-----------------------------------------------------------------------------
//#define SERIAL_RX_BUFFER_SIZE 64 // Serial Buffer Size
#define BUTTON_PIN 5       // define button to digital PIN 4
#define MODE_LED_PIN 13    // define mode LED to digital PIN 13
#define WS2812B_PIN 4      // define WS2812B to digital PIN 2

#define MODE_NUMS   6      // Nubmer of modes
#define MODEEX_NUMS 6      // Nubmer of modes
#define NUM_LEDS 8         // Numbers of WS2812B LED as 8
#define NUM_METEORS 5      // Length of meteors
#define INCREASE_BRIGHTNESS (256 / NUM_METEORS) // Meteor light delay time ( HSV )
#define STAY_TIME 45                            // Meteor light stay time ( ms)

#define SERIAL_RESERVE 128 // define serail port reserve
#define START_BYTE       0x1B
#define TRIGGER_BYTE     0x31
#define TRIGGER_EX_BYTE  0x32
#define TRIGGER_CUS_BYTE 0x33
#define STOP_BYTE        0x30

#endif
