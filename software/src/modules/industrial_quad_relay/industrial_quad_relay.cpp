/* esp32-firmware
 * Copyright (C) 2023 Thomas Hein
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

#include "industrial_quad_relay.h"

IndustrialQuadRelay::IndustrialQuadRelay()
  : DeviceModule("industrial_quad_relay", "Industrial Quad Relay", "Industrial Quad Relay", [this](){this->setup_IndustrialQuadRelay();})
{
}

IndustrialQuadRelay::~IndustrialQuadRelay()
{
}

void IndustrialQuadRelay::pre_setup()
{
  this->DeviceModule::pre_setup();
}

void IndustrialQuadRelay::setup_IndustrialQuadRelay()
{
  if (!this->DeviceModule::setup_device()) {
    return;
  }

  tf_industrial_quad_relay_v2_set_status_led_config(&device, TF_INDUSTRIAL_QUAD_RELAY_V2_STATUS_LED_CONFIG_SHOW_STATUS);

  for (uint8_t channel = 0; channel < 4; channel++) {
    tf_industrial_quad_relay_v2_set_channel_led_config(&device, channel, TF_INDUSTRIAL_QUAD_RELAY_V2_CHANNEL_LED_CONFIG_SHOW_CHANNEL_STATUS);
  }

  initialized = true;
}

void IndustrialQuadRelay::setup()
{
  setup_IndustrialQuadRelay();

  if (!device_found) {
    return;
  }
}

void IndustrialQuadRelay::register_urls()
{
  this->DeviceModule::register_urls();
}

void IndustrialQuadRelay::loop()
{
  this->DeviceModule::loop();
}

bool IndustrialQuadRelay::getValue(uint8_t channel)
{
  if (channel < 4) {
    bool values[4] = { false };
    int rc = tf_industrial_quad_relay_v2_get_value(&device, values);

    if (rc == TF_E_OK) {
      return values[channel];
    }

    logger.printfln("tf_industrial_quad_relay_v2_get_value returned %d", rc);
    is_in_bootloader(rc);
  }

  return false;
}

void IndustrialQuadRelay::setValue(uint8_t channel, bool value)
{
  if (channel < 4) {
    int rc = tf_industrial_quad_relay_v2_set_selected_value(&device, channel, value);

    if (rc == TF_E_OK) {
      return;
    }

    logger.printfln("tf_industrial_quad_relay_v2_set_selected_value returned %d", rc);
    is_in_bootloader(rc);
  }
}
