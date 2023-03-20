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

import { h, createRef, Component, ComponentChild } from "preact";
import { __ } from "../../ts/translation";
import uPlot from "uplot";

export interface UplotData {
    timestamps: number[];
    samples: number[];
}

export interface UplotWrapperProps {
    id: string;
    class: string;
    sidebar_id: string;
    y_min?: number;
    y_max?: number;
    y_diff_min?: number;
    y_step?: number;
}

export class UplotWrapper extends Component<UplotWrapperProps, {}> {
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

export function calculate_live_data(offset: number, samples_per_second: number, samples: number[]): UplotData {
    let data: UplotData = {timestamps: new Array(samples.length), samples: samples};
    let now = Date.now();
    let start;
    let step;

    if (samples_per_second == 0) { // implies samples.length == 1
        start = now - offset;
        step = 0;
    }
    else {
        // (samples.length - 1) because samples_per_second defines the gaps between
        // two samples. with N samples there are (N - 1) gaps, while the lastest/newest
        // sample is offset milliseconds old
        start = now - (samples.length - 1) / samples_per_second * 1000 - offset;
        step = 1 / samples_per_second * 1000;
    }

    for (let i = 0; i < samples.length; ++i) {
        data.timestamps[i] = (start + i * step) / 1000;
    }

    return data;
}

export function calculate_history_data(offset: number, samples: number[]): UplotData {
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

export function array_append<T>(a: Array<T>, b: Array<T>, tail: number): Array<T> {
    a.push(...b);

    return a.slice(-tail);
}
