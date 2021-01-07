//this code is a modified version of https://github.com/Makuna/NeoPixelBus/issues/103
#ifndef NpbWrapper_h
#define NpbWrapper_h

//PIN CONFIGURATION
#ifndef LEDPIN
#define LEDPIN 1  // Legacy pin def required by some other portions of code. This pin is not used do drive LEDs.
//  #define LEDPIN strip.getStripPin(0)  // Legacy pin def required by some other portions of code.
#endif
//#define USE_APA102  // Uncomment for using APA102 LEDs.
//#define USE_WS2801  // Uncomment for using WS2801 LEDs (make sure you have NeoPixelBus v2.5.6 or newer)
//#define USE_LPD8806 // Uncomment for using LPD8806
//#define USE_TM1814  // Uncomment for using TM1814 LEDs (make sure you have NeoPixelBus v2.5.7 or newer)
//#define USE_P9813   // Uncomment for using P9813 LEDs (make sure you have NeoPixelBus v2.5.8 or newer)
//#define WLED_USE_ANALOG_LEDS //Uncomment for using "dumb" PWM controlled LEDs (see pins below, default R: gpio5, G: 12, B: 15, W: 13)
//#define WLED_USE_H801 //H801 controller. Please uncomment #define WLED_USE_ANALOG_LEDS as well
//#define WLED_USE_5CH_LEDS  //5 Channel H801 for cold and warm white
//#define WLED_USE_BWLT11
//#define WLED_USE_SHOJO_PCB

#ifndef BTNPIN
#define BTNPIN  -1  //button pin. Needs to have pullup (gpio0 recommended)
#endif

#ifndef TOUCHPIN
//#define TOUCHPIN T0 //touch pin. Behaves the same as button. ESP32 only.
#endif

#ifndef IRPIN
#define IRPIN -1  //infrared pin (-1 to disable)  MagicHome: 4, H801 Wifi: 0
#endif

#ifndef RLYPIN
#define RLYPIN -1  //pin for relay, will be set HIGH if LEDs are on (-1 to disable). Also usable for standby leds, triggers,...
#endif

#ifndef AUXPIN
#define AUXPIN -1  //debug auxiliary output pin (-1 to disable)
#endif

#ifndef RLYMDE
#define RLYMDE 1  //mode for relay, 0: LOW if LEDs are on 1: HIGH if LEDs are on
#endif

//enable color order override for a specific range of the strip
//This can be useful if you want to chain multiple strings with incompatible color order
//#define COLOR_ORDER_OVERRIDE
#define COO_MIN    0
#define COO_MAX   35 //not inclusive, this would set the override for LEDs 0-26
#define COO_ORDER COL_ORDER_GRB

//END CONFIGURATION

#ifdef WLED_USE_ANALOG_LEDS
  //PWM pins - PINs 15,13,12,14 (W2 = 04)are used with H801 Wifi LED Controller
  #ifdef WLED_USE_H801
    #define RPIN 15   //R pin for analog LED strip   
    #define GPIN 13   //G pin for analog LED strip
    #define BPIN 12   //B pin for analog LED strip
    #define WPIN 14   //W pin for analog LED strip 
    #define W2PIN 04  //W2 pin for analog LED strip
    #undef BTNPIN
    #undef IRPIN
    #define IRPIN  0 //infrared pin (-1 to disable)  MagicHome: 4, H801 Wifi: 0
  #elif defined(WLED_USE_BWLT11)
  //PWM pins - to use with BW-LT11
    #define RPIN 12  //R pin for analog LED strip
    #define GPIN 4   //G pin for analog LED strip
    #define BPIN 14  //B pin for analog LED strip
    #define WPIN 5   //W pin for analog LED strip
  #elif defined(WLED_USE_SHOJO_PCB)
  //PWM pins - to use with Shojo PCB (https://www.bastelbunker.de/esp-rgbww-wifi-led-controller-vbs-edition/)
    #define RPIN 14  //R pin for analog LED strip
    #define GPIN 4   //G pin for analog LED strip
    #define BPIN 5   //B pin for analog LED strip
    #define WPIN 15  //W pin for analog LED strip
    #define W2PIN 12 //W2 pin for analog LED strip
  #elif defined(WLED_USE_PLJAKOBS_PCB)
  // PWM pins - to use with esp_rgbww_controller from patrickjahns/pljakobs (https://github.com/pljakobs/esp_rgbww_controller)
    #define RPIN 12  //R pin for analog LED strip
    #define GPIN 13  //G pin for analog LED strip
    #define BPIN 14  //B pin for analog LED strip
    #define WPIN 4   //W pin for analog LED strip
    #define W2PIN 5  //W2 pin for analog LED strip
    #undef IRPIN
  #else
  //Enable override of Pins by using the platformio_override.ini file
  //PWM pins - PINs 5,12,13,15 are used with Magic Home LED Controller
    #ifndef RPIN
      #define RPIN 5   //R pin for analog LED strip
    #endif
    #ifndef GPIN
      #define GPIN 12  //G pin for analog LED strip
    #endif
    #ifndef BPIN
      #define BPIN 15  //B pin for analog LED strip
    #endif
    #ifndef WPIN
      #define WPIN 13  //W pin for analog LED strip
    #endif
  #endif
  #undef RLYPIN
  #define RLYPIN -1 //disable as pin 12 is used by analog LEDs
#endif

//automatically uses the right driver method for each platform
#ifdef ARDUINO_ARCH_ESP32
 #ifdef USE_APA102
  #define PIXELMETHOD DotStarMethod
 #elif defined(USE_WS2801)
  #define PIXELMETHOD NeoWs2801Method
 #elif defined(USE_LPD8806)
  #define PIXELMETHOD Lpd8806Method
 #elif defined(USE_TM1814)
  #define PIXELMETHOD NeoTm1814Method  
 #elif defined(USE_P9813)
  #define PIXELMETHOD P9813Method  
 #else
  #define PIXELMETHOD NeoEsp32Rmt0Ws2812xMethod
  #define NeoPixelBrightnessBusGrb0 NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>
  #define NeoPixelBrightnessBusGrb1 NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>
  #define NeoPixelBrightnessBusGrb2 NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp32Rmt2Ws2812xMethod>
  #define NeoPixelBrightnessBusGrb3 NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp32Rmt3Ws2812xMethod>
  #define NeoPixelBrightnessBusGrb4 NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp32Rmt4Ws2812xMethod>
  #define NeoPixelBrightnessBusGrb5 NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp32Rmt5Ws2812xMethod>
  #define NeoPixelBrightnessBusGrb6 NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp32Rmt6Ws2812xMethod>
  #define NeoPixelBrightnessBusGrb7 NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp32Rmt7Ws2812xMethod>
  #define NeoPixelBrightnessBusGrbw0 NeoPixelBrightnessBus<NeoGrbwFeature, NeoEsp32Rmt0Ws2812xMethod>
  #define NeoPixelBrightnessBusGrbw1 NeoPixelBrightnessBus<NeoGrbwFeature, NeoEsp32Rmt1Ws2812xMethod>
  #define NeoPixelBrightnessBusGrbw2 NeoPixelBrightnessBus<NeoGrbwFeature, NeoEsp32Rmt2Ws2812xMethod>
  #define NeoPixelBrightnessBusGrbw3 NeoPixelBrightnessBus<NeoGrbwFeature, NeoEsp32Rmt3Ws2812xMethod>
  #define NeoPixelBrightnessBusGrbw4 NeoPixelBrightnessBus<NeoGrbwFeature, NeoEsp32Rmt4Ws2812xMethod>
  #define NeoPixelBrightnessBusGrbw5 NeoPixelBrightnessBus<NeoGrbwFeature, NeoEsp32Rmt5Ws2812xMethod>
  #define NeoPixelBrightnessBusGrbw6 NeoPixelBrightnessBus<NeoGrbwFeature, NeoEsp32Rmt6Ws2812xMethod>
  #define NeoPixelBrightnessBusGrbw7 NeoPixelBrightnessBus<NeoGrbwFeature, NeoEsp32Rmt7Ws2812xMethod>
 #endif
#else //esp8266
 //autoselect the right method depending on strip pin
 #ifdef USE_APA102
  #define PIXELMETHOD DotStarMethod
 #elif defined(USE_WS2801)
  #define PIXELMETHOD NeoWs2801Method
 #elif defined(USE_LPD8806)
  #define PIXELMETHOD Lpd8806Method
 #elif defined(USE_TM1814)
  #define PIXELMETHOD NeoTm1814Method  
 #elif defined(USE_P9813)
  #define PIXELMETHOD P9813Method  
 #else
  #define PIXELMETHOD NeoEsp8266BitBang800KbpsMethod
//  #pragma message "Software BitBang will be used because of your selected LED pin. This may cause flicker. Use GPIO 2 or 3 for best results."
  #define NeoPixelBrightnessBusGrb0 NeoPixelBrightnessBus<NeoGrbFeature,NeoEsp8266BitBang800KbpsMethod>
  #define NeoPixelBrightnessBusGrb2 NeoPixelBrightnessBus<NeoGrbFeature,NeoEsp8266Uart1Ws2813Method>
  #define NeoPixelBrightnessBusGrb3 NeoPixelBrightnessBus<NeoGrbFeature,NeoEsp8266Dma800KbpsMethod>
  #define NeoPixelBrightnessBusGrbw0 NeoPixelBrightnessBus<NeoGrbwFeature,NeoEsp8266BitBang800KbpsMethod>
  #define NeoPixelBrightnessBusGrbw2 NeoPixelBrightnessBus<NeoGrbwFeature,NeoEsp8266Uart1Ws2813Method>
  #define NeoPixelBrightnessBusGrbw3 NeoPixelBrightnessBus<NeoGrbwFeature,NeoEsp8266Dma800KbpsMethod>
 #endif
#endif


//you can now change the color order in the web settings
#ifdef USE_APA102
 #define PIXELFEATURE3 DotStarBgrFeature
 #define PIXELFEATURE4 DotStarLbgrFeature
#elif defined(USE_LPD8806)
 #define PIXELFEATURE3 Lpd8806GrbFeature 
 #define PIXELFEATURE4 Lpd8806GrbFeature 
#elif defined(USE_WS2801)
 #define PIXELFEATURE3 NeoRbgFeature
 #define PIXELFEATURE4 NeoRbgFeature
#elif defined(USE_TM1814)
  #define PIXELFEATURE3 NeoWrgbTm1814Feature
  #define PIXELFEATURE4 NeoWrgbTm1814Feature
#elif defined(USE_P9813)
 #define PIXELFEATURE3 P9813BgrFeature 
 #define PIXELFEATURE4 NeoGrbwFeature   
#else
 #define PIXELFEATURE3 NeoGrbFeature
 #define PIXELFEATURE4 NeoGrbwFeature
#endif


#include <NeoPixelBrightnessBus.h>
#include "const.h"

enum NeoPixelType
{
  NeoPixelType_None = 0,
  NeoPixelType_Grb  = 1,
  NeoPixelType_Grbw = 2,
  NeoPixelType_End  = 3
};

class NeoPixelWrapper
{
public:
  NeoPixelWrapper() :
    _type(NeoPixelType_None),
    pixelStrips(1)
  {
    for (uint8_t i=0; i < MAX_NUMBER_OF_STRIPS; i++ )
    {
      _pGRB[i] = NULL;
      _pGRBW[i] = NULL;
    }
  }

  ~NeoPixelWrapper()
  {
    cleanup();
  }

  void initStrips(uint8_t numStrips, int8_t *stripPin, int8_t *stripPinClk, uint16_t *stripLen)
  {
    uint16_t totalPixels = 0;
    pixelStrips = numStrips;
    pixelCounts = stripLen;
    pixelStripPins = stripPin;
    pixelStripPinsClk = stripPinClk;
    for (uint8_t idx = 0; idx < numStrips; idx++)
    {
      pixelStripStartIdx[idx] = totalPixels;
      totalPixels += pixelCounts[idx];
    }
  }

  void Begin(NeoPixelType type, uint16_t pixelCount)
  {
    cleanup();
    _type = type;

    switch (_type)
    {
      case NeoPixelType_Grb:
      {
      #if defined(USE_APA102) || defined(USE_WS2801) || defined(USE_LPD8806) || defined(USE_P9813)
        _pGRB[0] = new NeoPixelBrightnessBus<PIXELFEATURE3,PIXELMETHOD>(pixelCounts[0], pixelStripPinsClk[0], pixelStripPins[0]);
      #else
        for (uint8_t idx = 0; idx < pixelStrips; idx++)
        {
          switch (pixelStripPins[idx])
          {
          #ifdef ARDUINO_ARCH_ESP32
            case 0:
              _pGRB[idx] = (void*) new NeoPixelBrightnessBusGrb0(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrb0*)_pGRB[idx])->Begin(); break;
            case 1:
              _pGRB[idx] = (void*) new NeoPixelBrightnessBusGrb1(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrb1*)_pGRB[idx])->Begin(); break;
            case 2:
              _pGRB[idx] = (void*) new NeoPixelBrightnessBusGrb2(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrb2*)_pGRB[idx])->Begin(); break;
            case 3:
              _pGRB[idx] = (void*) new NeoPixelBrightnessBusGrb3(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrb3*)_pGRB[idx])->Begin(); break;
            case 4:
              _pGRB[idx] = (void*) new NeoPixelBrightnessBusGrb4(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrb4*)_pGRB[idx])->Begin(); break;
            case 5:
              _pGRB[idx] = (void*) new NeoPixelBrightnessBusGrb5(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrb5*)_pGRB[idx])->Begin(); break;
            case 6:
              _pGRB[idx] = (void*) new NeoPixelBrightnessBusGrb6(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrb6*)_pGRB[idx])->Begin(); break;
            case 7:
              _pGRB[idx] = (void*) new NeoPixelBrightnessBusGrb7(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrb7*)_pGRB[idx])->Begin(); break;
          #else
            case 2:
              _pGRB[idx] = (void*) new NeoPixelBrightnessBusGrb2(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrb2*)_pGRB[idx])->Begin(); break;
            case 3:
              _pGRB[idx] = (void*) new NeoPixelBrightnessBusGrb3(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrb3*)_pGRB[idx])->Begin(); break;
            default:
              _pGRB[idx] = (void*) new NeoPixelBrightnessBusGrb0(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrb0*)_pGRB[idx])->Begin(); break;
          #endif
          }
        }
        break;
      #endif
      }

      case NeoPixelType_Grbw:
      {
      #if defined(USE_APA102) || defined(USE_WS2801) || defined(USE_LPD8806) || defined(USE_P9813)
        _pGRBW[0] = new NeoPixelBrightnessBus<PIXELFEATURE4,PIXELMETHOD>(pixelCounts[0], pixelStripPinsClk[0], pixelStripPins[0]);
      #else
        for (uint8_t idx = 0; idx < pixelStrips; idx++)
        {
          #ifdef ARDUINO_ARCH_ESP32
          switch (idx)
          {
            case 0:
              _pGRBW[idx] = (void*) new NeoPixelBrightnessBusGrbw0(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrbw0*)_pGRBW[idx])->Begin(); break;
            case 1:
              _pGRBW[idx] = (void*) new NeoPixelBrightnessBusGrbw1(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrbw1*)_pGRBW[idx])->Begin(); break;
            case 2:
              _pGRBW[idx] = (void*) new NeoPixelBrightnessBusGrbw2(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrbw2*)_pGRBW[idx])->Begin(); break;
            case 3:
              _pGRBW[idx] = (void*) new NeoPixelBrightnessBusGrbw3(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrbw3*)_pGRBW[idx])->Begin(); break;
            case 4:
              _pGRBW[idx] = (void*) new NeoPixelBrightnessBusGrbw4(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrbw4*)_pGRBW[idx])->Begin(); break;
            case 5:
              _pGRBW[idx] = (void*) new NeoPixelBrightnessBusGrbw5(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrbw5*)_pGRBW[idx])->Begin(); break;
            case 6:
              _pGRBW[idx] = (void*) new NeoPixelBrightnessBusGrbw6(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrbw6*)_pGRBW[idx])->Begin(); break;
            case 7:
              _pGRBW[idx] = (void*) new NeoPixelBrightnessBusGrbw7(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrbw7*)_pGRBW[idx])->Begin(); break;
          }
          #else
          switch (pixelStripPins[idx])
          {
            case 2:
              _pGRBW[idx] = (void*) new NeoPixelBrightnessBusGrbw2(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrbw2*)_pGRBW[idx])->Begin(); break;
            case 3:
              _pGRBW[idx] = (void*) new NeoPixelBrightnessBusGrbw3(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrbw3*)_pGRBW[idx])->Begin(); break;
            default:
              _pGRBW[idx] = (void*) new NeoPixelBrightnessBusGrbw0(pixelCounts[idx], pixelStripPins[idx]); ((NeoPixelBrightnessBusGrbw0*)_pGRBW[idx])->Begin(); break;
          }
          #endif
        }
        break;
      #endif
      }
    }

    #ifdef WLED_USE_ANALOG_LEDS 
      #ifdef ARDUINO_ARCH_ESP32
        ledcSetup(0, 5000, 8);
        ledcAttachPin(RPIN, 0);
        ledcSetup(1, 5000, 8);
        ledcAttachPin(GPIN, 1);
        ledcSetup(2, 5000, 8);        
        ledcAttachPin(BPIN, 2);
        if(_type == NeoPixelType_Grbw) 
        {
          ledcSetup(3, 5000, 8);        
          ledcAttachPin(WPIN, 3);
          #ifdef WLED_USE_5CH_LEDS
            ledcSetup(4, 5000, 8);        
            ledcAttachPin(W2PIN, 4);
          #endif
        }
      #else  // ESP8266
        //init PWM pins
        pinMode(RPIN, OUTPUT);
        pinMode(GPIN, OUTPUT);
        pinMode(BPIN, OUTPUT); 
        if(_type == NeoPixelType_Grbw) 
        {
          pinMode(WPIN, OUTPUT); 
          #ifdef WLED_USE_5CH_LEDS
            pinMode(W2PIN, OUTPUT);
          #endif
        }
        analogWriteRange(255);  //same range as one RGB channel
        analogWriteFreq(880);   //PWM frequency proven as good for LEDs
      #endif 
    #endif
  }

#ifdef WLED_USE_ANALOG_LEDS      
  void SetRgbwPwm(uint8_t r, uint8_t g, uint8_t b, uint8_t w, uint8_t w2=0)
  {
    #ifdef ARDUINO_ARCH_ESP32
      ledcWrite(0, r);
      ledcWrite(1, g);
      ledcWrite(2, b);
      switch (_type) {
        case NeoPixelType_Grb:                                                  break;
        #ifdef WLED_USE_5CH_LEDS
          case NeoPixelType_Grbw: ledcWrite(3, w); ledcWrite(4, w2);            break;
        #else
          case NeoPixelType_Grbw: ledcWrite(3, w);                              break;
        #endif
      }        
    #else   // ESP8266
      analogWrite(RPIN, r);
      analogWrite(GPIN, g);
      analogWrite(BPIN, b);
      switch (_type) {
        case NeoPixelType_Grb:                                                  break;
        #ifdef WLED_USE_5CH_LEDS
          case NeoPixelType_Grbw: analogWrite(WPIN, w); analogWrite(W2PIN, w2); break;
        #else
          case NeoPixelType_Grbw: analogWrite(WPIN, w);                         break;
        #endif
      }
    #endif 
  }
#endif

  void Show()
  {
    switch (_type)
    {
      case NeoPixelType_Grb:
      {
        for (uint8_t idx = 0; idx < pixelStrips; idx++)
        {
          #ifdef ARDUINO_ARCH_ESP32
          switch (idx)
          {
            case 0: ((NeoPixelBrightnessBusGrb0*)_pGRB[idx])->Show(); break;
            case 1: ((NeoPixelBrightnessBusGrb1*)_pGRB[idx])->Show(); break;
            case 2: ((NeoPixelBrightnessBusGrb2*)_pGRB[idx])->Show(); break;
            case 3: ((NeoPixelBrightnessBusGrb3*)_pGRB[idx])->Show(); break;
            case 4: ((NeoPixelBrightnessBusGrb4*)_pGRB[idx])->Show(); break;
            case 5: ((NeoPixelBrightnessBusGrb5*)_pGRB[idx])->Show(); break;
            case 6: ((NeoPixelBrightnessBusGrb6*)_pGRB[idx])->Show(); break;
            case 7: ((NeoPixelBrightnessBusGrb7*)_pGRB[idx])->Show(); break;
          }
          #else
          switch (pixelStripPins[idx])
          {
            case 2:  ((NeoPixelBrightnessBusGrb2*)_pGRB[idx])->Show(); break;
            case 3:  ((NeoPixelBrightnessBusGrb3*)_pGRB[idx])->Show(); break;
            default: ((NeoPixelBrightnessBusGrb0*)_pGRB[idx])->Show(); break;
          }
          #endif
        }
        break;
      }
      case NeoPixelType_Grbw:
      {
        for (uint8_t idx = 0; idx < pixelStrips; idx++)
        {
          #ifdef ARDUINO_ARCH_ESP32
          switch (idx)
          {
            case 0: ((NeoPixelBrightnessBusGrbw0*)_pGRBW[idx])->Show(); break;
            case 1: ((NeoPixelBrightnessBusGrbw1*)_pGRBW[idx])->Show(); break;
            case 2: ((NeoPixelBrightnessBusGrbw2*)_pGRBW[idx])->Show(); break;
            case 3: ((NeoPixelBrightnessBusGrbw3*)_pGRBW[idx])->Show(); break;
            case 4: ((NeoPixelBrightnessBusGrbw4*)_pGRBW[idx])->Show(); break;
            case 5: ((NeoPixelBrightnessBusGrbw5*)_pGRBW[idx])->Show(); break;
            case 6: ((NeoPixelBrightnessBusGrbw6*)_pGRBW[idx])->Show(); break;
            case 7: ((NeoPixelBrightnessBusGrbw7*)_pGRBW[idx])->Show(); break;
          }
          #else
          switch (pixelStripPins[idx])
          {
            case 2:  ((NeoPixelBrightnessBusGrbw2*)_pGRBW[idx])->Show(); break;
            case 3:  ((NeoPixelBrightnessBusGrbw3*)_pGRBW[idx])->Show(); break;
            default: ((NeoPixelBrightnessBusGrbw0*)_pGRBW[idx])->Show(); break;
          }
          #endif
        }
        break;
      }
    }
  }

  bool CanShow()
  {
    bool canShow = true;
    switch (_type)
    {
      case NeoPixelType_Grb:
      {
        for (uint8_t idx = 0; idx < pixelStrips; idx++)
        {
          #ifdef ARDUINO_ARCH_ESP32
          switch (idx)
          {
            case 0: canShow &= ((NeoPixelBrightnessBusGrb0*)_pGRB[idx])->CanShow(); break;
            case 1: canShow &= ((NeoPixelBrightnessBusGrb1*)_pGRB[idx])->CanShow(); break;
            case 2: canShow &= ((NeoPixelBrightnessBusGrb2*)_pGRB[idx])->CanShow(); break;
            case 3: canShow &= ((NeoPixelBrightnessBusGrb3*)_pGRB[idx])->CanShow(); break;
            case 4: canShow &= ((NeoPixelBrightnessBusGrb4*)_pGRB[idx])->CanShow(); break;
            case 5: canShow &= ((NeoPixelBrightnessBusGrb5*)_pGRB[idx])->CanShow(); break;
            case 6: canShow &= ((NeoPixelBrightnessBusGrb6*)_pGRB[idx])->CanShow(); break;
            case 7: canShow &= ((NeoPixelBrightnessBusGrb7*)_pGRB[idx])->CanShow(); break;
          }
          #else
          switch (pixelStripPins[idx])
          {
            case 2:  canShow &= ((NeoPixelBrightnessBusGrb2*)_pGRB[idx])->CanShow(); break;
            case 3:  canShow &= ((NeoPixelBrightnessBusGrb3*)_pGRB[idx])->CanShow(); break;
            default: canShow &= ((NeoPixelBrightnessBusGrb0*)_pGRB[idx])->CanShow(); break;
          }
          #endif
        }
        break;
      }
      case NeoPixelType_Grbw:
      {
        for (uint8_t idx = 0; idx < pixelStrips; idx++)
        {
          #ifdef ARDUINO_ARCH_ESP32
          switch (idx)
          {
            case 0: canShow &= ((NeoPixelBrightnessBusGrbw0*)_pGRBW[idx])->CanShow(); break;
            case 1: canShow &= ((NeoPixelBrightnessBusGrbw1*)_pGRBW[idx])->CanShow(); break;
            case 2: canShow &= ((NeoPixelBrightnessBusGrbw2*)_pGRBW[idx])->CanShow(); break;
            case 3: canShow &= ((NeoPixelBrightnessBusGrbw3*)_pGRBW[idx])->CanShow(); break;
            case 4: canShow &= ((NeoPixelBrightnessBusGrbw4*)_pGRBW[idx])->CanShow(); break;
            case 5: canShow &= ((NeoPixelBrightnessBusGrbw5*)_pGRBW[idx])->CanShow(); break;
            case 6: canShow &= ((NeoPixelBrightnessBusGrbw6*)_pGRBW[idx])->CanShow(); break;
            case 7: canShow &= ((NeoPixelBrightnessBusGrbw7*)_pGRBW[idx])->CanShow(); break;
          }
          #else
          switch (pixelStripPins[idx])
          {
            case 2:  canShow &= ((NeoPixelBrightnessBusGrbw2*)_pGRBW[idx])->CanShow(); break;
            case 3:  canShow &= ((NeoPixelBrightnessBusGrbw3*)_pGRBW[idx])->CanShow(); break;
            default: canShow &= ((NeoPixelBrightnessBusGrbw0*)_pGRBW[idx])->CanShow(); break;
          }
          #endif
        }
        break;
      }
    }
    return canShow;
  }

  void SetPixelColorRaw(uint16_t indexPixel, RgbwColor c)
  {
    // figure out which strip this pixel index is on
    uint8_t stripIdx = 0;
    for (uint8_t idx = 0; idx < pixelStrips; idx++)
    {
      if (indexPixel >= pixelStripStartIdx[idx])
      {
        stripIdx = idx;
      }
      else
      {
        break;
      }
    }
    // subtract strip start index so we're addressing just this strip instead of all pixels on all strips
    indexPixel -= pixelStripStartIdx[stripIdx];
    RgbColor rgb = RgbColor(c.R, c.G, c.B);
    switch (_type)
    {
      case NeoPixelType_Grb:
      {
        #ifdef ARDUINO_ARCH_ESP32
        switch (stripIdx)
        {
          case 0: ((NeoPixelBrightnessBusGrb0*)_pGRB[stripIdx])->SetPixelColor(indexPixel, rgb); break;
          case 1: ((NeoPixelBrightnessBusGrb1*)_pGRB[stripIdx])->SetPixelColor(indexPixel, rgb); break;
          case 2: ((NeoPixelBrightnessBusGrb2*)_pGRB[stripIdx])->SetPixelColor(indexPixel, rgb); break;
          case 3: ((NeoPixelBrightnessBusGrb3*)_pGRB[stripIdx])->SetPixelColor(indexPixel, rgb); break;
          case 4: ((NeoPixelBrightnessBusGrb4*)_pGRB[stripIdx])->SetPixelColor(indexPixel, rgb); break;
          case 5: ((NeoPixelBrightnessBusGrb5*)_pGRB[stripIdx])->SetPixelColor(indexPixel, rgb); break;
          case 6: ((NeoPixelBrightnessBusGrb6*)_pGRB[stripIdx])->SetPixelColor(indexPixel, rgb); break;
          case 7: ((NeoPixelBrightnessBusGrb7*)_pGRB[stripIdx])->SetPixelColor(indexPixel, rgb); break;
        }
        #else
        switch (pixelStripPins[stripIdx])
        {
          case 2:  ((NeoPixelBrightnessBusGrb2*)_pGRB[stripIdx])->SetPixelColor(indexPixel, rgb); break;
          case 3:  ((NeoPixelBrightnessBusGrb3*)_pGRB[stripIdx])->SetPixelColor(indexPixel, rgb); break;
          default: ((NeoPixelBrightnessBusGrb0*)_pGRB[stripIdx])->SetPixelColor(indexPixel, rgb); break;
        }
        #endif
        break;
      }
      case NeoPixelType_Grbw:
      {
        #if defined(USE_LPD8806) || defined(USE_WS2801)
          #define COL rgb
        #else
          #define COL c
        #endif
        #ifdef ARDUINO_ARCH_ESP32
        switch (stripIdx)
        {
          case 0: ((NeoPixelBrightnessBusGrbw0*)_pGRBW[stripIdx])->SetPixelColor(indexPixel, COL); break;
          case 1: ((NeoPixelBrightnessBusGrbw1*)_pGRBW[stripIdx])->SetPixelColor(indexPixel, COL); break;
          case 2: ((NeoPixelBrightnessBusGrbw2*)_pGRBW[stripIdx])->SetPixelColor(indexPixel, COL); break;
          case 3: ((NeoPixelBrightnessBusGrbw3*)_pGRBW[stripIdx])->SetPixelColor(indexPixel, COL); break;
          case 4: ((NeoPixelBrightnessBusGrbw4*)_pGRBW[stripIdx])->SetPixelColor(indexPixel, COL); break;
          case 5: ((NeoPixelBrightnessBusGrbw5*)_pGRBW[stripIdx])->SetPixelColor(indexPixel, COL); break;
          case 6: ((NeoPixelBrightnessBusGrbw6*)_pGRBW[stripIdx])->SetPixelColor(indexPixel, COL); break;
          case 7: ((NeoPixelBrightnessBusGrbw7*)_pGRBW[stripIdx])->SetPixelColor(indexPixel, COL); break;
        }
        #else
        switch (pixelStripPins[stripIdx])
        {
          case 2:  ((NeoPixelBrightnessBusGrbw2*)_pGRBW[stripIdx])->SetPixelColor(indexPixel, COL); break;
          case 3:  ((NeoPixelBrightnessBusGrbw3*)_pGRBW[stripIdx])->SetPixelColor(indexPixel, COL); break;
          default: ((NeoPixelBrightnessBusGrbw0*)_pGRBW[stripIdx])->SetPixelColor(indexPixel, COL); break;
        }
        #endif
        break;
      }
    }
  }

  void SetPixelColor(uint16_t indexPixel, RgbwColor c)
  {
    /*
    Set pixel color with necessary color order conversion.
    */

    RgbwColor col;

    uint8_t co = _colorOrder;
    #ifdef COLOR_ORDER_OVERRIDE
    if (indexPixel >= COO_MIN && indexPixel < COO_MAX) co = COO_ORDER;
    #endif

    //reorder channels to selected order
    switch (co)
    {
      case  0: col.G = c.G; col.R = c.R; col.B = c.B; break; //0 = GRB, default
      case  1: col.G = c.R; col.R = c.G; col.B = c.B; break; //1 = RGB, common for WS2811
      case  2: col.G = c.B; col.R = c.R; col.B = c.G; break; //2 = BRG
      case  3: col.G = c.R; col.R = c.B; col.B = c.G; break; //3 = RBG
      case  4: col.G = c.B; col.R = c.G; col.B = c.R; break; //4 = BGR
      default: col.G = c.G; col.R = c.B; col.B = c.R; break; //5 = GBR
    }
    col.W = c.W;

    SetPixelColorRaw(indexPixel, col);
  }

  void SetBrightness(byte b)
  {
    switch (_type)
    {
      case NeoPixelType_Grb:
      {
        for (uint8_t idx = 0; idx < pixelStrips; idx++)
        {
          #ifdef ARDUINO_ARCH_ESP32
          switch (idx)
          {
            case 0: ((NeoPixelBrightnessBusGrb0*)_pGRB[idx])->SetBrightness(b); break;
            case 1: ((NeoPixelBrightnessBusGrb1*)_pGRB[idx])->SetBrightness(b); break;
            case 2: ((NeoPixelBrightnessBusGrb2*)_pGRB[idx])->SetBrightness(b); break;
            case 3: ((NeoPixelBrightnessBusGrb3*)_pGRB[idx])->SetBrightness(b); break;
            case 4: ((NeoPixelBrightnessBusGrb4*)_pGRB[idx])->SetBrightness(b); break;
            case 5: ((NeoPixelBrightnessBusGrb5*)_pGRB[idx])->SetBrightness(b); break;
            case 6: ((NeoPixelBrightnessBusGrb6*)_pGRB[idx])->SetBrightness(b); break;
            case 7: ((NeoPixelBrightnessBusGrb7*)_pGRB[idx])->SetBrightness(b); break;
          }
          #else
          switch (pixelStripPins[idx])
          {
            case 2:  ((NeoPixelBrightnessBusGrb2*)_pGRB[idx])->SetBrightness(b); break;
            case 3:  ((NeoPixelBrightnessBusGrb3*)_pGRB[idx])->SetBrightness(b); break;
            default: ((NeoPixelBrightnessBusGrb0*)_pGRB[idx])->SetBrightness(b); break;
          }
          #endif
        }
        break;
      }
      case NeoPixelType_Grbw:
      {
        for (uint8_t idx = 0; idx < pixelStrips; idx++)
        {
          #ifdef ARDUINO_ARCH_ESP32
          switch (idx)
          {
            case 0: ((NeoPixelBrightnessBusGrbw0*)_pGRBW[idx])->SetBrightness(b); break;
            case 1: ((NeoPixelBrightnessBusGrbw1*)_pGRBW[idx])->SetBrightness(b); break;
            case 2: ((NeoPixelBrightnessBusGrbw2*)_pGRBW[idx])->SetBrightness(b); break;
            case 3: ((NeoPixelBrightnessBusGrbw3*)_pGRBW[idx])->SetBrightness(b); break;
            case 4: ((NeoPixelBrightnessBusGrbw4*)_pGRBW[idx])->SetBrightness(b); break;
            case 5: ((NeoPixelBrightnessBusGrbw5*)_pGRBW[idx])->SetBrightness(b); break;
            case 6: ((NeoPixelBrightnessBusGrbw6*)_pGRBW[idx])->SetBrightness(b); break;
            case 7: ((NeoPixelBrightnessBusGrbw7*)_pGRBW[idx])->SetBrightness(b); break;
          }
          #else
          switch (pixelStripPins[idx])
          {
            case 2:  ((NeoPixelBrightnessBusGrbw2*)_pGRBW[idx])->SetBrightness(b); break;
            case 3:  ((NeoPixelBrightnessBusGrbw3*)_pGRBW[idx])->SetBrightness(b); break;
            default: ((NeoPixelBrightnessBusGrbw0*)_pGRBW[idx])->SetBrightness(b); break;
          }
          #endif
        }
        break;
      }
    }
  }

  void SetColorOrder(byte colorOrder)
  {
    _colorOrder = colorOrder;
  }

  uint8_t GetColorOrder()
  {
    return _colorOrder;
  }

  RgbwColor GetPixelColorRaw(uint16_t indexPixel) const
  {
    // figure out which strip this pixel index is on
    uint8_t stripIdx = 0;
    for (uint8_t idx = 0; idx < pixelStrips; idx++)
    {
      if (indexPixel >= pixelStripStartIdx[idx])
      {
        stripIdx = idx;
      }
      else
      {
        break;
      }
    }
    // subtract strip start index so we're addressing just this strip instead of all pixels on all strips
    indexPixel -= pixelStripStartIdx[stripIdx];
    switch (_type)
    {
    case NeoPixelType_Grb:
    {
      #ifdef ARDUINO_ARCH_ESP32
      switch (stripIdx)
      {
        case 0: ((NeoPixelBrightnessBusGrb0*)_pGRB[stripIdx])->GetPixelColor(indexPixel); break;
        case 1: ((NeoPixelBrightnessBusGrb1*)_pGRB[stripIdx])->GetPixelColor(indexPixel); break;
        case 2: ((NeoPixelBrightnessBusGrb2*)_pGRB[stripIdx])->GetPixelColor(indexPixel); break;
        case 3: ((NeoPixelBrightnessBusGrb3*)_pGRB[stripIdx])->GetPixelColor(indexPixel); break;
        case 4: ((NeoPixelBrightnessBusGrb4*)_pGRB[stripIdx])->GetPixelColor(indexPixel); break;
        case 5: ((NeoPixelBrightnessBusGrb5*)_pGRB[stripIdx])->GetPixelColor(indexPixel); break;
        case 6: ((NeoPixelBrightnessBusGrb6*)_pGRB[stripIdx])->GetPixelColor(indexPixel); break;
        case 7: ((NeoPixelBrightnessBusGrb7*)_pGRB[stripIdx])->GetPixelColor(indexPixel); break;
      }
      #else
      switch (pixelStripPins[stripIdx])
      {
        case 2:  ((NeoPixelBrightnessBusGrb2*)_pGRB[stripIdx])->GetPixelColor(indexPixel); break;
        case 3:  ((NeoPixelBrightnessBusGrb3*)_pGRB[stripIdx])->GetPixelColor(indexPixel); break;
        default: ((NeoPixelBrightnessBusGrb0*)_pGRB[stripIdx])->GetPixelColor(indexPixel); break;
      }
      #endif
      break;
    }
    case NeoPixelType_Grbw:
      #ifdef ARDUINO_ARCH_ESP32
      switch (stripIdx)
      {
        case 0: ((NeoPixelBrightnessBusGrbw0*)_pGRBW[stripIdx])->GetPixelColor(indexPixel); break;
        case 1: ((NeoPixelBrightnessBusGrbw1*)_pGRBW[stripIdx])->GetPixelColor(indexPixel); break;
        case 2: ((NeoPixelBrightnessBusGrbw2*)_pGRBW[stripIdx])->GetPixelColor(indexPixel); break;
        case 3: ((NeoPixelBrightnessBusGrbw3*)_pGRBW[stripIdx])->GetPixelColor(indexPixel); break;
        case 4: ((NeoPixelBrightnessBusGrbw4*)_pGRBW[stripIdx])->GetPixelColor(indexPixel); break;
        case 5: ((NeoPixelBrightnessBusGrbw5*)_pGRBW[stripIdx])->GetPixelColor(indexPixel); break;
        case 6: ((NeoPixelBrightnessBusGrbw6*)_pGRBW[stripIdx])->GetPixelColor(indexPixel); break;
        case 7: ((NeoPixelBrightnessBusGrbw7*)_pGRBW[stripIdx])->GetPixelColor(indexPixel); break;
      }
      #else
      switch (pixelStripPins[stripIdx])
      {
        case 2:  ((NeoPixelBrightnessBusGrbw2*)_pGRBW[stripIdx])->GetPixelColor(indexPixel); break;
        case 3:  ((NeoPixelBrightnessBusGrbw3*)_pGRBW[stripIdx])->GetPixelColor(indexPixel); break;
        default: ((NeoPixelBrightnessBusGrbw0*)_pGRBW[stripIdx])->GetPixelColor(indexPixel); break;
      }
      #endif
      break;
    }
    return 0;
  }

  // NOTE: Due to feature differences, some support RGBW but the method name
  // here needs to be unique, thus GetPixeColorRgbw
  uint32_t GetPixelColorRgbw(uint16_t indexPixel) const
  {
    RgbwColor col = GetPixelColorRaw(indexPixel);
    uint8_t co = _colorOrder;
    #ifdef COLOR_ORDER_OVERRIDE
    if (indexPixel >= COO_MIN && indexPixel < COO_MAX) co = COO_ORDER;
    #endif

    switch (co)
    {
      //                    W               G              R               B
      case  0: return ((col.W << 24) | (col.G << 8) | (col.R << 16) | (col.B)); //0 = GRB, default
      case  1: return ((col.W << 24) | (col.R << 8) | (col.G << 16) | (col.B)); //1 = RGB, common for WS2811
      case  2: return ((col.W << 24) | (col.B << 8) | (col.R << 16) | (col.G)); //2 = BRG
      case  3: return ((col.W << 24) | (col.B << 8) | (col.G << 16) | (col.R)); //3 = RBG
      case  4: return ((col.W << 24) | (col.R << 8) | (col.B << 16) | (col.G)); //4 = BGR
      case  5: return ((col.W << 24) | (col.G << 8) | (col.B << 16) | (col.R)); //5 = GBR
    }

    return 0;

  }
  

private:
  NeoPixelType _type;
  byte _colorOrder = 0;

  uint16_t *pixelCounts;         // number of pixels on each strip
  int8_t *pixelStripPins;        // strip GPIO pin
  int8_t *pixelStripPinsClk;     // strip GPIO pin
  uint8_t pixelStrips;                                // number of strips
  uint16_t pixelStripStartIdx[MAX_NUMBER_OF_STRIPS];  // start index in a single virtual strip

  void *_pGRB[MAX_NUMBER_OF_STRIPS];
  void *_pGRBW[MAX_NUMBER_OF_STRIPS];

  void cleanup()
  {
    switch (_type)
    {
      case NeoPixelType_Grb:
        for (uint8_t idx = 0; idx < pixelStrips; idx++)
        {
          #ifdef ARDUINO_ARCH_ESP32
          switch (idx)
          {
            case 0: delete ((NeoPixelBrightnessBusGrb0*)_pGRB[idx]); _pGRB[idx] = NULL; break;
            case 1: delete ((NeoPixelBrightnessBusGrb1*)_pGRB[idx]); _pGRB[idx] = NULL; break;
            case 2: delete ((NeoPixelBrightnessBusGrb2*)_pGRB[idx]); _pGRB[idx] = NULL; break;
            case 3: delete ((NeoPixelBrightnessBusGrb3*)_pGRB[idx]); _pGRB[idx] = NULL; break;
            case 4: delete ((NeoPixelBrightnessBusGrb4*)_pGRB[idx]); _pGRB[idx] = NULL; break;
            case 5: delete ((NeoPixelBrightnessBusGrb5*)_pGRB[idx]); _pGRB[idx] = NULL; break;
            case 6: delete ((NeoPixelBrightnessBusGrb6*)_pGRB[idx]); _pGRB[idx] = NULL; break;
            case 7: delete ((NeoPixelBrightnessBusGrb7*)_pGRB[idx]); _pGRB[idx] = NULL; break;
          }
          #else
          switch (pixelStripPins[idx])
          {
            case 2:  delete ((NeoPixelBrightnessBusGrb2*)_pGRB[idx]); _pGRB[idx] = NULL; break;
            case 3:  delete ((NeoPixelBrightnessBusGrb3*)_pGRB[idx]); _pGRB[idx] = NULL; break;
            default: delete ((NeoPixelBrightnessBusGrb0*)_pGRB[idx]); _pGRB[idx] = NULL; break;
          }
          #endif
        }
        break;

      case NeoPixelType_Grbw:
        for (uint8_t idx = 0; idx < pixelStrips; idx++)
        {
          #ifdef ARDUINO_ARCH_ESP32
          switch (idx)
          {
            case 0: delete ((NeoPixelBrightnessBusGrbw0*)_pGRBW[idx]); _pGRBW[idx] = NULL; break;
            case 1: delete ((NeoPixelBrightnessBusGrbw1*)_pGRBW[idx]); _pGRBW[idx] = NULL; break;
            case 2: delete ((NeoPixelBrightnessBusGrbw2*)_pGRBW[idx]); _pGRBW[idx] = NULL; break;
            case 3: delete ((NeoPixelBrightnessBusGrbw3*)_pGRBW[idx]); _pGRBW[idx] = NULL; break;
            case 4: delete ((NeoPixelBrightnessBusGrbw4*)_pGRBW[idx]); _pGRBW[idx] = NULL; break;
            case 5: delete ((NeoPixelBrightnessBusGrbw5*)_pGRBW[idx]); _pGRBW[idx] = NULL; break;
            case 6: delete ((NeoPixelBrightnessBusGrbw6*)_pGRBW[idx]); _pGRBW[idx] = NULL; break;
            case 7: delete ((NeoPixelBrightnessBusGrbw7*)_pGRBW[idx]); _pGRBW[idx] = NULL; break;
          }
          #else
          switch (pixelStripPins[idx])
          {
            case 2:  delete ((NeoPixelBrightnessBusGrbw2*)_pGRBW[idx]); _pGRBW[idx] = NULL; break;
            case 3:  delete ((NeoPixelBrightnessBusGrbw3*)_pGRBW[idx]); _pGRBW[idx] = NULL; break;
            default: delete ((NeoPixelBrightnessBusGrbw0*)_pGRBW[idx]); _pGRBW[idx] = NULL; break;
          }
          #endif
        }
        break;
    }
  }
};
#endif
