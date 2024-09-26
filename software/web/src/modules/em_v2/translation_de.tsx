/** @jsxImportSource preact */
import { h } from "preact";
let x = {
    "em_v2": {
        "automation": {
            "input_switches": "Eingang geschaltet",
            "input_index": "Eingangsnummer",

            "sgready_switch": "Switch SG Ready output",
            "sgready_index": "SG Ready Ausgang",
            "sgready_action_text": /*FFN*/(index: number, state: boolean) => {
                let ret = state ? <><b>schließen</b></> : <><b>öffnen</b></>
                return <>SG Ready Ausgang {index} {ret}.</>
            }/*NF*/
        }
    }
}