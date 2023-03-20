export interface values {
    power: number
}

export type all_values = number[];

export interface live {
    offset: number,
    samples_per_second: number,
    samples: number[]
}

export interface live_samples {
    samples_per_second: number,
    samples: number[]
}

export interface history {
    offset: number,
    samples: number[]
}

export interface history_samples {
    samples: number[]
}
