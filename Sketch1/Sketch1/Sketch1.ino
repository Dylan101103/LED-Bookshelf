/*
 Name:		Sketch1.ino
 Created:	06-Jan-18 21:47:42
 Author:	dylan
*/

#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_NeoPixel.h>

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

#define MODE_PIN     2    // Digital IO pin connected to the button.  This will be  "Blue Button"

#define TIME_PIN     4     //Digital IO pin connected to the button for time selction  "Green Button"

#define PIXEL_PIN    3    // Digital IO pin connected to the NeoPixels data pin.

#define PIXEL_COUNT 120   // Total attached pixels

#define BUTTON_LED   5   // Digital IO pin connected to the button leds pin.

// Pattern types supported: Removed  THEATER_CHASE,
enum  pattern { NONE, RAINBOW_CYCLE, COLOR_WIPE, SCANNER, FADE };
// Patern directions supported:
enum  direction { FORWARD, REVERSE };

// NeoPattern Class - derived from the Adafruit_NeoPixel class
class NeoPatterns : public Adafruit_NeoPixel
{
public:

	// Member Variables:
	pattern  ActivePattern;  // which pattern is running
	direction Direction;     // direction to run the pattern

	unsigned long Interval;   // milliseconds between updates
	unsigned long lastUpdate; // last update of position

	uint64_t Color1, Color2;  // What colors are in use
	uint16_t TotalSteps;  // total number of steps in the pattern
	uint16_t Index;  // current step within the pattern

	void(*OnComplete)();  // Callback on completion of pattern

						  // Constructor - calls base-class constructor to initialize strip
	NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void(*callback)())
		:Adafruit_NeoPixel(pixels, pin, type)
	{
		OnComplete = callback;
	}

	// Update the pattern
	void Update()
	{
		if ((millis() - lastUpdate) > Interval) // time to update
		{
			lastUpdate = millis();
			switch (ActivePattern)
			{
			case RAINBOW_CYCLE:
				RainbowCycleUpdate();
				break;
			//case THEATER_CHASE:
			//	TheaterChaseUpdate();
			//	break;
			case COLOR_WIPE:
				ColorWipeUpdate();
				break;
			case SCANNER:
				ScannerUpdate();
				break;
			case FADE:
				FadeUpdate();
				break;
			default:
				break;
			}
		}
	}

	// Increment the Index and reset at the end
	void Increment()
	{
		if (Direction == FORWARD)
		{
			Index++;
			if (Index >= TotalSteps)
			{
				Index = 0;
				if (OnComplete != NULL)
				{
					OnComplete(); // call the comlpetion callback
				}
			}
		}
		else // Direction == REVERSE
		{
			--Index;
			if (Index <= 0)
			{
				Index = TotalSteps - 1;
				if (OnComplete != NULL)
				{
					OnComplete(); // call the comlpetion callback
				}
			}
		}
	}

	// Reverse pattern direction
	void Reverse()
	{
		if (Direction == FORWARD)
		{
			Direction = REVERSE;
			Index = TotalSteps - 1;
		}
		else
		{
			Direction = FORWARD;
			Index = 0;
		}
	}

	// Initialize for a RainbowCycle
	void RainbowCycle(uint8_t interval, direction dir = FORWARD)
	{
		ActivePattern = RAINBOW_CYCLE;
		Interval = interval;
		TotalSteps = 255;
		Index = 0;
		Direction = dir;
	}

	// Update the Rainbow Cycle Pattern
	void RainbowCycleUpdate()
	{
		for (int i = 0; i < numPixels(); i++)
		{
			setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
		}
		show();
		Increment();
	}

	// Initialize for a Theater Chase
	//void TheaterChase(uint32_t color1, uint32_t color2, uint8_t interval, direction dir = FORWARD)
	//{
	//	ActivePattern = THEATER_CHASE;
	//	Interval = interval;
	//	TotalSteps = numPixels();
	//	Color1 = color1;
	//	Color2 = color2;
	//	Index = 0;
	//	Direction = dir;
	//}

	//// Update the Theater Chase Pattern
	//void TheaterChaseUpdate()
	//{
	//	for (int i = 0; i < numPixels(); i++)
	//	{
	//		if ((i + Index) % 3 == 0)
	//		{
	//			setPixelColor(i, Color1);
	//		}
	//		else
	//		{
	//			setPixelColor(i, Color2);
	//		}
	//	}
	//	show();
	//	Increment();
	//}

	// Initialize for a ColorWipe
	void ColorWipe(uint32_t color, uint8_t interval, direction dir = FORWARD)
	{
		ActivePattern = COLOR_WIPE;
		Interval = interval;
		TotalSteps = numPixels();
		Color1 = color;
		Index = 0;
		Direction = dir;
	}

	// Update the Color Wipe Pattern
	void ColorWipeUpdate()
	{
		setPixelColor(Index, Color1);
		show();
		Increment();
	}

	// Initialize for a SCANNNER
	void Scanner(uint32_t color1, uint8_t interval)
	{
		ActivePattern = SCANNER;
		Interval = interval;
		TotalSteps = (numPixels() - 1) * 2;
		Color1 = color1;
		Index = 0;
	}

	// Update the Scanner Pattern
	void ScannerUpdate()
	{
		for (int i = 0; i < numPixels(); i++)
		{
			if (i == Index)  // Scan Pixel to the right
			{
				setPixelColor(i, Color1);
			}
			else if (i == TotalSteps - Index) // Scan Pixel to the left
			{
				setPixelColor(i, Color1);
			}
			else // Fading tail
			{
				setPixelColor(i, DimColor(getPixelColor(i)));
			}
		}
		show();
		Increment();
	}

	// Initialize for a Fade
	void Fade(uint32_t color1, uint32_t color2, uint16_t steps, uint8_t interval, direction dir = FORWARD)
	{
		ActivePattern = FADE;
		Interval = interval;
		TotalSteps = steps;
		Color1 = color1;
		Color2 = color2;
		Index = 0;
		Direction = dir;
	}

	// Update the Fade Pattern
	void FadeUpdate()
	{
		// Calculate linear interpolation between Color1 and Color2
		// Optimise order of operations to minimize truncation error
		uint8_t red = ((Red(Color1) * (TotalSteps - Index)) + (Red(Color2) * Index)) / TotalSteps;
		uint8_t green = ((Green(Color1) * (TotalSteps - Index)) + (Green(Color2) * Index)) / TotalSteps;
		uint8_t blue = ((Blue(Color1) * (TotalSteps - Index)) + (Blue(Color2) * Index)) / TotalSteps;

		ColorSet(Color(red, green, blue));
		show();
		Increment();
	}

	// Calculate 50% dimmed version of a color (used by ScannerUpdate)
	uint32_t DimColor(uint32_t color)
	{
		// Shift R, G and B components one bit to the right
		uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
		return dimColor;
	}

	// Set all pixels to a color (synchronously)
	void ColorSet(uint32_t color)
	{
		for (int i = 0; i < numPixels(); i++)
		{
			setPixelColor(i, color);
		}
		show();
	}

	// Returns the Red component of a 32-bit color
	uint8_t Red(uint32_t color)
	{
		return (color >> 16) & 0xFF;
	}

	// Returns the Green component of a 32-bit color
	uint8_t Green(uint32_t color)
	{
		return (color >> 8) & 0xFF;
	}

	// Returns the Blue component of a 32-bit color
	uint8_t Blue(uint32_t color)
	{
		return color & 0xFF;
	}

	// Input a value 0 to 255 to get a color value.
	// The colours are a transition r - g - b - back to r.
	uint32_t Wheel(byte WheelPos)
	{
		WheelPos = 255 - WheelPos;
		if (WheelPos < 85)
		{
			return Color(255 - WheelPos * 3, 0, WheelPos * 3);
		}
		else if (WheelPos < 170)
		{
			WheelPos -= 85;
			return Color(0, WheelPos * 3, 255 - WheelPos * 3);
		}
		else
		{
			WheelPos -= 170;
			return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
		}
	}
};

void Strip1Complete();

// Define some NeoPatterns for the two rings and the stick
//  as well as some completion routines
NeoPatterns Strip1(PIXEL_COUNT, PIXEL_PIN, NEO_GRBW + NEO_KHZ800, &Strip1Complete);

uint32_t timeOff = 0;
uint32_t off = Strip1.Color(0, 0, 0, 0);
uint64_t full = Strip1.Color(255, 255, 255, 255);

uint32_t settime(uint32_t time, DateTime now)
{
	uint32_t timeunix = now.unixtime();
	timeunix = timeunix + time;
	return timeunix;

}

//Initializes and declairs the varables needed in loop()
int pressed1 = 0;
int pressed2 = 0;

void lightsOut() 
{
	pressed1 = 0;
	pressed2 = 0;
	Strip1.ActivePattern = NONE;
	for (int i = 0; i < PIXEL_COUNT; i++)
	{
		Strip1.setPixelColor(i, off);
		Strip1.show();
	}
}

// Initialize everything and prepare to start
void setup()
{
	Serial.begin(9600);

	if (!rtc.begin()) 
	{
		Serial.println("Couldn't find RTC");
		while (1);
	}

	if (rtc.lostPower()) 
	{
		Serial.println("RTC lost power, lets set the time!");
		// following line sets the RTC to the date & time this sketch was compiled
		rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
		// This line sets the RTC with an explicit date & time, for example to set
		// January 21, 2014 at 3am you would call:
		// rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
	}

	pinMode(MODE_PIN, INPUT_PULLUP);
	pinMode(TIME_PIN, INPUT_PULLUP);
	pinMode(BUTTON_LED, OUTPUT);
	digitalWrite(BUTTON_LED, HIGH);

	// Initialize all the pixelStrips
	Strip1.begin();
	//Strip1.show();

	// Kick off a pattern
	Strip1.RainbowCycle(3);
	Strip1.Interval = 50;
	//Ring2.RainbowCycle(3);
	// Ring2.Color1 = Ring1.Color1;
	// Stick.Scanner(Ring1.Color(255,0,0), 55);
	//Initializes and declairs the varables needed in loop()
}

// Main loop
void loop() 
{
	DateTime now = rtc.now();

	// Update the rings.
	Strip1.Update();
	// Ring2.Update();
	//digitalWrite(BUTTON_LED, HIGH);

	if (now.unixtime() == timeOff)
	{
		lightsOut();
	}

	// Turn off Led Button Lights
	//if (now.hour() == )

	// Switch patterns on a button press:
	if (digitalRead(MODE_PIN) == LOW) // Button #1 pressed
	{
		Strip1.ActivePattern = COLOR_WIPE;
		//Strip1.ColorWipe();
		Strip1.Color1 = (Strip1.Color(0, 0, 0, 255));
		//Strip1.Interval = 50;

		//Strip1.ActivePattern = SCANNER;
		////Strip1.ColorWipe();
		//Strip1.Color1 = (Strip1.Color(0, 0, 0, 255));
		//Strip1.Interval = 50;

		//Strip1.ColorSet(Strip1.Color(0, 0, 0, 255));
		digitalWrite(BUTTON_LED, LOW);
	}
	else if (digitalRead(TIME_PIN) == LOW) // Button #2 pressed
	{
		++pressed2;
		switch (pressed2)
		{
		case 1:
			timeOff = settime(25, now);  //1 minute
			delay(15);
			break;
		case 2:
			timeOff = settime(30, now);   //5 minutes
			delay(15);
			break;
		case 3:
			timeOff = settime(35, now);   //10 minutes
			delay(15);
			break;
		case 4:
			timeOff = settime(40, now);  // 1 hour
			delay(15);
			break;
		}
		delay(20);
		Serial.print(pressed2);
		digitalWrite(BUTTON_LED, LOW);
		// Switch to alternating color wipes on Rings1 and 2
		//Strip1.ActivePattern = COLOR_WIPE;
		//Strip1.setBrightness(64);
		// Ring2.ActivePattern = COLOR_WIPE;
		// Ring2.TotalSteps = Ring2.numPixels();
		// And update tbe stick
		// Stick.Update();
	}
	else // Back to normal operation
	{
		digitalWrite(BUTTON_LED, HIGH);
		//reset event
		/*if (now.unixtime() == timeOff)
		{
			lightsOut():
		}
		else

		{*/
		//Strip1.clear();

			// Restore all pattern parameters to normal values
			//Strip1.ActivePattern = RAINBOW_CYCLE;
			//Strip1.Color1 = 255, 255, 255, 0;
			//Strip1.Interval = 65;
			// Ring2.ActivePattern = RAINBOW_CYCLE;
			// Ring2.TotalSteps = 255;
			// Ring2.Interval = min(10, Ring2.Interval);
			// And update tbe stick
			// Stick.Update();
		//}
	}
}

//------------------------------------------------------------
//Completion Routines - get called on completion of a pattern
//------------------------------------------------------------

// Strip1 Completion Callback
void Strip1Complete()
{
	if (digitalRead(TIME_PIN) == LOW)  // Button #2 pressed
	{
		// Alternate color-wipe patterns with Ring2
		// Ring2.Interval = 40;
		//Strip1.Color1 = Strip1.Wheel(random(255));
		//Strip1.Interval = 20000;
	}
	else  // Retern to normal
	{
		Strip1.Reverse();
	}
}



//uint32_t red = strip.Color(255, 0, 0, 0);
//uint32_t orange = strip.Color(255, 165, 0, 0);
//uint32_t yellow = strip.Color(255, 255, 0, 0);
//uint32_t green = strip.Color(0, 128, 0, 0);
//uint32_t blue = strip.Color(0, 0, 255, 0);
//uint32_t purple = strip.Color(128, 0, 128, 0);
//
//red;
//strip.show();

// Red 255, 0, 0, 0
// Orange 255, 165, 0, 0
// Yellow 255, 255, 0, 0
// Green 0, 128, 0, 0
// Blue 0, 0, 255, 0
// Purple 128, 0, 128, 0
