//this code is a modified version of https://github.com/Makuna/NeoPixelBus/issues/103
#ifndef NpbWrapper_h
#define NpbWrapper_h

// make sure we're using esp32 platform
#ifdef ARDUINO_ARCH_ESP32
  #error This version of NbpWrapper.h only works with ESP8266 hardware.
#endif

#ifndef NUM_STRIPS
  #error Need to define number of LED strips using build flag -D NUM_STRIPS=4 for 4 LED strips
#endif

#ifndef PIXEL_COUNTS
  #error Need to define pixel counts using build flag -D PIXEL_COUNTS="25, 25, 25, 25" for 4 LED strips with 25 LEDs each
#endif

#ifndef LEDPIN0
  #error At least LEDPIN0 has to be defined.
#endif

// //PIN CONFIGURATION
#ifndef LEDPIN
  #define LEDPIN 1  // Legacy pin def required by some other portions of code. This pin is not used do drive LEDs.
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

#include <NeoPixelBrightnessBus.h>
#include "const.h"

const uint8_t numStrips = NUM_STRIPS;  // max 4 strips allowed on esp8266
const uint16_t pixelCounts[numStrips] = {PIXEL_COUNTS}; // number of pixels on each strip

#define PIXELFEATURE3 NeoGrbFeature
#define PIXELFEATURE4 NeoGrbwFeature

#ifdef LEDPIN0
  #if LEDPIN0 == 2
    #define PIXELMETHOD0 NeoEsp8266Uart1Ws2813Method //if you get an error here, try to change to NeoEsp8266UartWs2813Method or update Neopixelbus
  #elif LEDPIN0 == 3
    #define PIXELMETHOD0 NeoEsp8266Dma800KbpsMethod
  #else
    #define PIXELMETHOD0 NeoEsp8266BitBang800KbpsMethod
    #pragma message "Software BitBang will be used because of your selected LED pin. This may cause flicker. Use GPIO 2 or 3 for best results."
  #endif
#else
  #error You have to define LEDPIN0 for the first strip (GPIO).
#endif

#ifdef LEDPIN1
  #if LEDPIN1 == 2
    #define PIXELMETHOD1 NeoEsp8266Uart1Ws2813Method //if you get an error here, try to change to NeoEsp8266UartWs2813Method or update Neopixelbus
  #elif LEDPIN1 == 3
    #define PIXELMETHOD1 NeoEsp8266Dma800KbpsMethod
  #else
    #define PIXELMETHOD1 NeoEsp8266BitBang800KbpsMethod
    #pragma message "Software BitBang will be used because of your selected LED pin. This may cause flicker. Use GPIO 2 or 3 for best results."
  #endif
#elif NUM_STRIPS > 1
  #error You have to define LEDPIN1 for the second strip (GPIO).
#else
  #define PIXELMETHOD1 NeoEsp8266BitBang800KbpsMethod
#endif

#ifdef LEDPIN2
  #if LEDPIN2 == 2
    #define PIXELMETHOD2 NeoEsp8266Uart1Ws2813Method //if you get an error here, try to change to NeoEsp8266UartWs2813Method or update Neopixelbus
  #elif LEDPIN2 == 3
    #define PIXELMETHOD2 NeoEsp8266Dma800KbpsMethod
  #else
    #define PIXELMETHOD2 NeoEsp8266BitBang800KbpsMethod
    #pragma message "Software BitBang will be used because of your selected LED pin. This may cause flicker. Use GPIO 2 or 3 for best results."
  #endif
#elif NUM_STRIPS > 2
  #error You have to define LEDPIN2 for the third strip (GPIO).
#else
  #define PIXELMETHOD2 NeoEsp8266BitBang800KbpsMethod
#endif

#ifdef LEDPIN3
  #if LEDPIN3 == 2
    #define PIXELMETHOD3 NeoEsp8266Uart1Ws2813Method //if you get an error here, try to change to NeoEsp8266UartWs2813Method or update Neopixelbus
  #elif LEDPIN3 == 3
    #define PIXELMETHOD3 NeoEsp8266Dma800KbpsMethod
  #else
    #define PIXELMETHOD3 NeoEsp8266BitBang800KbpsMethod
    #pragma message "Software BitBang will be used because of your selected LED pin. This may cause flicker. Use GPIO 2 or 3 for best results."
  #endif
#elif NUM_STRIPS > 3
  #error You have to define LEDPIN3 for the fourth strip (GPIO).
#else
  #define PIXELMETHOD3 NeoEsp8266BitBang800KbpsMethod
#endif

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
    _type(NeoPixelType_None)
  {
    // On initialization fill in the pixelStripStartIdx array with the beginning index of each strip
    // relative to th entire array.
    uint16_t totalPixels = 0;
    for (uint8_t idx = 0; idx < numStrips; idx++)
    {
      pixelStripStartIdx[idx] = totalPixels;
      totalPixels += pixelCounts[idx];
    }
  }

  ~NeoPixelWrapper()
  {
    cleanup();
  }

  void Begin(NeoPixelType type, uint16_t pixelCount)
  {

    cleanup();

    _type = type;

    switch (_type)
    {
      case NeoPixelType_Grb:
      {
        for (uint8_t idx = 0; idx < numStrips; idx++)
        {
          switch (idx)
          {
            case 0: pGrb0 = new NeoPixelBrightnessBus<PIXELFEATURE3,PIXELMETHOD0>(pixelCounts[idx], LEDPIN0); pGrb0->Begin(); break;
            #ifdef LEDPIN1
            case 1: pGrb1 = new NeoPixelBrightnessBus<PIXELFEATURE3,PIXELMETHOD1>(pixelCounts[idx], LEDPIN1); pGrb1->Begin(); break;
            #endif
            #ifdef LEDPIN2
            case 2: pGrb2 = new NeoPixelBrightnessBus<PIXELFEATURE3,PIXELMETHOD2>(pixelCounts[idx], LEDPIN2); pGrb2->Begin(); break;
            #endif
            #ifdef LEDPIN3
            case 3: pGrb3 = new NeoPixelBrightnessBus<PIXELFEATURE3,PIXELMETHOD3>(pixelCounts[idx], LEDPIN3); pGrb3->Begin(); break;
            #endif
          }
        }
        break;
      }

      case NeoPixelType_Grbw:
      {
        for (uint8_t idx = 0; idx < numStrips; idx++)
        {
          switch (idx)
          {
            case 0: pGrbw0 = new NeoPixelBrightnessBus<PIXELFEATURE4,PIXELMETHOD0>(pixelCounts[idx], LEDPIN0); pGrbw0->Begin(); break;
            #ifdef LEDPIN1
            case 1: pGrbw1 = new NeoPixelBrightnessBus<PIXELFEATURE4,PIXELMETHOD1>(pixelCounts[idx], LEDPIN1); pGrbw1->Begin(); break;
            #endif
            #ifdef LEDPIN2
            case 2: pGrbw2 = new NeoPixelBrightnessBus<PIXELFEATURE4,PIXELMETHOD2>(pixelCounts[idx], LEDPIN2); pGrbw2->Begin(); break;
            #endif
            #ifdef LEDPIN3
            case 3: pGrbw3 = new NeoPixelBrightnessBus<PIXELFEATURE4,PIXELMETHOD3>(pixelCounts[idx], LEDPIN3); pGrbw3->Begin(); break;
            #endif
          }
        }
        break;
      }
    }
  }

  void Show()
  {
    switch (_type)
    {
      case NeoPixelType_Grb:
      {
        for (uint8_t idx = 0; idx < numStrips; idx++)
        {
          switch (idx)
          {
            case 0: pGrb0->Show(); break;
            case 1: pGrb1->Show(); break;
            case 2: pGrb2->Show(); break;
            case 3: pGrb3->Show(); break;
          }
        }
        break;
      }
      case NeoPixelType_Grbw:
      {
        for (uint8_t idx = 0; idx < numStrips; idx++)
        {
          switch (idx)
          {
            case 0: pGrbw0->Show(); break;
            case 1: pGrbw1->Show(); break;
            case 2: pGrbw2->Show(); break;
            case 3: pGrbw3->Show(); break;
          }
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
        for (uint8_t idx = 0; idx < numStrips; idx++)
        {
          switch (idx)
          {
            case 0: canShow &= pGrb0->CanShow(); break;
            case 1: canShow &= pGrb1->CanShow(); break;
            case 2: canShow &= pGrb2->CanShow(); break;
            case 3: canShow &= pGrb3->CanShow(); break;
          }
        }
        break;
      }
      case NeoPixelType_Grbw:
      {
        for (uint8_t idx = 0; idx < numStrips; idx++)
        {
          switch (idx)
          {
            case 0: canShow &= pGrbw0->CanShow(); break;
            case 1: canShow &= pGrbw1->CanShow(); break;
            case 2: canShow &= pGrbw2->CanShow(); break;
            case 3: canShow &= pGrbw3->CanShow(); break;
          }
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
    for (uint8_t idx = 0; idx < numStrips; idx++)
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
        RgbColor rgb = RgbColor(c.R, c.G, c.B);
        switch (stripIdx)
        {
          case 0: pGrb0->SetPixelColor(indexPixel, rgb); break;
          case 1: pGrb1->SetPixelColor(indexPixel, rgb); break;
          case 2: pGrb2->SetPixelColor(indexPixel, rgb); break;
          case 3: pGrb3->SetPixelColor(indexPixel, rgb); break;
        }
        break;
      }
      case NeoPixelType_Grbw:
      {
        switch (stripIdx)
        {
          case 0: pGrbw0->SetPixelColor(indexPixel, c); break;
          case 1: pGrbw1->SetPixelColor(indexPixel, c); break;
          case 2: pGrbw2->SetPixelColor(indexPixel, c); break;
          case 3: pGrbw3->SetPixelColor(indexPixel, c); break;
        }
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
        for (uint8_t idx = 0; idx < numStrips; idx++)
        {
          switch (idx)
          {
            case 0: pGrb0->SetBrightness(b); break;
            case 1: pGrb1->SetBrightness(b); break;
            case 2: pGrb2->SetBrightness(b); break;
            case 3: pGrb3->SetBrightness(b); break;
          }
        }
        break;
      }
      case NeoPixelType_Grbw:
      {
        for (uint8_t idx = 0; idx < numStrips; idx++)
        {
          switch (idx)
          {
            case 0: pGrbw0->SetBrightness(b); break;
            case 1: pGrbw1->SetBrightness(b); break;
            case 2: pGrbw2->SetBrightness(b); break;
            case 3: pGrbw3->SetBrightness(b); break;
          }
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
    for (uint8_t idx = 0; idx < numStrips; idx++)
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
      switch (stripIdx)
      {
        case 0: return pGrb0->GetPixelColor(indexPixel);
        case 1: return pGrb1->GetPixelColor(indexPixel);
        case 2: return pGrb2->GetPixelColor(indexPixel);
        case 3: return pGrb3->GetPixelColor(indexPixel);
      }
      break;
    }
    case NeoPixelType_Grbw:
      switch (stripIdx)
      {
        case 0: return pGrbw0->GetPixelColor(indexPixel);
        case 1: return pGrbw1->GetPixelColor(indexPixel);
        case 2: return pGrbw2->GetPixelColor(indexPixel);
        case 3: return pGrbw3->GetPixelColor(indexPixel);
      }
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

  uint16_t pixelStripStartIdx[numStrips];

  // pointers for every possible type for up to 8 strips
  NeoPixelBrightnessBus<PIXELFEATURE3,PIXELMETHOD0> *pGrb0;
  NeoPixelBrightnessBus<PIXELFEATURE3,PIXELMETHOD1> *pGrb1;
  NeoPixelBrightnessBus<PIXELFEATURE3,PIXELMETHOD2> *pGrb2;
  NeoPixelBrightnessBus<PIXELFEATURE3,PIXELMETHOD3> *pGrb3;
  NeoPixelBrightnessBus<PIXELFEATURE4,PIXELMETHOD0> *pGrbw0;
  NeoPixelBrightnessBus<PIXELFEATURE4,PIXELMETHOD1> *pGrbw1;
  NeoPixelBrightnessBus<PIXELFEATURE4,PIXELMETHOD2> *pGrbw2;
  NeoPixelBrightnessBus<PIXELFEATURE4,PIXELMETHOD3> *pGrbw3;

  void cleanup()
  {
    switch (_type)
    {
      case NeoPixelType_Grb:
      {
        for (uint8_t idx = 0; idx < numStrips; idx++)
        {
          switch (idx)
          {
            case 0: delete pGrb0; pGrb0 = NULL; break;
            case 1: delete pGrb1; pGrb1 = NULL; break;
            case 2: delete pGrb2; pGrb2 = NULL; break;
            case 3: delete pGrb3; pGrb3 = NULL; break;
          }
        }
        break;
      }
      case NeoPixelType_Grbw:
      {
        for (uint8_t idx = 0; idx < numStrips; idx++)
        {
          switch (idx)
          {
            case 0: delete pGrbw0; pGrbw0 = NULL; break;
            case 1: delete pGrbw1; pGrbw1 = NULL; break;
            case 2: delete pGrbw2; pGrbw2 = NULL; break;
            case 3: delete pGrbw3; pGrbw3 = NULL; break;
          }
        }
      }
    }
  }
};
#endif
