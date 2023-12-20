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

#include "api.h"
#include "event_log.h"
#include "evse_cpc.h"

#include "bindings/hal_common.h"
#include "bindings/errors.h"

extern TF_HAL hal;

#define CP_CHANNEL 0

static const char* toString(bool cp_disconnect)
{
  return cp_disconnect ? "disconnected" : "connected";
}

EvseCPC::EvseCPC()
{
}

void EvseCPC::pre_setup()
{
    state = Config::Object({
      {"connected", Config::Bool(false)},
    });
}

void EvseCPC::setup()
{
  if (tf_industrial_quad_relay_v2_create(&device, nullptr, &hal) != TF_E_OK) {
    logger.printfln("EvseCPC::setup(): No Industrial Quad Relay Bricklet 2.0 found, disabling feature CP-Disconnect.");
    return;
  }

  tf_industrial_quad_relay_v2_set_status_led_config(&device, TF_INDUSTRIAL_QUAD_RELAY_V2_STATUS_LED_CONFIG_SHOW_STATUS);

  for (uint8_t channel = 0; channel < 4; channel++) {
    tf_industrial_quad_relay_v2_set_channel_led_config(&device, channel, TF_INDUSTRIAL_QUAD_RELAY_V2_CHANNEL_LED_CONFIG_SHOW_CHANNEL_STATUS);
    tf_industrial_quad_relay_v2_set_selected_value(&device, channel, false);
  }

  state.get("connected")->updateBool(false);

  initialized = true;
  api.addFeature("cp_disconnect");
  set_control_pilot_disconnect(false, nullptr);
}

void EvseCPC::register_urls()
{
  api.addState("evse_cpc/state", &state);
}

bool EvseCPC::get_control_pilot_disconnect()
{
  if (initialized) {
    bool values[4] = { false };
    int rc = tf_industrial_quad_relay_v2_get_value(&device, values);

    if (rc != TF_E_OK) {
      logger.printfln("EvseCPC::get_control_pilot_disconnect(): tf_industrial_quad_relay_v2_get_value() returned %d", rc);
      return false;
    }

    return !values[CP_CHANNEL];
  }

  return false;
}

void EvseCPC::set_control_pilot_disconnect(bool cp_disconnect, bool* cp_disconnected)
{
  if (initialized) {
    bool old_cp_disconnect = get_control_pilot_disconnect();

    if (cp_disconnect != old_cp_disconnect) {
      int rc = tf_industrial_quad_relay_v2_set_selected_value(&device, CP_CHANNEL, !cp_disconnect);

      if (rc != TF_E_OK) {
        logger.printfln("EvseCPC::set_control_pilot_disconnect(): tf_industrial_quad_relay_v2_set_selected_value() returned %d", rc);
        return;
      }

      cp_disconnect = get_control_pilot_disconnect();
      state.get("connected")->updateBool(!cp_disconnect);
      logger.printfln("EvseCPC::set_control_pilot_disconnect(): %s => %s", toString(old_cp_disconnect), toString(cp_disconnect));
    }

    if (cp_disconnected) {
      *cp_disconnected = cp_disconnect;
    }
  }
}
