#ifndef _Adafruit_SSD1306Plus_H_
#define _Adafruit_SSD1306Plus_H_


#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>

#include <Adafruit_SSD1306.h>


//
// Command/Data byte defines
//

#define SSD1306_COMMAND                      0x00
#define SSD1306_DATA                         0xC0

//
// Command byte defines
//

#define SSD1306_SETLOWCOLUMN                 0x00 ///< Not currently used
#define SSD1306_UNKNOWN_0x01                 0x01
#define SSD1306_UNKNOWN_0x02                 0x02
#define SSD1306_UNKNOWN_0x03                 0x03
#define SSD1306_UNKNOWN_0x04                 0x04
#define SSD1306_UNKNOWN_0x05                 0x05
#define SSD1306_UNKNOWN_0x06                 0x06
#define SSD1306_LASTLINE                     0x07 // No Adafruit label available?
#define SSD1306_UNKNOWN_0x08                 0x08
#define SSD1306_UNKNOWN_0x09                 0x09
#define SSD1306_UNKNOWN_0x0A                 0x0A
#define SSD1306_UNKNOWN_0x0B                 0x0B
#define SSD1306_UNKNOWN_0x0C                 0x0C
#define SSD1306_UNKNOWN_0x0D                 0x0D
#define SSD1306_UNKNOWN_0x0E                 0x0E
#define SSD1306_UNKNOWN_0x0F                 0x0F
#define SSD1306_SETHIGHCOLUMN                0x10 ///< Not currently used
#define SSD1306_UNKNOWN_0x11                 0x11
#define SSD1306_UNKNOWN_0x12                 0x12
#define SSD1306_UNKNOWN_0x13                 0x13
#define SSD1306_UNKNOWN_0x14                 0x14
#define SSD1306_UNKNOWN_0x15                 0x15
#define SSD1306_UNKNOWN_0x16                 0x16
#define SSD1306_UNKNOWN_0x17                 0x17
#define SSD1306_UNKNOWN_0x18                 0x18
#define SSD1306_UNKNOWN_0x19                 0x19
#define SSD1306_UNKNOWN_0x1A                 0x1A
#define SSD1306_UNKNOWN_0x1B                 0x1B
#define SSD1306_UNKNOWN_0x1C                 0x1C
#define SSD1306_UNKNOWN_0x1D                 0x1D
#define SSD1306_UNKNOWN_0x1E                 0x1E
#define SSD1306_UNKNOWN_0x1F                 0x1F
#define SSD1306_MEMORYMODE                   0x20 ///< See datasheet
#define SSD1306_COLUMNADDR                   0x21 ///< See datasheet
#define SSD1306_PAGEADDR                     0x22 ///< See datasheet
#define SSD1306_SET_FADE_BLINK               0x23 // New to v1.5
#define SSD1306_UNKNOWN_0x24                 0x24
#define SSD1306_UNKNOWN_0x25                 0x25
#define SSD1306_RIGHT_HORIZONTAL_SCROLL      0x26 ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL       0x27 ///< Init left scroll
#define SSD1306_UNKNOWN_0x28                 0x28
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL     0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL      0x2A ///< Init diag scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL_ONE 0x24 ///< Init diag scroll 1-pxl
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL_ONE  0x25 ///< Init diag scroll 1-pxl
#define SSD1306_UNKNOWN_0x2B                 0x2B
#define SSD1306_RIGHT_HORIZONTAL_SCROLL_ONE  0x2C ///< Init right scroll 1-pxl
#define SSD1306_LEFT_HORIZONTAL_SCROLL_ONE   0x2D ///< Init left scroll 1-pxl
#define SSD1306_DEACTIVATE_SCROLL            0x2E ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL              0x2F ///< Start scroll
#define SSD1306_UNKNOWN_0x30                 0x30
#define SSD1306_UNKNOWN_0x31                 0x31
#define SSD1306_UNKNOWN_0x32                 0x32
#define SSD1306_UNKNOWN_0x33                 0x33
#define SSD1306_UNKNOWN_0x34                 0x34
#define SSD1306_UNKNOWN_0x35                 0x35
#define SSD1306_UNKNOWN_0x36                 0x36
#define SSD1306_UNKNOWN_0x37                 0x37
#define SSD1306_UNKNOWN_0x38                 0x38
#define SSD1306_UNKNOWN_0x39                 0x39
#define SSD1306_UNKNOWN_0x3A                 0x3A
#define SSD1306_UNKNOWN_0x3B                 0x3B
#define SSD1306_UNKNOWN_0x3C                 0x3C
#define SSD1306_UNKNOWN_0x3D                 0x3D
#define SSD1306_UNKNOWN_0x3E                 0x3E
#define SSD1306_UNKNOWN_0x3F                 0x3F
#define SSD1306_SETSTARTLINE                 0x40 ///< See datasheet
#define SSD1306_DATA_CONTINUE                0x40 // No Adafruit label available?
#define SSD1306_UNKNOWN_0x41                 0x41
#define SSD1306_UNKNOWN_0x42                 0x42
#define SSD1306_UNKNOWN_0x43                 0x43
#define SSD1306_UNKNOWN_0x44                 0x44
#define SSD1306_UNKNOWN_0x45                 0x45
#define SSD1306_UNKNOWN_0x46                 0x46
#define SSD1306_UNKNOWN_0x47                 0x47
#define SSD1306_UNKNOWN_0x48                 0x48
#define SSD1306_UNKNOWN_0x49                 0x49
#define SSD1306_UNKNOWN_0x4A                 0x4A
#define SSD1306_UNKNOWN_0x4B                 0x4B
#define SSD1306_UNKNOWN_0x4C                 0x4C
#define SSD1306_UNKNOWN_0x4D                 0x4D
#define SSD1306_UNKNOWN_0x4E                 0x4E
#define SSD1306_UNKNOWN_0x4F                 0x4F
#define SSD1306_UNKNOWN_0x50                 0x50
#define SSD1306_UNKNOWN_0x51                 0x51
#define SSD1306_UNKNOWN_0x52                 0x52
#define SSD1306_UNKNOWN_0x53                 0x53
#define SSD1306_UNKNOWN_0x54                 0x54
#define SSD1306_UNKNOWN_0x55                 0x55
#define SSD1306_UNKNOWN_0x56                 0x56
#define SSD1306_UNKNOWN_0x57                 0x57
#define SSD1306_UNKNOWN_0x58                 0x58
#define SSD1306_UNKNOWN_0x59                 0x59
#define SSD1306_UNKNOWN_0x5A                 0x5A
#define SSD1306_UNKNOWN_0x5B                 0x5B
#define SSD1306_UNKNOWN_0x5C                 0x5C
#define SSD1306_UNKNOWN_0x5D                 0x5D
#define SSD1306_UNKNOWN_0x5E                 0x5E
#define SSD1306_UNKNOWN_0x5F                 0x5F
#define SSD1306_UNKNOWN_0x60                 0x60
#define SSD1306_UNKNOWN_0x61                 0x61
#define SSD1306_UNKNOWN_0x62                 0x62
#define SSD1306_UNKNOWN_0x63                 0x63
#define SSD1306_UNKNOWN_0x64                 0x64
#define SSD1306_UNKNOWN_0x65                 0x65
#define SSD1306_UNKNOWN_0x66                 0x66
#define SSD1306_UNKNOWN_0x67                 0x67
#define SSD1306_UNKNOWN_0x68                 0x68
#define SSD1306_UNKNOWN_0x69                 0x69
#define SSD1306_UNKNOWN_0x6A                 0x6A
#define SSD1306_UNKNOWN_0x6B                 0x6B
#define SSD1306_UNKNOWN_0x6C                 0x6C
#define SSD1306_UNKNOWN_0x6D                 0x6D
#define SSD1306_UNKNOWN_0x6E                 0x6E
#define SSD1306_UNKNOWN_0x6F                 0x6F
#define SSD1306_UNKNOWN_0x70                 0x70
#define SSD1306_UNKNOWN_0x71                 0x71
#define SSD1306_UNKNOWN_0x72                 0x72
#define SSD1306_UNKNOWN_0x73                 0x73
#define SSD1306_UNKNOWN_0x74                 0x74
#define SSD1306_UNKNOWN_0x75                 0x75
#define SSD1306_UNKNOWN_0x76                 0x76
#define SSD1306_UNKNOWN_0x77                 0x77
#define SSD1306_UNKNOWN_0x78                 0x78
#define SSD1306_UNKNOWN_0x79                 0x79
#define SSD1306_UNKNOWN_0x7A                 0x7A
#define SSD1306_UNKNOWN_0x7B                 0x7B
#define SSD1306_UNKNOWN_0x7C                 0x7C
#define SSD1306_UNKNOWN_0x7D                 0x7D
#define SSD1306_UNKNOWN_0x7E                 0x7E
#define SSD1306_UNKNOWN_0x7F                 0x7F
#define SSD1306_UNKNOWN_0x80                 0x80
#define SSD1306_SETCONTRAST                  0x81 ///< See datasheet
#define SSD1306_UNKNOWN_0x82                 0x82
#define SSD1306_UNKNOWN_0x83                 0x83
#define SSD1306_UNKNOWN_0x84                 0x84
#define SSD1306_UNKNOWN_0x85                 0x85
#define SSD1306_UNKNOWN_0x86                 0x86
#define SSD1306_UNKNOWN_0x87                 0x87
#define SSD1306_UNKNOWN_0x88                 0x88
#define SSD1306_UNKNOWN_0x89                 0x89
#define SSD1306_UNKNOWN_0x8A                 0x8A
#define SSD1306_UNKNOWN_0x8B                 0x8B
#define SSD1306_UNKNOWN_0x8C                 0x8C
#define SSD1306_CHARGEPUMP                   0x8D ///< See datasheet
#define SSD1306_UNKNOWN_0x8E                 0x8E
#define SSD1306_UNKNOWN_0x8F                 0x8F
#define SSD1306_UNKNOWN_0x90                 0x90
#define SSD1306_UNKNOWN_0x91                 0x91
#define SSD1306_UNKNOWN_0x92                 0x92
#define SSD1306_UNKNOWN_0x93                 0x93
#define SSD1306_UNKNOWN_0x94                 0x94
#define SSD1306_UNKNOWN_0x95                 0x95
#define SSD1306_UNKNOWN_0x96                 0x96
#define SSD1306_UNKNOWN_0x97                 0x97
#define SSD1306_UNKNOWN_0x98                 0x98
#define SSD1306_UNKNOWN_0x99                 0x99
#define SSD1306_UNKNOWN_0x9A                 0x9A
#define SSD1306_UNKNOWN_0x9B                 0x9B
#define SSD1306_UNKNOWN_0x9C                 0x9C
#define SSD1306_UNKNOWN_0x9D                 0x9D
#define SSD1306_UNKNOWN_0x9E                 0x9E
#define SSD1306_UNKNOWN_0x9F                 0x9F
#define SSD1306_SEGREMAP                     0xA0 ///< See datasheet
#define SSD1306_UNKNOWN_0xA1                 0xA1
#define SSD1306_UNKNOWN_0xA2                 0xA2
#define SSD1306_SET_VERTICAL_SCROLL_AREA     0xA3 ///< Set scroll range
#define SSD1306_DISPLAYALLON_RESUME          0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON                 0xA5 ///< Not currently used
#define SSD1306_NORMALDISPLAY                0xA6 ///< See datasheet
#define SSD1306_INVERTDISPLAY                0xA7 ///< See datasheet
#define SSD1306_SETMULTIPLEX                 0xA8 ///< See datasheet
#define SSD1306_UNKNOWN_0xA9                 0xA9
#define SSD1306_UNKNOWN_0xAA                 0xAA
#define SSD1306_UNKNOWN_0xAB                 0xAB
#define SSD1306_UNKNOWN_0xAC                 0xAC
#define SSD1306_UNKNOWN_0xAD                 0xAD
#define SSD1306_DISPLAYOFF                   0xAE ///< See datasheet
#define SSD1306_DISPLAYON                    0xAF ///< See datasheet
#define SSD1306_UNKNOWN_0xB0                 0xB0
#define SSD1306_UNKNOWN_0xB1                 0xB1
#define SSD1306_UNKNOWN_0xB2                 0xB2
#define SSD1306_UNKNOWN_0xB3                 0xB3
#define SSD1306_UNKNOWN_0xB4                 0xB4
#define SSD1306_UNKNOWN_0xB5                 0xB5
#define SSD1306_UNKNOWN_0xB6                 0xB6
#define SSD1306_UNKNOWN_0xB7                 0xB7
#define SSD1306_UNKNOWN_0xB8                 0xB8
#define SSD1306_UNKNOWN_0xB9                 0xB9
#define SSD1306_UNKNOWN_0xBA                 0xBA
#define SSD1306_UNKNOWN_0xBB                 0xBB
#define SSD1306_UNKNOWN_0xBC                 0xBC
#define SSD1306_UNKNOWN_0xBD                 0xBD
#define SSD1306_UNKNOWN_0xBE                 0xBE
#define SSD1306_UNKNOWN_0xBF                 0xBF
#define SSD1306_COMSCANINC                   0xC0 ///< Not currently used
#define SSD1306_UNKNOWN_0xC1                 0xC1
#define SSD1306_UNKNOWN_0xC2                 0xC2
#define SSD1306_UNKNOWN_0xC3                 0xC3
#define SSD1306_UNKNOWN_0xC4                 0xC4
#define SSD1306_UNKNOWN_0xC5                 0xC5
#define SSD1306_UNKNOWN_0xC6                 0xC6
#define SSD1306_UNKNOWN_0xC7                 0xC7
#define SSD1306_COMSCANDEC                   0xC8 ///< See datasheet
#define SSD1306_UNKNOWN_0xC9                 0xC9
#define SSD1306_UNKNOWN_0xCA                 0xCA
#define SSD1306_UNKNOWN_0xCB                 0xCB
#define SSD1306_UNKNOWN_0xCC                 0xCC
#define SSD1306_UNKNOWN_0xCD                 0xCD
#define SSD1306_UNKNOWN_0xCE                 0xCE
#define SSD1306_UNKNOWN_0xCF                 0xCF
#define SSD1306_UNKNOWN_0xD0                 0xD0
#define SSD1306_UNKNOWN_0xD1                 0xD1
#define SSD1306_UNKNOWN_0xD2                 0xD2
#define SSD1306_SETDISPLAYOFFSET             0xD3 ///< See datasheet
#define SSD1306_UNKNOWN_0xD4                 0xD4
#define SSD1306_SETDISPLAYCLOCKDIV           0xD5 ///< See datasheet
#define SSD1306_SET_ZOOM                     0xD6 // New to v1.5
#define SSD1306_UNKNOWN_0xD7                 0xD7
#define SSD1306_UNKNOWN_0xD8                 0xD8
#define SSD1306_SETPRECHARGE                 0xD9 ///< See datasheet
#define SSD1306_SETCOMPINS                   0xDA ///< See datasheet
#define SSD1306_SETVCOMDETECT                0xDB ///< See datasheet
#define SSD1306_UNKNOWN_0xDC                 0xDC
#define SSD1306_UNKNOWN_0xDD                 0xDD
#define SSD1306_UNKNOWN_0xDE                 0xDE
#define SSD1306_UNKNOWN_0xDF                 0xDF
#define SSD1306_UNKNOWN_0xE0                 0xE0
#define SSD1306_UNKNOWN_0xE1                 0xE1
#define SSD1306_UNKNOWN_0xE2                 0xE2
#define SSD1306_NOP                          0xE3 // No Adafruit label available?
#define SSD1306_POSSIBLE_RESET               0xE4 // See note below
#define SSD1306_UNKNOWN_0xE5                 0xE5
#define SSD1306_UNKNOWN_0xE6                 0xE6
#define SSD1306_UNKNOWN_0xE7                 0xE7
#define SSD1306_UNKNOWN_0xE8                 0xE8
#define SSD1306_UNKNOWN_0xE9                 0xE9
#define SSD1306_UNKNOWN_0xEA                 0xEA
#define SSD1306_UNKNOWN_0xEB                 0xEB
#define SSD1306_UNKNOWN_0xEC                 0xEC
#define SSD1306_UNKNOWN_0xED                 0xED
#define SSD1306_UNKNOWN_0xEE                 0xEE
#define SSD1306_UNKNOWN_0xEF                 0xEF
#define SSD1306_UNKNOWN_0xF0                 0xF0
#define SSD1306_UNKNOWN_0xF1                 0xF1
#define SSD1306_UNKNOWN_0xF2                 0xF2
#define SSD1306_UNKNOWN_0xF3                 0xF3
#define SSD1306_UNKNOWN_0xF4                 0xF4
#define SSD1306_UNKNOWN_0xF5                 0xF5
#define SSD1306_UNKNOWN_0xF6                 0xF6
#define SSD1306_UNKNOWN_0xF7                 0xF7
#define SSD1306_UNKNOWN_0xF8                 0xF8
#define SSD1306_UNKNOWN_0xF9                 0xF9
#define SSD1306_UNKNOWN_0xFA                 0xFA
#define SSD1306_UNKNOWN_0xFB                 0xFB
#define SSD1306_UNKNOWN_0xFC                 0xFC
#define SSD1306_UNKNOWN_0xFD                 0xFD
#define SSD1306_UNKNOWN_0xFE                 0xFE
#define SSD1306_UNKNOWN_0xFF                 0xFF

//
// Note: For SSD1306_POSSIBLE_RESET = 0xE4
// URL: https://forum.arduino.cc/t/ssd1306-reset-pin/291767/14
//

//
// Fade/Blink data bytes
//

#define SSD1306_SFB_OFF      0x00
#define SSD1306_SFB_FADE     0x20
#define SSD1306_SFB_BLINK    0x30

//
// COM pin configuration - Sect. 4, Table 9-1 datasheet
//

#define SSD1306_SEQCOM_NOLR  0x02  // Sequential COM pin configuration
                                   // Disable COM Left/Right remap
#define SSD1306_ALTCOM_NOLR  0x12  // Alternative COM pin configuration
                                   // Disable COM Left/Right remap
#define SSD1306_SEQCOM_ENLR  0x22  // Sequential COM pin configuration
                                   // Enable COM Left/Right remap
#define SSD1306_ALTCOM_ENLR  0x32  // Alternative COM pin configuration
                                   // Enable COM Left/Right remap


//
// Charge pump configuration - Sect. 7, Table 9-1 datasheet
//

#define SSD1306_CH_PUMP_OFF  0x10
#define SSD1306_CH_PUMP_ON   0x14

//
// Zoom configuration
//

#define SSD1306_ZOOM_ON      0x01
#define SSD1306_ZOOM_OFF     0x00


//
// Class 
//

class Adafruit_SSD1306Plus:public Adafruit_SSD1306 {    //Adafruit_SSD1306Plus class inherits base class Adafruit_SSD1306
public:
  // NEW CONSTRUCTORS -- recommended for new projects
  Adafruit_SSD1306Plus(uint8_t w, uint8_t h, TwoWire *twi = &Wire,
                   int8_t rst_pin = -1, uint32_t clkDuring = 400000UL,
                   uint32_t clkAfter = 100000UL);
  Adafruit_SSD1306Plus(uint8_t w, uint8_t h, int8_t mosi_pin, int8_t sclk_pin,
                   int8_t dc_pin, int8_t rst_pin, int8_t cs_pin);
  Adafruit_SSD1306Plus(uint8_t w, uint8_t h, SPIClass *spi, int8_t dc_pin,
                   int8_t rst_pin, int8_t cs_pin, uint32_t bitrate = 8000000UL);

  // DEPRECATED CONSTRUCTORS - for back compatibility, avoid in new projects
  Adafruit_SSD1306Plus(int8_t mosi_pin, int8_t sclk_pin, int8_t dc_pin,
                   int8_t rst_pin, int8_t cs_pin);
  Adafruit_SSD1306Plus(int8_t dc_pin, int8_t rst_pin, int8_t cs_pin);
  Adafruit_SSD1306Plus(int8_t rst_pin = -1);
  ~Adafruit_SSD1306Plus(void);

  // Single Pixel Scroll
  void startscrollrightone(uint8_t start, uint8_t stop);
  void startscrollleftone(uint8_t start, uint8_t stop);

  void startscrolldiagrightone(uint8_t start, uint8_t stop);
  void startscrolldiagleftone(uint8_t start, uint8_t stop);

  void startscrollrightonewait(uint8_t start, uint8_t stop, int scrollWaitTime);
  void startscrollleftonewait(uint8_t start, uint8_t stop, int scrollWaitTime);
  void startscrolldiagrightonewait(uint8_t start, uint8_t stop, int scrollWaitTime);
  void startscrolldiagleftonewait(uint8_t start, uint8_t stop, int scrollWaitTime);

  // Raw commands
  void ssd1306_sendCommandList(const uint8_t *c, uint8_t n);

  // New features from v1.5 datasheet
  void fade(uint8_t time);
  void blink(uint8_t time);
  void stopFadeBlink(void);
  void stopFade(void);
  void stopBlink(void);
  void setZoom(bool i);

  // Extra bonus features!
  // COM Pins configuration
  void setCOMPins(bool com, bool lr);
  void setCOMPins_SEQCOM_NOLR(void);
  void setCOMPins_ALTCOM_NOLR(void);
  void setCOMPins_SEQCOM_ENLR(void);
  void setCOMPins_ALTCOM_ENLR(void);
  // Charge Pump
  void setChargePump(bool i);
  void setChargePumpOn(void);
  void setChargePumpOff(void);
  void setChargePumpDisplay(bool i);
  void setChargePumpOnDisplay(void);
  void setChargePumpOffDisplay(void);
};

#endif // _Adafruit_SSD1306Plus_H_

