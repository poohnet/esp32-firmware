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

#include "bindings/bricklet_piezo_speaker_v2.h"
#include "piezo_speaker_v2_bricklet_firmware_bin.embedded.h"

class PiezoSpeaker : public DeviceModule<TF_PiezoSpeakerV2,
                                piezo_speaker_v2_bricklet_firmware_bin_data,
                                piezo_speaker_v2_bricklet_firmware_bin_length,
                                tf_piezo_speaker_v2_create,
                                tf_piezo_speaker_v2_get_bootloader_mode,
                                tf_piezo_speaker_v2_reset,
                                tf_piezo_speaker_v2_destroy> {
public:
    PiezoSpeaker();
    ~PiezoSpeaker();

    void pre_setup();
    void setup();
    void register_urls();
    void loop();

    void beep();

private:
    void setup_PiezoSpeaker();
};
