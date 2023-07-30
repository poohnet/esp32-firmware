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

#include "industrial_dual_ac_relay.h"

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


IndustrialDualACRelay::IndustrialDualACRelay()
  : DeviceModule("industrial_dual_ac_relay", "Industrial Dual AC Relay", "Industrial Dual AC Relay", std::bind(&IndustrialDualACRelay::setup_IndustrialDualACRelay, this))
{
}

IndustrialDualACRelay::~IndustrialDualACRelay()
{
}

void IndustrialDualACRelay::pre_setup()
{
}

void IndustrialDualACRelay::setup_IndustrialDualACRelay()
{
  if (!this->DeviceModule::setup_device()) {
    return;
  }
}

void IndustrialDualACRelay::setup()
{
  setup_IndustrialDualACRelay();

  if (!device_found) {
    return;
  }
}

void IndustrialDualACRelay::register_urls()
{
  this->DeviceModule::register_urls();
}

void IndustrialDualACRelay::loop()
{
  this->DeviceModule::loop();
}
