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
  void setZoom(bool i);
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

## Important Notes

### 1-pixel scroll

#### 1-pixel scrolling and `SSD1306_ACTIVATE_SCROLL`

Note that, unlike the continuous scroll methods of the parent library `Adafruit_SSD1306`, the 1-pixel scroll methods do not send the `SSD1306_ACTIVATE_SCROLL` byte, as the last byte, as it is not required, for the 1-pixel scroll methods *only*.

#### 1-pixel scrolling and `stopscroll()` 

Note that the 1-pixel scroll methods do not require *repeated* calls to `stopscroll()` either before or after *each* 1-pixel scroll method call, as it is not required, nor efficient, to do so, for the 1-pixel scroll methods *only*. 

However, it *is* good practice to call `stopScroll()` *once* before commencing a series of 1-pixel scroll method calls – or any scroll commmands for that matter – just to be certain that any continuous scrolling, that *might* be occurring, is halted – otherwise the display's RAM buffer may become corrupted.

#### Use of 'interval' byte for 1-pixel scroll

The 'interval' byte, `C[2:0]`, as detailed in the datasheet on page 31, Table 9-1 section 2, obviously has no meaning for a 1-pixel scroll.

Setting the 'interval' byte to `0b111` prevents wrap-around scrolling and instead causes a white "background" to appear, from the edge of the screen, when scrolling. Setting to `0b000` eliminates this white background effect, and enables wrap-around scrolling. 

#### Waiting for the 1-pixel scroll

##### Seeeduino Xiao

I found, on the Seeeduino Xiao (SAMD21) at least, that a delay ***is*** required, *immediately* after issuing a 1-pixel scroll command, or when ***repeatedly*** issuing 1-pixel scroll commands. 

```none
    display.startscrollrightone(0x00, 0x0F);
    delay(wait_time);
```

Without any `delay(wait_time)` then no scrolling occurred at all. If `wait_time` is less than 12 ms delay, then that causes some of the scrolls to fail, and the full 50 pixels are *not* scrolled. The closer that the delay is to zero, the less distance, in pixels scrolled, the text/images actually move. The minimum length of the required delay probably depends upon the frequency of the µController, so you may need to experiment for your particular setup.

##### Arduino Uno on Wokwi

For the landscape example, using an Arduino Uno on Wokwi, I found that only the top page, 0x00, i.e. the top third of the clouds, scrolled, and the remaining pages did not. So, even for one of the 1-pixel scroll commands,

```none
    display.startscrollleftone(0x00, 0x02);
```
that is *meant to* scroll pages 0x00 to 0x02, only *partially* scrolled the first of the three pages. I am unsure whether this is a bug in the Wokwi implementation of the SSD1306, or whether the Uno can not handle the code. See animated gif:

[![Arduino Uno on Wokwi SSD1306 Landscape Scroll][11]][11]

I also found for the Arduino Uno simulation in Wokwi, that the `wait_time` had to be *significantly* increased, from the 12  milliseconds required for the Seeeduino Xiao, up to 100 milliseconds:

| Wait Time (ms)|Scroll result|
|---|---|
|100 | good |
|75 | almost, but jerky |
|68 | jerky 4 pixel jumpy scroll |
|63 | jerky 2 pixel jumpy scroll |
|50 | no scrolling |

##### Pairing of 1-pixel scroll and delay

Seeing as a *brief* delay is *always* required, after a 1-pixel scroll, it seemed expedient to combine the two together in wrapper methods that just combine the two into one simple call:

```none
void Adafruit_SSD1306Plus::startscrollrightonewait(uint8_t start, uint8_t stop, int scrollWaitTime);
void Adafruit_SSD1306Plus::startscrollleftonewait(uint8_t start, uint8_t stop, int scrollWaitTime);
void Adafruit_SSD1306Plus::startscrolldiagrightonewait(uint8_t start, uint8_t stop, int scrollWaitTime);
void Adafruit_SSD1306Plus::startscrolldiagleftonewait(uint8_t start, uint8_t stop, int scrollWaitTime)
```

### Vertical 1-pixel scroll

Feel free to experiment! You can try to find the *mythical* 1-pixel vertical scroll yourself by changing the values of the two `#define` lines

```none
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL_ONE  0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL_ONE   0x2A ///< Init diag scroll
```

in `Adafruit_SSD1306Plus/src/Adafruit_SSD1306Plus.h`.

As stated in [OnePixelVertScroll](xtras/OnePixelVertScroll.md), the two `#define` lines are currently set to `0x24` and `0x25` which do not *seem* to work, as expected, i.e. not at all!

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

