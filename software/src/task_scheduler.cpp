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

#include "task_scheduler.h"

#include "web_server.h"

// Global definition here to match the declaration in task_scheduler.h.
TaskScheduler task_scheduler;

static uint64_t last_task_id = 0;

Task::Task(std::function<void(void)> fn, uint64_t task_id, uint32_t first_run_delay_ms, uint32_t delay_ms, bool once) :
          fn(std::move(fn)),
          task_id(task_id),
          next_deadline_ms(millis() + first_run_delay_ms),
          delay_ms(delay_ms),
          once(once) {

}

/*
A Compare type providing a strict weak ordering.

Note that the Compare parameter is defined such that it returns true
if its first argument comes before its second argument in a weak ordering.
But because the priority queue outputs largest elements first,
the elements that "come before" are actually output last.
That is, the front of the queue contains the "last" element
according to the weak ordering imposed by Compare.
(https://en.cppreference.com/w/cpp/container/priority_queue)
*/
bool compare(const Task *a, const Task *b)
{
    if (millis() > 0x7FFFFFFF) {
        // We are close to a timer overflow
        if (a->next_deadline_ms <= 0x7FFFFFFF && b->next_deadline_ms > 0x7FFFFFFF)
            // b is close to the overflow, a is behind the overflow
            return true;
        if (b->next_deadline_ms <= 0x7FFFFFFF && a->next_deadline_ms > 0x7FFFFFFF)
            // b is behind to the overflow, a is close to the overflow
            return false;
    }

    return a->next_deadline_ms >= b->next_deadline_ms;
}

// https://stackoverflow.com/a/36711682
bool TaskQueue::removeByTaskID(uint64_t task_id)  {
    auto it = std::find_if(this->c.begin(), this->c.end(), [task_id](const Task *t){return t->task_id == task_id;});

    if (it == this->c.end()) {
        // not found
        return false;
    }

    if (it == this->c.begin()) {
        // Mark top element as invalid. This allows cancelling a repeated task even if it is currently executed.
        (*it)->task_id = 0;
        return true;
    }

    // remove element and re-heap
    this->c.erase(it);
    std::make_heap(this->c.begin(), this->c.end(), this->comp);
    return true;
}

void TaskScheduler::pre_setup()
{
    mainTaskHandle = xTaskGetCurrentTaskHandle();
}

void TaskScheduler::setup()
{
    initialized = true;
}

void TaskScheduler::register_urls()
{
}

void TaskScheduler::loop()
{
    std::unique_ptr<Task> task;

    {
        std::lock_guard<std::mutex> l{this->task_mutex};
        if(tasks.empty()) {
            return;
        }

        if(!deadline_elapsed(tasks.top()->next_deadline_ms)) {
            return;
        }

        task = std::unique_ptr<Task>(tasks.top());
        // Don't remove the task from the queue yet:
        // We want to be able to cancel a task while it is being executed.

        bool cancelled = task->task_id == 0;
        if (cancelled) {
            tasks.pop();
            return;
        }
        this->current_task = task->task_id;
    }

    // Run task without holding the lock.
    // This allows a task to schedule tasks (could also be done with a recursive mutex)
    // and also allows other threads to schedule tasks while one is executed.
    if (!task->fn) {
        logger.printfln("Invalid task");
    } else {
        task->fn();
    }

    {
        std::lock_guard<std::mutex> l{this->task_mutex};
        this->current_task = 0;

        tasks.pop();

        if (task->once) {
            return;
        }

        // Check whether a repeated task was cancelled while it was executed.
        bool cancelled = task->task_id == 0;
        if (cancelled) {
            return;
        }

        task->next_deadline_ms = millis() + task->delay_ms;

        tasks.push(task.release());
    }
}

uint64_t TaskScheduler::scheduleOnce(std::function<void(void)> &&fn, uint32_t delay_ms)
{
    std::lock_guard<std::mutex> l{this->task_mutex};
    uint64_t task_id = ++last_task_id;
    tasks.emplace(new Task(fn, task_id, delay_ms, 0, true));
    return task_id;
}

uint64_t TaskScheduler::scheduleWithFixedDelay(std::function<void(void)> &&fn, uint32_t first_delay_ms, uint32_t delay_ms)
{
    std::lock_guard<std::mutex> l{this->task_mutex};
    uint64_t task_id = ++last_task_id;
    tasks.emplace(new Task(fn, task_id, first_delay_ms, delay_ms, false));
    return task_id;
}

bool TaskScheduler::cancel(uint64_t task_id) {
    if (task_id == 0)
        return false;

    std::lock_guard<std::mutex> l{this->task_mutex};
    return tasks.removeByTaskID(task_id);
}

uint64_t TaskScheduler::currentTask() {
    // currentTask is intended to write a self-canceling task.
    // Don't allow other threads to cancel tasks without knowing their ID.
    if (this->mainTaskHandle != xTaskGetCurrentTaskHandle()) {
        logger.printfln("Calling TaskScheduler::currentTask is only allowed in the main thread!");
        return 0;
    }

    std::lock_guard<std::mutex> l{this->task_mutex};
    return this->current_task;
}
