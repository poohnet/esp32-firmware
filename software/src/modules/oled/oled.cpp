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

OLED::OLED() : DeviceModule("oled", "OLED", "OLED", std::bind(&OLED::setup_oled, this))
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

    if (!device_found)
        return;
}

void OLED::register_urls()
{
    if (!device_found)
        return;

    this->DeviceModule::register_urls();
}

#define WIDTH 128
#define HEIGHT 64

void OLED::loop()
{
    if (!device_found)
        return;

    this->DeviceModule::loop();
}
