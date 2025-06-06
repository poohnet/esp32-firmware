/* esp32-firmware
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
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

#include "module.h"
#include "config.h"


class Heating final : public IModule
{
private:
    void update();

    ConfigRoot config;
    ConfigRoot state;
    ConfigRoot sgr_blocking_override;
    uint32_t last_sg_ready_change = 0;

    uint32_t override_task_id = 0;

    size_t trace_buffer_index;

public:
    enum class Status : uint8_t {
        Idle,
        Blocking,
        Extended,
        BlockingP14
    };

    Heating(){}
    void pre_setup() override;
    void setup() override;
    void register_urls() override;
    bool is_active();
    bool is_p14enwg_active();
    Status get_status();
};
