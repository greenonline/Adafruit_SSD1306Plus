#if defined(__AVR__) || defined(ARDUINO_ARCH_RTTHREAD)
#include <avr/pgmspace.h>
#elif defined(ARDUINO_ARDUINO_NANO33BLE) ||                                    \
    defined(ARDUINO_ARCH_MBED_RP2040) || defined(ARDUINO_ARCH_RP2040)
#include <api/deprecated-avr-comp/avr/pgmspace.h>
#elif defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#define pgm_read_byte(addr)                                                    \
  (*(const unsigned char *)(addr)) ///< PROGMEM workaround for non-AVR
#endif

#if !defined(__ARM_ARCH) && !defined(ENERGIA) && !defined(ESP8266) &&          \
    !defined(ESP32) && !defined(__arc__) && !defined(__RL78__) &&              \
    !defined(CH32V20x) && !defined(PICO_RISCV)
#include <util/delay.h>
#endif

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306Plus.h>
#include "splash.h"









#ifdef HAVE_PORTREG
#define SSD1306_SELECT *csPort &= ~csPinMask;       ///< Device select
#define SSD1306_DESELECT *csPort |= csPinMask;      ///< Device deselect
#define SSD1306_MODE_COMMAND *dcPort &= ~dcPinMask; ///< Command mode
#define SSD1306_MODE_DATA *dcPort |= dcPinMask;     ///< Data mode
#else
#define SSD1306_SELECT digitalWrite(csPin, LOW);       ///< Device select
#define SSD1306_DESELECT digitalWrite(csPin, HIGH);    ///< Device deselect
#define SSD1306_MODE_COMMAND digitalWrite(dcPin, LOW); ///< Command mode
#define SSD1306_MODE_DATA digitalWrite(dcPin, HIGH);   ///< Data mode
#endif


#if (ARDUINO >= 157) && !defined(ARDUINO_STM32_FEATHER)
#define SETWIRECLOCK wire->setClock(wireClk)    ///< Set before I2C transfer
#define RESWIRECLOCK wire->setClock(restoreClk) ///< Restore after I2C xfer
#else // setClock() is not present in older Arduino Wire lib (or WICED)
#define SETWIRECLOCK ///< Dummy stand-in define
#define RESWIRECLOCK ///< keeps compiler happy
#endif

#if defined(SPI_HAS_TRANSACTION)
#define SPI_TRANSACTION_START spi->beginTransaction(spiSettings) ///< Pre-SPI
#define SPI_TRANSACTION_END spi->endTransaction()                ///< Post-SPI
#else // SPI transactions likewise not present in older Arduino SPI lib
#define SPI_TRANSACTION_START ///< Dummy stand-in define
#define SPI_TRANSACTION_END   ///< keeps compiler happy
#endif


// The definition of 'transaction' is broadened a bit in the context of
// this library -- referring not just to SPI transactions (if supported
// in the version of the SPI library being used), but also chip select
// (if SPI is being used, whether hardware or soft), and also to the
// beginning and end of I2C transfers (the Wire clock may be sped up before
// issuing data to the display, then restored to the default rate afterward
// so other I2C device types still work).  All of these are encapsulated
// in the TRANSACTION_* macros.

// Check first if Wire, then hardware SPI, then soft SPI:
#define TRANSACTION_START                                                      \
  if (wire) {                                                                  \
    SETWIRECLOCK;                                                              \
  } else {                                                                     \
    if (spi) {                                                                 \
      SPI_TRANSACTION_START;                                                   \
    }                                                                          \
    SSD1306_SELECT;                                                            \
  } ///< Wire, SPI or bitbang transfer setup
#define TRANSACTION_END                                                        \
  if (wire) {                                                                  \
    RESWIRECLOCK;                                                              \
  } else {                                                                     \
    SSD1306_DESELECT;                                                          \
    if (spi) {                                                                 \
      SPI_TRANSACTION_END;                                                     \
    }                                                                          \
  } ///< Wire, SPI or bitbang transfer end



// CONSTRUCTORS, DESTRUCTOR ------------------------------------------------

/*!
    @brief  Constructor for I2C-interfaced SSD1306 displays.
    @param  w
            Display width in pixels
    @param  h
            Display height in pixels
    @param  twi
            Pointer to an existing TwoWire instance (e.g. &Wire, the
            microcontroller's primary I2C bus).
    @param  rst_pin
            Reset pin (using Arduino pin numbering), or -1 if not used
            (some displays might be wired to share the microcontroller's
            reset pin).
    @param  clkDuring
            Speed (in Hz) for Wire transmissions in SSD1306 library calls.
            Defaults to 400000 (400 KHz), a known 'safe' value for most
            microcontrollers, and meets the SSD1306 datasheet spec.
            Some systems can operate I2C faster (800 KHz for ESP32, 1 MHz
            for many other 32-bit MCUs), and some (perhaps not all)
            SSD1306's can work with this -- so it's optionally be specified
            here and is not a default behavior. (Ignored if using pre-1.5.7
            Arduino software, which operates I2C at a fixed 100 KHz.)
    @param  clkAfter
            Speed (in Hz) for Wire transmissions following SSD1306 library
            calls. Defaults to 100000 (100 KHz), the default Arduino Wire
            speed. This is done rather than leaving it at the 'during' speed
            because other devices on the I2C bus might not be compatible
            with the faster rate. (Ignored if using pre-1.5.7 Arduino
            software, which operates I2C at a fixed 100 KHz.)
    @return Adafruit_SSD1306Plus object.
    @note   Call the object's begin() function before use -- buffer
            allocation is performed there!
*/
Adafruit_SSD1306Plus::Adafruit_SSD1306Plus(uint8_t w, uint8_t h, TwoWire *twi,
                                   int8_t rst_pin, uint32_t clkDuring,
                                   uint32_t clkAfter)
    : Adafruit_SSD1306(w, h, twi, rst_pin, clkDuring, clkAfter) {}

/*!
    @brief  Constructor for SPI SSD1306 displays, using software (bitbang)
            SPI.
    @param  w
            Display width in pixels
    @param  h
            Display height in pixels
    @param  mosi_pin
            MOSI (master out, slave in) pin (using Arduino pin numbering).
            This transfers serial data from microcontroller to display.
    @param  sclk_pin
            SCLK (serial clock) pin (using Arduino pin numbering).
            This clocks each bit from MOSI.
    @param  dc_pin
            Data/command pin (using Arduino pin numbering), selects whether
            display is receiving commands (low) or data (high).
    @param  rst_pin
            Reset pin (using Arduino pin numbering), or -1 if not used
            (some displays might be wired to share the microcontroller's
            reset pin).
    @param  cs_pin
            Chip-select pin (using Arduino pin numbering) for sharing the
            bus with other devices. Active low.
    @return Adafruit_SSD1306Plus object.
    @note   Call the object's begin() function before use -- buffer
            allocation is performed there!
*/
Adafruit_SSD1306Plus::Adafruit_SSD1306Plus(uint8_t w, uint8_t h, int8_t mosi_pin,
                                   int8_t sclk_pin, int8_t dc_pin,
                                   int8_t rst_pin, int8_t cs_pin)
    : Adafruit_SSD1306(w, h, mosi_pin, sclk_pin, dc_pin, rst_pin, cs_pin) {}

/*!
    @brief  Constructor for SPI SSD1306 displays, using native hardware SPI.
    @param  w
            Display width in pixels
    @param  h
            Display height in pixels
    @param  spi_ptr
            Pointer to an existing SPIClass instance (e.g. &SPI, the
            microcontroller's primary SPI bus).
    @param  dc_pin
            Data/command pin (using Arduino pin numbering), selects whether
            display is receiving commands (low) or data (high).
    @param  rst_pin
            Reset pin (using Arduino pin numbering), or -1 if not used
            (some displays might be wired to share the microcontroller's
            reset pin).
    @param  cs_pin
            Chip-select pin (using Arduino pin numbering) for sharing the
            bus with other devices. Active low.
    @param  bitrate
            SPI clock rate for transfers to this display. Default if
            unspecified is 8000000UL (8 MHz).
    @return Adafruit_SSD1306Plus object.
    @note   Call the object's begin() function before use -- buffer
            allocation is performed there!
*/
Adafruit_SSD1306Plus::Adafruit_SSD1306Plus(uint8_t w, uint8_t h, SPIClass *spi_ptr,
                                   int8_t dc_pin, int8_t rst_pin, int8_t cs_pin,
                                   uint32_t bitrate)
    : Adafruit_SSD1306(w, h, spi_ptr, dc_pin, rst_pin, cs_pin, bitrate) {}

/*!
    @brief  DEPRECATED constructor for SPI SSD1306 displays, using software
            (bitbang) SPI. Provided for older code to maintain compatibility
            with the current library. Screen size is determined by enabling
            one of the SSD1306_* size defines in Adafruit_SSD1306Plus.h. New
            code should NOT use this.
    @param  mosi_pin
            MOSI (master out, slave in) pin (using Arduino pin numbering).
            This transfers serial data from microcontroller to display.
    @param  sclk_pin
            SCLK (serial clock) pin (using Arduino pin numbering).
            This clocks each bit from MOSI.
    @param  dc_pin
            Data/command pin (using Arduino pin numbering), selects whether
            display is receiving commands (low) or data (high).
    @param  rst_pin
            Reset pin (using Arduino pin numbering), or -1 if not used
            (some displays might be wired to share the microcontroller's
            reset pin).
    @param  cs_pin
            Chip-select pin (using Arduino pin numbering) for sharing the
            bus with other devices. Active low.
    @return Adafruit_SSD1306Plus object.
    @note   Call the object's begin() function before use -- buffer
            allocation is performed there!
*/
Adafruit_SSD1306Plus::Adafruit_SSD1306Plus(int8_t mosi_pin, int8_t sclk_pin,
                                   int8_t dc_pin, int8_t rst_pin, int8_t cs_pin)
    : Adafruit_SSD1306(mosi_pin, sclk_pin, dc_pin, rst_pin, cs_pin) {}

/*!
    @brief  DEPRECATED constructor for SPI SSD1306 displays, using native
            hardware SPI. Provided for older code to maintain compatibility
            with the current library. Screen size is determined by enabling
            one of the SSD1306_* size defines in Adafruit_SSD1306Plus.h. New
            code should NOT use this. Only the primary SPI bus is supported,
            and bitrate is fixed at 8 MHz.
    @param  dc_pin
            Data/command pin (using Arduino pin numbering), selects whether
            display is receiving commands (low) or data (high).
    @param  rst_pin
            Reset pin (using Arduino pin numbering), or -1 if not used
            (some displays might be wired to share the microcontroller's
            reset pin).
    @param  cs_pin
            Chip-select pin (using Arduino pin numbering) for sharing the
            bus with other devices. Active low.
    @return Adafruit_SSD1306Plus object.
    @note   Call the object's begin() function before use -- buffer
            allocation is performed there!
*/
Adafruit_SSD1306Plus::Adafruit_SSD1306Plus(int8_t dc_pin, int8_t rst_pin, int8_t cs_pin)
    : Adafruit_SSD1306(dc_pin, rst_pin, cs_pin) {}

/*!
    @brief  DEPRECATED constructor for I2C SSD1306 displays. Provided for
            older code to maintain compatibility with the current library.
            Screen size is determined by enabling one of the SSD1306_* size
            defines in Adafruit_SSD1306Plus.h. New code should NOT use this.
            Only the primary I2C bus is supported.
    @param  rst_pin
            Reset pin (using Arduino pin numbering), or -1 if not used
            (some displays might be wired to share the microcontroller's
            reset pin).
    @return Adafruit_SSD1306Plus object.
    @note   Call the object's begin() function before use -- buffer
            allocation is performed there!
*/
Adafruit_SSD1306Plus::Adafruit_SSD1306Plus(int8_t rst_pin)
    : Adafruit_SSD1306(rst_pin) {}


/*!
    @brief  Destructor for Adafruit_SSD1306Plus object.
*/
Adafruit_SSD1306Plus::~Adafruit_SSD1306Plus(void) {
  if (buffer) {
    free(buffer);
    buffer = NULL;
  }
}




/*!
    @brief  Activate a 1-pixel right-handed scroll for all or part of the display.
    @param  start
            First row.
    @param  stop
            Last row.
    @return None (void).
*/
// To scroll the whole display, run: display.startscrollrightone(0x00, 0x0F)
void Adafruit_SSD1306Plus::startscrollrightone(uint8_t start, uint8_t stop) {
  TRANSACTION_START
  static const uint8_t PROGMEM scrollList1a[] = {
      SSD1306_RIGHT_HORIZONTAL_SCROLL_ONE, 0X00};
  ssd1306_commandList(scrollList1a, sizeof(scrollList1a));
  ssd1306_command1(start);
  ssd1306_command1(0X00);
  ssd1306_command1(stop);
  static const uint8_t PROGMEM scrollList1b[] = {0X00, 0XFF,
                                                 SSD1306_ACTIVATE_SCROLL};
  ssd1306_commandList(scrollList1b, sizeof(scrollList1b));
  TRANSACTION_END
}

/*!
    @brief  Activate a 1-pixel left-handed scroll for all or part of the display.
    @param  start
            First row.
    @param  stop
            Last row.
    @return None (void).
*/
// To scroll the whole display, run: display.startscrollleftone(0x00, 0x0F)
void Adafruit_SSD1306Plus::startscrollleftone(uint8_t start, uint8_t stop) {
  TRANSACTION_START
  static const uint8_t PROGMEM scrollList2a[] = {SSD1306_LEFT_HORIZONTAL_SCROLL_ONE,
                                                 0X00};
  ssd1306_commandList(scrollList2a, sizeof(scrollList2a));
  ssd1306_command1(start);
  ssd1306_command1(0X00);
  ssd1306_command1(stop);
  static const uint8_t PROGMEM scrollList2b[] = {0X00, 0XFF,
                                                 SSD1306_ACTIVATE_SCROLL};
  ssd1306_commandList(scrollList2b, sizeof(scrollList2b));
  TRANSACTION_END
}

/*!
    @brief  Activate a 1-pixel diagonal scroll for all or part of the display.
    @param  start
            First row.
    @param  stop
            Last row.
    @return None (void).
*/
// display.startscrolldiagright(0x00, 0x0F)
void Adafruit_SSD1306Plus::startscrolldiagrightone(uint8_t start, uint8_t stop) {
  TRANSACTION_START
  static const uint8_t PROGMEM scrollList3a[] = {
      SSD1306_SET_VERTICAL_SCROLL_AREA, 0X00};
  ssd1306_commandList(scrollList3a, sizeof(scrollList3a));
  ssd1306_command1(HEIGHT);
  static const uint8_t PROGMEM scrollList3b[] = {
      SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL_ONE, 0X00};
  ssd1306_commandList(scrollList3b, sizeof(scrollList3b));
  ssd1306_command1(start);
  ssd1306_command1(0X00);
  ssd1306_command1(stop);
  static const uint8_t PROGMEM scrollList3c[] = {0X01, SSD1306_ACTIVATE_SCROLL};
  ssd1306_commandList(scrollList3c, sizeof(scrollList3c));
  TRANSACTION_END
}

/*!
    @brief  Activate alternate 1-pixel diagonal scroll for all or part of the display.
    @param  start
            First row.
    @param  stop
            Last row.
    @return None (void).
*/
// To scroll the whole display, run: display.startscrolldiagleft(0x00, 0x0F)
void Adafruit_SSD1306Plus::startscrolldiagleftone(uint8_t start, uint8_t stop) {
  TRANSACTION_START
  static const uint8_t PROGMEM scrollList4a[] = {
      SSD1306_SET_VERTICAL_SCROLL_AREA, 0X00};
  ssd1306_commandList(scrollList4a, sizeof(scrollList4a));
  ssd1306_command1(HEIGHT);
  static const uint8_t PROGMEM scrollList4b[] = {
      SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL_ONE, 0X00};
  ssd1306_commandList(scrollList4b, sizeof(scrollList4b));
  ssd1306_command1(start);
  ssd1306_command1(0X00);
  ssd1306_command1(stop);
  static const uint8_t PROGMEM scrollList4c[] = {0X01, SSD1306_ACTIVATE_SCROLL};
  ssd1306_commandList(scrollList4c, sizeof(scrollList4c));
  TRANSACTION_END
}

void Adafruit_SSD1306Plus::ssd1306_sendCommandList(const uint8_t *c, uint8_t n) {
  ssd1306_commandList(c, n);
}


// New commands to v1.5

#define SSD1306_SFB_FADE  0x20
#define SSD1306_SFB_BLINK 0x30

void Adafruit_SSD1306Plus::fade(uint8_t time){
  TRANSACTION_START
  static const uint8_t PROGMEM fadeblinkList1a[] = {
      SSD1306_SET_FADE_BLINK, SSD1306_SFB_FADE + time};
  ssd1306_commandList(fadeblinkList1a, sizeof(fadeblinkList1a));
  TRANSACTION_END
}

void Adafruit_SSD1306Plus::blink(uint8_t time){
  TRANSACTION_START
  static const uint8_t PROGMEM fadeblinkList1b[] = {
      SSD1306_SET_FADE_BLINK, SSD1306_SFB_BLINK + time};
  ssd1306_commandList(fadeblinkList1b, sizeof(fadeblinkList1b));
  TRANSACTION_END
}

void Adafruit_SSD1306Plus::stopFadeBlink(void){
  TRANSACTION_START
  static const uint8_t PROGMEM fadeblinkList1c[] = {
      SSD1306_SET_FADE_BLINK, 0X00};
  ssd1306_commandList(fadeblinkList1c, sizeof(fadeblinkList1c));
  TRANSACTION_END
}

void Adafruit_SSD1306Plus::stopFade(void){
  stopFadeBlink();
}

void Adafruit_SSD1306Plus::stopBlink(void){
  stopFadeBlink();
}

void Adafruit_SSD1306Plus::setZoom(bool i){
  TRANSACTION_START
  static const uint8_t PROGMEM zoomList1a[] = {
      SSD1306_SET_ZOOM, i};
  ssd1306_commandList(zoomList1a, sizeof(zoomList1a));
  TRANSACTION_END
}


