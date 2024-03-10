/* esp32-firmware
 * Copyright (C) 2024 Thomas Hein
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
import { h, Component } from "preact";
import { Button, ButtonGroup } from "react-bootstrap";
import { __ } from "../../ts/translation";
import { FormRow } from "../../ts/components/form_row";
import { IndicatorGroup  } from "../../ts/components/indicator_group";
import { StatusSection } from "../../ts/components/status_section";
import { CheckCircle, Circle } from "react-feather";

export class PhaseSwitcherStatus extends Component {
    render() {
        if (!util.render_allowed() || !API.hasFeature("phase_switcher")) {
            return <StatusSection name="phase_switcher" />;
        }

        let state = API.get('phase_switcher/state');

        return <StatusSection name="phase_switcher">
            <FormRow label={__("phase_switcher.status.cp")} labelColClasses="col-lg-4" contentColClasses="col-lg-8 col-xl-4">
                <ButtonGroup className="flex-wrap m-n1" style="width: calc(100% + 0.5rem);">
                    <Button
                        style="display: flex;align-items: center;justify-content: center;"
                        className="m-1 rounded-left rounded-right"
                        variant={state.cp_disconnect ? "warning" : "secondary"}
                        disabled={true}>
                        {state.cp_disconnect ? <CheckCircle size="20"/> : <Circle size="20"/>} <span>&nbsp;&nbsp;</span><span>{__("phase_switcher.status.disconnected")}</span>
                    </Button>
                    <Button
                        style="display: flex;align-items: center;justify-content: center;"
                        className="m-1 rounded-left rounded-right"
                        variant={!state.cp_disconnect ? "success" : "secondary"}
                        disabled={true}>
                        {!state.cp_disconnect ? <CheckCircle size="20"/> : <Circle size="20"/>} <span>&nbsp;&nbsp;</span><span>{__("phase_switcher.status.connected")}</span>
                    </Button>
                </ButtonGroup>
            </FormRow>

            <FormRow label={__("phase_switcher.status.phases_wanted")} labelColClasses="col-lg-4" contentColClasses="col-lg-8 col-xl-4">
                <IndicatorGroup
                    style="width: 100%"
                    class="flex-wrap"
                    value={(state.phases_wanted == 1) ? 1 : (state.phases_wanted == 3) ? 2 : 0}
                    items={[
                        ["secondary", __("phase_switcher.status.none")],
                        ["success", __("phase_switcher.status.one_phase")],
                        ["success", __("phase_switcher.status.three_phase")],
                    ]}
                />
            </FormRow>

            <FormRow label={__("phase_switcher.status.phases_active")} labelColClasses="col-lg-4" contentColClasses="col-lg-8 col-xl-4">
                <IndicatorGroup
                    style="width: 100%"
                    class="flex-wrap"
                    value={(state.phases_active == 1) ? 1 : (state.phases_active == 3) ? 2 : 0}
                    items={[
                        ["secondary", __("phase_switcher.status.none")],
                        ["success", __("phase_switcher.status.one_phase")],
                        ["success", __("phase_switcher.status.three_phase")],
                    ]}
                />
            </FormRow>
        </StatusSection>;
    }
}

export function init() {
}
