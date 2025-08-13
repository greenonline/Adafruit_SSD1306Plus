## Important Notes

### 1-pixel scroll

#### <A NAME="markdown-header-1-pixel-scrolling-and-SSD1306_ACTIVATE_SCROLL"></a>1-pixel scrolling and `SSD1306_ACTIVATE_SCROLL`

Note that, unlike the continuous scroll methods of the parent library `Adafruit_SSD1306`, the 1-pixel scroll methods do not send the `SSD1306_ACTIVATE_SCROLL` byte, as the last byte, as it is not required, for the 1-pixel scroll methods *only*.

#### <A NAME="markdown-header-1-pixel-scrolling-and-stopscroll"></a>1-pixel scrolling and `stopscroll()` 

Note that the 1-pixel scroll methods do not require *repeated* calls to `stopscroll()` either before or after *each* 1-pixel scroll method call, as it is not required, nor efficient, to do so, for the 1-pixel scroll methods *only*. 

However, it *is* good practice to call `stopScroll()` *once* before commencing a series of 1-pixel scroll method calls – or any scroll commmands for that matter – just to be certain that any continuous scrolling, that *might* be occurring, is halted – otherwise the display's RAM buffer may become corrupted.

#### <A NAME="markdown-header-use-of-interval-byte-for-1-pixel-scroll"></a>Use of 'interval' byte for 1-pixel scroll

The 'interval' byte, `C[2:0]`, as detailed in the datasheet on page 31, Table 9-1 section 2, obviously has no meaning for a 1-pixel scroll.

Setting the 'interval' byte to `0b111` prevents wrap-around scrolling and instead causes a white "background" to appear, from the edge of the screen, when scrolling. Setting to `0b000` eliminates this white background effect, and enables wrap-around scrolling. 

#### <A NAME="markdown-header-waiting-for-the-1-pixel-scroll"></a>Waiting for the 1-pixel scroll

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

### <A NAME="markdown-header-pairing-of-1-pixel-scroll-and-delay"></a>Pairing of 1-pixel scroll and delay

Seeing as a *brief* delay is *always* required, after a 1-pixel scroll, it seemed expedient to combine the two together in wrapper methods that just combine the two into one simple call:

```none
void Adafruit_SSD1306Plus::startscrollrightonewait(uint8_t start, uint8_t stop, int scrollWaitTime);
void Adafruit_SSD1306Plus::startscrollleftonewait(uint8_t start, uint8_t stop, int scrollWaitTime);
void Adafruit_SSD1306Plus::startscrolldiagrightonewait(uint8_t start, uint8_t stop, int scrollWaitTime);
void Adafruit_SSD1306Plus::startscrolldiagleftonewait(uint8_t start, uint8_t stop, int scrollWaitTime)
```

### <A NAME="markdown-header-vertical-1-pixel-scroll"></a>Vertical 1-pixel scroll

Feel free to experiment! You can try to find the *mythical* 1-pixel vertical scroll yourself by changing the values of the two `#define` lines

```none
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL_ONE  0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL_ONE   0x2A ///< Init diag scroll
```

in `Adafruit_SSD1306Plus/src/Adafruit_SSD1306Plus.h`.

As stated in [OnePixelVertScroll](xtras/OnePixelVertScroll.md), the two `#define` lines are currently set to `0x24` and `0x25` which do not *seem* to work, as expected, i.e. not at all!

### <A NAME="markdown-header-do-this-before-commencing-display"></a>Do this BEFORE commencing display

Be sure to stop scroll, fade, blink, and especially zoom, at the beginning of a sketch, as the SSD1306 may has a previous configuration in RAM that needs to be cleared:

```none
  // Stop scroll, fade, blink and zoom
  display.stopFadeBlink();
  display.stopscroll();
  display.stopZoom();
```

Zoom can be left configured from a previous sketch, leading to confusing results. This occurs if the display is in zoom mode, whilst the new sketch is being uploaded.

### <A NAME="markdown-header-inconsistent-method-names-Adafruit_1306-only"></a>Inconsistent method names (`Adafruit_SSD1306` only)

Three diferent naming schemes appear to have been used in the parent Adafruit_SSD1306 library:

 - `Underscores_in_Names` for constructors and sending commands
 - `alllowercasenames` for the continuous scroll commands
 - `someCamelCaseNames` for the sending commands

I have used the camel case for the new methods, which admittedly causes *some* inconsistency for some of the zoom commands method names, namely `stopscroll()` (all lower case), versus `stopBlink()` (camel case).
