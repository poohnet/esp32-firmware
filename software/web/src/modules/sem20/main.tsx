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

import $ from "../../ts/jq";

import * as util from "../../ts/util";
import * as API from "../../ts/api";

import { h, render, createRef, Fragment, Component, ComponentChild } from "preact";
import { __, translate_unchecked } from "../../ts/translation";
import { PageHeader } from "../../ts/components/page_header";
import { FormRow } from "../../ts/components/form_row";
import { FormSeparator } from "../../ts/components/form_separator";
import { OutputFloat } from "src/ts/components/output_float";
import { CollapsedSection } from "src/ts/components/collapsed_section";
import { InputSelect } from "src/ts/components/input_select";
import uPlot from 'uplot';

interface SEM20State {
    values: Readonly<API.getType['sem20/values']>;
    all_values: Readonly<API.getType['sem20/all_values']>;
    chart_selected: "history"|"live";
}

interface section {
    id: number,
    name: string
}

function section(id: number, name: string) : section {
    return {id: id, name: translate_unchecked(`sem20.content.${name}`)};
}

const sections: section[] = [
    section(1, "bezug"),
    section(2, "einspeisung"), 
    section(3, "sonstiges")
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

const entries: entry[] = [
    entry(1, 4, "wirkleistung",      "aktuell",  "W",    0),
    entry(1, 4, "wirkleistung",      "zaehler",  "kWh",  3),
    entry(1, 4, "blindleistung",     "aktuell",  "W",    0),
    entry(1, 4, "blindleistung",     "zaehler",  "kWh",  3),
    entry(1, 4, "scheinleistung",    "aktuell",  "W",    0),
    entry(1, 4, "scheinleistung",    "zaehler",  "kWh",  3),
    entry(2, 4, "wirkleistung",      "aktuell",  "W",    0),
    entry(2, 4, "wirkleistung",      "zaehler",  "kWh",  3),
    entry(2, 4, "blindleistung",     "aktuell",  "W",    0),
    entry(2, 4, "blindleistung",     "zaehler",  "kWh",  3),
    entry(2, 4, "scheinleistung",    "aktuell",  "W",    0),
    entry(2, 4, "scheinleistung",    "zaehler",  "kWh",  3),
    entry(3, 4, "leistungsfaktor",   "aktuell",  "",     2),
    entry(3, 3, "spannung",          "aktuell",  "V",    2),
    entry(3, 3, "strom",             "aktuell",  "A",    2),
    entry(3, 1, "netzfrequenz",      "aktuell",  "Hz",   2),
];


interface UplotData {
    timestamps: number[];
    samples: number[];
}

interface UplotWrapperProps {
    id: string;
    class: string;
    sidebar_id: string;
    y_min?: number;
    y_max?: number;
    y_diff_min?: number;
    y_step?: number;
}

class UplotWrapper extends Component<UplotWrapperProps, {}> {
    uplot: uPlot;
    data: UplotData;
    pending_data: UplotData;
    visible: boolean = false;
    div_ref = createRef();
    observer: ResizeObserver;

    shouldComponentUpdate() {
        return false;
    }

    componentDidMount() {
        if (this.uplot) {
            return;
        }

        // FIXME: special hack for status page that is visible by default
        //        and doesn't receive an initial shown event because of that
        this.visible = this.props.sidebar_id === "status";

        // We have to use jquery here or else the events don't fire?
        // This can be removed once the sidebar is ported to preact.
        $(`#sidebar-${this.props.sidebar_id}`).on('shown.bs.tab', () => {
            this.visible = true;

            if (this.pending_data !== undefined) {
                this.set_data(this.pending_data);
            }
        });

        $(`#sidebar-${this.props.sidebar_id}`).on('hidden.bs.tab', () => {
            this.visible = false;
        });

        let get_size = () => {
            let div = this.div_ref.current;
            let aspect_ratio = parseFloat(getComputedStyle(div).aspectRatio);

            return {
                width: div.clientWidth,
                height: Math.floor(div.clientWidth / aspect_ratio),
            }
        }

        let options = {
            ...get_size(),
            pxAlign: 0,
            cursor: {
                drag: {
                    x: false, // disable zoom
                },
            },
            series: [
                {
                    label: __("sem20.script.time"),
                    value: __("sem20.script.time_legend_format"),
                },
                {
                    show: true,
                    pxAlign: 0,
                    spanGaps: false,
                    label: __("sem20.script.power"),
                    value: (self: uPlot, rawValue: number) => rawValue !== null ? rawValue + " W" : null,
                    stroke: "#007bff",
                    width: 2,
                },
            ],
            axes: [
                {
                    incrs: [
                        60,
                        60 * 2,
                        3600,
                        3600 * 2,
                        3600 * 4,
                        3600 * 6,
                        3600 * 8,
                        3600 * 12,
                        3600 * 24,
                    ],
                    // [0]:   minimum num secs in found axis split (tick incr)
                    // [1]:   default tick format
                    // [2-7]: rollover tick formats
                    // [8]:   mode: 0: replace [1] -> [2-7], 1: concat [1] + [2-7]
                    values: [
                        // tick incr  default      year                                             month  day                                               hour  min   sec   mode
                        [3600,        "{HH}:{mm}", "\n" + __("sem20.script.time_long_date_format"), null,  "\n" + __("sem20.script.time_short_date_format"), null, null, null, 1],
                        [60,          "{HH}:{mm}", null,                                            null,  null,                                             null, null, null, 1],
                    ],
                },
                {
                    size: 55,
                }
            ],
            scales: {
                y: {
                    range: {
                        min: {
                            mode: 1 as uPlot.Range.SoftMode,
                        },
                        max: {
                            mode: 1 as uPlot.Range.SoftMode,
                        },
                    },
                },
            },
            plugins: [
                {
                    hooks: {
                        setSeries: (self: uPlot, seriesIdx: number, opts: uPlot.Series) => {
                            this.update_internal_data();
                        },
                    },
                },
            ],
        };

        let div = this.div_ref.current;
        this.uplot = new uPlot(options, [], div);

        let resize = () => {
            let size = get_size();

            if (size.width == 0 || size.height == 0) {
                return;
            }

            this.uplot.setSize(size);
        };

        try {
            this.observer = new ResizeObserver(() => {
                resize();
            });

            this.observer.observe(div);
        } catch (e) {
            setInterval(() => {
                resize();
            }, 500);

            window.addEventListener("resize", e => {
                resize();
            });
        }
    }

    render(props?: UplotWrapperProps, state?: Readonly<{}>, context?: any): ComponentChild {
        return <div><div ref={this.div_ref} id={props.id} class={props.class} /></div>;
    }

    update_internal_data() {
        let y_min: number = this.props.y_min;
        let y_max: number = this.props.y_max;

        for (let i = 0; i < this.data.samples.length; ++i) {
            let value = this.data.samples[i];

            if (value !== null) {
                if (y_min === undefined || value < y_min) {
                    y_min = value;
                }

                if (y_max === undefined || value > y_max) {
                    y_max = value;
                }
            }
        }

        if (y_min === undefined && y_max === undefined) {
            y_min = 0;
            y_max = 0;
        }
        else if (y_min === undefined) {
            y_min = y_max;
        }
        else if (y_max === undefined) {
            y_max = y_min;
        }

        let y_diff_min = this.props.y_diff_min;

        if (y_diff_min !== undefined) {
            let y_diff = y_max - y_min;

            if (y_diff < y_diff_min) {
                let y_center = y_min + y_diff / 2;

                let new_y_min = Math.floor(y_center - y_diff_min / 2);
                let new_y_max = Math.ceil(y_center + y_diff_min / 2);

                if (new_y_min < 0 && y_min >= 0) {
                    // avoid negative range, if actual minimum is positive
                    y_min = 0;
                    y_max = y_diff_min;
                } else {
                    y_min = new_y_min;
                    y_max = new_y_max;
                }
            }
        }

        let y_step = this.props.y_step;

        if (y_step !== undefined) {
            y_min = Math.floor(y_min / y_step) * y_step;
            y_max = Math.ceil(y_max / y_step) * y_step;
        }

        this.uplot.setScale('y', {min: y_min, max: y_max});
        this.uplot.setData([this.data.timestamps, this.data.samples]);
    }

    set_data(data: UplotData) {
        if (!this.uplot || !this.visible) {
            this.pending_data = data;
            return;
        }

        this.data = data;
        this.pending_data = undefined;

        this.update_internal_data();
    }
}

function calculate_live_data(offset: number, samples_per_second: number, samples: number[]): UplotData {
    let data: UplotData = {timestamps: new Array(samples.length), samples: samples};
    let now = Date.now();
    let start;
    let step;

    if (samples_per_second == 0) { // implies samples.length == 1
        start = now - offset;
        step = 0;
    } else {
        // (samples.length - 1) because samples_per_second defines the gaps between
        // two samples. with N samples there are (N - 1) gaps, while the lastest/newest
        // sample is offset milliseconds old
        start = now - (samples.length - 1) / samples_per_second * 1000 - offset;
        step = 1 / samples_per_second * 1000;
    }

    for(let i = 0; i < samples.length; ++i) {
        data.timestamps[i] = (start + i * step) / 1000;
    }

    return data;
}

function calculate_history_data(offset: number, samples: number[]): UplotData {
    const HISTORY_MINUTE_INTERVAL = 4;

    let data: UplotData = {timestamps: new Array(samples.length), samples: samples};
    let now = Date.now();
    let step = HISTORY_MINUTE_INTERVAL * 60 * 1000;
    // (samples.length - 1) because step defines the gaps between two samples.
    // with N samples there are (N - 1) gaps, while the lastest/newest sample is
    // offset milliseconds old. there might be no data point on a full hour
    // interval. to get nice aligned ticks nudge the ticks by at most half of a
    // sampling interval
    let start = Math.round((now - (samples.length - 1) * step - offset) / step) * step;

    for(let i = 0; i < samples.length; ++i) {
        data.timestamps[i] = (start + i * step) / 1000;
    }

    return data;
}

function array_append<T>(a: Array<T>, b: Array<T>, tail: number): Array<T> {
    a.push(...b);

    return a.slice(-tail);
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
                        <FormRow label={__("sem20.content.wirkleistung")} labelColClasses="col-sm-6" contentColClasses="col-sm-6">
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
                                        <h5>L1</h5>
                                    </div>
                                    <div class="mb-1 col px-1 text-center">
                                        <h5>L2</h5>
                                    </div>
                                    <div class="mb-1 col px-1 text-center">
                                        <h5>L3</h5>
                                    </div>
                                    <div class="mb-1 col px-1 text-center">
                                        <h5>Gesamt</h5>
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
