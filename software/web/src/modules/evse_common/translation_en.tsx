/** @jsxImportSource preact */
import { h } from "preact";
let x = {
    "evse": {
        "status": {
            "evse": "Vehicle state",
            "not_connected": "Disconnected",
            "waiting_for_charge_release": "Waiting for release",
            "ready_to_charge": "Ready",
            "charging": "Charging",
            "error": "Error",
            "configured_charging_current": "Configured charging current",
            "allowed_charging_current": "Allowed charging current",
            "charge_control": "Charge control",
            "start_charging": "Start",
            "stop_charging": "Stop"
        },
        "navbar": {
            "evse": "Charge Status",
            "evse_settings": "Charge Settings"
        },
        "content": {
            "status": "Charge Status",
            "settings": "Charge Settings",
            "iec_state": "IEC 61851 state",
            "iec_state_a": "A (disconnected)",
            "iec_state_b": "B (connected)",
            "iec_state_c": "C (charging)",
            "iec_state_d": "D (not supported)",
            "iec_state_ef": "E/F (error)",
            "contactor_state": "Contactor check",
            "contactor_names": /*SFN*/(is_evse_v3: boolean) => is_evse_v3 ? "contactor L1+N, contactor L2+L3, state" : "before contactor, behind contactor, state"/*NF*/,
            "contactor_not_live": "Not live",
            "contactor_live": "Live",
            "contactor_ok": "OK",
            "contactor_error": /*SFN*/(contactor_error: number) => {
                if (contactor_error == 0)
                    return "Error";
                return "Error code " + contactor_error.toString();
                }/*NF*/,
            "allowed_charging_current": "Allowed charging current",
            "error_state": "Error state",
            "error_state_desc": <><a href="{{{manual_url}}}">see manual for details</a></>,
            "error_ok": "OK",
            "error_switch": "Switch error",
            "error_contactor": /*SFN*/(pe_error: boolean, contactor_error: boolean) => pe_error ? "PE" : (contactor_error ? "Contactor error" : "Contactor/PE error")/*NF*/,
            "error_communication": "Communication error",
            "error_pe": "PE",
            "lock_state": "Cable lock",
            "lock_init": "Init",
            "lock_open": "Open",
            "lock_closing": "Closing",
            "lock_close": "Close",
            "lock_opening": "Opening",
            "lock_error": "Error",
            "time_since_state_change": "Time since state change",
            "state_change": "State change",
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
            "low_level_state": "Low-level state",
            "led_state": "LED state",
            "led_state_off": "Off",
            "led_state_on": "On",
            "led_state_blinking": "Blinking",
            "led_state_flickering": "Flickering",
            "led_state_breathing": "Breathing",
            "led_state_api": "API",
            "led_duration": "Duration",
            "cp_pwm_dc": "CP PWM duty cycle",
            "adc_values": "ADC values",
            "voltages": "Voltages",
            "resistances": "Resistances",
            "resistance_names": "CP/PE, PP/PE",
            "gpios": "GPIOs",
            "gpio_low": "Low",
            "gpio_high": "High",
            "debug": "Charge log",
            "debug_start": "Start",
            "debug_stop": "Stop+Download",
            "debug_description": "Create charge log",
            "debug_description_muted": "to diagnose charging problems",
            "active_high": "If open",
            "active_low": "If closed",
            "gpio_state": "State",
            "gpio_out_high": "High impedance",
            "gpio_out_low": "Connected to ground",
            "gpio_out": "General purpose output",
            "gpio_in": "General purpose input",
            "gpio_shutdown": "Shutdown input",
            "button_pressed": "At button press",
            "button_released": "At button release",
            "button_configuration": "Button configuration",

            "auto_start_description": "Manual charge release",
            "auto_start_description_muted": <><a href="https://www.warp-charger.com/evcc.html?v=2">see manual for details</a></>,
            "auto_start_enable": "Requires that charges are always manually released via the web interface, the API or (depending on the button configuration) the button.",

            "external_description": "External control",
            "external_description_muted": <><a href="https://www.warp-charger.com/evcc.html?v=2">see setup tutorial for details</a></>,
            "external_enable": <>Enables an external control system (for example <a href="https://evcc.io/">EVCC</a>) to control this charger</>,

            "boost_mode_desc": "Boost Mode",
            "boost_mode_desc_muted": <><a href="https://www.warp-charger.com/evcc.html?v=2">see manual for details</a></>,
            "boost_mode": "Allows the vehicle's charge controller a slightly higher charge current (+ 0.24 A) to compensate measurement errors of the vehicle. Use only if your vehicle charges with less than the allowed charge current!",

            "reset_description": "Actions",
            "reset_description_muted": "",
            "reset_evse": "Restart",
            "reflash_evse": "Reflash",
            "charging_time": "Charging for",

            "meter_monitoring": "Meter monitoring",
            "meter_monitoring_desc": "Monitors the energy meter and blocks charging if a malfunction is detected.",

            "enable_led_api": "Status-LED control",
            "enable_led_api_desc": "Allows an external source to control the Status-LED.",
            "api_must_be_enabled": "API must be enabled to use this feature.",
            "cron_state_change_trigger": /*FFN*/(state: string) => <>If the charge status changes to "<b>{state}</b>",{" "}</>/*NF*/,
            "cron_action_text": /*FFN*/(current: number) => <>set the allowed charging current to <b>{current} A</b>.</>/*NF*/,
            "cron_led_action_text": /*FFN*/(state: string, duration: number) => state == "An" || state == "Aus" ? <>turn the status-LED <b>{state}</b> for <b>{duration} seconds</b>.</> : <>show <b>{state}</b> for <b>{duration / 1000} seconds</b> on the status-LED.</>/*NF*/,

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
                14: "Cron"
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
            "energy_meter_type": "Energy meter type",
            "gpio_names_0": "current configuration 0, motor fault, DC fault, current configuration 1",
            "gpio_names_1": "DC fault protector test, shutdown, button, CP-PWM",
            "gpio_names_2": "motor input switch, contactor, GP output, CP separation",
            "gpio_names_3": "motor active, motor phase, contactor check before, contactor check behind",
            "gpio_names_4": "GP input, DC X6, DC X30, LED",
            "gpio_names_5": "unused",
            "gpio_shutdown_muted": <><a href="{{{manual_url}}}">see manual for details</a></>,
            "gpio_shutdown_not_configured": "Not configured",
            "gpio_shutdown_on_open": "Shut down on open",
            "gpio_shutdown_on_close": "Shut down on close",
            "not_configured": "Not configured",
            "active_low_blocked": "Blocks if closed",
            "active_low_prefix": "Limits charge current to ",
            "active_low_suffix": " if closed",
            "active_high_blocked": "Blocks if open",
            "active_high_prefix": "Limits charge current to ",
            "active_high_suffix": " if open",

            "todo": "Have a feature request? Write an e-mail to info@tinkerforge.com",
            "gpio_in_muted": "Readable on GPIO 16",
            "gpio_out_muted": <><a href="https://en.wikipedia.org/wiki/Open_collector">open collector</a></>,
            "button_configuration_muted": "Action to be executed when the button is pressed",
            "button_configuration_deactivated": "No action",
            "button_configuration_start_charging": "Start charging",
            "button_configuration_stop_charging": "Stop charging",
            "button_configuration_start_and_stop_charging": "Start or stop charging",

            "ev_wakeup_desc": "EV Wakeup",
            "ev_wakeup_desc_muted": <><a href="{{{manual_url}}}">see manual for details</a></>,
            "ev_wakeup": "Attempts to wake up the charge controller of the connected vehicle by fake-un and -re-plugging the charge cable.",

            "dc_fault_current_state": "DC fault protector state",
            "dc_fault_current_state_desc": "",
            "dc_fault_current_ok": "OK",
            "dc_fault_current_6_ma": "DC fault",
            "dc_fault_current_system": "System error",
            "dc_fault_current_unknown": "Unknown error",
            "dc_fault_current_calibration": /*SFN*/ (dc_fault_state: number, dc_fault_pins: number) => "Calibration error" + (dc_fault_state != 0 ? dc_fault_pins.toString() : "")/*NF*/,
            "dc_fault_current_reset": "Reset",
            "dc_fault_current_20_ma": "AC fault",
            "dc_fault_current_6_ma_20_ma": "DC+AC fault",

            "reset_dc_fault_title": "Reset the DC fault protector",
            "reset_dc_fault_content": <>Resetting the DC fault protector restores the ability to charge. <b>Ensure that the reason why the DC fault protector triggered is resolved!</b> <a href="{{{manual_url}}}">See manual for details.</a> Really reset the DC fault protector?</>,
            "abort": "Abort",
            "reset": "Reset",
            "trigger_dc_fault_test": "Test DC fault protector",
            "time_since_dc_fault_check": "Time since last DC fault protector test",
            "cron_sd_trigger_text": /*FFN*/(state: boolean) => <>If the shutdown input switches to <b>{state ? "open" : "closed"}</b>{" "}</>/*NF*/,
            "cron_gpin_trigger_text": /*FFN*/(state: boolean) => <>If the General Purpose input switches to <b>{state ? "open" : "closed"}</b>{" "}</>/*NF*/,
            "cron_button_trigger_text": /*FFN*/(state: boolean) => <>If the button is <b>{state ? "pressed" : "released"}</b>{" "}</>/*NF*/,
            "cron_gpout_action_text": /*FFN*/(state: number) => state ? <>set general purpose output to <b>high impedance</b>.</> : <><b>connect</b> general purpose output <b>to ground</b>.</>/*NF*/,

            // EVSE version specific value for common placeholder
            "error_2": /*SFN*/(is_evse_v2: boolean) => is_evse_v2 ? "DC fault protector error" : "Calibration error"/*NF*/,
            "adc_names": /*FFN*/(is_evse_v2: boolean) => is_evse_v2 ? <>CP/PE before resistor (PWM high), CP/PE after resistor (PWM high)<br/>CP/PE before resistor (PWM low), CP/PE after resistor (PWM low)<br/>PP/PE, +12V rail<br/>-12V rail</> : <>CP/PE, PP/PE</>/*NF*/,
            "voltage_names": /*FFN*/(is_evse_v2: boolean) => is_evse_v2 ? <>CP/PE before resistor (PWM high), CP/PE after resistor (PWM high)<br/>CP/PE before resistor (PWM low), CP/PE after resistor (PWM low)<br/>PP/PE, +12V rail<br/>-12V rail</> : <>CP/PE, PP/PE,<br/> CP/PE (high)</>/*NF*/,
            "dc_fault_sensor_type": "DC fault protector version",
            "dc_fault_pins": "DC fault protector pins"
        },
        "script": {
            "error_code": "Error code",
            "set_charging_current_failed": "Failed to set charging current",
            "start_charging_failed": "Failed to start charging",
            "stop_charging_failed": "Failed to stop charging",
            "tab_close_warning": "Charge log capture will be aborted if this tab is closed.",
            "loading_debug_report": "Loading debug report",
            "loading_debug_report_failed": "Loading debug report failed",
            "loading_event_log": "Loading event log",
            "loading_event_log_failed": "Loading event log failed",
            "starting_debug": "Starting charge log capture",
            "starting_debug_failed": "Starting charge log capture failed.",
            "debug_running": "Capturing. Don't close tab!",
            "debug_stop_failed": "Stopping charge log capture failed",
            "debug_stopped": "Stopped charge log capture",
            "debug_done": "Done",

            "acc_blocked": "Blocked",
            "by": "by",

            "save_failed": "Failed to save charge settings.",

            "reset_slot_failed": "Releasing the current limit failed",

            "slot_disabled": "Not active",
            "slot_blocks": "Blocks",
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
                14: "cron"
            }[i];}/*NF*/,

            "reboot_content_changed": "charge settings",

            // EVSE V1 only
            "user_calibration_upload_failed": "Updating the calibration failed",

            // EVSE V2 only
            "reset_dc_fault_current_failed": "Reset of the DC fault protector failed",
            "gpio_configuration_failed": "Updating the GPIO configuration failed",

            "meter_type_0": "No energy meter found",
            "meter_type_1": "SDM72",
            "meter_type_2": "SDM630",
            "meter_type_3": "SDM72V2",
            "meter_type_254": "internal"
        }
    }
}
