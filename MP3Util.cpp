#include "MP3Util.h"
#include "Configure.h"

//AltSoftSerial g_dfPlayerSoftwareSerial(2, 3, false); // RX, TX
SoftwareSerial g_dfPlayerSoftwareSerial(2, 3); // g_dfPlayerSoftwareSerial(RX, TX), initialize SoftwareSerial for DFPlayerMini communication.
DFRobotDFPlayerMini g_dfPlayer;


//-----------------------------------------------------------------------------
bool initDFPlayer()
{
  g_dfPlayerSoftwareSerial.begin(9600); // Set DFPlayer SoftwareSerial
  delay(100);

  if (!g_dfPlayer.begin(g_dfPlayerSoftwareSerial))
  { // Use softwareSerial to communicate with mp3.
#ifdef DEBUG
    Serial.println(F("Unable to begin: "));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
#endif
    return false;
  }
#ifdef DEBUG
  Serial.println(F("DFPlayer Mini online."));
#endif

  g_dfPlayer.setTimeOut(500); // Set serial communictaion time out 500ms

  //----Set volume----
  g_dfPlayer.volume(SPEAKER_VOLUME); // Set volume value (0~30).
  g_dfPlayer.volumeUp();             // Volume Up
  g_dfPlayer.volumeDown();           // Volume Down

  //----Set different EQ----
  g_dfPlayer.EQ(DFPLAYER_EQ_NORMAL);
  //  g_dfPlayer.EQ(DFPLAYER_EQ_POP);
  //  g_dfPlayer.EQ(DFPLAYER_EQ_ROCK);
  //  g_dfPlayer.EQ(DFPLAYER_EQ_JAZZ);
  //  g_dfPlayer.EQ(DFPLAYER_EQ_CLASSIC);
  //  g_dfPlayer.EQ(DFPLAYER_EQ_BASS);

  //----Set device we use SD as default----
  //  g_dfPlayer.outputDevice(DFPLAYER_DEVICE_U_DISK);
  g_dfPlayer.outputDevice(DFPLAYER_DEVICE_SD);
//  g_dfPlayer.outputDevice(DFPLAYER_DEVICE_AUX);
//  g_dfPlayer.outputDevice(DFPLAYER_DEVICE_SLEEP);
//  g_dfPlayer.outputDevice(DFPLAYER_DEVICE_FLASH);

//----Mp3 control----
//  g_dfPlayer.sleep();      // sleep
//  g_dfPlayer.reset();      // Reset the module
//  g_dfPlayer.enableDAC();  // Enable On-chip DAC
//  g_dfPlayer.disableDAC(); // Disable On-chip DAC
//  g_dfPlayer.outputSetting(true, 15); // output setting, enable the output and set the gain to 15

//g_dfPlayer.enableLoop(); // enable loop

//----Read imformation----
#ifdef DEBUG
  Serial.print(F("MP3 state:"));
  Serial.print("\t");
  Serial.println(g_dfPlayer.readState()); // read mp3 state
  Serial.print(F("Current volume:"));
  Serial.print("\t");
  Serial.println(g_dfPlayer.readVolume()); // read current volume
  Serial.print(F("EQ setting:"));
  Serial.print("\t");
  Serial.println(g_dfPlayer.readEQ()); // read EQ setting
  Serial.print(F("File counts in SD card:"));
  Serial.print("\t");
  Serial.println(g_dfPlayer.readFileCounts()); // read all file counts in SD card
  Serial.print(F("Current play file number:"));
  Serial.print("\t");
  Serial.println(g_dfPlayer.readCurrentFileNumber()); // read current play file number
  Serial.print(F("Fill counts in folder:"));
  Serial.print("\t");
  Serial.println(g_dfPlayer.readFileCountsInFolder(1)); // read fill counts in folder SD:/03
#endif

  return true;
}

void playWithID(int id)
{
  g_dfPlayer.play(id);
  delay(100);

#ifdef DEBUG
    if (g_dfPlayer.available())
    {
      printDetail(g_dfPlayer.readType(), g_dfPlayer.read()); // Print the detail message from DFPlayer to handle different errors and states.
    }
#endif
}

//-----------------------------------------------------------------------------
void printDetail(uint8_t type, int value)
{
  switch (type)
  {
  case TimeOut:
    Serial.println(F("Time Out!"));
    break;
  case WrongStack:
    Serial.println(F("Stack Wrong!"));
    break;
  case DFPlayerCardInserted:
    Serial.println(F("Card Inserted!"));
    break;
  case DFPlayerCardRemoved:
    Serial.println(F("Card Removed!"));
    break;
  case DFPlayerCardOnline:
    Serial.println(F("Card Online!"));
    break;
  case DFPlayerPlayFinished:
    Serial.print(F("Number:"));
    Serial.print(value);
    Serial.println(F(" Play Finished!"));
    break;
  case DFPlayerError:
    Serial.print(F("DFPlayerError:"));
    switch (value)
    {
    case Busy:
      Serial.println(F("Card not found"));
      break;
    case Sleeping:
      Serial.println(F("Sleeping"));
      break;
    case SerialWrongStack:
      Serial.println(F("Get Wrong Stack"));
      break;
    case CheckSumNotMatch:
      Serial.println(F("Check Sum Not Match"));
      break;
    case FileIndexOut:
      Serial.println(F("File Index Out of Bound"));
      break;
    case FileMismatch:
      Serial.println(F("Cannot Find File"));
      break;
    case Advertise:
      Serial.println(F("In Advertise"));
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}
