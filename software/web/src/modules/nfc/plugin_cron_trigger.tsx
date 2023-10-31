/* esp32-firmware
 * Copyright (C) 2023 Frederic Henrichs <frederic@tinkerforge.com>
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

import { h, Fragment } from "preact";
import { __, translate_unchecked } from "../../ts/translation";
import { CronTriggerID } from "../cron/cron_defs";
import { CronTrigger } from "../cron/types";
import { Cron } from "../cron/main";
import { InputText } from "../../ts/components/input_text";
import { InputSelect } from "../../ts/components/input_select";
import { FormRow } from "../../ts/components/form_row";
import { ListGroup, ListGroupItem } from "react-bootstrap";
import * as API from "../../ts/api";
import * as util from "../../ts/util";

export type NfcCronTrigger = [
    CronTriggerID.NFC,
    {
        tag_type: number,
        tag_id: string
    }
];

function get_nfc_table_children(trigger: CronTrigger) {
    const value = (trigger as NfcCronTrigger)[1];
    return __("nfc.content.cron_trigger_text")(value.tag_id, translate_unchecked("nfc.content.type_" + value.tag_type))
}

function new_nfc_config(): CronTrigger {
    return [
        CronTriggerID.NFC,
        {
            tag_type: 0,
            tag_id: ""
        }
    ]
}

function get_nfc_edit_children(cron: Cron, trigger: CronTrigger) {
    const value = (trigger as NfcCronTrigger)[1];
    const tags = API.get("nfc/seen_tags");
    const known_tags = API.get("nfc/config").authorized_tags;
    const seen_tags = tags.filter(t => t.tag_id != "" && !known_tags.find(tag => t.tag_id == tag.tag_id)).map(t => <ListGroupItem action type="button" onClick={() => {
        if (t.tag_id != "") {
            value.tag_id = t.tag_id;
            value.tag_type = t.tag_type;
            cron.setTriggerFromComponent(trigger);
        }
        }}>
            <h5 class="mb-1 pr-2">{t.tag_id}</h5>
            <div class="d-flex w-100 justify-content-between">
                <span class="text-left">{translate_unchecked(`nfc.content.type_${t.tag_type}`)}</span>
                <span class="text-right">{__("nfc.content.last_seen") + util.format_timespan_ms(t.last_seen) + __("nfc.content.last_seen_suffix")}</span>
            </div>
        </ListGroupItem>);

    const users = API.get("users/config").users;
    const known_items = API.get("nfc/config").authorized_tags.map(t => <ListGroupItem action type="button" onClick={() => {
        if (t.tag_id != "") {
            value.tag_id = t.tag_id;
            value.tag_type = t.tag_type;
            cron.setTriggerFromComponent(trigger);
        }
        }}>
            <h5 class="mb-1 pr-2">{t.tag_id}</h5>
            <div class="d-flex w-100 justify-content-between">
                <span class="text-left">{translate_unchecked(`nfc.content.type_${t.tag_type}`)}</span>
                <span class="text-right">{__("nfc.content.table_user_id") + ": " + users.find(u => u.id == t.user_id).display_name}</span>
            </div>
        </ListGroupItem>);

    const all_tags = known_items.concat(seen_tags);

    return [<>
        <FormRow label={__("nfc.content.last_seen_and_known_tags")}>
            {all_tags.length > 0 ?
                <ListGroup>{all_tags}</ListGroup>
                : <span>{__("nfc.content.add_tag_description")}</span>}
        </FormRow>
        <FormRow label={__("nfc.content.table_tag_id")}>
            <InputText
                value={value.tag_id}
                required
                onValue={(v) => {
                    value.tag_id = v;
                    cron.setTriggerFromComponent(trigger);
                }}
                minLength={8} maxLength={29}
                pattern="^([0-9a-fA-F]{2}:?){3,9}[0-9a-fA-F]{2}$"
                invalidFeedback={__("nfc.content.tag_id_invalid_feedback")} />
        </FormRow>
        <FormRow label={__("nfc.content.table_tag_type")}>
            <InputSelect
                items={[
                    ["0",__("nfc.content.type_0")],
                    ["1",__("nfc.content.type_1")],
                    ["2",__("nfc.content.type_2")],
                    ["3",__("nfc.content.type_3")],
                    ["4",__("nfc.content.type_4")],
                ]}
                value={value.tag_type.toString()}
                onValue={(v) => {
                    value.tag_type = parseInt(v);
                    cron.setTriggerFromComponent(trigger);
                }} />
        </FormRow>
    </>]
}

export function init() {
    return {
        trigger_components: {
            [CronTriggerID.NFC]: {
                name: __("nfc.content.cron_trigger_nfc"),
                new_config: new_nfc_config,
                clone_config: (trigger: CronTrigger) => [trigger[0], {...trigger[1]}] as CronTrigger,
                get_edit_children: get_nfc_edit_children,
                get_table_children: get_nfc_table_children,
            },
        },
    };
}
