/* esp32-firmware
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
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

import { h, JSX, Context } from "preact";
import { useId, useContext, useRef } from "preact/hooks";
import { Button } from "react-bootstrap";
import { Minus, Plus } from "react-feather";
import { __ } from "../translation";
import { register_id_context_component_type } from "./form_row";

import * as util from "../util";

interface SwitchableInputNumberProps extends Omit<JSX.InputHTMLAttributes<HTMLInputElement>, "class" | "id" | "type" | "onInput"> {
    idContext?: Context<string>;
    value: number;
    onValue: (value: number) => void;
    unit?: string;
    invalidFeedback?: string;
    disabled?: boolean;
    checked: boolean;
    onClick: JSX.MouseEventHandler<HTMLInputElement>;
    switch_label_active?: string;
    switch_label_inactive?: string;
    switch_label_min_width?: string;
}

export function SwitchableInputNumber(props: SwitchableInputNumberProps) {
    const id = !props.idContext ? useId() : useContext(props.idContext);
    const label_desktop_id = id + "-1";
    const label_mobile_id = id + "-2";

    const input = useRef<HTMLInputElement>();
    const value = parseInt(props.value?.toString(), 10);

    const invalid = isNaN(value) || (props.min !== undefined && value < parseInt(props.min.toString())) || (props.max !== undefined && value > parseInt(props.max.toString()));

    let invalidFeedback = undefined;
    if ("invalidFeedback" in props && props.invalidFeedback) {
        invalidFeedback = <div class="invalid-feedback">{props.invalidFeedback}</div>;
    } else if (invalid) {
        if (props.required && isNaN(value)) {
            invalidFeedback = <div class="invalid-feedback">{__("component.input_number.required")}</div>;
        } else if ("min" in props && !("max" in props)) {
            invalidFeedback = <div class="invalid-feedback">{__("component.input_number.min_only")(props.min.toString(), props.unit ? props.unit : "")}</div>;
        } else if (!("min" in props) && "max" in props) {
            invalidFeedback = <div class="invalid-feedback">{__("component.input_number.max_only")(props.max.toString(), props.unit ? props.unit : "")}</div>;
        } else if ("min" in props && "max" in props) {
            invalidFeedback = <div class="invalid-feedback">{__("component.input_number.min_max")(props.min.toString(), props.max.toString(), props.unit ? props.unit : "")}</div>;
        }
    }

    return (
        <div class="row no-gutters input-group rounded">
            <div class="d-none d-sm-block input-group-prepend input-group-text custom-control custom-switch" style={"padding-left: 2.75rem; padding-right: 0.6rem; border-bottom-right-radius: 0; border-top-right-radius: 0; text-align: left; " + (props.switch_label_min_width ? ("min-width: " + props.switch_label_min_width) : "")}>
                <input type="checkbox" class="custom-control-input" id={label_desktop_id} checked={props.checked} onClick={props.onClick} disabled={props.disabled}/>
                <label class="custom-control-label" for={label_desktop_id}>{props.checked ? props.switch_label_active : props.switch_label_inactive}</label>
            </div>
            <div class="d-block d-sm-none input-group-prepend input-group-text custom-control custom-switch" style={"padding-left: 2.75rem; padding-right: 0; border-bottom-right-radius: 0; border-top-right-radius: 0;"}>
                <input type="checkbox" class="custom-control-input" id={label_mobile_id} checked={props.checked} onClick={props.onClick} disabled={props.disabled}/>
                <label class="custom-control-label" for={label_mobile_id}></label>
            </div>
            <input class="form-control no-spin"
                ref={input}
                id={id}
                type="number"
                onInput={(e) => {
                        // Chrome prints a console warning if NaN is assigned as an input's value; null works.
                        let value = parseInt((e.target as HTMLInputElement).value, 10);
                        if (isNaN(value))
                            value = null;
                        props.onValue(value);
                    }}
                inputMode="numeric"
                {...props}
                onClick={undefined}
                disabled={!props.checked || props.disabled}
            />
            <div class="input-group-append">
                {props.unit ? <div class="form-control input-group-text">{props.unit}</div> : undefined}
                <Button variant="primary"
                        disabled={!props.checked || props.value == props.min || props.disabled}
                        className="form-control px-1"
                        style="margin-right: .125rem !important;"
                        onClick={() => {
                            if (!props.checked || props.disabled) {
                                return;
                            }

                            if (util.hasValue(props.value) && !isNaN(props.value)) {
                                props.onValue(util.clamp(props.min as number, props.value - 1, props.max as number));
                            }
                            else {
                                props.onValue(props.min as number);
                            }

                            input.current.parentNode.dispatchEvent(new Event('input', {bubbles: true}));
                        }}>
                    <Minus/>
                </Button>
                <Button variant="primary"
                        disabled={!props.checked || props.value == props.max || props.disabled}
                        className="form-control px-1 rounded-right"
                        onClick={() => {
                            if (!props.checked || props.disabled) {
                                return;
                            }

                            if (util.hasValue(props.value) && !isNaN(props.value)) {
                                props.onValue(util.clamp(props.min as number, props.value + 1, props.max as number));
                            }
                            else {
                                props.onValue(props.max as number);
                            }

                            input.current.parentNode.dispatchEvent(new Event('input', {bubbles: true}));
                        }}>
                    <Plus/>
                </Button>
            </div>
            {invalidFeedback}
        </div>
    );
}

register_id_context_component_type(SwitchableInputNumber);
