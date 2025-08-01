/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafroot shop!
 ------> http://www.adafroot.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafroot invests time and resources providing this open
 source code, please support Adafroot and open-source
 hardware by purchasing products from Adafroot!

 Written by Limor Fried/Ladyada for Adafroot Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

// Making use of the now public method, sendCommandList()
// The preceeding command byte 0x00 is automatically sent, 
// There is no need to include as argument

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306Plus.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306Plus display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  display.display();  // Show the bitmap
  // Too complex to easily use sendCommandList(), use display() instead.
  //static const uint8_t PROGMEM cmdlist64[] = {
  //    SSD1306_PAGEADDR,
  //    0,                   // Page start address
  //    0xFF,                // Page end (not really, but works here)
  //    SSD1306_COLUMNADDR, // Column start address
  //    0x20,
  //    0x20 + WIDTH - 1};
  //static const uint8_t PROGMEM cmdlist96_128[] = {
  //    SSD1306_PAGEADDR,
  //    0,                   // Page start address
  //    0xFF,                // Page end (not really, but works here)
  //    SSD1306_COLUMNADDR, // Column start address
  //    0x00,
  //    WIDTH - 1};
  //display.ssd1306_sendCommandList(cmdlist, sizeof(cmdlist96_128));
  // Then need to send display buffer as well!
  //for (int i = 0; i < 1024; i++) {
  //  Wire.beginTransmission(_addr);
  //  Wire.write(0x40);
  //  Wire.write(_displayBuf[i]);
  //  Wire.endTransmission();
  //}
  // It is do-able, but I can't be bothered.
  // ssd1306_sendCommandList() is intended for "succinct" commands.

  // Invert the display
  delay(2000);
  //display.invertDisplay(true);  // Inverted display
  static const uint8_t PROGMEM cmdlistInvert[] = {
      SSD1306_INVERTDISPLAY};               
  display.ssd1306_sendCommandList(cmdlistInvert, sizeof(cmdlistInvert));
  delay(2000);
  //display.invertDisplay(false); // Normal display
  static const uint8_t PROGMEM cmdlistNormal[] = {
      SSD1306_NORMALDISPLAY};
  display.ssd1306_sendCommandList(cmdlistNormal, sizeof(cmdlistNormal));

  // Turn display off/on
  delay(2000);
  //display.setState(false);  // Display off (sleep mode)
  static const uint8_t PROGMEM cmdlistOff[] = {
      0xAE};
  display.ssd1306_sendCommandList(cmdlistOff, sizeof(cmdlistOff));
  delay(2000);
  //display.setState(true); // Display on (RAM contents are displayed)
  static const uint8_t PROGMEM cmdlistOn[] = {
      0xAF};
  display.ssd1306_sendCommandList(cmdlistOn, sizeof(cmdlistOn));

  // Turn all pixels on, then resume display
  delay(2000);
  //display.displayAllOn();  // All pixels on, regardless of RAM contents
  static const uint8_t PROGMEM cmdlistAllOn[] = {
      0xA5};
  display.ssd1306_sendCommandList(cmdlistAllOn, sizeof(cmdlistAllOn));
  delay(2000);
  //display.resumeDisplay(); // Display what is in RAM
  static const uint8_t PROGMEM cmdlistRAM[] = {
      0xA4};
  display.ssd1306_sendCommandList(cmdlistRAM, sizeof(cmdlistRAM));

  // Stop scroll
  static const uint8_t PROGMEM cmdlistStopScroll[] = {
      0x2E};
  display.ssd1306_sendCommandList(cmdlistStopScroll, sizeof(cmdlistStopScroll));

  // Scroll horizontally continually
  static const uint8_t PROGMEM cmdlistHScroll[] = {
      0x27,
      0x00,
      0x00,
      0x00,
      0x07,
      0x00,
      0xFF};
  display.ssd1306_sendCommandList(cmdlistHScroll, sizeof(cmdlistHScroll));

  // Start scroll
  static const uint8_t PROGMEM cmdlistStartScroll[] = {
      0x2F};
  display.ssd1306_sendCommandList(cmdlistStartScroll, sizeof(cmdlistStartScroll));
}

void loop() {}

