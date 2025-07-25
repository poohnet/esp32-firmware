/** @jsxImportSource preact */
import { h } from "preact";
import * as options from "../../options";
import { removeUnicodeHacks } from "../../ts/translation";
let x = {
    "evse": {
        "status": {
            "evse": "Charge status",
            "not_connected": "Disconnected",
            "waiting_for_charge_release": "Waiting for release",
            "ready_to_charge": "Ready",
            "charging": "Charging",
            "error": "Error",
            "configured_charging_current": "Configured charging current",
            "allowed_charging_current": "Allowed charging current",
            "boost_current": "Dynamic boost",
            "charge_control": "Charge control",
            "start_charging": "Start",
            "stop_charging": "Stop",
            "start_charge_for_user": "For user",
            "start_charge_no_tags": "No user with NFC tag configured"
        },
        "navbar": {
            "evse": "Charge Status",
            "evse_settings": "Settings"
        },
        "content": {
            "status": "Charge Status",
            "evse_settings": "Charger",
            "iec_state": "IEC 61851 state",
            "iec_state_a": "A (disconnected)",
            "iec_state_b": "B (connected)",
            "iec_state_c": "C (charging)",
            "iec_state_d": "D (not supported)",
            "iec_state_ef": "E/F (error)",
            "contactor_state": "Contactor check",
            "contactor_names": /*SFN*/(is_evse_v3: boolean) => is_evse_v3 ? "contactor L1+N, contactor L2+L3, state" : "before contactor, behind contactor, state"/*NF*/,
            "contactor_not_live": /*SFN*/(is_evse_v3: boolean) => is_evse_v3 ? "Open" : "Not live"/*NF*/,
            "contactor_live": /*SFN*/(is_evse_v3: boolean) => is_evse_v3 ? "Closed" : "Live"/*NF*/,
            "contactor_ok": "OK",
            "contactor_error": /*SFN*/(contactor_error: number) => {
                if (contactor_error == 0)
                    return "Error";
                return "Error code " + contactor_error.toString();
                }/*NF*/,
            "allowed_charging_current": "Allowed charging current",
            "error_state": "Error state",
            "error_state_help": <>
                <ul class="mb-0">
                    <li><p><strong>Switch error:</strong> The wallbox was not installed correctly. The charge controller's switch setting remains at its factory default.{options.WARP_DOC_BASE_URL.length > 0 ? <span> See <a href={removeUnicodeHacks(`${options.WARP_DOC_BASE_URL}/docs/warp_charger/assembly_and_installation#einstellen-des-ladestroms`)}>Charging current configuration</a>.</span> : undefined}</p></li>
                    <li><p><strong>DC error:</strong> A DC fault current has been detected. The error can be reset either via the wallbox's website or by briefly disconnecting the wallbox's power supply.</p> </li>
                    <li><p><strong>Contactor/PE error:</strong> One of the contactors is not switching correctly (see contactor check/status), phase L1 is de-energized, or an earth fault is present.</p></li>
                    <li><p><strong>Communication error:</strong> An error occurred during communication with the electric vehicle. On first occurrence, disconnect the charging cable from the vehicle, wait 10 seconds, and then reconnect the cable (restart the charging process).{options.WARP_DOC_BASE_URL.length > 0 ? <span> If the problem persists, see the <a href={removeUnicodeHacks(`${options.WARP_DOC_BASE_URL}/docs/warp_charger/troubleshooting#led-blinkt-f%C3%BCnfmal-rot-im-intervall-dass-webinterface-zeigt-kommunikationsfehler`)}>instructions</a>.</span> : undefined}</p></li>
                </ul>
            </>,
            "error_ok": "OK",
            "error_switch": "Switch",
            "error_contactor": /*SFN*/(pe_error: boolean, contactor_error: boolean) => (contactor_error == pe_error ? "Contactor/PE" : (pe_error ? "PE" : "Contactor"))/*NF*/,
            "error_communication": "Communication",
            "lock_state": "Cable lock",
            "lock_init": "Init",
            "lock_open": "Open",
            "lock_closing": "Closing",
            "lock_close": "Close",
            "lock_opening": "Opening",
            "lock_error": "Error",
            "time_since_state_change": "Time since state change",
            "uptime": "Uptime",
            "configuration": "Hardware configuration",
            "has_lock_switch": "Cable lock available",
            "lock_no": "No",
            "lock_yes": "Yes",
            "jumper_config_max_current": "Max current of supply cable",
            "jumper_config": "switch configured",
            "jumper_config_software": "Software",
            "jumper_config_unconfigured": "Unconfigured",
            "evse_version": "Charge controller hardware version",
            "evse_fw_version": "Charge controller firmware version",
            "charging_current": "Charging current limits",
            "charging_current_muted": "minimum of the charging current limits",
            "reset_slot": "Reset",
            "led_state": "LED state",
            "led_state_off": "Off",
            "led_state_on": "On",
            "led_state_blinking": "Blinking",
            "led_state_flickering": "Flickering",
            "led_state_breathing": "Breathing",
            "led_state_api": "API",
            "cp_pwm_dc": "CP PWM duty cycle",
            "adc_values": "ADC values",
            "voltages": "Voltages",
            "resistances": "Resistances",
            "resistance_names": "CP/PE, PP/PE",
            "gpios": "GPIOs",
            "gpio_low": "Low",
            "gpio_high": "High",
            "debug": "Charge log",
            "debug_description": "Create charge log",
            "debug_description_muted": "to diagnose charging problems",
            "gpio_out_high": "High impedance",
            "gpio_out_low": "Connected to ground",
            "gpio_out": "General purpose output",
            "gpio_in": "General purpose input",
            "gpio_shutdown": "Shutdown input",
            "button_configuration": "Button configuration",

            "auto_start_description": "Manual charge release",
            "auto_start_description_help": <><p>If manual charge release is enabled, a charging process will never begin automatically. Every charging process must be initiated via the web interface, the API, or (depending on the button configuration) the physical button. Manual charge release also prevents any other active charge current limits from taking effect. This means it does not need to be enabled if charging is controlled via NFC tag user release or through OCPP.</p></>,
            "auto_start_enable": "Requires that charges are always manually released via the web interface, the API or (depending on the button configuration) the button.",

            "external_description": "External control",
            "external_description_muted": <>{options.WARP_DOC_BASE_URL.length > 0 ? <a href={removeUnicodeHacks(`${options.WARP_DOC_BASE_URL}/docs/smart_home/evcc/`)}>see setup tutorial for details</a> : undefined}</>,
            "external_enable": <>Enables an external control system (for example <a href="https://evcc.io/">EVCC</a>) to control this charger</>,

            "boost_mode_desc": "Boost Mode",
            "boost_mode_desc_help": <><p>Some vehicles' charging electronics may interpret the prescribed current __("from_the_device") as too low. Boost mode compensates for this by communicating a slightly higher charging current.</p></>,
            "boost_mode": "Allows the vehicle's charge controller a slightly higher charge current (+ 0.24 A) to compensate measurement errors of the vehicle. Use only if your vehicle charges with less than the allowed charge current!",

            "reset_description": "Actions",
            "reset_description_muted": "",
            "reset_evse": "Restart",
            "reflash_evse": "Reflash",
            "vehicle_stopped_charging": "Vehicle stopped charging",
            "vehicle_stopped_false": "No",
            "vehicle_stopped_true": "Yes",

            "meter_monitoring": "Meter monitoring",
            "meter_monitoring_desc": "Monitors the energy meter and blocks charging if a malfunction is detected.",

            "enable_led_api": "Status LED control",
            "enable_led_api_desc": "Allows an external source to control the status LED.",

            "slot": /*SFN*/(i: number) => { return {
                0: "Supply cable",
                1: "Type 2 charging cable",
                2: "Shutdown input",
                3: "Configurable input",
                4: "Manual charge release",
                5: "Configuration",
                6: "User/NFC",
                7: "Charge manager",
                8: "External control",
                9: "Modbus TCP current",
                10: "Modbus TCP clearance",
                11: "OCPP",
                12: "Energy/Time limit",
                13: "Meter monitoring",
                14: "Automation",
                15: "Phase Switcher"
            }[i];}/*NF*/,

            // EVSE V1 only
            "gpio_names": "input, output, motor input switch, relay, motor fault",
            "user_calibration": "Calibration state",
            "user_calibration_state_disabled": "Factory settings",
            "user_calibration_state_enabled": "Modified",
            "voltage_calibration": "Voltage calibration",
            "voltage_calibration_names": "diff, mul, div",
            "resistance_2700": "Resistance calibration 2700Ω",
            "resistance_880": "Resistance calibration 880Ω",
            "user_calibration_description": "Calibration",
            "user_calibration_description_muted": "",
            "user_calibration_download": "Download",
            "user_calibration_upload": "Upload",
            "user_calibration_browse": "Browse",
            "user_calibration_select_file": "Select calibration file...",
            "user_calibration_reset": "Reset",

            // EVSE V2 only
            "evse_v2_gpio_names_0": "current configuration 0, motor fault, DC fault, current configuration 1",
            "evse_v2_gpio_names_1": "DC fault protector test, GP shutdown, button, CP PWM",
            "evse_v2_gpio_names_2": "motor input switch, contactor, GP output, CP disconnect",
            "evse_v2_gpio_names_3": "motor active, motor phase, contactor check before, contactor check behind",
            "evse_v2_gpio_names_4": "GP input, DC X6, DC X30, LED",
            "evse_v2_gpio_names_5": "unused",

            // EVSE V3 only
            "evse_v3_gpio_names_0": "DC X30, DC X6, DC fault, DC fault protector test",
            "evse_v3_gpio_names_1": "EVSE status LED, button, LED red, LED blue",
            "evse_v3_gpio_names_2": "LED green, CP PWM, contactor 1, contactor 0",
            "evse_v3_gpio_names_3": "contactor 1 feedback, contactor 0 feedback, PE check, current configuration 1",
            "evse_v3_gpio_names_4": "CP disconnect, current configuration 0, GP shutdown, version detect",
            "evse_v3_gpio_names_5": "unused",

            // EVSE V2 and V3
            "energy_meter_type": "Energy meter type",
            "gpio_shutdown_help": <><p>At the shutdown input, for example, a ripple control receiver can be connected. Here you can configure how changes to the shutdown input are handled. In the wallbox's standard configuration, power is limited to 4200 W when the shutdown input is closed.{options.WARP_DOC_BASE_URL.length > 0 ? <span> See <a href={removeUnicodeHacks(`${options.WARP_DOC_BASE_URL}/docs/tutorials/verbrauchseinrichtung`)}>controllable consumption device according to §14a EnWG</a>.</span> : undefined}</p></>,
            "gpio_shutdown_not_configured": "Not configured",
            "gpio_shutdown_on_open": "Shut down on open",
            "gpio_shutdown_on_close": "Shut down on close",
            "gpio_4200w_on_open": "Limit to 4200 W on open (§14a EnWG)",
            "gpio_4200w_on_close": "Limit to 4200 W on close (§14a EnWG)",
            "not_configured": "Not configured",
            "active_low_blocked": "Blocks if closed",
            "active_low_prefix": "Limits charge current to ",
            "active_low_suffix": " if closed",
            "active_high_blocked": "Blocks if open",
            "active_high_prefix": "Limits charge current to ",
            "active_high_suffix": " if open",

            "todo": "Have a feature request? Write an e-mail to info@tinkerforge.com",
            "gpio_in_muted": "readable on GPIO 16",
            "gpio_out_muted": <><a href="https://en.wikipedia.org/wiki/Open_collector">open collector</a></>,
            "button_configuration_muted": "Action to be executed when the button is pressed.",
            "button_configuration_disabled": "No action",
            "button_configuration_start_charging": "Start charging",
            "button_configuration_stop_charging": "Stop charging",
            "button_configuration_start_and_stop_charging": "Start or stop charging",

            "ev_wakeup_desc": "EV Wakeup",
            "ev_wakeup_desc_help": <><p>Some vehicles' charging electronics enter a power-saving mode if a charging process is not initiated within a certain time. The vehicle wakeup function attempts to automatically wake up such electronics if the vehicle does not respond within 30 seconds when power is available. This is implemented by temporarily disconnecting the Control Pilot (CP) signal.</p></>,
            "ev_wakeup": "Attempts to wake up the charge controller of the connected vehicle by fake-un and -re-plugging the charge cable.",

            "phase_auto_switch_desc": "Automatic phase switch",
            "phase_auto_switch_desc_help": <><p>If enabled, the internal current meter will determine during an ongoing charging process whether the vehicle is charging on a single phase. If that is the case, the charging station will automatically switch to single-phase charging.</p></>,
            "phase_auto_switch": "Automatically switches to single phase charging if the connected vehicle only draws current on L1.",

            "dc_fault_current_state": "DC fault protector state",
            "dc_fault_current_state_desc": "",
            "dc_fault_current_ok": "OK",
            "dc_fault_current_6_ma": "DC fault",
            "dc_fault_current_system": "System error",
            "dc_fault_current_unknown": "Unknown error",
            "dc_fault_current_calibration": /*SFN*/ (dc_fault_state: number, dc_fault_pins: number) => "Calibration error" + (dc_fault_state != 0 ? (": " + dc_fault_pins.toString()) : "")/*NF*/,
            "dc_fault_current_reset": "Reset",
            "dc_fault_current_20_ma": "AC fault",
            "dc_fault_current_6_ma_20_ma": "AC and DC fault",

            "reset_dc_fault_title": "Reset the DC fault protector",
            "reset_dc_fault_content": <>Resetting the DC fault protector restores the ability to charge. <b>Ensure that the reason why the DC fault protector triggered is resolved!</b> {options.MANUAL_URL.length > 0 ? <a href={options.MANUAL_URL}>See manual for details.</a> : undefined} Really reset the DC fault protector?</>,
            "abort": "Abort",
            "reset": "Reset",
            "trigger_dc_fault_test": "Test DC fault protector",
            "time_since_dc_fault_check": "Time since last DC fault protector test",

            // EVSE version specific value for common placeholder
            "error_2": /*SFN*/(is_evse_v2: boolean) => is_evse_v2 ? "DC fault protector" : "Calibration"/*NF*/,
            "adc_names": /*FFN*/(is_evse_v2: boolean) => is_evse_v2 ? <>CP/PE before resistor (PWM high), CP/PE after resistor (PWM high)<br/>CP/PE before resistor (PWM low), CP/PE after resistor (PWM low)<br/>PP/PE, +12V rail<br/>-12V rail</> : <>CP/PE, PP/PE</>/*NF*/,
            "voltage_names": /*FFN*/(is_evse_v2: boolean) => is_evse_v2 ? <>CP/PE before resistor (PWM high), CP/PE after resistor (PWM high)<br/>CP/PE before resistor (PWM low), CP/PE after resistor (PWM low)<br/>PP/PE, +12V rail<br/>-12V rail</> : <>CP/PE, PP/PE,<br/> CP/PE (high)</>/*NF*/,
            "dc_fault_sensor_type": "DC fault protector version",
            "dc_fault_pins": "DC fault protector pins",
            "temperature": "Temperature",
            "phases_current": "Phases current",
            "phases_requested": "Phases requested",
            "phases_state": "Phase switching state",

            "phases_connected": "Supply",
            "phases_connected_help": <><p>This setting determines whether the charging station is connected in three-phase or single-phase. If it is connected as single-phase and configured accordingly here, the second relay for phases L2 and L3 will never be activated, and the charging power limit according to §14a EnWG will be applied accordingly.</p></>,
            "phases_connected_1": "Single-phase",
            "phases_connected_3": "Three-phase"
        },
        "automation": {
            "external_current_wd": "External control watchdog triggered",
            "external_current_wd_trigger": <>When the <b>external</b> control <b>watchdog</b> is triggered, </>,
            "state_change": "Charge status changed",
            "led_duration": "Duration",
            "indication": "Indication",
            "color": "Color",
            "led_indication": "Show on status LED",
            "led_indication_off": "Off",
            "led_indication_on": "On",
            "led_indication_blinking": "Acknowledge blinking",
            "led_indication_flickering": "Rejecting blinking",
            "led_indication_breathing": "Demanding blinking",
            "led_indication_error": /*SFN*/(count: number) => {
                return "Blinking (" + count + " x)";
            }/*NF*/,
            "from": "From",
            "to": "To",
            "any": "Any state",
            "automation_action_block": "Block charging",
            "automation_action_allow": "Allow Charging",
            "automation_action_limit_current": "Limit charging current",
            "allowed_charging_current": "Allowed charging current",
            "action_allowed_charging_current": "Control charge",
            "automation_state_change_trigger": /*FFN*/(old_state: string, new_state: string) => <>When the charge state changes from "<b>{old_state}</b>" to "<b>{new_state}</b>",{" "}</>/*NF*/,
            "automation_action_text": /*FFN*/(current: string) => {
                if (current == "0") {
                    return <><b>block</b> charging.</>;
                }
                else if (current == "32") {
                    return <><b>allow</b> charging.</>;
                }
                return <>limit the allowed charging current to <b>{current} A</b>.</>
            }/*NF*/,
            "automation_led_action_text": /*FFN*/(indication_number: number, indication_text: string, duration: number, color: string) => {
                let c = color == "" ? "" : <>in <span class="px-2 mr-1" style={"background-color: " + color + "; border: 1px solid black;"}></span></>
                if (indication_number == 0)
                    return <>turn the status LED <b>{indication_text}</b> for <b>{duration / 1000} seconds</b>.</>
                if (indication_number == 255)
                    return <>turn the status LED <b>{indication_text}</b> {c} for <b>{duration / 1000} seconds</b>.</>
                return<>show <b>{indication_text}</b> {c} for <b>{duration / 1000} seconds</b> on the status LED.</>
            }/*NF*/
        },
        "script": {
            "set_charging_current_failed": "Failed to set charging current",
            "start_charging_failed": "Failed to start charging",
            "stop_charging_failed": "Failed to stop charging",
            "debug_name": "EVSE",
            "debug_filename": "EVSE-charge-protocol",

            "acc_blocked": "Blocked",
            "by": "by",

            "save_failed": "Failed to save charge settings.",

            "reset_slot_failed": "Releasing the current limit failed",

            "slot_disabled": "Not active",
            "slot_blocks": "Blocked",
            "slot_no_limit": "Released",

            "slot": /*SFN*/(i: number, is_evse_v2: boolean) => {return {
                0: "supply cable",
                1: "charging cable",
                2: "shutdown input",
                3: "general purpose input",
                4: is_evse_v2 ? "Auto-start or button" : "Auto-start, button or key switch",
                5: "configuration",
                6: "user/NFC",
                7: "charge management",
                8: "external control",
                9: "Modbus TCP",
                10: "Modbus TCP clearance",
                11: "OCPP",
                12: "energy/time limit",
                13: "meter monitoring",
                14: "automation",
                15: "Phase Switcher"
            }[i];}/*NF*/,

            "reboot_content_changed": "charge settings",

            // EVSE V1 only
            "user_calibration_upload_failed": "Updating the calibration failed",

            // EVSE V2 only
            "reset_dc_fault_current_failed": "Reset of the DC fault protector failed",
            "gpio_configuration_failed": "Updating the GPIO configuration failed",

            "meter_type_0": "No energy meter found",
            "meter_type_1": "Eastron SDM72",
            "meter_type_2": "Eastron SDM630",
            "meter_type_3": "Eastron SDM72V2",
            "meter_type_6": "Eltako DSZ15DZMOD",
            "meter_type_7": "YTL DEM4A",
            "meter_type_254": "internal"
        }
    }
}
