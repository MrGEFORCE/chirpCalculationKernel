#ifndef CHIRPPARAMETERS_CFAR_H
#define CHIRPPARAMETERS_CFAR_H

#include <vector>
#include <string>
#include <unordered_map>

#include "config.h"
#include "common.h"

#define STR_SEC_CFAR "cfar parameters"

#define STR_KEY_cfarParamEnabled "Enabled"
#define STR_KEY_cfarParamDim "Dim"
#define STR_KEY_cfarParamType "Type"
#define STR_KEY_cfarParamGuard "Guard"
#define STR_KEY_cfarParamReference "Reference"
#define STR_KEY_cfarParamAlpha "Alpha"
#define STR_KEY_cfarParamBias "Bias"

#define STR_KEY_cfarDimRange "Range"
#define STR_KEY_cfarDimDoppler "Doppler"
#define STR_KEY_cfarDimAzimuth "Azimuth"
#define STR_KEY_cfarDimElevation "Elevation"

#define CFAR_SAVED_SETTING_NUMS 7U

enum cfarType_e {
    CFAR_CA = 0,
    CFAR_GO,
    CFAR_SO
};

enum cfarDim_e {
    dimRange = 0,
    dimDoppler,
    dimAzimuth,
    dimElevation,
    cfarDimTotalNums
};

const int cfarDataTypeList[CFAR_SAVED_SETTING_NUMS] = {typeBool, typeInt, typeInt, typeInt, typeInt, typeFloat, typeFloat};

enum CfarDataBool_e {
    enabledIdx = 0,
    CfarDataBoolCount
};

typedef struct {
    bool enabled;
} CfarBoolData_t;

union CfarBoolData_u {
    CfarBoolData_t t;
    bool v[CfarDataBoolCount];
};

enum CfarDataInt_e {
    dimIdx = 0,
    typeIdx,
    guardIdx,
    refIdx,
    CfarDataIntCount
};

typedef struct {
    cfarDim_e dim;
    cfarType_e type;
    int guard;
    int ref;
} CfarIntData_t;

union CfarIntData_u {
    CfarIntData_t t;
    int v[CfarDataIntCount];
};

enum CfarDataFloat_e {
    alphaIdx = 0,
    biasIdx,
    CfarDataFloatCount
};

typedef struct {
    float alpha;
    float bias;
} CfarFloatData_t;

union CfarFloatData_u {
    CfarFloatData_t t;
    float v[CfarDataFloatCount];
};

typedef struct {
    // settings
    CfarBoolData_u b;
    CfarIntData_u i;
    CfarFloatData_u f;
    // results
    int N;  // total length
    float falseAlarmRate;
} basicCfarData_t;

void from_rate_to_alpha_ca(basicCfarData_t *config);

void false_alarm_rate_ca(basicCfarData_t *config);

void false_alarm_rate_go(basicCfarData_t *config);

void false_alarm_rate_so(basicCfarData_t *config);

class CFAR {
public:
    std::unordered_map<std::string, int> strRefIdxMap;
    std::vector<std::string> keyStringsCfarParams;
    std::vector<std::string> keyStringsCfarDims;
    basicCfarData_t data[cfarDimTotalNums]{};
    basicCfarData_t defaultCfarValue;
    bool errFlag;

    CFAR();

    void set_default();

    void set_single_default(cfarDim_e dim);

    void cal();

    bool is_enabled();

    void save(std::ofstream *saveFile);

    void load(config::RrConfig *cfg, std::string *text);

    ~CFAR();
};


#endif //CHIRPPARAMETERS_CFAR_H
