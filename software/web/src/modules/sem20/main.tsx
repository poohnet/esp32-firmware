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

import { h, render, createRef, Fragment, Component } from "preact";
import { __, translate_unchecked } from "../../ts/translation";
import { PageHeader } from "../../ts/components/page_header";
import { FormRow } from "../../ts/components/form_row";
import { FormSeparator } from "../../ts/components/form_separator";
import { OutputFloat } from "src/ts/components/output_float";
import { CollapsedSection } from "src/ts/components/collapsed_section";
import { InputSelect } from "src/ts/components/input_select";

import { UplotData, UplotWrapper, calculate_live_data, calculate_history_data, array_append } from "./meter";

interface section {
    id: 1|2|3,
    name: string
}

function section(id: 1|2|3, name: string) : section {
    return {id: id, name: translate_unchecked(`sem20.content.${name}`)};
}

const ID_CONSUMPTION    = 1;
const ID_FEEDIN         = 2;
const ID_OTHER          = 3;

const sections: section[] = [
    section(ID_CONSUMPTION, "consumption"),
    section(ID_FEEDIN,      "feedin"), 
    section(ID_OTHER,       "other")
];


interface entry {
    i: number,
    sectionId: number,
    cols: 1|3|4,
    name: string,
    desc: string,
    digits: 0|1|2|3,
    unit: string,
}

function entry(sectionId: number, cols: 1|3|4, name: string, desc: string, unit: string, digits: 0|1|2|3) : entry {
    return {i: 0, sectionId: sectionId, cols: cols, name: translate_unchecked(`sem20.content.${name}`), desc: translate_unchecked(`sem20.content.${desc}`), unit: unit, digits: digits}
}

const ID_ACTIVE         = "active";
const ID_REACTIVE       = "reactive";
const ID_APPARENT       = "apparent";
const ID_POWERFACTOR    = "powerfactor";
const ID_VOLTAGE        = "voltage";
const ID_CURRENT        = "current";
const ID_FREQUENCY      = "frequency";
const ID_AVERAGE        = "average";
const ID_ENERGY         = "energy";
const ID_UNIT_W         = "W";
const ID_UNIT_KWH       = "kWh";
const ID_UNIT_V         = "V";
const ID_UNIT_A         = "A";
const ID_UNIT_HZ        = "Hz";

const entries: entry[] = [
    entry(ID_CONSUMPTION,   4, ID_ACTIVE,        ID_AVERAGE,  ID_UNIT_W,    0),
    entry(ID_CONSUMPTION,   4, ID_ACTIVE,        ID_ENERGY,   ID_UNIT_KWH,  3),
    entry(ID_CONSUMPTION,   4, ID_REACTIVE,      ID_AVERAGE,  ID_UNIT_W,    0),
    entry(ID_CONSUMPTION,   4, ID_REACTIVE,      ID_ENERGY,   ID_UNIT_KWH,  3),
    entry(ID_CONSUMPTION,   4, ID_APPARENT,      ID_AVERAGE,  ID_UNIT_W,    0),
    entry(ID_CONSUMPTION,   4, ID_APPARENT,      ID_ENERGY,   ID_UNIT_KWH,  3),
    entry(ID_FEEDIN,        4, ID_ACTIVE,        ID_AVERAGE,  ID_UNIT_W,    0),
    entry(ID_FEEDIN,        4, ID_ACTIVE,        ID_ENERGY,   ID_UNIT_KWH,  3),
    entry(ID_FEEDIN,        4, ID_REACTIVE,      ID_AVERAGE,  ID_UNIT_W,    0),
    entry(ID_FEEDIN,        4, ID_REACTIVE,      ID_ENERGY,   ID_UNIT_KWH,  3),
    entry(ID_FEEDIN,        4, ID_APPARENT,      ID_AVERAGE,  ID_UNIT_W,    0),
    entry(ID_FEEDIN,        4, ID_APPARENT,      ID_ENERGY,   ID_UNIT_KWH,  3),
    entry(ID_OTHER,         4, ID_POWERFACTOR,   ID_AVERAGE,  "",           2),
    entry(ID_OTHER,         3, ID_VOLTAGE,       ID_AVERAGE,  ID_UNIT_V,    2),
    entry(ID_OTHER,         3, ID_CURRENT,       ID_AVERAGE,  ID_UNIT_A,    2),
    entry(ID_OTHER,         1, ID_FREQUENCY,     ID_AVERAGE,  ID_UNIT_HZ,   2),
];


interface SEM20State {
    values: Readonly<API.getType['sem20/values']>;
    all_values: Readonly<API.getType['sem20/all_values']>;
    chart_selected: "history"|"live";
}

export class SEM20 extends Component<{}, SEM20State> {
    live_data: UplotData;
    pending_live_data: UplotData = {timestamps: [], samples: []};
    history_data: UplotData;
    uplot_wrapper_ref = createRef();

    constructor() {
        super();

        let i = 0;

        for (let e of entries) {
            e.i = i;
            i += e.cols;
        }

        util.addApiEventListener("sem20/values", () => {
            this.setState({values: API.get("sem20/values")});
        });

        util.addApiEventListener("sem20/all_values", () => {
            this.setState({all_values: API.get("sem20/all_values")});
        });

        util.addApiEventListener("sem20/live", () => {
            let live = API.get("sem20/live");

            this.live_data = calculate_live_data(live.offset, live.samples_per_second, live.samples);
            this.pending_live_data = {timestamps: [], samples: []};

            if (this.state.chart_selected == "live") {
                this.update_uplot(this.live_data);
            }
        });

        util.addApiEventListener("sem20/live_samples", () => {
            let live = API.get("sem20/live_samples");
            let live_extra = calculate_live_data(0, live.samples_per_second, live.samples);

            this.pending_live_data.timestamps.push(...live_extra.timestamps);
            this.pending_live_data.samples.push(...live_extra.samples);

            if (this.pending_live_data.samples.length >= 5) {
                this.live_data.timestamps = array_append(this.live_data.timestamps, this.pending_live_data.timestamps, 720);
                this.live_data.samples = array_append(this.live_data.samples, this.pending_live_data.samples, 720);

                this.pending_live_data.timestamps = [];
                this.pending_live_data.samples = [];

                if (this.state.chart_selected == "live") {
                    this.update_uplot(this.live_data);
                }
            }
        });

        util.addApiEventListener("sem20/history", () => {
            let history = API.get("sem20/history");

            this.history_data = calculate_history_data(history.offset, history.samples);

            if (this.state.chart_selected == "history") {
                this.update_uplot(this.history_data);
            }
        });

        util.addApiEventListener("sem20/history_samples", () => {
            let history = API.get("sem20/history_samples");

            this.history_data = calculate_history_data(0, array_append(this.history_data.samples, history.samples, 720));

            if (this.state.chart_selected == "history") {
                this.update_uplot(this.history_data);
            }
        });

        this.state = {
            chart_selected: "history",
        } as any;
    }

    update_uplot(data: UplotData) {
        if (!this.uplot_wrapper_ref || !this.uplot_wrapper_ref.current) {
            return;
        }

        this.uplot_wrapper_ref.current.set_data(data);
    }

    render(props: {}, state: Readonly<SEM20State>) {
        if (!util.allow_render) {
            return (<></>);
        }

        return (
            <>
                <PageHeader title={__("sem20.content.sem20")} colClasses="col-xl-10"/>

                <div class="row">
                    <div class="col-lg-6">
                        <div class="row mb-3 pt-3 pr-0 pr-lg-3">
                            <div class="d-flex justify-content-between align-items-center border-bottom col">
                                <span class="h3">{__("sem20.status.power_history")}</span>
                                <div class="mb-2">
                                    <InputSelect value={this.state.chart_selected} onValue={
                                        (v) => {
                                            let chart_selected: "live"|"history" = v as any;

                                            this.setState({chart_selected: chart_selected});
                                            this.update_uplot(this[`${chart_selected}_data`]);
                                        }
                                    }
                                        items={[
                                            ["history", __("sem20.content.history")],
                                            ["live", __("sem20.content.live")],
                                        ]}
                                    />
                                </div>
                            </div>
                        </div>

                        <UplotWrapper ref={this.uplot_wrapper_ref} id="sem20_chart" class="meter-chart" sidebar_id="sem20" y_diff_min={100} y_step={10}/>
                    </div>

                    <div class="col-lg-6 col-xl-4">
                        <div class="row mb-3 pt-3">
                            <div class="d-flex justify-content-between align-items-center border-bottom col">
                                <span class="h3">{__("sem20.content.statistics")}</span>
                                <div class="mb-2" style="visibility: hidden;">
                                    <InputSelect items={[["a", "a"]]} />
                                </div>
                            </div>
                        </div>

                        <FormRow label={__("sem20.content.active")} labelColClasses="col-sm-6" contentColClasses="col-sm-6">
                            <OutputFloat value={this.state.values.power} digits={0} scale={0} unit="W"/>
                        </FormRow>
                    </div>
                </div>

                <CollapsedSection colClasses="col-xl-10" label="Details">
                {
                    sections.map(s =>
                        <>
                            <FormSeparator heading={s.name}/> 
                            <FormRow label="">
                                <div class="row mx-n1">
                                    <div class="mb-1 col px-1 text-center">
                                        <h5>{__("sem20.content.l1")}</h5>
                                    </div>
                                    <div class="mb-1 col px-1 text-center">
                                        <h5>{__("sem20.content.l2")}</h5>
                                    </div>
                                    <div class="mb-1 col px-1 text-center">
                                        <h5>{__("sem20.content.l3")}</h5>
                                    </div>
                                    <div class="mb-1 col px-1 text-center">
                                        <h5>{__("sem20.content.total")}</h5>
                                    </div>
                                </div>
                            </FormRow>
                            {
                                entries.filter(e => e.sectionId == s.id).map(e =>
                                    <FormRow label={e.name} label_muted={e.desc}>
                                        <div class="row mx-n1">
                                            <div class="mb-1 col px-1"> {
                                                e.cols >= 1 ? <OutputFloat value={this.state.all_values[e.i+0]} digits={e.digits} scale={0} unit={e.unit}/> : <div></div>
                                            }
                                            </div>
                                            <div class="mb-1 col px-1"> {
                                                e.cols >= 2 ? <OutputFloat value={this.state.all_values[e.i+1]} digits={e.digits} scale={0} unit={e.unit}/> : <div></div>
                                            }
                                            </div>
                                            <div class="mb-1 col px-1"> {
                                                e.cols >= 3 ? <OutputFloat value={this.state.all_values[e.i+2]} digits={e.digits} scale={0} unit={e.unit}/> : <div></div>
                                            }
                                            </div>
                                            <div class="mb-1 col px-1"> {
                                                e.cols >= 4 ? <OutputFloat value={this.state.all_values[e.i+3]} digits={e.digits} scale={0} unit={e.unit}/> : <div></div>
                                            }
                                            </div>
                                        </div>
                                    </FormRow>
                                )
                            }
                        </>
                    )
                }
                </CollapsedSection>
            </>
        )
    }
}

render(<SEM20 />, $('#sem20')[0]);

export function init() {}

export function add_event_listeners(source: API.APIEventTarget) {}

export function update_sidebar_state(module_init: any) {
    $('#sidebar-sem20').prop('hidden', !module_init.sem20);
}
