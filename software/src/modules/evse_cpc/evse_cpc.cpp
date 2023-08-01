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

#include "evse_cpc.h"
#include "module_dependencies.h"

EvseCPC::EvseCPC()
{
}

EvseCPC::~EvseCPC()
{
}

void EvseCPC::pre_setup()
{
}

void EvseCPC::setup()
{
  if (!industrial_quad_relay.initialized) {
    logger.printfln("EvseCPC: Industrial Quad Relay Bricklet not initialized. Disabling feature CP-Disconnect.");
    return;
  }

  initialized = true;
  api.addFeature("cp_disconnect");

  logger.printfln("EvseCPC: Enabling CP.");
  industrial_quad_relay.setValue(0, true);
}

void EvseCPC::register_urls()
{
}

void EvseCPC::loop()
{
}

bool EvseCPC::get_control_pilot_disconnect()
{
  if (initialized) {
    return !industrial_quad_relay.getValue(0);
  }

  return false;
}

void EvseCPC::set_control_pilot_disconnect(bool cp_disconnect, bool* cp_disconnected)
{
  if (initialized) {
    industrial_quad_relay.setValue(0, !cp_disconnect);
  }

  if (cp_disconnected) {
    *cp_disconnected = get_control_pilot_disconnect();
  }
}
