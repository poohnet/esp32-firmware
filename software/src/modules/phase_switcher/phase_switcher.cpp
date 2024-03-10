/* esp32-firmware
 * Copyright (C) 2023-2024 Thomas Hein
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
#include "task_scheduler.h"
#include "phase_switcher.h"

#include "bindings/errors.h"

static const char* toString(bool cp_disconnect)
{
  return cp_disconnect ? "disconnected" : "connected";
}

PhaseSwitcher::PhaseSwitcher()
  : DeviceModule("phase_switcher", "Phase Switcher", "Phase Switcher", [this](){this->setup_phase_switcher();})
{
}

void PhaseSwitcher::pre_setup()
{
  this->DeviceModule::pre_setup();

  state = Config::Object({
    {"cp_disconnect", Config::Bool(false)},
    {"phases_wanted", Config::Uint8(0)},
    {"phases_active", Config::Uint8(0)}
  });
}

void PhaseSwitcher::setup()
{
  setup_phase_switcher();

  if (!device_found) {
    return;
  }
}

void PhaseSwitcher::register_urls()
{
  api.addState("phase_switcher/state", &state);

  this->DeviceModule::register_urls();
}

void PhaseSwitcher::loop()
{
  this->DeviceModule::loop();
}

void PhaseSwitcher::setup_phase_switcher()
{
  if (!this->DeviceModule::setup_device()) {
    return;
  }

  tf_phase_switcher_set_status_led_config(&device, TF_PHASE_SWITCHER_STATUS_LED_CONFIG_SHOW_HEARTBEAT);

  int rc = tf_phase_switcher_set_control_pilot_disconnect(&device, true);

  if (rc != TF_E_OK) {
    if (!is_in_bootloader(rc)) {
      logger.printfln("PhaseSwitcher::setup_phase_switcher(): tf_phase_switcher_set_control_pilot_disconnect() failed with rc %d", rc);
    }

    return;
  }

  rc = tf_phase_switcher_set_phases_wanted(&device, 0);

  if (rc != TF_E_OK) {
    if (!is_in_bootloader(rc)) {
      logger.printfln("PhaseSwitcher::setup_phase_switcher(): tf_phase_switcher_set_phases_wanted() failed with rc %d", rc);
    }

    return;
  }

  initialized = true;
  api.addFeature("phase_switcher");

  update_all_data();
  set_control_pilot_disconnect(false, nullptr);

  task_scheduler.scheduleWithFixedDelay([this]() {
    update_all_data();
  }, 500, 500);

  task_scheduler.scheduleWithFixedDelay([this]() {
    static int step = 0;
    switch (step++) {
      case 0:
        set_phases_wanted(0);
        break;
      case 1:
        set_control_pilot_disconnect(true, nullptr);
        break;
      case 2:
        set_phases_wanted(1);
        break;
      case 3:
        set_control_pilot_disconnect(false, nullptr);
        break;
      case 4:
        set_control_pilot_disconnect(true, nullptr);
        break;
      case 5:
        set_phases_wanted(3);
        break;
      case 6:
        set_control_pilot_disconnect(false, nullptr);
        break;
      case 7:
        set_control_pilot_disconnect(true, nullptr);
        break;
      default:
        step = 0;
        break;
    }
  }, 5000, 5000);

}

bool PhaseSwitcher::get_control_pilot_disconnect() const
{
  return state.get("cp_disconnect")->asBool();
}

void PhaseSwitcher::set_control_pilot_disconnect(bool cp_disconnect, bool* cp_disconnected)
{
  if (!initialized) {
    return;
  }

  bool old_cp_disconnect = get_control_pilot_disconnect();

  if (cp_disconnect != old_cp_disconnect) {
    int rc = tf_phase_switcher_set_control_pilot_disconnect(&device, cp_disconnect);

    if (rc != TF_E_OK) {
      if (!is_in_bootloader(rc)) {
        logger.printfln("PhaseSwitcher::set_control_pilot_disconnect(): tf_phase_switcher_set_control_pilot_disconnect() failed with rc %d", rc);
      }

      return;
    }

    logger.printfln("PhaseSwitcher::set_control_pilot_disconnect(): %s => %s", toString(old_cp_disconnect), toString(cp_disconnect));
  }

  if (cp_disconnected) {
    *cp_disconnected = cp_disconnect;
  }
}

uint8_t PhaseSwitcher::get_phases_wanted() const
{
  return static_cast<uint8_t>(state.get("phases_wanted")->asUint());
}

void PhaseSwitcher::set_phases_wanted(uint8_t phases_wanted)
{
  if (!initialized) {
    return;
  }

  uint8_t old_phases_wanted = get_phases_wanted();

  //if ((get_phases_active() == 0) && (phases_wanted != old_phases_wanted)) {
  if (phases_wanted != old_phases_wanted) {
    int rc = tf_phase_switcher_set_phases_wanted(&device, phases_wanted);

    if (rc != TF_E_OK) {
      if (!is_in_bootloader(rc)) {
        logger.printfln("PhaseSwitcher::set_phases(): tf_phase_switcher_set_phases_wanted() failed with rc %d", rc);
      }

      return;
    }

    logger.printfln("PhaseSwitcher::set_phases_wanted(): %u => %u", old_phases_wanted, phases_wanted);
  }
}

uint8_t PhaseSwitcher::get_phases_active() const
{
  return static_cast<uint8_t>(state.get("phases_active")->asUint());
}

void PhaseSwitcher::update_all_data()
{
  if (!initialized) {
    return;
  }

  bool cp_disconnect;
  uint8_t phases_wanted, phases_active;
  int rc = tf_phase_switcher_get_all_data(&device, &cp_disconnect, &phases_wanted, &phases_active);

  if (rc != TF_E_OK) {
    if (!is_in_bootloader(rc)) {
      logger.printfln("PhaseSwitcher::update_all_data(): tf_phase_switcher_get_all_data() failed with rc %d", rc);
    }

    return;
  }

  state.get("cp_disconnect")->updateBool(cp_disconnect);
  state.get("phases_wanted")->updateUint(phases_wanted);
  state.get("phases_active")->updateUint(phases_active);
}
