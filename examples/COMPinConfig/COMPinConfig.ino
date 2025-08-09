// COMPinConfig.ino
// A COM pins example, using both command bytes as well as the methods.

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

  // Uncomment only if you know what you are doing!
  // Please refer to the data sheet for more information
  //doCOMPinsStuff();

}

void loop() {
  // Uncomment only if you know what you are doing!
  // Please refer to the data sheet for more information
  //doCOMPinsStuff();
}



void doCOMPinsStuff(void){
  bool com = false, lr = false;

  display.setCOMPins(com, lr);
  delay(2000); // Pause for 2 seconds
  display.setCOMPins_SEQCOM_NOLR();
  delay(2000); // Pause for 2 seconds
  display.setCOMPins_ALTCOM_NOLR();
  delay(2000); // Pause for 2 seconds
  display.setCOMPins_SEQCOM_ENLR();
  delay(2000); // Pause for 2 seconds
  display.setCOMPins_ALTCOM_ENLR();
  delay(2000); // Pause for 2 seconds

  com = true, lr = false;
  display.setCOMPins(com, lr);
  delay(2000); // Pause for 2 seconds

  com = false, lr = true;
  display.setCOMPins(com, lr);
  delay(2000); // Pause for 2 seconds

  com = true, lr = true;
  display.setCOMPins(com, lr);
  delay(2000); // Pause for 2 seconds
}


/*

  //
  // COM Pins setting
  //

#define SSD1306_SEQCOM_NOLR 0x02  // Sequential COM pin configuration
                                  // Disable COM Left/Right remap
#define SSD1306_ALTCOM_NOLR 0x12  // Alternative COM pin configuration
                                  // Disable COM Left/Right remap
#define SSD1306_SEQCOM_ENLR 0x22  // Sequential COM pin configuration
                                  // Enable COM Left/Right remap
#define SSD1306_ALTCOM_ENLR 0x32  // Alternative COM pin configuration
                                  // Enable COM Left/Right remap


  static const uint8_t PROGMEM comLRList1a[] = {
      SSD1306_SETCOMPINS, SSD1306_SEQCOM_NOLR};
  static const uint8_t PROGMEM comLRList1b[] = {
      SSD1306_SETCOMPINS, SSD1306_ALTCOM_NOLR};
  static const uint8_t PROGMEM comLRList1c[] = {
      SSD1306_SETCOMPINS, SSD1306_SEQCOM_ENLR};
  static const uint8_t PROGMEM comLRList1d[] = {
      SSD1306_SETCOMPINS, SSD1306_ALTCOM_ENLR};
  ssd1306_commandList(comLRList1b, sizeof(comLRList1b));



*/

