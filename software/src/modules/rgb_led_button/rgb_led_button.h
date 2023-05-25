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

#include "config.h"
#include "device_module.h"

#include "bindings/bricklet_rgb_led_button.h"
#include "rgb_led_button_bricklet_firmware_bin.embedded.h"

class RGBLEDButton : public DeviceModule<TF_RGBLEDButton,
                                rgb_led_button_bricklet_firmware_bin_data,
                                rgb_led_button_bricklet_firmware_bin_length,
                                tf_rgb_led_button_create,
                                tf_rgb_led_button_get_bootloader_mode,
                                tf_rgb_led_button_reset,
                                tf_rgb_led_button_destroy> {
public:
    RGBLEDButton();
    ~RGBLEDButton();

    void pre_setup();
    void setup();
    void register_urls();
    void loop();

private:
    void setup_RGBLEDButton();
};
