#ifndef CPP_CHIRPPARAMETERS_H
#define CPP_CHIRPPARAMETERS_H

#include <vector>
#include <string>
#include <unordered_map>

#include "config.h"

namespace chirpParameters {

#define CP_ERR false
#define CP_OK true

#define STR_SEC_CHIRP "chirp parameters"
#define STR_SEC_CFAR "cfar parameters"

#define STR_KEY_startFrequency_MHz "startFrequency_MHz"
#define STR_KEY_bandWidth_MHz "bandWidth_MHz"
#define STR_KEY_idleTime_us "idleTime_us"
#define STR_KEY_rampTime_us "rampTime_us"
#define STR_KEY_periodicity_ms "periodicity_ms"
#define STR_KEY_ADCPoints "ADCPoints"
#define STR_KEY_sampleRate_ksps "sampleRate_ksps"
#define STR_KEY_ADCDelay_us "ADCDelay_us"
#define STR_KEY_chirpLoops "chirpLoops"
#define STR_KEY_iqSample "iqSample"
#define STR_KEY_antTDM "antTDM"
#define STR_KEY_rangeFFTSize "rangeFFTSize"
#define STR_KEY_dopplerFFTSize "dopplerFFTSize"
#define STR_KEY_minADCPoints "minADCPoints"
#define STR_KEY_minChirpLoops "minChirpLoops"
#define STR_KEY_minRangeFFTSize "minRangeFFTSize"
#define STR_KEY_minDopplerFFTSize "minDopplerFFTSize"
#define STR_KEY_maxADCPoints "maxADCPoints"
#define STR_KEY_maxChirpLoops "maxChirpLoops"
#define STR_KEY_maxRangeFFTSize "maxRangeFFTSize"
#define STR_KEY_maxDopplerFFTSize "maxDopplerFFTSize"
#define STR_KEY_staticClutterRemoval "staticClutterRemoval"
#define CFG_BASIC_LINES (22U)

#define STR_KEY_rangeGuard "cfarRangeGuard"
#define STR_KEY_rangeReference "cfarRangeReference"
#define STR_KEY_rangeAlpha "cfarRangeAlpha"
#define STR_KEY_rangeBias_dB "cfarRangeBias_dB"
#define STR_KEY_dopplerGuard "cfarDopplerGuard"
#define STR_KEY_dopplerReference "cfarDopplerReference"
#define STR_KEY_dopplerAlpha "cfarDopplerAlpha"
#define STR_KEY_dopplerBias_dB "cfarDopplerBias_dB"
#define CFG_CFAR_LINES (8U)

    enum dataType_e {
        typeInt = 0,
        typeFloat,
        typeBool
    };

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
        CfarParameterDataInt_u intData;
        CfarParameterDataFloat_u floatData;
        CfarParameterDataBool_u boolData;
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
        ChirpParameterDataInt_u intData;
        ChirpParameterDataFloat_u floatData;
        ChirpParameterDataBool_u boolData;
    } ChirpParameterData_t;

    class ChirpParameterHandler {
    public:
        ChirpParameterData_t data{};
        std::string errMsg;
        std::vector<std::string> keyStringsBasic;
        std::vector<std::string> keyStringsCfar;
        std::unordered_map <std::string, int > strRefTypeMap;
        std::unordered_map <std::string, int > strRefIdxMap;
        config::RrConfig configParser;

        bool computeErrorFlag;
        slopePriority_e slopePriority;

        ChirpParameterHandler();

        void set_default();

        void compute_and_validate();

        void save_cfg(const std::string& saveFileName);

        void load_cfg(const std::string& loadFileName);

        ~ChirpParameterHandler();
    };

} // chirpParameters

#endif //CPP_CHIRPPARAMETERS_H
