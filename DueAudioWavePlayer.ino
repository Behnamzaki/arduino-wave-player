/*

  Simple Audio Player

 Demonstrates the use of the Audio library for the Arduino Due

 Hardware required :

 * Arduino shield with a SD card on CS4

 * A sound file named "test.wav" in the root directory of the SD card

 * An audio amplifier to connect to the DAC0 and ground

 * A speaker to connect to the audio amplifier

 Original by Massimo Banzi September 20, 2012

 Modified by Scott Fitzgerald October 19, 2012

 Modified by Arturo Guadalupi December 18, 2015

 This example code is in the public domain

 http://www.arduino.cc/en/Tutorial/SimpleAudioPlayer

*/

#include <SD.h>
#include <SPI.h>
#include <Audio.h>

#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


void setup() {
  u8g2.begin();
  u8g2.clearBuffer();          // clear the internal memory


  u8g2.setFont(u8g2_font_tenstamps_mf);
  u8g2.drawStr(45,13,"ARM");
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(0,30,"test.wav"); // write something to the internal memory
  u8g2.setFont(u8g2_font_iconquadpix_m_all); // choose a suitable font
u8g2.drawStr(0,50,"i");
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(20,50,"Playing...");
  u8g2.sendBuffer();          // transfer internal memory to the display

  // debug output at 9600 baud

  Serial.begin(9600);

  // setup SD-card

  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {

    Serial.println(" failed!");

    while(true);

  }

  Serial.println(" done.");

  // hi-speed SPI transfers

  // 44100kHz stereo => 88200 sample rate

  // 100 mSec of prebuffering.

  Audio.begin(88200, 100);
}

void loop() {

  int count = 0;

  // open wave file from sdcard

  File myFile = SD.open("test.wav");

  if (!myFile) {

    // if the file didn't open, print an error and stop

    Serial.println("error opening test.wav");

    while (true);

  }

  const int S = 1024; // Number of samples to read in block

  short buffer[S];

  Serial.print("Playing");

  // until the file is not finished

  while (myFile.available()) {

    // read from the file into buffer

    myFile.read(buffer, sizeof(buffer));

    // Prepare samples

    int volume = 1024;

    Audio.prepare(buffer, S, volume);

    // Feed samples to audio

    Audio.write(buffer, S);

    // Every 100 block print a '.'

    count++;

    if (count == 100) {

      Serial.print(".");

      count = 0;

    }

  }

  myFile.close();

  Serial.println("End of file. Thank you for listening!");

  while (true) ;
}
