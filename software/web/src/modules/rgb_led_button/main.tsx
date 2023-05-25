/* esp32-firmware
 * Copyright (C) 2022 Matthias Bolte <matthias@tinkerforge.com>
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

import $ from "../../ts/jq";

import * as API from "../../ts/api";

import { h, render } from "preact";
import { __ } from "../../ts/translation";
import { PageHeader } from "../../ts/components/page_header";

render(<PageHeader title={__("rgb_led_button.content.rgb_led_button")} />, $('#rgb_led_button_header')[0]);

function update_config()
{
    // Get current config from state "rgb_led_button/config" after receiving
    // a change from the backend
    let config = API.get("rgb_led_button/config");

    // Update HTML element with current color value
    $("#rgb_led_button_color").val(config.color);
}

function save_config()
{
    // Get current color value from the HTML element and create new config
    let config = {"color": $("#rgb_led_button_color").val().toString()}

    // Send new config to backend as state "rgb_led_button/config"
    API.save("rgb_led_button/config", config, __("rgb_led_button.script.save_config_failed"));
}

function update_state()
{
    // Get current state from state "rgb_led_button/state" after receiving
    // a change from the backend
    let state = API.get("rgb_led_button/state");

    // Update HTML element with current button value
    $("#rgb_led_button_button").val(state.button ? __("rgb_led_button.script.button_pressed") : __("rgb_led_button.script.button_released"));
}

export function init()
{
    // Attach the save_config function to the change event of the HTML
    // element to be able to send color changes to the backend.
    $("#rgb_led_button_color").on("change", save_config);
}

export function add_event_listeners(source: API.APIEventTarget)
{
    // Create event listener for state "rgb_led_button/config" to call the
    // update_config function if changes to that state are reported.
    source.addEventListener("rgb_led_button/config", update_config);

    // Create event listener for state "rgb_led_button/state" to call the
    // update_state function if changes to that state are reported.
    source.addEventListener("rgb_led_button/state", update_state);
}

export function update_sidebar_state(module_init: any)
{
    $("#sidebar-rgb-led-button").prop("hidden", !module_init.rgb_led_button);
}
