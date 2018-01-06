//https://forum.arduino.cc/index.php?topic=344768.0
#include <Adafruit_NeoPixel.h>

#define PIN 6
#define PIN2 7
#define LED_COUNT 16

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip_a = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_b = Adafruit_NeoPixel(LED_COUNT, PIN2, NEO_GRB + NEO_KHZ800);
void setup() {
  strip_b.begin();
  strip_b.show(); // Initialize all pixels to 'off'
  strip_a.begin();
  strip_a.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  colorWipe(strip_a.Color(255, 0, 0), 50); // Red
  colorWipe(strip_a.Color(0, 255, 0), 50); // Green
  colorWipe(strip_a.Color(0, 0, 255), 50); // Blue
  rainbow(20);
  rainbowCycle(20);
    // Some example procedures showing how to display to the pixels:
  colorWipe(strip_b.Color(255, 0, 0), 50); // Red
  colorWipe(strip_b.Color(0, 255, 0), 50); // Green
  colorWipe(strip_b.Color(0, 0, 255), 50); // Blue
  rainbow(20);
  rainbowCycle(20);
}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip_a.numPixels(); i++) {
      strip_a.setPixelColor(i, c);
      strip_a.show();
      
  for(uint16_t i=0; i<strip_b.numPixels(); i++) {
      strip_b.setPixelColor(i, c);
      strip_b.show();
      delay(wait);
  }
}
}
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip_a.numPixels(); i++) {
      strip_a.setPixelColor(i, Wheel((i+j) & 255));
    }
    for(i=0; i<strip_b.numPixels(); i++) {
      strip_b.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip_b.show();
    strip_a.show();
    delay(wait);
  }
}
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip_a.numPixels(); i++) {
      strip_a.setPixelColor(i, Wheel(((i * 256 / strip_a.numPixels()) + j) & 255));
    }
    for(i=0; i< strip_b.numPixels(); i++) {
      strip_b.setPixelColor(i, Wheel(((i * 256 / strip_b.numPixels()) + j) & 255));
    }
    strip_a.show();
    strip_b.show();
    delay(wait);
  }
}


//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip_a.numPixels(); i=i+3) {
        strip_a.setPixelColor(i+q, c);    //turn every third pixel on
      }
      for (int i=0; i < strip_b.numPixels(); i=i+3) {
        strip_b.setPixelColor(i+q, c);    //turn every third pixel on
      }
      
      strip_a.show();
      strip_b.show();
      delay(wait);
     
      for (int i=0; i < strip_a.numPixels(); i=i+3) {
        strip_a.setPixelColor(i+q, 0);        //turn every third pixel off
      }
      for (int i=0; i < strip_b.numPixels(); i=i+3) {
        strip_b.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip_a.numPixels(); i=i+3) {
          strip_a.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        for (int i=0; i < strip_b.numPixels(); i=i+3) {
          strip_b.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        
        strip_a.show();
        strip_b.show();
        delay(wait);
       
        for (int i=0; i < strip_a.numPixels(); i=i+3) {
          strip_a.setPixelColor(i+q, 0);        //turn every third pixel off
        }
        for (int i=0; i < strip_b.numPixels(); i=i+3) {
          strip_b.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip_a.Color(255 - WheelPos * 3, 0, WheelPos * 3);
   return strip_b.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip_a.Color(0, WheelPos * 3, 255 - WheelPos * 3);
   return strip_b.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip_a.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
   return strip_b.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
