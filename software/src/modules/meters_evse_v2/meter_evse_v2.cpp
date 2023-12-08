/* esp32-firmware
 * Copyright (C) 2023 Mattias Schäffersmann <mattias@tinkerforge.com>
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

#include "meter_evse_v2.h"

#include "modules/meters/meter_value_id.h"
#include "modules/meters/sdm_helpers.h"
#include "task_scheduler.h"
#include "tools.h"

#include "gcc_warnings.h"

_ATTRIBUTE((const))
MeterClassID MeterEVSEV2::get_class() const
{
    return MeterClassID::EVSEV2;
}

void MeterEVSEV2::setup(Config &ephemeral_config)
{
    // Trigger meter value update, in case other modules expect meter values during setup.
    evse_v2.update_all_data();
}

void MeterEVSEV2::update_from_evse_v2_all_data(EVSEV2::meter_data *meter_data)
{
    // TODO: Add stale data check.
    // Reject stale data older than five seconds.
    //if (deadline_elapsed(all_data.last_update + 5 * 1000))
    //    return;

    // Always update error counters, even if no meter could be detected.
    errors->get("local_timeout"       )->updateUint(meter_data->error_count[0]);
    errors->get("global_timeout"      )->updateUint(meter_data->error_count[1]);
    errors->get("illegal_function"    )->updateUint(meter_data->error_count[2]);
    errors->get("illegal_data_access" )->updateUint(meter_data->error_count[3]);
    errors->get("illegal_data_value"  )->updateUint(meter_data->error_count[4]);
    errors->get("slave_device_failure")->updateUint(meter_data->error_count[5]);

    // No data to handle if no meter was detected.
    if (meter_data->meter_type == METER_TYPE_NONE)
        return;

    if (meter_type != meter_data->meter_type) {
        if (meter_type != METER_TYPE_NONE) {
            if (!meter_change_warning_printed) {
                logger.printfln("meter_em: Meter change detected. This is not supported.");
                meter_change_warning_printed = true;
            }
            return;
        }

        meter_type = meter_data->meter_type;
        state->get("type")->updateUint(meter_type);

        MeterValueID ids[METER_ALL_VALUES_COUNT];
        uint32_t id_count = METER_ALL_VALUES_COUNT;
        sdm_helper_get_value_ids(meter_type, ids, &id_count);
        meters.declare_value_ids(slot, ids, id_count);

        value_index_power      = meters_find_id_index(ids, id_count, MeterValueID::PowerActiveLSumImExDiff);
        value_index_energy_rel = meters_find_id_index(ids, id_count, MeterValueID::EnergyActiveLSumImExSumResettable);
        value_index_energy_abs = meters_find_id_index(ids, id_count, MeterValueID::EnergyActiveLSumImExSum);

        task_scheduler.scheduleWithFixedDelay([this](){
            update_all_values();
        }, 0, 990);
    }

    meters.update_value(slot, value_index_power, meter_data->power);
    //meters.update_value(slot, value_index_energy_rel, meter_data->energy_relative); // TODO: Add me
    meters.update_value(slot, value_index_energy_abs, meter_data->energy_absolute);

    //TODO API change: replace import/export with phase currents
    //METER_ALL_VALUES_CURRENT_L1_A
    //METER_ALL_VALUES_CURRENT_L2_A
    //METER_ALL_VALUES_CURRENT_L3_A
}

void MeterEVSEV2::update_all_values()
{
    // No need to initialize the array because either all values are written or it is rejected entirely.
    float values[METER_ALL_VALUES_COUNT];
    if (evse_v2.get_all_energy_meter_values(values) != METER_ALL_VALUES_COUNT)
        return;

    uint32_t values_len = ARRAY_SIZE(values);
    sdm_helper_pack_all_values(meter_type, values, &values_len);

    if (values_len == 0) {
        logger.printfln("meter_em: Cannot pack values into array of size %u.", ARRAY_SIZE(values));
    } else {
        meters.update_all_values(slot, values);
    }
}

bool MeterEVSEV2::reset()
{
    evse_v2.reset_energy_meter_relative_energy();
    return true;
}
