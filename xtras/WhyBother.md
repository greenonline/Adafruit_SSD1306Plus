## Dev notes - Why bother?

The SSD1306 has the ability to scroll the display, continuously, by issuing a single command.

However, the ability to *only* scroll continuously, presents some problems, with respect to the timing of display updates. One example of such display updates is this scrolling sinusoidal wave, as described in this video, [Tutorial-6: Plotting rolling graph on OLED](https://www.youtube.com/watch?v=4azujWZVrUk). The method used to time the display updates involves calcualting the time to scroll one whole screen width and then divide that time by the number of pixels. Then use the resulting time to scroll *one pixel*, to set an internal timer interrupt to trigger display updates. This is not the most accurate of methods, I am sure that you will agree, with it almost bordering upon the haphazard - indeed, some misplaced pixels can be seen in the video.

Unfortunately, there does not appear to be a method to detect, via an internal register or interrupt, *when exactly* a scroll has just occurred, which would provie a bit more accuracy and sychronisation.

However, if you can scroll *just one pixel* at a time, then no longer do you have to calculate the scroll period, and as a result scrolling images can be a lot less "hit and miss".

Consider this:

   - The "concept" is analoguous to a conveyor belt upon which you have to place cup cakes at equal and pre-defined intervals, or distances from each other. If the conveyor belt never stops (i.e. continually scrolls), then you have to basically throw the cup cakes on to the belt at intervals that you can best judge the optimal time in which to launch the cup cake at the conveyor belt. With some practice and after some time, you might get pretty good at it and most of the cup cakes will hit the spot, but due to the inherent "estimation" of launch times, it is inevitable that some cup cakes may overshoot or fall short of the mark, and overall look a bit messy.
   - It would be much "better" to just advance the conveyor belt by an amount equal to the desired, or target, distance between cup cakes, then pause the conveyor belt momentarily, so that the cup cake can be accurately placed. Once the cup cake has been placed in the desired location, the conveyor belt can resume advancing, again by the same distance, and then pausing for cup cake placement. The process of advancement and placement is repeated, ad infinitum. 
   - The conveyor belt may move (slightly) slower, on average, but none of the cup cakes would end up in the incorrect position, so *overall* a better experience (i.e. presentation) is had (by all). 

