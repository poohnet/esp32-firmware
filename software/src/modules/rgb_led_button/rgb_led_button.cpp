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

#include "rgb_led_button.h"

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


RGBLEDButton::RGBLEDButton()
  : DeviceModule("rgb_led_button", "RGB LED Button", "RGB LED Button", std::bind(&RGBLEDButton::setup_RGBLEDButton, this))
{
}

RGBLEDButton::~RGBLEDButton()
{
}

void RGBLEDButton::pre_setup()
{
}

void RGBLEDButton::setup_RGBLEDButton()
{
  if (!this->DeviceModule::setup_device()) {
    return;
  }
}

void RGBLEDButton::setup()
{
  setup_RGBLEDButton();

  if (!device_found) {
    return;
  }
}

void RGBLEDButton::register_urls()
{
  this->DeviceModule::register_urls();
}

void RGBLEDButton::loop()
{
  this->DeviceModule::loop();
}
