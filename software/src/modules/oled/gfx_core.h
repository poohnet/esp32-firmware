#pragma once

#include <Arduino.h>

#define BLACK   0   ///< Draw 'off' pixels
#define WHITE   1   ///< Draw 'on' pixels
#define INVERSE 2   ///< Invert pixels

class GFXCore {
  protected:
    GFXCore(int16_t width, int16_t height);
    virtual ~GFXCore();

    virtual void startWrite() = 0;
    virtual void endWrite() = 0;

  public:
    const bool* buffer() const;
    int16_t width() const;
    int16_t height() const;
    int16_t physicalWidth() const;
    int16_t physicalHeight() const;
    uint8_t rotation() const;
    
    void setRotation(uint8_t rotation);

    void clearDisplay();
    void invertDisplay();
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);

  private:
    bool* _buffer;
    int16_t _width;
    int16_t _height;
    int16_t _physicalWidth;
    int16_t _physicalHeight;
    uint8_t _rotation;

    void drawLineHelper(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawVLineHelper(int16_t x, int16_t y, int16_t h, uint16_t color);
    void drawHLineHelper(int16_t x, int16_t y, int16_t w, uint16_t color);
    void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color);
};


//////////////////////////////////////////////////////////////////////////////////////////


inline const bool* GFXCore::buffer() const
{
  return _buffer;
}

inline int16_t GFXCore::width() const
{
  return _width;
}

inline int16_t GFXCore::height() const
{
  return _height;
}

inline int16_t GFXCore::physicalWidth() const
{
  return _physicalWidth;
}

inline int16_t GFXCore::physicalHeight() const
{
  return _physicalHeight;
}

inline uint8_t GFXCore::rotation() const
{
  return _rotation;
}
