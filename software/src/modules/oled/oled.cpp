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
#include "splash.h"

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


OLED::OLED()
  : DeviceModule("oled", "OLED", "OLED", std::bind(&OLED::setup_oled, this))
  , GFXCore(128, 64)
{
  _splash = Config::Object({
    {"no", Config::Int16(0)},
    {"color", Config::Int16(0)},
    {"clearDisplay", Config::Bool(true)}
  });

  _line = Config::Object({
    {"x0", Config::Int16(0)},
    {"y0", Config::Int16(0)},
    {"x1",  Config::Int16(0)},
    {"y1", Config::Int16(0)},
    {"color", Config::Int16(0)},
    {"clearDisplay", Config::Bool(true)}
  });

  _circle = Config::Object({
    {"x0", Config::Int16(0)},
    {"y0", Config::Int16(0)},
    {"r",  Config::Int16(0)},
    {"color", Config::Int16(0)},
    {"clearDisplay", Config::Bool(true)}
  });
}

OLED::~OLED()
{
}

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

  api.addCommand("oled/showSplash", &_splash, {}, [this]() {
    if (_splash.get("clearDisplay")->asBool()) {
      clearDisplay();
    }
    switch (_splash.get("no")->asInt()) {
      case 0:
        drawBitmap((width() - splash0_width) / 2, (height() - splash0_height) / 2, splash0_data, splash0_width, splash0_height, _splash.get("color")->asInt());
        break;
      case 1:
        drawBitmap((width() - splash1_width) / 2, (height() - splash1_height) / 2, splash1_data, splash1_width, splash1_height, _splash.get("color")->asInt());
        break;
    }
  }, true);

  api.addCommand("oled/drawLine", &_line, {}, [this]() {
    if (_line.get("clearDisplay")->asBool()) {
      clearDisplay();
    }
    drawLine(_line.get("x0")->asInt(), _line.get("y0")->asInt(), _line.get("x1")->asInt(), _line.get("y1")->asInt(), _line.get("color")->asInt());
  }, true);

  api.addCommand("oled/drawCircle", &_circle, {}, [this]() {
    if (_circle.get("clearDisplay")->asBool()) {
      clearDisplay();
    }
    drawCircle(_circle.get("x0")->asInt(), _circle.get("y0")->asInt(), _circle.get("r")->asInt(), _circle.get("color")->asInt());
  }, true);

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


void OLED::startWrite()
{
}

void OLED::endWrite()
{
  tf_oled_128x64_v2_write_pixels(&device, 0, 0, physicalWidth()-1, physicalHeight()-1, buffer(), physicalWidth()*physicalHeight());
}
