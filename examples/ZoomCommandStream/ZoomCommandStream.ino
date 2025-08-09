// ZoomCommandStream.ino
// A zoom example, but using command bytes instead of the methods.
// Test sketch to debug why zoom disable fails, directly after zoom enable.
// The issue is due to the static keyword when defining the commandList array
//    !!! Don't use static, for arrays containing variables, obviously !!!

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306Plus.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306Plus display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds
}

void loop() {
  //testzoom();
  //DoZoomCommandStream();
  DoZoomCommandStreamRaw();
}

void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

void testdrawstyles(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Hello, world!"));

  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display.println(3.141592);

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.print(F("0x")); display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}


void testzoom(void) {

  display.setZoom(true);
  delay(2000);

  display.setZoom(false);
  delay(2000);
}

void commandStream(void) {
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
      0x27,   // command - left continuous horizontal scroll
      0x00,   // dummy 0x00
      0x00,   // start
      0x00,   // interval
      0x07,   // end
      0x00,   // dummy 0x00
      0xFF};  // dummy 0xFF
  display.ssd1306_sendCommandList(cmdlistHScroll, sizeof(cmdlistHScroll));

  // Start scroll
  static const uint8_t PROGMEM cmdlistStartScroll[] = {
      0x2F};
  display.ssd1306_sendCommandList(cmdlistStartScroll, sizeof(cmdlistStartScroll));

  delay(2000);

  // Stop scroll and start right one pixel scroll, 21 times.
  display.ssd1306_sendCommandList(cmdlistStopScroll, sizeof(cmdlistStopScroll));
  static const uint8_t PROGMEM cmdlistHScrollOne[] = {
      0x2C,   // command - right 1-pixel horizontal scroll
      0x00,   // dummy 0x00
      0x00,   // start
      0x00,   // interval
      0x07,   // end
      0x00,   // dummy 0x00
      0xFF};  // dummy 0xFF
  display.ssd1306_sendCommandList(cmdlistHScrollOne, sizeof(cmdlistHScrollOne));

  int wait_time = 12;
  delay(wait_time);

  for (int i=0; i<20; i++) {
    display.ssd1306_sendCommandList(cmdlistHScrollOne, sizeof(cmdlistHScrollOne));
    delay(wait_time);
  }
}

void DoZoomCommandStream(void) {
  //
  // Zoom commands
  //

  // Zoom commands - On
  static const uint8_t PROGMEM cmdlistZoomOn[] = {
      SSD1306_SET_ZOOM,   // command - Zoom
      SSD1306_ZOOM_ON};   // Zoom ON
  display.ssd1306_sendCommandList(cmdlistZoomOn, sizeof(cmdlistZoomOn));

  delay(2000);

  // Zoom commands - Off
  static const uint8_t PROGMEM cmdlistZoomOff[] = {
      SSD1306_SET_ZOOM,   // command - Zoom
      SSD1306_ZOOM_OFF};  // Zoom OFF
  display.ssd1306_sendCommandList(cmdlistZoomOff, sizeof(cmdlistZoomOff));

  delay(2000);

  boolean i = true;

  // Zoom commands - Set
  static const uint8_t PROGMEM cmdlistZoomSet[] = {
      SSD1306_SET_ZOOM,                         // command - Zoom
      i ? SSD1306_ZOOM_ON : SSD1306_ZOOM_OFF};  // Zoom ON/OFF
  display.ssd1306_sendCommandList(cmdlistZoomSet, sizeof(cmdlistZoomSet));

  delay(2000);

  i = false;

  // Zoom commands - Set
  static const uint8_t PROGMEM cmdlistZoomSet2[] = {
      SSD1306_SET_ZOOM,                         // command - Zoom
      i ? SSD1306_ZOOM_ON : SSD1306_ZOOM_OFF};  // Zoom ON/OFF
  display.ssd1306_sendCommandList(cmdlistZoomSet2, sizeof(cmdlistZoomSet2));

  delay(2000);

}

void DoZoomCommandStreamRaw(void) {
  ZoomCommandsRaw(true);
  delay(2000);

  ZoomCommandsRaw(false);
  delay(2000);
}


//
// Contents of the zoom methods dumped here
//
void ZoomCommandsRaw(boolean i) {
  /*static*/ const uint8_t PROGMEM zoomListRaw[] = {
      SSD1306_SET_ZOOM, i};
  display.ssd1306_sendCommandList(zoomListRaw, sizeof(zoomListRaw));
}
