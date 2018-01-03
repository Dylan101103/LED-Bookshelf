# LED-Bookshelf
THIS PROJECT IS STILL BEING DEVELOPED AND HAS NOT BEEN TESTED. USE AT YOUR OWN RISK.

Parts List

 1x Arduino Uno R3
 
 5M Adafruit NeoPixel Digital RGBW LED Strip - Black PCB 30 LED/m
 
 1x Female DC Power adapter - 2.1mm jack to screw terminal block
 
 1x 5V 10A switching power supply
 
 5x 3-pin JST SM Plug + Receptacle Cable Set
 
 1x Rugged Metal Pushbutton with Blue LED Ring
 
 1x Rugged Metal Pushbutton with Green LED Ring
 
 1x Rugged Metal Pushbutton with Red LED Ring
 
 2x Silicone Cicoil Wire Cable - 4-pin 24AWG
 
 5x Silicone Clips for NeoPixel LED Strips - set of 5
 
 2x Arcade/Button Quick-Connect Wire Pair - Set of 10 pairs
 
 25Ft of 14-2  Romex
 
 1x Can of black quick-dry spray paint
 
 Most items can be found on this adafruit wishlist http://www.adafruit.com/wishlists/452308

The "Arduino_Bookshelf_Adafruit" sketch is build using the Adafruit NeoPixel libaray.

The "Arduino_Bookshelf_FastLED" sketch is build using the FastLED NeoPixel libaray.
  NOTE:FastLED does NOT currently support adafruit RGBW Neopixlels

Notes:

Bookshelf LED Setup


Shelf address destination
0 = 1-20
1 = 21-41
2 = 42-62
3 = 63-83
4 = 104-124

Triggers
   Time specific
   Button control 
   Blue Button Pin2 - Effect
   Green Button Pin4 - Time  
   Red Button -  Unknown\Extra\Reset
   Button LEDS - 5(PWM)

Time
 1 = all shelfs on white for 5 Min
   15min
   30min
   1hour
   5hour
   10 hours
   24 hours

 Auto on
  Times On
   7:30 Sunrise
   23:30 Red
  Times Off
   9:00
   00:30

Effects
   Rainbow
   White
   color fade
   Random color fade
   Scanner
   Color alternate on shelf
   preset color on each shelf

Colors / Patterns
 Off
 Red
 Green
 Blue
 White
 Theater chase white
 Rainbow 
 Rainbow cycle
 Theater Chase Rainbow



Button Wiring 
1 gnd           Green
2 Button LED    Blue
3 Button Blue   Purple
4 Button Green  Gray
5 Button Red    White
6 Not Used      Black
