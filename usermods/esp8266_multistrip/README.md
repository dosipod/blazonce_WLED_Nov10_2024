# esp8266_multistrip

This usermod enables up to 4 data pins to be used from an esp8266 module to drive separate LED strands. This only works with one-wire LEDs like the WS2812.

The esp8266 UART or DMA hardware is used for data output on pins 2 & 3, bitbangged for others. See here for hardware driver implementation details: https://github.com/Makuna/NeoPixelBus/wiki/ESP32-NeoMethods#neoesp32rmt-methods

Pass the following variables to the compiler as build flags:

 - `ESP8266_MULTISTRIP`
   - Define this to use usermod NpbWrapper.h instead of default one in WLED.
 - `NUM_STRIPS`
   - Number of strips in use
 - `PIXEL_COUNTS`
   - List of pixel counts in each strip
 - `LEDPIN0`
   - GPIO pin for first strip.
 - `LEDPIN1`
   - GPIO pin for second strip (if NUM_STRIPS>1).
 - `LEDPIN2`
   - GPIO pin for third strip (if NUM_STRIPS>2).
 - `LEDPIN3`
   - GPIO pin for fourth strip (if NUM_STRIPS>3).

From the perspective of WLED software, the LEDs are addressed as one long strand. The modified NbpWrapper.h file addresses the appropriate strand from the overall LED index based on the number of LEDs defined in each strand.

See `platformio_override.ini` for example configuration.

Tested on low cost Wemos D1 mini dev boards from Aliexpress or Amazon.

## TODO:

Implement software definition for pixel counts.