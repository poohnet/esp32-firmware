/* esp32-firmware
 * Copyright (C) 2020-2021 Erik Fleckstein <erik@tinkerforge.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "oled.h"

#include "bindings/errors.h"

#include "api.h"
#include "event_log.h"
#include "tools.h"
#include "task_scheduler.h"
#include "modules.h"

extern EventLog logger;

extern TF_HAL hal;
extern TaskScheduler task_scheduler;

extern API api;

#define splash0_width  128
#define splash0_height 16

const uint8_t PROGMEM splash0_data[] = {
 0x80,0x02,0x00,0x18,0x7f,0xff,0xff,0xfc,0x3f,0xff,0xff,0xfe,0x1f,0xff,0xff,0xff
,0x80,0x02,0x00,0x18,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x03
,0x80,0x02,0x00,0x18,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x01
,0x80,0x02,0x00,0x18,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x01
,0x80,0x02,0x00,0x18,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x01
,0x80,0x02,0x00,0x18,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x01
,0x80,0x02,0x00,0x18,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x01
,0x80,0x02,0x00,0x18,0x7f,0xff,0xff,0xfc,0x3f,0xff,0xff,0xfe,0x0f,0xff,0xff,0xff
,0x80,0x02,0x00,0x18,0x7f,0xff,0xff,0xfc,0x3f,0xff,0xff,0xfe,0x1f,0xff,0xff,0xff
,0x80,0x02,0x00,0x18,0x40,0x00,0x00,0x0c,0x30,0x00,0x00,0x00,0x18,0x00,0x00,0x00
,0x80,0x02,0x00,0x18,0x40,0x00,0x00,0x0c,0x30,0x00,0x00,0x00,0x18,0x00,0x00,0x00
,0x80,0x02,0x00,0x18,0x40,0x00,0x00,0x0c,0x30,0x00,0x00,0x02,0x18,0x00,0x00,0x00
,0x80,0x00,0x00,0x00,0x40,0x00,0x00,0x0c,0x30,0x00,0x00,0x02,0x18,0x00,0x00,0x00
,0x80,0x00,0x00,0x00,0x40,0x00,0x00,0x0c,0x30,0x00,0x00,0x02,0x18,0x00,0x00,0x00
,0xc0,0x00,0x00,0x00,0x40,0x00,0x00,0x0c,0x30,0x00,0x00,0x02,0x18,0x00,0x00,0x00
,0xff,0xff,0xff,0xf8,0x40,0x00,0x00,0x0c,0x30,0x00,0x00,0x02,0x18,0x00,0x00,0x00
};

#define splash1_width  82
#define splash1_height 64

const uint8_t PROGMEM splash1_data[] = {
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000001,0b10000000,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000011,0b10000000,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000111,0b11000000,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000111,0b11000000,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00001111,0b11000000,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00011111,0b11100000,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00011111,0b11100000,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00111111,0b11100000,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00111111,0b11110000,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b01111111,0b11110000,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00011111,0b11111000,0b01111111,0b11110000,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00111111,0b11111110,0b01111111,0b11110000,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00111111,0b11111111,0b01111111,0b11110000,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00011111,0b11111111,0b11111011,0b11100000,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00001111,0b11111111,0b11111001,0b11111111,0b11000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00001111,0b11111111,0b11111001,0b11111111,0b11111000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000111,0b11111111,0b11110001,0b11111111,0b11111111,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000011,0b11111100,0b01110011,0b11111111,0b11111111,0b10000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000001,0b11111110,0b00111111,0b11111111,0b11111111,0b10000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,0b00011110,0b00001111,0b11111111,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b01111111,0b11111110,0b00011111,0b11111100,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00111111,0b11111111,0b11111111,0b11111000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00001111,0b11011111,0b11111111,0b11100000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00011111,0b00011001,0b11111111,0b11000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00111111,0b00111100,0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b01111110,0b01111100,0b11111000,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b01111111,0b11111110,0b01111100,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,0b11111111,0b11111100,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,0b11111111,0b11111110,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,0b11111111,0b11111110,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000001,0b11111111,0b11101111,0b11111110,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000001,0b11111111,0b11001111,0b11111110,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000011,0b11111111,0b00000111,0b11111110,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000011,0b11111100,0b00000111,0b11111110,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000011,0b11110000,0b00000011,0b11111110,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000001,0b10000000,0b00000000,0b11111110,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b01111110,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00111110,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00001100,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00000000,0b00000111,0b10000000,0b00000000,0b11111100,0b00000000,0b00000000,0b00000011,0b11000000,0b00000000,
  0b00000000,0b00000000,0b00000111,0b10000000,0b00000001,0b11111100,0b00000000,0b00000000,0b00000011,0b11000000,0b00000000,
  0b00000000,0b00000000,0b00000111,0b10000000,0b00000001,0b11111100,0b00000000,0b00000000,0b00000011,0b11000000,0b00000000,
  0b00000000,0b00000000,0b00000111,0b10000000,0b00000001,0b11100000,0b00000000,0b00000000,0b00000000,0b00011110,0b00000000,
  0b00000000,0b00000000,0b00000111,0b10000000,0b00000001,0b11100000,0b00000000,0b00000000,0b00000000,0b00011110,0b00000000,
  0b01111111,0b11100011,0b11110111,0b10011111,0b11111001,0b11111101,0b11100111,0b01111000,0b01111011,0b11011111,0b11000000,
  0b11111111,0b11110111,0b11111111,0b10111111,0b11111101,0b11111101,0b11111111,0b01111000,0b01111011,0b11011111,0b11000000,
  0b11111111,0b11110111,0b11111111,0b10111111,0b11111101,0b11111101,0b11111111,0b01111000,0b01111011,0b11011111,0b11000000,
  0b11110000,0b11110111,0b10000111,0b10111100,0b00111101,0b11100001,0b11111111,0b01111000,0b01111011,0b11011110,0b00000000,
  0b11110000,0b11110111,0b10000111,0b10111100,0b00111101,0b11100001,0b11110000,0b01111000,0b01111011,0b11011110,0b00000000,
  0b00000000,0b11110111,0b10000111,0b10000000,0b00111101,0b11100001,0b11100000,0b01111000,0b01111011,0b11011110,0b00000000,
  0b01111111,0b11110111,0b10000111,0b10011111,0b11111101,0b11100001,0b11100000,0b01111000,0b01111011,0b11011110,0b00000000,
  0b11111111,0b11110111,0b10000111,0b10111111,0b11111101,0b11100001,0b11100000,0b01111000,0b01111011,0b11011110,0b00000000,
  0b11110000,0b11110111,0b10000111,0b10111100,0b00111101,0b11100001,0b11100000,0b01111000,0b01111011,0b11011110,0b00000000,
  0b11110000,0b11110111,0b10000111,0b10111100,0b00111101,0b11100001,0b11100000,0b01111000,0b01111011,0b11011110,0b00000000,
  0b11110000,0b11110111,0b10000111,0b10111100,0b00111101,0b11100001,0b11100000,0b01111000,0b01111011,0b11011110,0b00000000,
  0b11111111,0b11110111,0b11111111,0b10111111,0b11111101,0b11100001,0b11100000,0b01111111,0b11111011,0b11011111,0b11000000,
  0b11111111,0b11110111,0b11111111,0b10111111,0b11111101,0b11100001,0b11100000,0b01111111,0b11111011,0b11011111,0b11000000,
  0b01111100,0b11110011,0b11110011,0b10011111,0b00111101,0b11100001,0b11100000,0b00111110,0b01111011,0b11001111,0b11000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11000000,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111101,0b01101000,0b11011011,0b00010001,0b00011010,0b00110001,0b11000000,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111101,0b00101011,0b01011010,0b11111011,0b01101010,0b11101111,0b11000000,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111101,0b01001011,0b01011011,0b00111011,0b00011010,0b00110011,0b11000000,
  0b11111111,0b11111111,0b11111111,0b11111111,0b11111101,0b01101011,0b01011011,0b11011011,0b01101010,0b11111101,0b11000000,
};

#define WIDTH 128
#define HEIGHT 64

#define TFOLED_BLACK 0   ///< Draw 'off' pixels
#define TFOLED_WHITE 1   ///< Draw 'on' pixels
#define TFOLED_INVERSE 2 ///< Invert pixels

#define swap(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation


OLED::OLED()
  : DeviceModule("oled", "OLED", "OLED", std::bind(&OLED::setup_oled, this))
  , _buffer(nullptr)
  , _width(WIDTH)
  , _height(HEIGHT)
  , _physicalWidth(WIDTH)
  , _physicalHeight(HEIGHT)
  , _rotation(0)
{
  _buffer = new bool[_physicalWidth*_physicalHeight];
  memset(_buffer, 0, _physicalWidth*_physicalHeight);
}

OLED::~OLED()
{
  delete[] _buffer;
}

int g_val = 0;
int g_rot = 0;

void OLED::setup_oled()
{
    if (!this->DeviceModule::setup_device()) {
        return;
    }
    
    int result = tf_oled_128x64_v2_clear_display(&device);
    if (result != TF_E_OK) {
        if (!is_in_bootloader(result)) {
            logger.printfln("OLED clear display failed (rc %d). Disabling OLED support.", result);
        }
        return;
    }

    tf_oled_128x64_v2_write_line(&device, 0, 0, "Hello WARP :-)");


    task_scheduler.scheduleWithFixedDelay([this](){
      switch (g_val++ % 6) {
        case 0:
          clearDisplay();
          drawBitmap((width() - splash0_width) / 2, (height() - splash0_height) / 2, splash0_data, splash0_width, splash0_height, 1);
          //drawBitmap((width() - splash1_width) / 2, (height() - splash1_height) / 2, splash1_data, splash1_width, splash1_height, 1);
          break;

        case 2:
          clearDisplay();
          drawRect(10, 10, 108, 44, TFOLED_WHITE);
          fillRect(20, 20, 88, 24, TFOLED_WHITE);
          fillRect(30, 30, 68, 4, TFOLED_BLACK);
          break;

        case 4:
          clearDisplay();
          drawCircle(64, 32, 30, TFOLED_WHITE);
          fillCircle(64, 32, 15, TFOLED_WHITE);
          fillCircle(64, 32, 8, TFOLED_BLACK);
          break;

        case 1:
        case 3:
        case 5:
          setRotation(g_rot++);
          //invertDisplay();
          break;
      }
    }, 2000, 2000);

    initialized = true;
}

void OLED::setup()
{
  setup_oled();

  if (!device_found) {
    return;
  }
}

void OLED::register_urls()
{
  if (!device_found) {
    return;
  }

  this->DeviceModule::register_urls();
}

void OLED::loop()
{
  if (!device_found) {
    return;
  }

  this->DeviceModule::loop();
}


//////////////////////////////////////////////////////////////////////////////////////////


void OLED::setRotation(uint8_t rotation)
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


void OLED::startWrite()
{
}

void OLED::endWrite()
{
  tf_oled_128x64_v2_write_pixels(&device, 0, 0, _physicalWidth-1, _physicalHeight-1, _buffer, _physicalWidth*_physicalHeight);
}

void OLED::clearDisplay()
{
  startWrite();
  memset(_buffer, 0, _physicalWidth*_physicalHeight);
  endWrite();
}

void OLED::invertDisplay()
{
  startWrite();
  for (auto idx = 0; idx < _physicalWidth*_physicalHeight; idx++) {
    _buffer[idx] = !_buffer[idx];
  }
  endWrite();
}

void OLED::drawPixel(int16_t x, int16_t y, uint16_t color)
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
      case TFOLED_WHITE:
        _buffer[x + (y*_physicalWidth)] = true;
        break;
      case TFOLED_BLACK:
        _buffer[x + (y*_physicalWidth)] = false;
        break;
      case TFOLED_INVERSE:
        _buffer[x + (y*_physicalWidth)] = !_buffer[x + (y*_physicalWidth)];
        break;
    }
  }
}

void OLED::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
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

void OLED::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
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

void OLED::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
  startWrite();
  drawVLineHelper(x0, y0 - r, 2 * r + 1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
  endWrite();
}

void OLED::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  startWrite();
  drawHLineHelper(x, y, w, color);
  drawHLineHelper(x, y + h - 1, w, color);
  drawVLineHelper(x, y, h, color);
  drawVLineHelper(x + w - 1, y, h, color);
  endWrite();
}

void OLED::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  startWrite();
  for (auto i = x; i < x + w; i++) {
    drawVLineHelper(i, y, h, color);
  }
  endWrite();
}

void OLED::drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color)
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





void OLED::drawLineHelper(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
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

void OLED::drawVLineHelper(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  drawLineHelper(x, y, x, y + h - 1, color);
}

void OLED::drawHLineHelper(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  drawLineHelper(x, y, x + w - 1, y, color);
}

void OLED::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color)
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

