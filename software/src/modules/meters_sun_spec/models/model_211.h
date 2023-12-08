// WARNING: This file is generated.

#pragma once

#include <stdint.h>
#include <stdlib.h>

#if defined(__GNUC__)
    #pragma GCC diagnostic push
    #include "gcc_warnings.h"
    #pragma GCC diagnostic ignored "-Wattributes"
#endif

// ========================
// 211 - Single Phase Meter
// ========================

struct SunSpecSinglePhaseMeterModel211_s {
                         uint16_t ID;              //   0
                         uint16_t L;               //   1
    _ATTRIBUTE((packed)) uint32_t A;               //   2
    _ATTRIBUTE((packed)) uint32_t AphA;            //   4
    _ATTRIBUTE((packed)) uint32_t AphB;            //   6
    _ATTRIBUTE((packed)) uint32_t AphC;            //   8
    _ATTRIBUTE((packed)) uint32_t PhV;             //  10
    _ATTRIBUTE((packed)) uint32_t PhVphA;          //  12
    _ATTRIBUTE((packed)) uint32_t PhVphB;          //  14
    _ATTRIBUTE((packed)) uint32_t PhVphC;          //  16
    _ATTRIBUTE((packed)) uint32_t PPV;             //  18
    _ATTRIBUTE((packed)) uint32_t PPVphAB;         //  20
    _ATTRIBUTE((packed)) uint32_t PPVphBC;         //  22
    _ATTRIBUTE((packed)) uint32_t PPVphCA;         //  24
    _ATTRIBUTE((packed)) uint32_t Hz;              //  26
    _ATTRIBUTE((packed)) uint32_t W;               //  28
    _ATTRIBUTE((packed)) uint32_t WphA;            //  30
    _ATTRIBUTE((packed)) uint32_t WphB;            //  32
    _ATTRIBUTE((packed)) uint32_t WphC;            //  34
    _ATTRIBUTE((packed)) uint32_t VA;              //  36
    _ATTRIBUTE((packed)) uint32_t VAphA;           //  38
    _ATTRIBUTE((packed)) uint32_t VAphB;           //  40
    _ATTRIBUTE((packed)) uint32_t VAphC;           //  42
    _ATTRIBUTE((packed)) uint32_t VAR;             //  44
    _ATTRIBUTE((packed)) uint32_t VARphA;          //  46
    _ATTRIBUTE((packed)) uint32_t VARphB;          //  48
    _ATTRIBUTE((packed)) uint32_t VARphC;          //  50
    _ATTRIBUTE((packed)) uint32_t PF;              //  52
    _ATTRIBUTE((packed)) uint32_t PFphA;           //  54
    _ATTRIBUTE((packed)) uint32_t PFphB;           //  56
    _ATTRIBUTE((packed)) uint32_t PFphC;           //  58
    _ATTRIBUTE((packed)) uint32_t TotWhExp;        //  60
    _ATTRIBUTE((packed)) uint32_t TotWhExpPhA;     //  62
    _ATTRIBUTE((packed)) uint32_t TotWhExpPhB;     //  64
    _ATTRIBUTE((packed)) uint32_t TotWhExpPhC;     //  66
    _ATTRIBUTE((packed)) uint32_t TotWhImp;        //  68
    _ATTRIBUTE((packed)) uint32_t TotWhImpPhA;     //  70
    _ATTRIBUTE((packed)) uint32_t TotWhImpPhB;     //  72
    _ATTRIBUTE((packed)) uint32_t TotWhImpPhC;     //  74
    _ATTRIBUTE((packed)) uint32_t TotVAhExp;       //  76
    _ATTRIBUTE((packed)) uint32_t TotVAhExpPhA;    //  78
    _ATTRIBUTE((packed)) uint32_t TotVAhExpPhB;    //  80
    _ATTRIBUTE((packed)) uint32_t TotVAhExpPhC;    //  82
    _ATTRIBUTE((packed)) uint32_t TotVAhImp;       //  84
    _ATTRIBUTE((packed)) uint32_t TotVAhImpPhA;    //  86
    _ATTRIBUTE((packed)) uint32_t TotVAhImpPhB;    //  88
    _ATTRIBUTE((packed)) uint32_t TotVAhImpPhC;    //  90
    _ATTRIBUTE((packed)) uint32_t TotVArhImpQ1;    //  92
    _ATTRIBUTE((packed)) uint32_t TotVArhImpQ1phA; //  94
    _ATTRIBUTE((packed)) uint32_t TotVArhImpQ1phB; //  96
    _ATTRIBUTE((packed)) uint32_t TotVArhImpQ1phC; //  98
    _ATTRIBUTE((packed)) uint32_t TotVArhImpQ2;    // 100
    _ATTRIBUTE((packed)) uint32_t TotVArhImpQ2phA; // 102
    _ATTRIBUTE((packed)) uint32_t TotVArhImpQ2phB; // 104
    _ATTRIBUTE((packed)) uint32_t TotVArhImpQ2phC; // 106
    _ATTRIBUTE((packed)) uint32_t TotVArhExpQ3;    // 108
    _ATTRIBUTE((packed)) uint32_t TotVArhExpQ3phA; // 110
    _ATTRIBUTE((packed)) uint32_t TotVArhExpQ3phB; // 112
    _ATTRIBUTE((packed)) uint32_t TotVArhExpQ3phC; // 114
    _ATTRIBUTE((packed)) uint32_t TotVArhExpQ4;    // 116
    _ATTRIBUTE((packed)) uint32_t TotVArhExpQ4phA; // 118
    _ATTRIBUTE((packed)) uint32_t TotVArhExpQ4phB; // 120
    _ATTRIBUTE((packed)) uint32_t TotVArhExpQ4phC; // 122
    _ATTRIBUTE((packed)) uint32_t Evt;             // 124
};

#define SUNSPEC_MODEL_211_NAME "Single Phase Meter"

// Total size in bytes, includes ID and length registers.
#define SUNSPEC_MODEL_211_SIZE (252)

// Register count without ID and length registers, should match content of length register.
#define SUNSPEC_MODEL_211_LENGTH (124)

union SunSpecSinglePhaseMeterModel211_u {
    struct SunSpecSinglePhaseMeterModel211_s model;
    uint16_t registers[SUNSPEC_MODEL_211_SIZE / sizeof(uint16_t)];
};

static_assert(sizeof(SunSpecSinglePhaseMeterModel211_s) == 252, "Incorrect Single Phase Meter length.");
static_assert(sizeof(SunSpecSinglePhaseMeterModel211_u) == 252, "Incorrect Single Phase Meter length.");
static_assert(sizeof(static_cast<SunSpecSinglePhaseMeterModel211_u *>(nullptr)->registers) == 252, "Incorrect Single Phase Meter length.");

#if defined(__GNUC__)
   #pragma GCC diagnostic pop
#endif
