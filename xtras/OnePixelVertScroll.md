## Single vertical scroll investigation

### Horizontal

Note the bits that *change*, i.e. toggle:

```none
0x26 = 0b100110 - Continuous right
0x27 = 0b100111 - Continuous left
0x2C = 0b101100 - One pixel right
0x2D = 0b101101 - One pixel left
```

The bits that change, from continuous to 1-pixel scroll, are `bit 1` and `bit 3`. 

**Is there a similar change, in a similar pattern, required in order to acheive the *mythical* 1-pixel vertical scroll?**

Note that `bit 0` just changes the direction, left or right for both continuous and 1-pixel horizontal scroll (i.e. `0x26`/`0x27` and `0x2C`/`0x2D`).

### Vertical

#### Unused command bytes that could be candidates

`0x24` and `0x25` would make good candidates, seeing as they are the *only adjascent pair* available, in the byte range that is "local" to the other scroll commands.

```none
#define SSD1306_UNKNOWN_0x24                 0x24
#define SSD1306_UNKNOWN_0x25                 0x25
#define SSD1306_UNKNOWN_0x28                 0x28
#define SSD1306_UNKNOWN_0x2B                 0x2B
```

#### Brainstorming possible bit toggling

```none
0x29 = 0b101001 - Continuous right
0x2A = 0b101010 - Continuous left
0x?? = 0b?????? - One pixel
0x?? = 0b10?0?1 - One pixel
0x23 = 0b100011 - Set fade Blink
0x24 = 0b100100 - One pixel?
0x25 = 0b100101 - One pixel?
0x28 = 0b101000 - ?
0x2B = 0b101011 = ? 
```

Note that, for continuous scrolling, whilst the toogling `bit 0` still changes direction (left/right), the order is reversed, as `bit 1` *also* toggles upon a direction change from right to left.


Taking `0x24` and `0x25`, assuming that left and right maintain the same, or reversed, bit order of `bit 1` and `bit 0`, then the bits that change, from continuous to single scroll, would be `bit 2`, and `bit 3` - which, at least, *seems* plausible.

```none
0x29 = 0b101001 - Continuous right
0x2A = 0b101010 - Continuous left
0x24 = 0b100100 - One pixel (left/right)?
0x25 = 0b100101 - One pixel (left/right)?
```

All that remains to test this, is to implement a couple of new methods:

For `Adafruit_SSD1306` library

 - `startscrolldiagrightone()`
 - `startscrolldiagleftone()`

with the following `#define` lines:

```none
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL_ONE  0x24
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL_ONE   0x25
```

Unfortunatelty, when tested, commands `0x24` and `0x25` did *not* scroll the display at all. Maybe some other currently unused bytes would work instead?  Further testing is ongoing.

