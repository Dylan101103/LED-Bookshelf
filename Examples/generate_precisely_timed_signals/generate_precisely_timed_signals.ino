//https://wp.josh.com/2014/05/13/ws2812-neopixels-are-not-so-finicky-once-you-get-to-know-them/

#define PIXELS 96*11 // Number of pixels in the string
 
// These values depend on which pin your string is connected to and what board you are using
// More info on how to find these at http://www.arduino.cc/en/Reference/PortManipulation
 
// These values are for digital pin 8 on an Arduino Yun or digital pin 12 on a DueMilinove
// Note that you could also include the DigitalWriteFast header file to not need to to this lookup.
 
#define PIXEL_PORT PORTB // Port of the pin the pixels are connected to
#define PIXEL_DDR DDRB // Port of the pin the pixels are connected to
#define PIXEL_BIT 4 // Bit of the pin the pixels are connected to
 
// These are the timing constraints taken mostly from the WS2812 datasheets
// These are chosen to be conservative and avoid problems rather than for maximum throughput 
 
#define T1H  900    // Width of a 1 bit in ns
#define T1L  600    // Width of a 1 bit in ns
 
#define T0H  400    // Width of a 0 bit in ns
#define T0L  900    // Width of a 0 bit in ns
 
#define RES 7000    // Width of the low gap between bits to cause a frame to latch
 
// Here are some convenience defines for using nanoseconds specs to generate actual CPU delays
 
#define NS_PER_SEC (1000000000L) // Note that this has to be SIGNED since we want to be able to check for negative values of derivatives
 
#define CYCLES_PER_SEC (F_CPU)
 
#define NS_PER_CYCLE ( NS_PER_SEC / CYCLES_PER_SEC )
 
#define NS_TO_CYCLES(n) ( (n) / NS_PER_CYCLE )
 
#define DELAY_CYCLES(n) ( ((n)&gt;0) ? __builtin_avr_delay_cycles( n ) : __builtin_avr_delay_cycles( 0 ) ) // Make sure we never have a delay less than zero
 
// Actually send a bit to the string. We turn off optimizations to make sure the compile does
// not reorder things and make it so the delay happens in the wrong place.
 
void sendBit(bool) __attribute__ ((optimize(0)));
 
void sendBit( bool bitVal ) {
 
    if ( bitVal ) {      // 1-bit
 
      bitSet( PIXEL_PORT , PIXEL_BIT );
 
      DELAY_CYCLES( NS_TO_CYCLES( T1H ) - 2 ); // 1-bit width less overhead for the actual bit setting
                                                     // Note that this delay could be longer and everything would still work
      bitClear( PIXEL_PORT , PIXEL_BIT );
 
      DELAY_CYCLES( NS_TO_CYCLES( T1L ) - 10 ); // 1-bit gap less the overhead of the loop
 
    } else {             // 0-bit
 
      cli();                                       // We need to protect this bit from being made wider by an interrupt 
 
      bitSet( PIXEL_PORT , PIXEL_BIT );
 
      DELAY_CYCLES( NS_TO_CYCLES( T0H ) - 2 ); // 0-bit width less overhead
                                                    // **************************************************************************
                                                    // This line is really the only tight goldilocks timing in the whole program!
                                                    // **************************************************************************
      bitClear( PIXEL_PORT , PIXEL_BIT );
 
      sei();
 
      DELAY_CYCLES( NS_TO_CYCLES( T0L ) - 10 ); // 0-bit gap less overhead of the loop
 
    }
 
    // Note that the inter-bit gap can be as long as you want as long as it doesn't exceed the 5us reset timeout (which is A long time)
    // Here I have been generous and not tried to squeeze the gap tight but instead erred on the side of lots of extra time.
    // This has thenice side effect of avoid glitches on very long strings becuase
 
}
 
void sendByte( unsigned char byte ) {
 
    for( unsigned char bit = 0 ; bit &lt; 8 ; bit++ ) {
 
      sendBit( bitRead( byte , 7 ) ); // Neopixel wants bit in highest-to-lowest order
                                                     // so send highest bit (bit #7 in an 8-bit byte since they start at 0)
      byte &lt;&lt;= 1; // and then shift left so bit 6 moves into 7, 5 moves into 6, etc
 
    }
}
 
/*
 
The following three functions are the public API:
  ledSetup() - set up the pin that is connected to the string. Call once at the beginning of the program.
  sendPixel( r , g , b ) - send a single pixel to the string. Call this once for each pixel in a frame.
  show() - latch the recently sent pixels on the LEDs . Call once per frame.
*/
 
// Set the specified pin up as digital out
 
void ledsetup() {
 
  bitSet( PIXEL_DDR , PIXEL_BIT );
 
}
 
void sendPixel( unsigned char r, unsigned char g , unsigned char b ) {
 
  sendByte(g); // Neopixel wants colors in green-then-red-then-blue order
  sendByte(r);
  sendByte(b);
 
}
 
// Just wait long enough without sending any bots to cause the pixels to latch and display the last sent frame
 
void show() {
    DELAY_CYCLES( NS_TO_CYCLES(RES) );
}
