/** @jsxImportSource preact */
import { h } from "preact";
import * as options from "../../options";
import { __, removeUnicodeHacks } from "../../ts/translation";
let x = {
    "charge_manager": {
        "status": {
            "charge_manager": null,
            "not_configured": "Deaktiviert",
            "manager": "Aktiviert",
            "error": "Fehler",
            "managed_boxes": "Kontrollierte Wallboxen",
            "available_current": "Verfügbarer Strom",
            "mode": "Lademodus",
            "mode_fast": "Schnell",
            "mode_off": "Aus",
            "mode_pv": "PV",
            "mode_min_pv": "Min + PV",
            "mode_default": "Standardmodus",
            "mode_min": "Min",
            "mode_eco": "Eco",
            "mode_eco_pv": "Eco + PV",
            "mode_eco_min": "Eco + Min",
            "mode_eco_min_pv": "Eco + Min + PV",
            "mode_by_index": /*SFN*/ (mode: number, default_mode?: number) => {
                const modes = [
                    __("charge_manager.status.mode_fast"),
                    __("charge_manager.status.mode_off"),
                    __("charge_manager.status.mode_pv"),
                    __("charge_manager.status.mode_min_pv"),
                    __("charge_manager.status.mode_default"),
                    __("charge_manager.status.mode_min"),
                    __("charge_manager.status.mode_eco"),
                    __("charge_manager.status.mode_eco_pv"),
                    __("charge_manager.status.mode_eco_min"),
                    __("charge_manager.status.mode_eco_min_pv"),
                ];

                if (mode == 4 && default_mode !== undefined) {
                    return modes[mode] + " (" + modes[default_mode] + ")";
                }
                return modes[mode];
            }/*NF*/
        },
        "navbar": {
            "charge_manager_settings": "Lastmanagement",
            "charge_manager_chargers": "Wallboxen"
        },
        "content": {
            "charge_manager_settings": "Lastmanagement",
            "charge_manager_chargers": "Wallboxen",
            "enable_charge_manager": "Fremdsteuerung",
            "enable_charge_manager_help": /*FFN*/(has_managed_mode: boolean) => <>
                <p>Auf dieser Seite werden die kontrollierten Wallboxen konfiguriert. Die hier vorgenommenen Einstellungen beeinflussen das {options.WARP_DOC_BASE_URL.length > 0 ? <a href={removeUnicodeHacks(`${options.WARP_DOC_BASE_URL}/docs/tutorials/chargemanagement`)}>Lastmanagement</a> : <span>Lastmanagement</span>} zwischen den Wallboxen.</p>
                <ul class="mb-0">
                    <li><strong>Deaktiviert:</strong> Es findet kein Lastmanagement statt. {__("This_device")} ist vollständig eigenständig.</li>
                    {has_managed_mode ?
                        <li><strong>Fremdgesteuert:</strong> {__("This_device")} wird von einem anderen Lastmanager verwaltet. Es müssen keine weiteren Einstellungen vorgenommen werden.</li>
                        : undefined}
                    <li><strong>Lastmanager / PV-Überschussladen:</strong> {__("This_device")} arbeitet als Lastmanager. Dies ist auch der Fall, wenn sie nur eigenständig die Funktion {options.WARP_DOC_BASE_URL.length > 0 ? <a href={removeUnicodeHacks(`${options.WARP_DOC_BASE_URL}/docs/tutorials/pv_excess_charging`)}>PV-Überschussladen</a> : <span>PV-Überschussladen</span>} ausführen soll. Es sind weitere Einstellungen vorzunehmen.</li>
                </ul>
            </>/*NF*/,
            "enable_watchdog": "Watchdog aktiviert",
            "enable_watchdog_muted": "nur bei API-Benutzung aktivieren (für den normalen Lastmanagement-Betrieb nicht notwendig!)",
            "enable_watchdog_desc": "Setzt den verfügbaren Strom auf die Voreinstellung, wenn er nicht spätestens alle 30 Sekunden aktualisiert wurde",
            "verbose": "Stromverteilungsprotokoll aktiviert",
            "verbose_desc": "Erzeugt Einträge im Ereignis-Log, wenn Strom umverteilt wird",
            "default_available_current": "Voreingestellt verfügbarer Strom",
            "default_available_current_muted": "wird nach Neustart des Lastmanagers verwendet, falls dynamisches Lastmanagement nicht aktiviert ist",
            "default_available_current_invalid": "Der voreingestellt verfügbare Strom darf höchstens so groß sein wie der maximale Gesamtstrom.",
            "maximum_available_current": "Maximaler Gesamtstrom",
            "maximum_available_current_muted": "Maximal zulässiger Ladestrom aller Wallboxen in Summe; üblicherweise die Nennbelastbarkeit der gemeinsamen Zuleitung.",
            "minimum_current_auto": "Automatischer minimaler Ladestrom",
            "minimum_current_auto_desc": "Minimaler Ladestrom wird abhängig vom gewählten Fahrzeugmodell eingestellt.",
            "minimum_current_vehicle_type": "Fahrzeugmodell",
            "minimum_current_vehicle_type_other": "Standard",
            "minimum_current_vehicle_type_zoe": "Renault ZOE R135, ZOE R110, Twingo Z.E. oder Smart ED/EQ",
            "minimum_current_1p": "Minimaler einphasiger Ladestrom",
            "minimum_current_1p_muted": "geringster Ladestrom, der vom Fahrzeug für einphasiges Laden unterstützt wird",
            "minimum_current_3p": "Minimaler dreiphasiger Ladestrom",
            "minimum_current_3p_muted": "geringster Ladestrom, der vom Fahrzeug für dreiphasiges Laden unterstützt wird",

            "requested_current_margin": "Spielraum des Phasenstroms",
            "requested_current_margin_muted": "",
            "requested_current_threshold": "Länge der Startphase",
            "requested_current_threshold_muted": "Wallboxen mit einem Stromzähler, der Phasenströme misst, werden nach Ablauf der Startphase auf den größten Phasenstrom plus den konfigurierten Spielraum limitiert. Damit kann der verfügbare Strom effizienter auf mehrere Wallboxen verteilt werden.",

            "configuration_mode": "Experteneinstellungen",
            "configuration_mode_muted": "",

            "table_charger_name": "Anzeigename",
            "table_charger_host": "Host",
            "table_charger_rotation": "Phasenrotation",

            "add_charger_title": "Wallbox hinzufügen",
            "add_charger_invalid_feedback": "Es muss mindestens eine Wallbox konfiguriert werden.",
            "add_charger_name": "Anzeigename",
            "add_charger_host": "Hostname oder IP-Adresse",
            "add_charger_found": "Gefundene Wallboxen",
            "add_charger_message": /*SFN*/(have: number, max: number) => `${have} von ${max} Wallboxen konfiguriert`/*NF*/,
            "add_charger_rotation": "Phasenrotation",
            "charger_rotation_help": <>
                <p>Gibt an, wie die Wallbox in Relation zum Netzanschluss- bzw. PV-Überschuss-Zähler oder zu den anderen Wallboxen angeschlossen ist. Typischerweise werden nur rechtsdrehende Phasenrotationen verwendet.</p>
                <p>Eine Wallbox, die, wenn sie einphasig lädt, die Netzanschlussphase L2 belastet, ist dann beispielsweise mit der Phasenrotation L231 angeschlossen.</p>
                <p>Wenn die Phasenrotation aller oder auch nur eines Teils der gesteuerten Wallboxen bekannt ist, können mehr Fahrzeuge parallel geladen werden und PV- und Netzanschlusslimits besser ausgereizt werden: Eine Wallbox mit unbekannter Phasenrotation wird, wenn sie einphasig lädt, vom Lastmanagement so behandelt als ob sie alle drei Phasen belasten würde.</p>
            </>,
            "rotation_0": "Unbekannt",
            "rotation_1": "L123",
            "rotation_2": "L132",
            "rotation_3": "L231",
            "rotation_4": "L213",
            "rotation_5": "L321",
            "rotation_6": "L312",
            "rotation_right": "Rechtsdrehend",
            "rotation_left": "Linksdrehend",

            "edit_charger_title": "Wallbox bearbeiten",
            "edit_charger_name": "Anzeigename",
            "edit_charger_host": "Hostname oder IP-Adresse",
            "edit_charger_rotation": "Phasenrotation",

            "multi_broadcast_modal_title": "Reservierte, Multi- oder Broadcast IP-Adresse(n) gefunden",
            "multi_broadcast_modal_body": "Folgende reservierte, Multi- oder Broadcast-IP-Adressen wurden gefunden: ",
            "multi_broadcast_modal_body_end": "Sind sie sicher, dass sie die Einstellungen übernehmen wollen?",
            "multi_broadcast_modal_save": "Übernehmen",
            "multi_broadcast_modal_cancel": "Abbrechen",

            "scan_error_1": "Firmwareversion inkompatibel",
            "scan_error_2": "Fremdsteuerung deaktiviert",

            "mode_disabled": "Deaktiviert",
            "mode_manager": "Lastmanager / PV-Überschussladen",
            "mode_manager_em_with_ps": "Lastmanager / PV-Überschussladen / Phasenumschaltung / fremdgesteuert von EVCC",
            "mode_managed": "Fremdgesteuert",
            "mode_managed_em_with_ps": "Fremdgesteuert von WARP Charger oder WARP Energy Manager / nur Phasenumschaltung",

            "managed_boxes": "Kontrollierte Wallboxen",

            "host_exists": "Host existiert bereits",

            "charge_manager_chargers_reset_modal_text": "Zurücksetzen der Liste der kontrollierten Wallboxen löscht auch die Lastmanagement-Konfiguration. Fortfahren?",
            "charge_manager_settings_reset_modal_text": "Zurücksetzen der Lastmanagement-Konfiguration löscht auch die Liste der kontrollierten Wallboxen. Fortfahren?",

            "header_load_management": "Dynamisches Lastmanagement",
            "dlm_enabled": "Dynamisches Lastmanagement aktiviert",
            "dlm_enabled_desc": "Regelt die kontrollierten Wallboxen so, dass der maximal zulässige Strom des Netzanschlusses auf keiner Phase überschritten wird.",
            "dlm_meter_slot_grid_currents": "Stromzähler",
            "dlm_meter_slot_grid_currents_none": "Kein Stromzähler konfiguriert",
            "dlm_meter_slot_grid_currents_missing_values": "Phasenströme fehlen; Bezug minus Einspeisung benötigt",
            "dlm_meter_slot_grid_currents_single_phase": "nur einphasig, L1",
            "dlm_meter_slot_grid_currents_two_phase": "nur zweiphasig, L1 + L2",
            "dlm_current_limit": "Maximaler Strom am Netzanschluss",
            "dlm_current_limit_muted": "Nennwert der Absicherung",
            "dlm_largest_consumer_current": "Strombedarf des größten Einzelverbrauchers",
            "dlm_largest_consumer_current_muted": "pro Phase",
            "dlm_largest_consumer_current_help": "Gibt den größten zu erwartenden plötzlichen Sprung des Strombezugs am Zähler an, den das dynamische Lastmanagement kurzfristig (< 30 Sekunden) kompensieren können muss. Dieser kann beispielsweise durch einen Durchlauferhitzer oder eine Wärmepumpe erzeugt werden. Die vom Lastmanagement gesteuerten Wallboxen müssen nicht berücksichtigt werden.",
            "dlm_safety_margin_pct": "Zusätzliche Sicherheitsmarge",
            "dlm_safety_margin_pct_muted": "Typischerweise ist keine zusätzliche Marge notwendig.",
            "dlm_safety_margin_pct_help": "Das Lastmanagement berechnet eine Sicherheitsmarge basierend auf dem maximalen Strom am Netzanschluss und dem Strombedarf des größten Einzelverbrauchers. Mit dieser Einstellung kann die Sicherheitsmarge über den berechneten Wert hinaus erhöht werden.",

            "managed_disabled": <>Damit {__("this_device")} als Lastmanager genutzt werden kann, muss die <a href="#charge_manager_chargers">{__("charge_manager.content.enable_charge_manager")}</a> auf „{__("charge_manager.content.mode_manager")}“ konfiguriert sein.</>,

            "charge_manager_debug": "Debug",
            "debug_description": "Protokoll erstellen",
            "debug_description_muted": "zur Diagnose bei Problemen",
            "debug_start": "Start",
            "debug_stop": "Stop + Download",

            "internal_state": "Interner Zustand",

            "peak_current": "Kurzzeitig erlaubter Spitzenstrom",
            "peak_current_muted": "Überlast, die das Lastmanagement innerhalb von 30 Sekunden kompensiert. Entspricht 140 % des maximalen Stroms am Netzanschluss.",

            "expected_peak_current": "Erwarteter Spitzenstrom",
            "expected_peak_current_muted": "Kurzzeitig erwarteter Spitzenstrom unter Berücksichtigung der zusätzlichen Sicherheitsmarge.",

            "target_constant_current": "Zielstrom",
            "target_constant_current_muted": "Wallboxen werden limitiert, wenn dieser Strom überschritten wird, damit der größte Verbraucher nicht den erwarteten Spitzenstrom überschreitet.",

            "em_controlled_charger": "Wallbox für Phasenumschaltung",
            "em_controlled_charger_muted": "Die Wallbox, die mit dem Schütz dieses Energy Managers verbunden ist.",
            "em_no_ps_charger": "Keine",
            "em_proxy_warning_not_enough": "Die eine Wallbox, für die der Energy Manager Phasenumschaltungen durchführt, muss hier konfiguriert werden.",
            "em_proxy_warning_too_many": "Der Energy Manager kann Phasenumschaltungen nur für eine Wallbox durchführen und es dürfen keine weiteren Wallboxen am Schütz des Energy Managers angeschlossen sein."
        },
        "automation": {
            "charge_manager_wd": "Lastmanagement-Watchdog ausgelöst",
            "automation_trigger_text": <>Wenn der <b>Watchdog</b> des <b>Lastmanagements</b> auslöst, </>,
            "set_charge_manager": "Setze verfügbaren Strom für Lastmanagement",
            "automation_action_text": /*FFN*/(current: string) => <>setze den für das <b>Lastmanagement</b> verfügbaren Strom auf <b>{current} A</b>.</> /*NF*/,
            "max_current": "Verfügbarer Strom",

            "charge_mode_switch": "Wechsle Lademodus",
            "charge_mode": "Lademodus",
            "charge_mode_switch_action_text": /*FFN*/(mode: number, default_mode: number) => {
                let mode_str = __("charge_manager.status.mode_by_index")(mode, default_mode);
                return <>wechsle Lademodus auf <b>{mode_str}</b>.</>
            }/*NF*/
        },
        "script": {
            "charge_state_0": "Kein Fahrzeug angeschlossen",
            "charge_state_1": "Warte auf lokale Freigabe",
            "charge_state_2": "Warte auf Manager-Freigabe",
            "charge_state_3": "Laden freigegeben",
            "charge_state_4": "Lädt",
            "charge_state_5": "Fehler",
            "charge_state_6": "Laden abgeschlossen",

            "charge_error_type_management": "Managementfehler",
            "charge_error_type_client": "Wallbox-Fehler",

            "charge_error_0": "OK",
            "charge_error_1": "Kommunikationsfehler",
            "charge_error_2": "Paketheader ungültig oder Firmware inkompatibel",
            "charge_error_3": "Fremdsteuerung deaktiviert",
            "charge_error_128": "Wallbox nicht erreichbar",
            "charge_error_129": "Ladecontroller nicht erreichbar",
            "charge_error_130": "Ladecontroller reagiert nicht",

            "charge_error_192": "OK",
            "charge_error_194": "Schalterfehler",
            "charge_error_195": "DC-Fehlerstromschutzfehler",
            "charge_error_196": "Schützfehler",
            "charge_error_197": "Fahrzeug-Kommunikationsfehler",

            "charge_state_blocked_by_other_box": "Blockiert",
            "charge_state_blocked_by_other_box_details": "Fehler bei anderer Wallbox",

            "ampere_allocated": "zugeteilt",
            "ampere_supported": "unterstützt",

            "last_update_prefix": "Gestört seit ",
            "last_update_suffix": "",
            "save_failed": "Speichern der Lastmanager-Einstellungen fehlgeschlagen.",
            "set_available_current_failed": "Setzen des verfügbaren Stroms fehlgeschlagen",

            "reboot_content_changed": "Lastmanagement-Einstellungen",

            "scan_failed": "Scan fehlgeschlagen",

            "mode_explainer_0": <>{__("This_device")} ist nicht Teil eines Lastmanagement-Verbunds mit anderen WARP Chargern und/oder einem WARP Energy Manager. PV-Überschussladen ist deaktiviert.</>,
            "mode_explainer_1": <>{__("This_device")} ist Teil eines Lastmanagement-Verbunds mit anderen WARP Chargern und/oder einem WARP Energy Manager. Ein anderes Gerät steuert diesen Lastmanagement-Verbund, um sicherzustellen, dass nie mehr als der verfügbare Strom bezogen wird.</>,
            "mode_explainer_2": <>{__("This_device")} verwendet entweder das PV-Überschussladen oder steuert einen Lastmanagement-Verbund mit anderen WARP Chargern um sicherzustellen, dass nie mehr als der verfügbare Strom bezogen wird.</>,

            "mode_explainer_0_em": <>{__("This_device")} steuert keine Wallboxen.</>,
            "mode_explainer_2_em": <>{__("This_device")} steuert eine oder mehrere Wallboxen.</>,

            "mode_explainer_0_em_with_ps": <>{__("This_device")} steuert keine Wallboxen. Phasenumschaltung ist deaktiviert.</>,
            "mode_explainer_1_em_with_ps": <>{__("This_device")} wird von einem anderen WARP Energy Manager oder WARP Charger gesteuert und führt Phasenumschaltungen für eine einzige mit seinem Schütz verbundene Wallbox durch.</>,
            "mode_explainer_2_em_with_ps": <>{__("This_device")} steuert einen oder mehrere WARP Charger und kann Phasenumschaltungen für eine einzige mit seinem Schütz verbundene Wallbox durchführen.</>,

            "mode_change_failed": "Wechsel des Lademodus fehlgeschlagen."
        }
    }
}
