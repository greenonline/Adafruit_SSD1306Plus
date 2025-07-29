# Adafruit_SSD1306Plus
A library that inherits and extends the Adafruit_SSD1306 Arduino library with undocumented 1-pixel scroll, zoom, fade and blink commands

The `Adafruit_SSD1306` library can be found here, [SSD1306 OLED Display with Arduino][1] 

## The new commands

### Undocumented commands

The 1-pixel scroll commands (`2Ch` and `2Dh`) are not documented in either the [v1.1][3] nor [v1.5][4] versions of the data sheet. However, they are mentioned in [this answer](https://stackoverflow.com/a/69200268/4424636) to [Scroll long text on OLED display][7] on Stack Overflow:

> The SSD1306 chip provides commands to enable both continuous scrolling and 1 pixel scroll. For our purpose of scrolling long text, the continuous scroll is not useful, as we want to scroll exactly one pixel. Therefore, we need to use the 1 pixel scroll command. The commands take the same parameters, except for the first opcode byte. For some reason in the widely circulated (and pretty old) datasheet online, the 1 pixel scroll command is missing, but it is there in HW.
> 
>     #define CMD_CONTINUOUS_SCROLL_H_RIGHT		0x26
>     #define CMD_CONTINUOUS_SCROLL_H_LEFT		0x27
>     #define CMD_ONE_COLUMN_SCROLL_H_RIGHT		0x2C
>     #define CMD_ONE_COLUMN_SCROLL_H_LEFT		0x2D
> 
> Use the latter two.

#### Important Note

I found, on the Seeeduino Xiao (SAMD21) at least, that a delay ***is*** required, *immediately* after issuing a 1-pixel scroll command, or when ***repeatedly*** issuing 1-pixel scroll commands. 

```none
    display.startscrollrightone(0x00, 0x0F);
    delay(wait_time);
```

Without any `delay(wait_time)` then no scrolling occurred at all. If `wait_time` is less than 12 ms delay, then that causes some of the scrolls to fail, and the full 50 pixels are *not* scrolled. The closer that the delay is to zero, the less distance, in pixels scrolled, the text/images actually move. The minimum length of the required delay probably depends upon the frequency of the µController, so you may need to experiment for your particular setup.

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

[![I've done it][8][8]


  [1]: https://github.com/adafruit/Adafruit_SSD1306
  [2]: https://www.aidansun.com/dl/SSD1306_I2C.zip
  [3]: https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
  [4]: https://www.rlocman.ru/i/File/2020/04/17/SSD1306.pdf
  [5]: https://arduino.stackexchange.com/q/51420/6936
  [6]: https://electronics.stackexchange.com/q/485198/64015
  [7]: https://stackoverflow.com/q/40564050/4424636
  [8]: https://gr33nonline3.wordpress.com/wp-content/uploads/2023/02/memento-ive-done-it.png "I've done it"
