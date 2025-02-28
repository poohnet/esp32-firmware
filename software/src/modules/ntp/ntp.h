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

#include <mutex>

#include "module.h"
#include "config.h"

class NTP final : public IModule
{
private:
    String ntp_server1;
    String ntp_server2;

    std::mutex mtx;
    uint32_t sync_counter = 0;
    micros_t last_sync = 0_us;
    bool first_sync = true;

    ConfigRoot config;
    ConfigRoot state;

public:
    NTP(){}
    void pre_setup() override;
    void setup() override;
    void register_urls() override;
    void set_synced(bool synced);
    void set_api_time(struct timeval time);


    void time_synced_NTPThread();
};
