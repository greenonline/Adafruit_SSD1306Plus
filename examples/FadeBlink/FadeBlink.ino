/*
   FadeBlink.ino
   Sketch to demonstrate fading and blinking.
   Created July 28, 2025
*/

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

#define STYLE_FADE 0
#define STYLE_BEEF 1
#define STYLE_FADE 2
#define STYLE_BLINK 3
#define STYLE_BREATHE 4



void setup() {
    Serial.begin(9600);

    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    //display.begin(SSD1306_SWITCHCAPVCC);  // Waaay too basic
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;  // Don't proceed, loop forever
    }
    // init done

    // Note: Stop scroll, fade, blink and (especially) zoom.
    display.stopFadeBlink();
    display.stopscroll();
    display.stopZoom();

    // Call here to run demo one-time only
    //DoFadeBlinkDemo();
}

void loop() {
    // Call here to cycle demo indefinitely
    DoFadeBlinkDemo();
}


void DoFadeBlinkDemo(void){
    // Clear the buffer.
    display.clearDisplay();

    // Text display
    testdrawstyles(STYLE_FADE);
    display.display();

    display.fade(0);
    delay(10000);                        // Wait 10 seconds to see the effect

    //testdrawstyles(STYLE_BLINK);
    display.stopFade();
    delay(500);                         // Wait half a second.
                                        // Note: The "FADE" logo will show for 500 ms,
                                        // which "looks" incongruous but is intentional.
                                        // Reduce delay or print "BLINK" *before* stopFade

    testdrawstyles(STYLE_BLINK);
    display.blink(0);
    delay(10000);                       // Wait 10 seconds to see the effect

    display.stopBlink();
    delay(500);                         // Wait half a second

    testdrawstyles(STYLE_BREATHE);
    display.blink(3);                   // Slowly blink, or "breathe", ad infinitum
    delay(10000);                       // Wait 10 seconds to see the effect

    display.stopFadeBlink();
    delay(500);                         // Wait half a second
}

void testdrawstyles(int iDisplay) {
  display.clearDisplay();

  display.setTextSize(1);               // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(0, 0);              // Start at top-left corner
  display.println(F("Hello, world!"));

  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);  // Draw 'inverse' text
  display.println(3.141592);

  display.setTextSize(2);  // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.print(F("0x"));
  if (iDisplay == STYLE_FADE) {
    display.println(0xFADE, HEX);
  } else if (iDisplay == STYLE_BLINK) {
    display.println("BLINK");
  } else if (iDisplay == STYLE_BREATHE) {
    display.println("BREATHE");
  } else {
    display.println(0xDEADBEEF, HEX);
  }

  display.display();
  delay(2000);
}

