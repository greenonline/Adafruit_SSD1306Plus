// ChargePump.ino
// A charge pump example, using both command bytes as well as the methods.

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

  //doChargePumpStuff();

}

void loop() {
  doChargePumpStuff();
}



void doChargePumpStuff(void){
  bool i = true;
  display.setChargePump(i);
  delay(2000); // Pause for 2 seconds
  display.setChargePumpOn();
  delay(2000); // Pause for 2 seconds
  display.setChargePumpOff();
  delay(2000); // Pause for 2 seconds

  i = false;
  display.setChargePump(i);
  delay(2000); // Pause for 2 seconds
  display.setChargePumpOn();
  delay(2000); // Pause for 2 seconds
  display.setChargePumpOff();
  delay(2000); // Pause for 2 seconds

  i = true;
  display.setChargePumpDisplay(i);
  delay(2000); // Pause for 2 seconds
  display.setChargePumpOnDisplay();
  delay(2000); // Pause for 2 seconds
  display.setChargePumpOffDisplay();
  delay(2000); // Pause for 2 seconds

  i = false;
  display.setChargePumpDisplay(i);
  delay(2000); // Pause for 2 seconds
  display.setChargePumpOnDisplay();
  delay(2000); // Pause for 2 seconds
  display.setChargePumpOffDisplay();
  delay(2000); // Pause for 2 seconds
}


/*

  //
  // Charge Pump setting
  //

  // Charge Pump setting - off
  static const uint8_t PROGMEM cmdlistChrgPmpOff[] = {
      SSD1306_CHARGEPUMP,   // command - Charge Pump setting
      SSD1306_CH_PUMPOFF};  // OFF/ON. 0x10/0x14
  display.ssd1306_sendCommandList(cmdlistChrgPmpOff, sizeof(cmdlistChrgPmpOff));
  // Charge Pump setting - off and display
  static const uint8_t PROGMEM cmdlistChrgPmpOffDisplay[] = {
      SSD1306_CHARGEPUMP,   // command - Charge Pump setting
      SSD1306_CH_PUMPOFF,   // OFF/ON. 0x10/0x14
      SSD1306_DISPLAYON};   // Display ON
  display.ssd1306_sendCommandList(cmdlistChrgPmpOffDisplay, sizeof(cmdlistChrgPmpOffDisplay));
  // Charge Pump setting - on
  static const uint8_t PROGMEM cmdlistChrgPmpOn[] = {
      SSD1306_CHARGEPUMP,   // command - Charge Pump setting
      SSD1306_CH_PUMPON};  // OFF/ON. 0x10/0x14
  display.ssd1306_sendCommandList(cmdlistChrgPmpOn, sizeof(cmdlistChrgPmpOn));
  // Charge Pump setting - on and display
  static const uint8_t PROGMEM cmdlistChrgPmpOnDisplay[] = {
      SSD1306_CHARGEPUMP,   // command - Charge Pump setting
      SSD1306_CH_PUMPON,   // OFF/ON. 0x10/0x14
      SSD1306_DISPLAYON};   // Display ON
  display.ssd1306_sendCommandList(cmdlistChrgPmpOnDisplay, sizeof(cmdlistChrgPmpOnDisplay));
  // Charge Pump setting - set
  static const uint8_t PROGMEM cmdlistChrgPmpSet[] = {
      SSD1306_CHARGEPUMP,                          // command - Charge Pump setting
      i ? SSD1306_CH_PUMPON : SSD1306_CH_PUMPOF};  // OFF/ON. 0x10/0x14
  display.ssd1306_sendCommandList(cmdlistChrgPmpSet, sizeof(cmdlistChrgPmpSet));
  // Charge Pump setting - set and display
  static const uint8_t PROGMEM cmdlistChrgPmpSetDisplay[] = {
      SSD1306_CHARGEPUMP,                          // command - Charge Pump setting
      i ? SSD1306_CH_PUMPON : SSD1306_CH_PUMPOF,   // OFF/ON. 0x10/0x14
      SSD1306_DISPLAYON};                          // Display ON
  display.ssd1306_sendCommandList(cmdlistChrgPmpSetDisplay, sizeof(cmdlistChrgPmpSetDisplay));


#define SSD1306_CH_PUMPOFF. 0x10
#define SSD1306_CH_PUMPON   0x14



*/
