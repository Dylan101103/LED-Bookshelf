#include <Wire.h>

#include <RTClib.h>

#include <Adafruit_NeoPixel.h>

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

/*
// Pattern types supported:
enum  pattern { NONE, RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE };
// Patern directions supported:
enum  direction { FORWARD, REVERSE };
 */

#define MODE_PIN   4    // Digital IO pin connected to the button.  This will be  "Blue Button"
                          
#define TIME_PIN   7     //Digital IO pin connected to the button for time selction  "Green Button"

#define PIXEL_PIN    3    // Digital IO pin connected to the NeoPixels data pin.

#define PIXEL_COUNT 20   // Total attached pixels.

// Parameter 3 = pixel type flags, add together as needed:
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRBW  + NEO_KHZ800);

bool oldState = HIGH;
int showType = 0;

void setup() {

  pinMode(MODE_PIN, INPUT_PULLUP);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
    delay(3000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
 // strip.begin();
 // strip.show();
}
/*
// NeoPattern Class - derived from the Adafruit_NeoPixel class
class NeoPatterns : public Adafruit_NeoPixel
{
    public:
 
    // Member Variables:  
    pattern  ActivePattern;  // which pattern is running
    direction Direction;     // direction to run the pattern
    
    unsigned long Interval;   // milliseconds between updates
    unsigned long lastUpdate; // last update of position
    
    uint32_t Color1, Color2;  // What colors are in use
    uint16_t TotalSteps;  // total number of steps in the pattern
    uint16_t Index;  // current step within the pattern
    
    void (*OnComplete)();  // Callback on completion of pattern
}
    */
void loop() {
  // Check if time equals MTWTF 7:45 and set all LED's to white
  /*.DateTime now = rtc.now();
    if(now.hour() == 8)
    {
       // turn the led on
    }
*/
    // Check if time equals MTWTF 9:15 and set all LED's to off
  /*.DateTime now = rtc.now();
    if(now.hour() == 8)
    {
       // turn the led off
    }
*/
    // Check if time equals MTWTF 5:30 and set LED's to white low
  /*.DateTime now = rtc.now();
    if(now.hour() == 8)
    {
       // turn the led on
    }
*/
    // Check if time equals x and set color or effect
  /*.DateTime now = rtc.now();
    if(now.hour() == 8)
    {
       // turn the led on
    }
*/
  
  // Get current button state.

  bool newState = digitalRead(MODE_PIN);


  // Check if state changed from high to low (button press).
  if (newState == LOW && oldState == HIGH) {
    // Short delay to debounce button.
    delay(10);
    // Check if button is still low after debounce.

    newState = digitalRead(MODE_PIN);

    if (newState == LOW) {
      showType++;
      if (showType > 7)
        showType=0;
      startShow(showType);
    }
  }

  // Set the last button state to the old state.
  oldState = newState;
}

void startShow(int i) {
  switch(i){

    case 0: colorWipe(strip.Color(0, 0, 0, 0), 50);    // Black/off
            break;
    case 1: colorWipe(strip.Color(255, 0, 0, 0), 5);  // Red
            break;
    case 2: colorWipe(strip.Color(0, 255, 0, 0), 25);  // Green
            break;
    case 3: colorWipe(strip.Color(0, 0, 255, 0), 100);  // Blue
            break;
    case 4: colorWipe(strip.Color(0, 0, 0, 255), 500);  // White
            break;    
   // case 5: theaterChase(strip.Color(0, 0, 0, 127), 50); // White Pauses
            break;
   // case 6: theaterChase(strip.Color(127,   0,   0,  0), 50); // Red Pauses
            break;
   // case 7: theaterChase(strip.Color(  0,   0, 127,  0), 50); // Blue Pauses
            break;
    case 5: rainbow(20); //Pauses
            break;
    case 6: rainbowCycle(20); //Pauses
            break;
    case 7 : theaterChaseRainbow(50); //Works

            break; 
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
/*
    // Initialize for a RainbowCycle
    void RainbowCycle(uint8_t interval, direction dir = FORWARD)
    {
        ActivePattern = RAINBOW_CYCLE;
        Interval = interval;
        TotalSteps = 255;
        Index = 0;
        Direction = dir;
    }
*/
/*Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<500; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
*/
//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
  
