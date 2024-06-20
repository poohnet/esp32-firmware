#include "gfx_core.h"

#define swap(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation

GFXCore::GFXCore(int16_t width, int16_t height)
  : _buffer(nullptr)
  , _width(width)
  , _height(height)
  , _physicalWidth(width)
  , _physicalHeight(height)
  , _rotation(0)

{
  _buffer = new bool[_physicalWidth*_physicalHeight];
  memset(_buffer, 0, _physicalWidth*_physicalHeight);
}

GFXCore::~GFXCore()
{
  delete[] _buffer;
}

void GFXCore::setRotation(uint8_t rotation)
{
  _rotation = (rotation & 3);

  switch (_rotation) {
    case 0:
    case 2:
      _width = _physicalWidth;
      _height = _physicalHeight;
      break;
    case 1:
    case 3:
      _width = _physicalHeight;
      _height = _physicalWidth;
      break;
  }
}

void GFXCore::clearDisplay()
{
  startWrite();
  memset(_buffer, 0, _physicalWidth*_physicalHeight);
  endWrite();
}

void GFXCore::invertDisplay()
{
  startWrite();
  for (auto idx = 0; idx < _physicalWidth*_physicalHeight; idx++) {
    _buffer[idx] = !_buffer[idx];
  }
  endWrite();
}

void GFXCore::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x >= 0) && (x < _width) && (y >= 0) && (y < _height)) {
    // Pixel is in-bounds. Rotate coordinates if needed.
    switch (_rotation) {
      case 1:
        swap(x, y);
        x = _physicalWidth - x - 1;
        break;
      case 2:
        x = _physicalWidth - x - 1;
        y = _physicalHeight - y - 1;
        break;
      case 3:
        swap(x, y);
        y = _physicalHeight - y - 1;
        break;
    }

    switch (color) {
      case WHITE:
        _buffer[x + (y*_physicalWidth)] = true;
        break;
      case BLACK:
        _buffer[x + (y*_physicalWidth)] = false;
        break;
      case INVERSE:
        _buffer[x + (y*_physicalWidth)] = !_buffer[x + (y*_physicalWidth)];
        break;
    }
  }
}

void GFXCore::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  startWrite();
  if (x0 == x1) {
    if (y0 > y1) {
      swap(y0, y1);
    }
    drawVLineHelper(x0, y0, y1 - y0 + 1, color);
  }
  else if (y0 == y1) {
    if (x0 > x1) {
      swap(x0, x1);
    }
    drawHLineHelper(x0, y0, x1 - x0 + 1, color);
  }
  else {
    drawLineHelper(x0, y0, x1, y1, color);
  }
  endWrite();
}

void GFXCore::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  startWrite();
  drawPixel(x0, y0 + r, color);
  drawPixel(x0, y0 - r, color);
  drawPixel(x0 + r, y0, color);
  drawPixel(x0 - r, y0, color);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 - x, color);
  }
  endWrite();
}

void GFXCore::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
  startWrite();
  drawVLineHelper(x0, y0 - r, 2 * r + 1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
  endWrite();
}

void GFXCore::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  startWrite();
  drawHLineHelper(x, y, w, color);
  drawHLineHelper(x, y + h - 1, w, color);
  drawVLineHelper(x, y, h, color);
  drawVLineHelper(x + w - 1, y, h, color);
  endWrite();
}

void GFXCore::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  startWrite();
  for (auto i = x; i < x + w; i++) {
    drawVLineHelper(i, y, h, color);
  }
  endWrite();
}

void GFXCore::drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color)
{
  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t byte = 0;

  startWrite();
  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7) {
        byte <<= 1;
      }
      else {
        byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
      }
      if (byte & 0x80) {
        drawPixel(x + i, y, color);
      }
    }
  }
  endWrite();
}


//////////////////////////////////////////////////////////////////////////////////////////


void GFXCore::drawLineHelper(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      drawPixel(y0, x0, color);
    } else {
      drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

void GFXCore::drawVLineHelper(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  drawLineHelper(x, y, x, y + h - 1, color);
}

void GFXCore::drawHLineHelper(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  drawLineHelper(x, y, x + w - 1, y, color);
}

void GFXCore::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  int16_t px = x;
  int16_t py = y;

  delta++; // Avoid some +1's in the loop

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    // These checks avoid double-drawing certain lines, important
    // for the SSD1306 library which has an INVERT drawing mode.
    if (x < (y + 1)) {
      if (corners & 1) {
        drawVLineHelper(x0 + x, y0 - y, 2 * y + delta, color);
      }
      if (corners & 2) {
        drawVLineHelper(x0 - x, y0 - y, 2 * y + delta, color);
      }
    }
    if (y != py) {
      if (corners & 1) {
        drawVLineHelper(x0 + py, y0 - px, 2 * px + delta, color);
      }
      if (corners & 2) {
        drawVLineHelper(x0 - py, y0 - px, 2 * px + delta, color);
      }
      py = y;
    }
    px = x;
  }
}
