/* esp32-firmware
 * Copyright (C) 2023 Thomas Hein
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
import * as util from "../../ts/util";
import * as API from "../../ts/api";
import { h, render, Fragment, Component } from "preact";
import { __ } from "../../ts/translation";
import { FormRow } from "../../ts/components/form_row";
import { IndicatorGroup  } from "../../ts/components/indicator_group";

export class EvseCPCStatus extends Component {
    override render() {
        if (!util.render_allowed() || !API.hasFeature("cp_disconnect")) {
            return <></>;
        }

        let state = API.get('evse_cpc/state');

        return <>
            <FormRow label={__("evse_cpc.status.cp")} labelColClasses="col-lg-4" contentColClasses="col-lg-8 col-xl-4">
                <IndicatorGroup
                    style="width: 100%"
                    class="flex-wrap"
                    value={state.connected ? 1 : 0}
                    items={[
                        ["warning", __("evse_cpc.status.disconnected")],
                        ["success", __("evse_cpc.status.connected")]
                    ]}
                />
            </FormRow>
        </>;
    }
}

export function init() {
}

render(<EvseCPCStatus />, $("#status-evse_cpc")[0]);

export function add_event_listeners(source: API.APIEventTarget) {
}

export function update_sidebar_state(module_init: any) {
    $("#sidebar-evse_cpc").prop("hidden", !module_init.evse_cpc);
}
