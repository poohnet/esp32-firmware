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

#include "module_dependencies.h"

#define EXTERNAL_CONTROL_STATE_AVAILABLE    0
#define EXTERNAL_CONTROL_STATE_DISABLED     1
#define EXTERNAL_CONTROL_STATE_UNAVAILABLE  2
#define EXTERNAL_CONTROL_STATE_SWITCHING    3

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

  pm_state = Config::Object({
    {"external_control", Config::Uint32(EXTERNAL_CONTROL_STATE_AVAILABLE)}
  });

  ps_state = Config::Object({
    {"cp_disconnect", Config::Bool(false)},
    {"phases_wanted", Config::Uint8(0)},
    {"phases_active", Config::Uint8(0)}
  });

  external_control = Config::Object({
    {"phases_wanted", Config::Uint32(0)},
  });

  external_control_update = external_control;
}

void PhaseSwitcher::setup()
{
  setup_phase_switcher();

  if (!device_found) {
    return;
  }

  initialized = true;
  api.addFeature("phase_switcher");
  evse_common.set_phase_switcher_enabled(true);

  task_scheduler.scheduleWithFixedDelay([this]() {
    update_all_data();
  }, 0, 250);

  task_scheduler.scheduleWithFixedDelay([this]() {
    do_the_stuff();
  }, 1000, 1000);
}

void PhaseSwitcher::register_urls()
{
  api.addState("power_manager/state", &pm_state);
  api.addState("phase_switcher/state", &ps_state);

  api.addCommand("power_manager/external_control_update", &external_control_update, {}, [this]() {
    auto phases_wanted = external_control.get("phases_wanted");
    uint32_t old_phases = phases_wanted->asUint();
    uint32_t new_phases = external_control_update.get("phases_wanted")->asUint();

    if (new_phases == old_phases) {
      logger.printfln("Phase Switcher: Ignoring external control phase change request: Value is already %u.", new_phases);
      return;
    }

    if (new_phases == 2 || new_phases > 3) {
      logger.printfln("Phase Switcher: Ignoring external control phase change request: Value %u is invalid.", new_phases);
      return;
    }

    logger.printfln("Phase Switcher: External control phase change request: Switching from %u to %u.", old_phases, new_phases);
    phases_wanted->updateUint(new_phases);
  }, true);

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

  int rc = tf_phase_switcher_set_control_pilot_disconnect(&device, get_control_pilot_disconnect());

  if (rc != TF_E_OK) {
    if (!is_in_bootloader(rc)) {
      logger.printfln("Phase Switcher (Setup): tf_phase_switcher_set_control_pilot_disconnect() failed with rc %d.", rc);
    }

    return;
  }

  rc = tf_phase_switcher_set_phases_wanted(&device, get_phases_wanted());

  if (rc != TF_E_OK) {
    if (!is_in_bootloader(rc)) {
      logger.printfln("Phase Switcher (Setup): tf_phase_switcher_set_phases_wanted() failed with rc %d.", rc);
    }

    return;
  }
}

bool PhaseSwitcher::get_control_pilot_disconnect() const
{
  return ps_state.get("cp_disconnect")->asBool();
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
        logger.printfln("Phase Switcher: tf_phase_switcher_set_control_pilot_disconnect() failed with rc %d.", rc);
      }

      return;
    }

    logger.printfln("Phase Switcher: Control Pilot changed from %s to %s.", toString(old_cp_disconnect), toString(cp_disconnect));
  }

  if (cp_disconnected) {
    *cp_disconnected = cp_disconnect;
  }
}

uint8_t PhaseSwitcher::get_phases_wanted() const
{
  return static_cast<uint8_t>(ps_state.get("phases_wanted")->asUint());
}

void PhaseSwitcher::set_phases_wanted(uint8_t phases_wanted)
{
  if (!initialized) {
    return;
  }

  uint8_t old_phases_wanted = get_phases_wanted();

  if ((get_phases_active() == 0) && get_control_pilot_disconnect() && (phases_wanted != old_phases_wanted)) {
    int rc = tf_phase_switcher_set_phases_wanted(&device, phases_wanted);

    if (rc != TF_E_OK) {
      if (!is_in_bootloader(rc)) {
        logger.printfln("Phase Switcher: tf_phase_switcher_set_phases_wanted() failed with rc %d.", rc);
      }

      return;
    }

    logger.printfln("Phase Switcher: Phases (wanted) changed from %u to %u.", old_phases_wanted, phases_wanted);
  }
}

uint8_t PhaseSwitcher::get_phases_active() const
{
  return static_cast<uint8_t>(ps_state.get("phases_active")->asUint());
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
      logger.printfln("Phase Switcher: tf_phase_switcher_get_all_data() failed with rc %d.", rc);
    }

    return;
  }

  ps_state.get("cp_disconnect")->updateBool(cp_disconnect);
  ps_state.get("phases_wanted")->updateUint(phases_wanted);
  ps_state.get("phases_active")->updateUint(phases_active);
}

void PhaseSwitcher::do_the_stuff()
{
  static PSSwitchingState prev_state = switching_state;

  if (!initialized) {
    return;
  }

  if (switching_state != prev_state) {
    logger.printfln("Phase Switcher: Now in state %i.", static_cast<int>(switching_state));
    prev_state = switching_state;
  }

  uint8_t external_phases_wanted = static_cast<uint8_t>(external_control.get("phases_wanted")->asUint());

  switch (switching_state) {
    case PSSwitchingState::Monitoring:
      evse_common.set_phase_switcher_blocking(false);
      pm_state.get("external_control")->updateUint(EXTERNAL_CONTROL_STATE_AVAILABLE);

      if (external_phases_wanted != get_phases_wanted()) {
        switching_state = PSSwitchingState::Stopping;
        pm_state.get("external_control")->updateUint(EXTERNAL_CONTROL_STATE_SWITCHING);
      }
      break;

    case PSSwitchingState::Stopping:
      evse_common.set_phase_switcher_blocking(true);

      if (get_phases_active() == 0) {
        switching_state = PSSwitchingState::DisconnectingCP;
      }
      break;

    case PSSwitchingState::DisconnectingCP:
      evse_common.set_control_pilot_disconnect(true, nullptr);

      if (evse_common.get_control_pilot_disconnect()) {
        switching_state = PSSwitchingState::TogglingContactor;
      }
      break;

    case PSSwitchingState::TogglingContactor:
      set_phases_wanted(external_phases_wanted);

      if (get_phases_wanted() == external_phases_wanted) {
        switching_state = PSSwitchingState::ConnectingCP;
      }
      break;

    case PSSwitchingState::ConnectingCP:
      evse_common.set_control_pilot_disconnect(false, nullptr);

      if (!evse_common.get_control_pilot_disconnect()) {
        switching_state = PSSwitchingState::Monitoring;
      }
      break;

    default:
      break;
  }
}
