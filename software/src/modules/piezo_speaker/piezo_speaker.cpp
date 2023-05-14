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

#include "piezo_speaker.h"

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


PiezoSpeaker::PiezoSpeaker()
  : DeviceModule("piezo_speaker", "Piezo Speaker", "Piezo Speaker", std::bind(&PiezoSpeaker::setup_PiezoSpeaker, this))
{
}

PiezoSpeaker::~PiezoSpeaker()
{
}

void PiezoSpeaker::pre_setup()
{
}

void PiezoSpeaker::setup_PiezoSpeaker()
{
  if (!this->DeviceModule::setup_device()) {
    return;
  }
}

void PiezoSpeaker::setup()
{
  setup_PiezoSpeaker();

  if (!device_found) {
    return;
  }

  beep();
}

void PiezoSpeaker::register_urls()
{
  this->DeviceModule::register_urls();
}

void PiezoSpeaker::loop()
{
  this->DeviceModule::loop();
}

void PiezoSpeaker::beep()
{
  tf_piezo_speaker_v2_set_beep(&device, 1000, 0, 500);
}
