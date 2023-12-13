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

#include "device_module.h"

#include "bindings/bricklet_industrial_quad_relay_v2.h"
#include "industrial_quad_relay_v2_bricklet_firmware_bin.embedded.h"

class IndustrialQuadRelay : public DeviceModule<TF_IndustrialQuadRelayV2,
                                industrial_quad_relay_v2_bricklet_firmware_bin_data,
                                industrial_quad_relay_v2_bricklet_firmware_bin_length,
                                tf_industrial_quad_relay_v2_create,
                                tf_industrial_quad_relay_v2_get_bootloader_mode,
                                tf_industrial_quad_relay_v2_reset,
                                tf_industrial_quad_relay_v2_destroy> {
public:
  IndustrialQuadRelay();
  ~IndustrialQuadRelay() override;

  void pre_setup() override;
  void setup() override;
  void register_urls() override;
  void loop() override;

  bool getValue(uint8_t channel);
  void setValue(uint8_t channel, bool value);

private:
  void setup_IndustrialQuadRelay();
};