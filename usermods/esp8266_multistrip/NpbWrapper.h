//this code is a modified version of https://github.com/Makuna/NeoPixelBus/issues/103
#ifndef NpbWrapper_h
#define NpbWrapper_h

// make sure we're using esp32 platform
#ifdef ARDUINO_ARCH_ESP32
  #error This version of NbpWrapper.h only works with ESP8266 hardware.
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
#else
  #define NeoPixelBrightnessBusGrb0 NeoPixelBrightnessBus<NeoGrbFeature,NeoEsp8266BitBang800KbpsMethod>
  #define NeoPixelBrightnessBusGrb2 NeoPixelBrightnessBus<NeoGrbFeature,NeoEsp8266Uart1Ws2813Method>
  #define NeoPixelBrightnessBusGrb3 NeoPixelBrightnessBus<NeoGrbFeature,NeoEsp8266Dma800KbpsMethod>
  #define NeoPixelBrightnessBusGrbw0 NeoPixelBrightnessBus<NeoGrbwFeature,NeoEsp8266BitBang800KbpsMethod>
  #define NeoPixelBrightnessBusGrbw2 NeoPixelBrightnessBus<NeoGrbwFeature,NeoEsp8266Uart1Ws2813Method>
  #define NeoPixelBrightnessBusGrbw3 NeoPixelBrightnessBus<NeoGrbwFeature,NeoEsp8266Dma800KbpsMethod>
#endif

//PIN CONFIGURATION
#ifndef LEDPIN
  #define LEDPIN 1  // Legacy pin def required by some other portions of code. This pin is not used do drive LEDs.
//  #define LEDPIN strip.getStripPin(0)  // Legacy pin def required by some other portions of code.
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

  void initStrips(uint8_t numStrips, uint8_t stripPin[], uint16_t stripLen[])
  {
    uint16_t totalPixels = 0;
    pixelStrips = numStrips;
    pixelCounts = stripLen;
    pixelStripPins = stripPin;
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
        for (uint8_t idx = 0; idx < pixelStrips; idx++)
        {
          switch (pixelStripPins[idx])
          {
            case 2:
              _pGRB[idx] = (void*) new NeoPixelBrightnessBusGrb2(pixelCounts[idx], pixelStripPins[idx]);
              ((NeoPixelBrightnessBusGrb2*)_pGRB[idx])->Begin();
              break;
            case 3:
              _pGRB[idx] = (void*) new NeoPixelBrightnessBusGrb3(pixelCounts[idx], pixelStripPins[idx]);
              ((NeoPixelBrightnessBusGrb3*)_pGRB[idx])->Begin();
              break;
            default:
              _pGRB[idx] = (void*) new NeoPixelBrightnessBusGrb0(pixelCounts[idx], pixelStripPins[idx]);
              ((NeoPixelBrightnessBusGrb0*)_pGRB[idx])->Begin();
              break;
          }
        }
        break;
      }

      case NeoPixelType_Grbw:
      {
        for (uint8_t idx = 0; idx < pixelStrips; idx++)
        {
          switch (pixelStripPins[idx])
          {
            case 2:
              _pGRB[idx] = (void*) new NeoPixelBrightnessBusGrbw2(pixelCounts[idx], pixelStripPins[idx]);
              ((NeoPixelBrightnessBusGrbw2*)_pGRB[idx])->Begin();
              break;
            case 3:
              _pGRB[idx] = (void*) new NeoPixelBrightnessBusGrbw3(pixelCounts[idx], pixelStripPins[idx]);
              ((NeoPixelBrightnessBusGrbw3*)_pGRB[idx])->Begin();
              break;
            default:
              _pGRB[idx] = (void*) new NeoPixelBrightnessBusGrbw0(pixelCounts[idx], pixelStripPins[idx]);
              ((NeoPixelBrightnessBusGrbw0*)_pGRB[idx])->Begin();
              break;
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
        for (uint8_t idx = 0; idx < pixelStrips; idx++)
        {
          switch (pixelStripPins[idx])
          {
            case 2:  ((NeoPixelBrightnessBusGrb2*)_pGRB[idx])->Show();    break;
            case 3:  ((NeoPixelBrightnessBusGrb3*)_pGRB[idx])->Show();     break;
            default: ((NeoPixelBrightnessBusGrb0*)_pGRB[idx])->Show(); break;
          }
        }
        break;
      }
      case NeoPixelType_Grbw:
      {
        for (uint8_t idx = 0; idx < pixelStrips; idx++)
        {
          switch (pixelStripPins[idx])
          {
            case 2:  ((NeoPixelBrightnessBusGrbw2*)_pGRBW[idx])->Show();    break;
            case 3:  ((NeoPixelBrightnessBusGrbw3*)_pGRBW[idx])->Show();     break;
            default: ((NeoPixelBrightnessBusGrbw0*)_pGRBW[idx])->Show(); break;
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
        for (uint8_t idx = 0; idx < pixelStrips; idx++)
        {
          switch (pixelStripPins[idx])
          {
            case 2:  canShow &= ((NeoPixelBrightnessBusGrb2*)_pGRB[idx])->CanShow();    break;
            case 3:  canShow &= ((NeoPixelBrightnessBusGrb3*)_pGRB[idx])->CanShow();     break;
            default: canShow &= ((NeoPixelBrightnessBusGrb0*)_pGRB[idx])->CanShow(); break;
          }
        }
        break;
      }
      case NeoPixelType_Grbw:
      {
        for (uint8_t idx = 0; idx < pixelStrips; idx++)
        {
          switch (pixelStripPins[idx])
          {
            case 2:  canShow &= ((NeoPixelBrightnessBusGrbw2*)_pGRBW[idx])->CanShow();    break;
            case 3:  canShow &= ((NeoPixelBrightnessBusGrbw3*)_pGRBW[idx])->CanShow();     break;
            default: canShow &= ((NeoPixelBrightnessBusGrbw0*)_pGRBW[idx])->CanShow(); break;
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
        RgbColor rgb = RgbColor(c.R, c.G, c.B);
        switch (pixelStripPins[stripIdx])
        {
          case 2:  ((NeoPixelBrightnessBusGrb2*)_pGRB[stripIdx])->SetPixelColor(indexPixel, rgb);    break;
          case 3:  ((NeoPixelBrightnessBusGrb3*)_pGRB[stripIdx])->SetPixelColor(indexPixel, rgb);     break;
          default: ((NeoPixelBrightnessBusGrb0*)_pGRB[stripIdx])->SetPixelColor(indexPixel, rgb); break;
        }
        break;
      }
      case NeoPixelType_Grbw:
      {
        switch (pixelStripPins[stripIdx])
        {
          case 2:  ((NeoPixelBrightnessBusGrbw2*)_pGRBW[stripIdx])->SetPixelColor(indexPixel, c);    break;
          case 3:  ((NeoPixelBrightnessBusGrbw3*)_pGRBW[stripIdx])->SetPixelColor(indexPixel, c);     break;
          default: ((NeoPixelBrightnessBusGrbw0*)_pGRBW[stripIdx])->SetPixelColor(indexPixel, c); break;
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
        for (uint8_t idx = 0; idx < pixelStrips; idx++)
        {
          switch (pixelStripPins[idx])
          {
            case 2:  ((NeoPixelBrightnessBusGrb2*)_pGRB[idx])->SetBrightness(b);    break;
            case 3:  ((NeoPixelBrightnessBusGrb3*)_pGRB[idx])->SetBrightness(b);     break;
            default: ((NeoPixelBrightnessBusGrb0*)_pGRB[idx])->SetBrightness(b); break;
          }
        }
        break;
      }
      case NeoPixelType_Grbw:
      {
        for (uint8_t idx = 0; idx < pixelStrips; idx++)
        {
          switch (pixelStripPins[idx])
          {
            case 2:  ((NeoPixelBrightnessBusGrbw2*)_pGRBW[idx])->SetBrightness(b);    break;
            case 3:  ((NeoPixelBrightnessBusGrbw3*)_pGRBW[idx])->SetBrightness(b);     break;
            default: ((NeoPixelBrightnessBusGrbw0*)_pGRBW[idx])->SetBrightness(b); break;
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
      switch (pixelStripPins[stripIdx])
      {
        case 2:  ((NeoPixelBrightnessBusGrb2*)_pGRB[stripIdx])->GetPixelColor(indexPixel);    break;
        case 3:  ((NeoPixelBrightnessBusGrb3*)_pGRB[stripIdx])->GetPixelColor(indexPixel);     break;
        default: ((NeoPixelBrightnessBusGrb0*)_pGRB[stripIdx])->GetPixelColor(indexPixel); break;
      }
      break;
    }
    case NeoPixelType_Grbw:
      switch (pixelStripPins[stripIdx])
      {
        case 2:  ((NeoPixelBrightnessBusGrbw2*)_pGRBW[stripIdx])->GetPixelColor(indexPixel);    break;
        case 3:  ((NeoPixelBrightnessBusGrbw3*)_pGRBW[stripIdx])->GetPixelColor(indexPixel);     break;
        default: ((NeoPixelBrightnessBusGrbw0*)_pGRBW[stripIdx])->GetPixelColor(indexPixel); break;
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

  uint16_t *pixelCounts;         // number of pixels on each strip
  uint8_t *pixelStripPins;       // strip GPIO pin
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
          switch (pixelStripPins[idx])
          {
            case 2:  delete ((NeoPixelBrightnessBusGrb2*)_pGRB[idx]);    _pGRB[idx] = NULL; break;
            case 3:  delete ((NeoPixelBrightnessBusGrb3*)_pGRB[idx]);     _pGRB[idx] = NULL; break;
            default: delete ((NeoPixelBrightnessBusGrb0*)_pGRB[idx]); _pGRB[idx] = NULL; break;
          }
        }
        break;

      case NeoPixelType_Grbw:
        for (uint8_t idx = 0; idx < pixelStrips; idx++)
        {
          switch (pixelStripPins[idx])
          {
            case 2:  delete ((NeoPixelBrightnessBusGrbw2*)_pGRBW[idx]);    _pGRBW[idx] = NULL; break;
            case 3:  delete ((NeoPixelBrightnessBusGrbw3*)_pGRBW[idx]);     _pGRBW[idx] = NULL; break;
            default: delete ((NeoPixelBrightnessBusGrbw0*)_pGRBW[idx]); _pGRBW[idx] = NULL; break;
          }
        }
        break;
    }
  }
};
#endif
