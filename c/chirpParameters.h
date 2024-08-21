#ifndef C_CHIRPPARAMETERS_H
#define C_CHIRPPARAMETERS_H

#define CP_ERR false
#define CP_OK true

#include <stdbool.h>

enum slopePriority_e {
    spSlopeFirst = 0,
    spBandwidthFirst,
};

enum CfarParameterDataInt_e {
    rangeGuardIdx = 0,
    rangeReferenceIdx,
    dopplerGuardIdx,
    dopplerReferenceIdx,
    CfarParameterDataIntCount
};

enum cpErr_e {
    errDutyCycleGT100 = 0,
    errADCPointsGTLimit,
    errChirpLoopsGTLimit,
    errRangeFFTSizeGTLimit,
    errDopplerFFTSizeGTLimit,
    errADCPointsLTLimit,
    errChirpLoopsLTLimit,
    errRangeFFTSizeLTLimit,
    errDopplerFFTSizeLTLimit,
    errADCDelayTooLong,
    errADCSampleTimeTooLong,
    errRangeFFTSIzeNotPower2,
    errRangeFFTSizeLTADCPoints,
    errDopplerFFTSIzeNotPower2,
    errDopplerFFTSizeLTChirpLoops,
    errNoErr
};

typedef struct {
    int rangeGuard;
    int rangeReference;
    int dopplerGuard;
    int dopplerReference;
} CfarParameterDataInt_t;

union CfarParameterDataInt_u {
    int v[CfarParameterDataIntCount];
    CfarParameterDataInt_t t;
};

enum CfarParameterDataFloat_e {
    rangeAlphaIdx = 0,
    rangeBias_dBIdx,
    dopplerAlphaIdx,
    dopplerBias_dBIdx,
    CfarParameterDataFloatCount
};

typedef struct {
    float rangeAlpha;
    float rangeBias_dB;
    float dopplerAlpha;
    float dopplerBias_dB;
} CfarParameterDataFloat_t;

union CfarParameterDataFloat_u {
    float v[CfarParameterDataFloatCount];
    CfarParameterDataFloat_t t;
};

enum CfarParameterDataBool_e {
    enabledIdx = 0,
    CfarParameterDataBoolCount
};

typedef struct {
    bool enabled;
} CfarParameterDataBool_t;

union CfarParameterDataBool_u {
    bool v[CfarParameterDataBoolCount];
    CfarParameterDataBool_t t;
};

typedef struct {
    union CfarParameterDataInt_u intData;
    union CfarParameterDataFloat_u floatData;
    union CfarParameterDataBool_u boolData;
} CfarParameterData_t;

enum ChirpParameterDataInt_e {
    ADCPointsIdx = 0,
    chirpLoopsIdx,
    antTDMIdx,
    rxIdx,
    rangeFFTSizeIdx,
    dopplerFFTSizeIdx,
    minADCPointsIdx,
    minChirpLoopsIdx,
    minRangeFFTSizeIdx,
    minDopplerFFTSizeIdx,
    maxADCPointsIdx,
    maxChirpLoopsIdx,
    maxRangeFFTSizeIdx,
    maxDopplerFFTSizeIdx,
    ChirpParameterDataIntCount
};

typedef struct {
    int ADCPoints;
    int chirpLoops;
    int antTDM;
    int rx;
    int rangeFFTSize;
    int dopplerFFTSize;
    int minADCPoints;  // opt
    int minChirpLoops;   // opt
    int minRangeFFTSize;   // opt
    int minDopplerFFTSize;   // opt
    int maxADCPoints;   // opt
    int maxChirpLoops;   // opt
    int maxRangeFFTSize;   // opt
    int maxDopplerFFTSize;   // opt
} ChirpParameterDataInt_t;

union ChirpParameterDataInt_u {
    int v[ChirpParameterDataIntCount];
    ChirpParameterDataInt_t t;
};

enum ChirpParameterDataFloat_e {
    startFrequency_MHzIdx = 0,
    bandWidth_MHzIdx,
    idleTime_usIdx,
    rampTime_usIdx,
    periodicity_msIdx,
    sampleRate_kspsIdx,
    ADCDelay_usIdx,
    ADCTime_usIdx,
    ADCBandWidth_MHzIdx,
    maxADCTime_usIdx,
    lambdaStart_mmIdx,
    lambdaCenter_mmIdx,
    dutyCycle_percentIdx,
    Tc_usIdx,
    TcTDM_usIdx,
    Tf_usIdx,
    dMax_mIdx,
    dRes_mIdx,
    dResCompute_mIdx,
    vMax_m_sIdx,
    vRes_m_sIdx,
    vResCompute_m_sIdx,
    dopplerSampleRate_spsIdx,
    oneMeterIF_kHzIdx,
    oneResBinIF_kHzIdx,
    oneResComputeBinIF_kHzIdx,
    MaxIF_kHzIdx,
    f32radarCube_kBIdx,
    ChirpParameterDataFloatCount
};

typedef struct {
    float startFrequency_MHz;
    float bandWidth_MHz;
    float idleTime_us;
    float rampTime_us;
    float periodicity_ms;
    float sampleRate_ksps;
    float ADCDelay_us;
    float slope_MHzus;
    float ADCTime_us;
    float ADCBandWidth_MHz;
    float maxADCTime_us;
    float lambdaStart_mm;
    float lambdaCenter_mm;
    float dutyCycle_percent;
    float Tc_us;
    float TcTDM_us;
    float Tf_us;
    float dMax_m;
    float dRes_m;
    float dResCompute_m;
    float vMax_m_s;
    float vRes_m_s;
    float vResCompute_m_s;
    float dopplerSampleRate_sps;
    float oneMeterIF_kHz;
    float oneResBinIF_kHz;
    float oneResComputeBinIF_kHz;
    float maxIF_kHz;
    float f32radarCube_kB;
} ChirpParameterDataFloat_t;

union ChirpParameterDataFloat_u {
    float v[ChirpParameterDataFloatCount];
    ChirpParameterDataFloat_t t;
};

enum ChirpParameterDataBool_e {
    iqSampleIdx = 0,
    staticClutterRemovalIdx,
    ChirpParameterDataBoolCount
};

typedef struct {
    bool iqSample;
    bool staticClutterRemoval;  // opt
} ChirpParameterDataBool_t;

union ChirpParameterDataBool_u {
    bool v[ChirpParameterDataBoolCount];
    ChirpParameterDataBool_t t;
};

typedef struct {
    CfarParameterData_t cfar;
    union ChirpParameterDataInt_u intData;
    union ChirpParameterDataFloat_u floatData;
    union ChirpParameterDataBool_u boolData;
} ChirpParameterData_t;

typedef struct {
    ChirpParameterData_t data;
    bool computeErrorFlag;
    enum slopePriority_e slopePriority;
    enum cpErr_e errType;
} ChirpParameterHandler_t;

void set_default(ChirpParameterHandler_t* h);

void compute_and_validate(ChirpParameterHandler_t* h);


#endif //C_CHIRPPARAMETERS_H
