# Adafruit_SSD1306Plus
A library that inherits and extends the Adafruit_SSD1306 Arduino library with undocumented 1-pixel scroll, zoom, fade and blink commands

The `Adafruit_SSD1306` library can be found here, [SSD1306 OLED Display with Arduino][1] 

## The new commands

### Undocumented commands

#### 1-pixel horizontal scroll

The 1-pixel scroll commands (`2Ch` and `2Dh`) are not documented in either the [v1.1][3] nor [v1.5][4] versions of the data sheet. However, they are mentioned in [this answer](https://stackoverflow.com/a/69200268/4424636) to [Scroll long text on OLED display][7] on Stack Overflow:

> The SSD1306 chip provides commands to enable both continuous scrolling and 1 pixel scroll. For our purpose of scrolling long text, the continuous scroll is not useful, as we want to scroll exactly one pixel. Therefore, we need to use the 1 pixel scroll command. The commands take the same parameters, except for the first opcode byte. For some reason in the widely circulated (and pretty old) datasheet online, the 1 pixel scroll command is missing, but it is there in HW.
> 
>     #define CMD_CONTINUOUS_SCROLL_H_RIGHT		0x26
>     #define CMD_CONTINUOUS_SCROLL_H_LEFT		0x27
>     #define CMD_ONE_COLUMN_SCROLL_H_RIGHT		0x2C
>     #define CMD_ONE_COLUMN_SCROLL_H_LEFT		0x2D
> 
> Use the latter two.

##### Methods

```none
  void startscrollrightone(uint8_t start, uint8_t stop);
  void startscrollleftone(uint8_t start, uint8_t stop);
```

### Commands from v1.5 of the datasheet

#### Fade/blink and zoom

There are two commands, that are missing from the commonly distributed v1.1 data sheet, documented in v1.5 of the data sheet:

 - Fade/Blink (`23h`)
 - Zoom (`D6h`)

##### Methods

```none
  void fade(uint8_t time);
  void blink(uint8_t time);
  void stopFadeBlink(void);
  void stopFade(void);
  void stopBlink(void);
  void zoom(void);
  void setZoom(bool i);
  void stopZoom();
```

### Bonus commands

Two sets of methods are provided, which deal with configuration of the COM pins and the charge pump, respectively. Please refer to the datasheet for more details.

#### COM pins configuration

Only use these methods if you know what you are doing.

##### Methods

```none
  void setCOMPins(bool com, bool lr);
  void setCOMPins_SEQCOM_NOLR(void);
  void setCOMPins_ALTCOM_NOLR(void);
  void setCOMPins_SEQCOM_ENLR(void);
  void setCOMPins_ALTCOM_ENLR(void);
```

#### Charge pump configuration

Only use these methods if you know what you are doing.

##### Methods


```none
  void setChargePump(bool i);
  void setChargePumpOn(void);
  void setChargePumpOff(void);
  void setChargePumpDisplay(bool i);
  void setChargePumpOnDisplay(void);
  void setChargePumpOffDisplay(void);
```

## Is there a 'hidden', or undocumented, single pixel vertical/horizontal scroll?

Seeing as there are two undocumented *horizontal* scroll commands, so, likewise, it makes one wonder whether there are undocumented equivalents for the two *continual vertical/horizontal* (a.k.a. 'diagonal') scroll commands - that would offer *1-pixel* vertical/horizontal (a.k.a. 'diagonal') scroll functionality..?

See [Undocumented 1-pixel vertical scroll](xtras/OnePixelVertScroll.md) for more details.

##### Methods

**Note**: *These are experimental, and do not work (yet)*.

```none
  void startscrolldiagrightone(uint8_t start, uint8_t stop);
  void startscrolldiagleftone(uint8_t start, uint8_t stop);
```

## Examples

There are multiple examples of how to use the new public methods, in the [`examples/`](examples/) directory.

### Example of 1-pixel scroll

The [LandscapeScroll](examples/LandscapeScroll/LandscapeScroll.ino) example illustrates a scrolling technique, that would be significantly more difficult to implement using continuous scrolling. No longer do you have to calculate the time that one pixel takes to be scrolled – when using continuous scroll – and then use that time for a timer based interrupt, in order to update the display.

The sketch uses this 128x64 bitmap of a crudely drawn landscape:

> [![128x64 Bitmap landscape][9]][9]

The above landscape is split in to three horizontal regions:

 - Background - Pages `0x00` - `0x02` - The clouds
 - Mid-ground - Pages `0x02` - `0x05` - The mountain range
 - Foreground - Pages `0x06` - `0x07` - The rocky ground

Each of the three regions are scrolled at varying rates, with the mid-ground scrolling twice as fast as the background, and the foreground scrolling twice as fast as the mid-ground:

[![Xiao SSD1306 Landscape Scroll][10]][10]

<sup>This is *actual* footage taken with an iPhone SE (1st gen), hence the poor quality and "rolling sync bars". The effect is much more fluid in real life.</sup>

## Dev Notes - Summary

 - [Why Bother?](xtras/WhyBother.md)
 - [Differences](xtras/DevNotes.md)
   - [1-pixel scrolling and `SSD1306_ACTIVATE_SCROLL` for Adafruit_SSD1306](xtras/DevNotes.md#user-content-1-pixel-scrolling-and-ssd1306_activate_scroll)
   - [Use of 'interval' byte for 1-pixel scroll](xtras/DevNotes.md#user-content-use-of-interval-byte-for-1-pixel-scroll)
   - [Waiting for the 1-pixel scroll](xtras/DevNotes.md#user-content-waiting-for-the-1-pixel-scroll)
   - [Pairing of 1-pixel scroll and delay](xtras/DevNotes.md#user-content-pairing-of-1-pixel-scroll-and-delay)
   - [Vertical 1-pixel scroll](xtras/DevNotes.md#user-content-vertical-1-pixel-scroll)
   - [Do this BEFORE commencing display](xtras/DevNotes.md#user-content-do-this-before-commencing-display)
   - [Inconsistent method names (`Adafruit_SSD1306` only)](xtras/DevNotes.md#user-content-inconsistent-method-names-adafruit_ssd1306-only)

## See also

A similarly derived library for the `SSD1306_I2C` Arduino library, [`SSD1306_I2CPlus`](https://github.com/greenonline/SSD1306_I2CPlus), that offers the same extended functionality.

## Links

### Datasheets
- [v1.1][3]
- [v1.5][4]


### Stack Exchange

 - [SSD1306 - Is there a way to scroll a certain number of pixels only?][5]
 - [Adafruit SSD1306 scrolling function][6]
 - [Scroll long text on OLED display][7]

### Blog

 - [SSD1306 tricks](https://gr33nonline.wordpress.com/2025/07/22/ssd1306-tricks/)

### Github issues

 - [Add one pixel horizontal scroll commands for 0x2C and 0x2D #293](https://github.com/adafruit/Adafruit_SSD1306/issues/293)

## Known Issues

 - None.

---

[![I've done it][12]][12]


  [1]: https://github.com/adafruit/Adafruit_SSD1306
  [2]: https://www.aidansun.com/dl/SSD1306_I2C.zip
  [3]: https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
  [4]: https://www.rlocman.ru/i/File/2020/04/17/SSD1306.pdf
  [5]: https://arduino.stackexchange.com/q/51420/6936
  [6]: https://electronics.stackexchange.com/q/485198/64015
  [7]: https://stackoverflow.com/q/40564050/4424636
  [9]: https://github.com/greenonline/Adafruit_SSD1306Plus/blob/main/xtras/Landscape.bmp "128x64 Bitmap landscape"
  [10]: https://github.com/greenonline/Adafruit_SSD1306Plus/blob/main/xtras/Landscape_Video_XiaoDevBoard480pCropped.gif "Xiao SSD1306 Landscape Scroll"
  [11]: https://github.com/greenonline/Adafruit_SSD1306Plus/blob/main/xtras/SSD1306_TopPageScroll.gif "Arduino Uno on Wokwi SSD1306 Landscape Scroll"
  [12]: https://gr33nonline3.wordpress.com/wp-content/uploads/2023/02/memento-ive-done-it.png "I've done it"

