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

#pragma once

#include "bindings/bricklet_oled_128x64_v2.h"

#include "config.h"
#include "device_module.h"
#include "oled_128x64_v2_firmware.h"

class OLED : public DeviceModule<TF_OLED128x64V2,
                                oled_128x64_v2_bricklet_firmware_bin,
                                oled_128x64_v2_bricklet_firmware_bin_len,
                                tf_oled_128x64_v2_create,
                                tf_oled_128x64_v2_get_bootloader_mode,
                                tf_oled_128x64_v2_reset,
                                tf_oled_128x64_v2_destroy> {
public:
    OLED();
    ~OLED();

    void setup();
    void register_urls();
    void loop();

protected:
    int16_t width() const { return _width; }
    int16_t height() const { return _height; }

    void setRotation(uint8_t rotation);

protected:
    void startWrite();
    void endWrite();

    void clearDisplay();
    void invertDisplay();
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);

    void drawLineHelper(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawVLineHelper(int16_t x, int16_t y, int16_t h, uint16_t color);
    void drawHLineHelper(int16_t x, int16_t y, int16_t w, uint16_t color);
    void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color);

private:
    void setup_oled();

    bool* _buffer;
    int16_t _width;
    int16_t _height;
    int16_t _physicalWidth;
    int16_t _physicalHeight;
    uint8_t _rotation;
};
