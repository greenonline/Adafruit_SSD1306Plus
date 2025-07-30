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

#### Example of 1-pixel scroll

This example illustrates a scrolling technique, that would be significantly more difficult to implement using continuous scrolling. It uses this 128x64 bitmap of a crudely drawn landscape:

> [![128x64 Bitmap landscape][9]][9]

The above landscape is split in to three horizontal regions:

 - Background - Pages `0x00` - `0x02` - The clouds
 - Mid-ground - Pages `0x02` - `0x05` - The mountain range
 - Foreground - Pages `0x06` - `0x07` - The rocky ground

Each of the three regions are scrolled at varying rates, with the mid-ground scrolling twice as fast as the background, and the foreground scrolling twice as fast as the mid-ground:

[![Xiao SSD1306 Landscape Scroll][10]][10]

<sup>This is *actual* footage taken with an iPhone SE (1st gen), hence the poor quality and "rolling sync bars". The effect is much more fluid in real life.</sup>

#### Important Notes

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

### Commands from v1.5 of the datasheet

There are two commands, that are missing from the commonly distributed v1.1 data sheet, documented in v1.5 of the data sheet:

 - Fade/Blink (`23h`)
 - Zoom (`D6h`).

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

 - Can not turn off Zoom, immediately after turning it on - weird bug, under investigation.

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

