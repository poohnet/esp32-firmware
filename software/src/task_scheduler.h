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

#include <Arduino.h>

#include <vector>
#include <queue>
#include <functional>
#include <mutex>

#include <time.h>
#include <iostream>

#include "tools.h"

struct Task {
    std::function<void(void)> fn;
    uint64_t task_id;
    uint32_t next_deadline_ms;
    uint32_t delay_ms;
    bool once;

    Task(std::function<void(void)> fn, uint64_t task_id, uint32_t first_run_delay_ms, uint32_t delay_ms, bool once);
};

bool compare(const Task *a, const Task *b);

class TaskQueue : public std::priority_queue<Task *, std::vector<Task *>, decltype(&compare)>
{
    using std::priority_queue<Task *, std::vector<Task *>, decltype(&compare)>::priority_queue;
public:
    bool removeByTaskID(uint64_t task_id);
};

class TaskScheduler
{
public:
    TaskScheduler() : tasks(&compare)
    {
    }
    void pre_setup();
    void setup();
    void register_urls();
    void loop();
    uint64_t currentTask();

    bool initialized = false;

    bool cancel(uint64_t task_id);
    uint64_t scheduleOnce(std::function<void(void)> &&fn, uint32_t delay_ms);
    uint64_t scheduleWithFixedDelay(std::function<void(void)> &&fn, uint32_t first_delay_ms, uint32_t delay_ms);

private:
    std::mutex task_mutex;
    TaskQueue tasks;
    uint64_t current_task = 0;
    TaskHandle_t mainTaskHandle;
};

// Make global variable available everywhere because it is not declared in modules.h.
// Definition is in task_scheduler.cpp.
extern TaskScheduler task_scheduler;
