#ifndef MP3_UTIL_H
#define MP3_UTIL_H
#include <DFRobotDFPlayerMini.h> // Using DFRobotDFPlayerMini Library
// #include <AltSoftSerial.h>
#include <SoftwareSerial.h>      // Using SoftwareSerial Library

bool initDFPlayer();
void playWithID(int id);
void setVolume(int volume);
void printDetail(uint8_t type, int value);

#endif