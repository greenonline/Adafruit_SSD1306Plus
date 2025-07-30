## Single vertical scroll investigation

### Horizontal

Note the bits that *change*, i.e. toggle:

```none
0x26 = 0b100110 - Continuous
0x2C = 0b101100 - One pixel
```

The bits that change are bits 1 and 3. Is there a similar change, in a similar pattern, required in order to acheive the mythical 1-pixel vertical scroll?

Note that bit 0 changes the direction, left or right for both continuous and 1-pixel horizontal scroll (i.e. `0x26`/`0x27` and `0x2C`/`0x2D`).

### Vertical 

```none
0x29 = 0b101001 - Continuous
0x?? = 0b?????? - One pixel
0x?? = 0b10?0?1 - One pixel
0x23 = 0b100011 - One pixel
0x24 = 0b100100 - One pixel
0x25 = 0b100101 - One pixel
```
